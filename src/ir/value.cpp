#include "value.h"

#include "ir/constant.h"
#include "ir/instruction.h"
#include "passes/module.h"
#include "ir/function.h"
#include "ir/type.h"
#include "user.h"
#include "utils.h"
#include "visitor/ir_visitor_base.h"

#include <cstddef>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <queue>
#include <type_traits>

void Value::addUse(User *user, unsigned value_num) {
    this->_user_list.push_back(new Use(this, value_num, user));
}
Value::Value(const std::string &name, Type *type) : _name(name), _type(type) {}
Value::Value(Type *type, const std::string &name) : _name(name), _type(type) {}
Type *Value::getType() const { return _type; }

Value *Value::getUncertainValue() {
    static auto a = Value(Type::getInt32Ty(), "uncertainValue");
    return &a;
};
/**
 * 用 new_val 替换掉使用自己的地方
 * @param new_val
 */
void Value::replaceAllUse(Value *new_val) {
    //    printf("[repalce all user of %s]\n",this->getPrintName().c_str());
    //    if(new_val )
    //        printf("new val %s\n",new_val->getPrintName().c_str());
    for (auto use : _user_list) {
        auto user = dynamic_cast<User *>(use->_user);
        //        printf(" user %s  argnum: %d
        //        \n",user->getPrintName().c_str(),use->_value_no);
        user->setOperand(use->_value_no, new_val);
    }

    this->_user_list.clear();
}
/**
 * 删掉自己在user 里作为第几个参数被使用的记录
 * @param user
 * @param value_no
 */
void Value::removeUse(Value *user, unsigned value_no) {
    Use *remove_use = nullptr;

    // printf("%s 被使用的记录：",this->getPrintName().c_str());
    // for(auto use:_user_list){
    //     printf("(%d,%s)",use->_value_no,use->_user->getPrintName().c_str());
    // }
    //     printf("\n");

    for (auto use : _user_list) {
        if (use->_value_no == value_no && use->_user == user) {
            // if(){
            //     MyAssert("can't get right use record ", use->_user == user);
            // }
            remove_use = use;
            break;
        }
    }
    MyAssert("can't get right use record ", remove_use!=nullptr,EXIT_CODE_ERROR_436 );
    auto iter = std::find(_user_list.begin(), _user_list.end(), remove_use);

    _user_list.erase(iter);
}

bool Value::isConstant() {
    auto constant = dynamic_cast<Constant *>(this);
    return constant != nullptr;
}
std::string Value::getName()const { return _name; }
void Value::accept(IrVisitorBase *v) { v->visit(this); }
void Value::setName(std::string name) {
    if (!_name.empty()) {
        WARNNING("rename the value:\"%s\" -> \"%s\"", _name.c_str(),
                 name.c_str());
    }
    _name = name;
}

Function *ValueCloner::copyFunc(Function *old_func) {
    _old2new.clear();
    for (auto gv : old_func->getParent()->getGlobalVariables()) {
        _old2new[gv] = gv;
    }
    auto new_func_ptr =
        new Function(old_func->getFunctionType(), old_func->getName());
    auto old_args = old_func->getArgs();
    auto new_args = new_func_ptr->getArgs();

    for (int i = 0; i < old_args.size(); i++) {
        _old2new[old_args.at(i)] = new_args.at(i);
    }

    for (auto bb : old_func->getBasicBlocks()) {
        auto new_bb =  BasicBlock::create("");
        _old2new[bb] =new_bb;
        new_func_ptr->addBasicBlock(new_bb);
        new_bb->setParentFunc(new_func_ptr);
    }

    // for (auto bb : old_func->getBasicBlocks()) {
    //     auto new_bb = dynamic_cast<BasicBlock *>(_old2new[bb]);
    //     MyAssert("null ptr", new_bb);
    //     for (auto pre_bb : bb->getPreBasicBlocks()) {
    //         auto new_pre = dynamic_cast<BasicBlock *>(_old2new[pre_bb]);
    //         MyAssert("null ptr", new_pre);
    //         // new_bb->addPreBasicBlock(new_pre);
    //     }
    //     for (auto suc_bb : bb->getSuccBasicBlocks()) {
    //         auto new_suc = dynamic_cast<BasicBlock *>(_old2new[suc_bb]);
    //         MyAssert("null ptr", new_suc);
    //         // new_bb->addPreBasicBlock(new_suc);
    //     }
    // }

    std::set<BasicBlock *> visited_bb;
    std::queue<BasicBlock *> work_list;
    work_list.push(old_func->getEntryBlock());
    visited_bb.insert(old_func->getEntryBlock());
    while (!work_list.empty()) {
        auto b = work_list.front();
        work_list.pop();
        copyBasicBlock(b);
        for (auto suc_b : b->getSuccBasicBlockList()) {
            if (visited_bb.find(suc_b) == visited_bb.end()) {
                visited_bb.insert(suc_b);
                work_list.push(suc_b);
            }
        }
    }
    new_func_ptr->movAllocaToEntry();
    std::set<PhiInstr*> phi_set;
    for(auto bb :old_func->getBasicBlocks()){
        for(auto instr : bb->getInstructions()){
            auto phi = dynamic_cast<PhiInstr*>(instr);
            if(phi !=nullptr){
                phi_set.insert(phi);
            }
        }
    }
    for(auto phi : phi_set){
        auto new_phi = dynamic_cast<PhiInstr*>(findValue(phi));
        if(new_phi!=nullptr){
            unsigned i = 0;
            for( ;i < phi->getOperandNumber();i=i+2){
                new_phi->setParams(
                    findValue(phi->getOperand(i)),
                    findValue(phi->getOperand(i+1))
                    );
            }
        }
        else {
            ERROR("null ptr",DYNAMIC_CAST_NULL_PTR);
        }
    }
    return new_func_ptr;
}
void ValueCloner::copyBasicBlock(BasicBlock *old_bb) {
    for (auto instr : old_bb->getInstructions()) {
        auto new_instr = copyInstr(instr);
        _old2new[instr] = new_instr;
    }
}
Instruction *ValueCloner::copyInstr(Instruction *old_instr) {
    Instruction *new_instr;
    auto new_parent_bb =
        dynamic_cast<BasicBlock *>(findValue(old_instr->getParent()));
    MyAssert("new_parent_bb null ptr !", new_parent_bb,DYNAMIC_CAST_NULL_PTR);
    if (old_instr->isBinary()) {
        if (old_instr->isCmp()) {
            auto old_cmp_instr = dynamic_cast<CmpInst *>(old_instr);
            MyAssert("null ptr", old_cmp_instr,DYNAMIC_CAST_NULL_PTR);
            new_instr = new CmpInst(
                old_cmp_instr->getType(), old_cmp_instr->getCmpOp(),
                findValue(old_cmp_instr->getOperand(0)),
                findValue(old_cmp_instr->getOperand(1)), new_parent_bb);
        } else {
            new_instr = new BinaryInst(
                old_instr->getType(), old_instr->getInstructionKind(),
                findValue(old_instr->getOperand(0)),
                findValue(old_instr->getOperand(1)), new_parent_bb);
        }
    } else if (old_instr->isNot()) {
        new_instr = UnaryInst::createNot(findValue(old_instr->getOperand(0)),
                                         new_parent_bb);
    } else if (old_instr->isCast()) {
        new_instr = UnaryInst::createCast(findValue(old_instr->getOperand(0)),
                                          old_instr->getType(), new_parent_bb);
    } else if (old_instr->isBr()) {
        auto br_instr = dynamic_cast<BranchInst *>(old_instr);
        if (br_instr->isConBranch()) {
            new_instr = BranchInst::createCondBr(
                findValue(br_instr->getOperand(0)),
                dynamic_cast<BasicBlock *>(findValue(br_instr->getOperand(1))),
                dynamic_cast<BasicBlock *>(findValue(br_instr->getOperand(2))),
                new_parent_bb);
        } else if (br_instr->isBranch()) {
            new_instr = BranchInst::createBranch(
                dynamic_cast<BasicBlock *>(findValue(br_instr->getOperand(0))),
                new_parent_bb);
        }
        else {
            ERROR("error type",ERROR_DEFUALT);
        }
    } else if (old_instr->isStore()) {
        new_instr = StoreInst::createStore(findValue(old_instr->getOperand(0)),
                                           findValue(old_instr->getOperand(1)),
                                           new_parent_bb);
    } else if (old_instr->isLoad()) {
        new_instr = LoadInst::createLoad(findValue(old_instr->getOperand(0)),
                                         new_parent_bb);
    } else if (old_instr->isCall()) {
        auto called_func = dynamic_cast<Function *>(old_instr->getOperand(0));
        MyAssert("nullptr ", called_func != nullptr,DYNAMIC_CAST_NULL_PTR);
        std::vector<Value *> args = {};
        for (auto i = 1; i < old_instr->getOperandNumber(); i++) {
            args.push_back(findValue(old_instr->getOperand(i)));
            // new_instr->addOperand(findValue(old_instr->getOperand(i)));
        }
        new_instr = CallInst::createCall(called_func, args, new_parent_bb);
    } else if (old_instr->isZext()) {
        new_instr = ZExtInst::creatZExtInst(old_instr->getType(),
                                            findValue(old_instr->getOperand(0)),
                                            new_parent_bb);
    } else if (old_instr->isAlloca()) {
        auto old_alloca_instr = dynamic_cast<AllocaInst*>(old_instr);
        new_instr =
            AllocaInst::createAlloca(old_alloca_instr->getAllocaType(), new_parent_bb);
    } else if (old_instr->isPhi()) {
        new_instr = PhiInstr::createPhi(
            old_instr->getType(), new_parent_bb);
    } else if (old_instr->isGep()) {

            std::vector<Value*>indexs;
            for(auto i = 1;i<old_instr->getOperandNumber();i++){
                indexs.push_back(findValue(old_instr->getOperand(i)));
            }
            auto ptr = findValue(old_instr->getOperand(0));
            new_instr = GetElementPtrInst::createGEP(ptr, indexs, new_parent_bb);
    } else if(old_instr->isRet() ){
        if(old_instr->getOperandNumber()==1){
            new_instr=ReturnInst::createRet(findValue(old_instr->getOperand(0)), new_parent_bb);
        }
        else if(old_instr->getOperandNumber()==0){
            new_instr=ReturnInst::createVoidRet(new_parent_bb);
        }
        else {
            ERROR("ERROR",ERROR_DEFUALT);
        }

    }else {
        new_instr = nullptr;
    }
    if(new_instr==nullptr){
        MyAssert("error", new_instr != nullptr,ERROR_DEFUALT);
    }
    return new_instr;
}
Value *ValueCloner::findValue(Value *old_val) {
    if (dynamic_cast<Constant *>(old_val)) {
        return old_val;
    }
    return _old2new[old_val];
}