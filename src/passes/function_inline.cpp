#include "function_inline.h"

#include <algorithm>
#include <iterator>
#include <list>

#include "ir/basic_block.h"
#include "ir/function.h"
#include "ir/instruction.h"
#include "ir/value.h"
#include "visitor/llvm_ir_printer.h"
#include "module.h"
#include "utils.h"

void FunctionInline::run() {
    for (auto func : _m->getFunctions()) {
        if (canFunctionInline(func)) {
            FUNC_LINE_LOG("inlining func [%s]",func->getName().c_str());
            inlineFunction(func);
        }
    }
}

bool FunctionInline::canFunctionInline(Function* f) {
    if (!f->isBuiltin() && _m->getMainFunction() != f) {
        for (auto caller : f->getCallerSet()) {
            for (auto callee : f->getCalleeSet()) {
                if (callee == caller) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}
void FunctionInline::inlineFunction(Function* f) {
    std::set<CallInst*> to_be_repalced_call_inst;
    for (auto caller : f->getCallerSet()) {
        FUNC_LINE_LOG("caller is %s",caller->getName().c_str());
        for (auto bb : caller->getBasicBlocks()) {
            for (auto instr : bb->getInstructions()) {
                auto call_instr = dynamic_cast<CallInst*>(instr);
                if (call_instr != nullptr &&
                    call_instr->getOperand(0)->getName() == f->getName()) {
                    to_be_repalced_call_inst.insert(call_instr);
                }
            }
        }
    }
    for (auto instr : to_be_repalced_call_inst) {
        inlineCallInstr(instr);
    }
    for (auto caller : f->getCallerSet()) {
        caller->getCalleeSet().erase(caller->getCalleeSet().find(f));
        caller->getCalleeSet().insert(f->getCalleeSet().begin(),
                                      f->getCalleeSet().end());
    }
    f->getCallerSet().clear();
}
void FunctionInline::inlineCallInstr(CallInst* callInstr) {
    // auto ret_type = callInstr->getType();
    auto caller_func = callInstr->getParent()->getParentFunc();
    auto func_wait_for_inline = callInstr->getFunction();
    ValueCloner cloner;
    auto copy_func = cloner.copyFunc(func_wait_for_inline);
    auto end_block =
        BasicBlock::create("");
        end_block->setParentFunc(callInstr->getParent()->getParentFunc());

    auto call_parent_bb = callInstr->getParent();

    // instr after call should be moved to end_block
    std::list<Instruction*> inst_after_call;
    auto call_pos =
        std::find(call_parent_bb->getInstructions().begin(),
                  call_parent_bb->getInstructions().end(), callInstr);
    call_pos++;
    end_block->getInstructions().assign(
        call_pos, call_parent_bb->getInstructions().end());
    for(auto instr:end_block->getInstructions()){
        instr->setParent(end_block);
    }
    call_pos--;
    inst_after_call.erase(call_pos, call_parent_bb->getInstructions().end());

    // 用end block 替代 call_parent_bb的前驱后继关系,同时把phi中call_parent_bb作为参数的替换成end_block
    for (auto suc_bb : call_parent_bb->getSuccBasicBlockList()) {
        end_block->addSuccBasicBlock(suc_bb);
        suc_bb->getPreBasicBlockList().remove(call_parent_bb);
        suc_bb->addPreBasicBlock(end_block);
    }
    std::list<Use*> wait_to_repalce;
    for(auto use : call_parent_bb->getUseList()){
        auto phi = dynamic_cast<PhiInstr*>(use->_user);
        if(phi){
            wait_to_repalce.push_back(use);
        }
    }
    for(auto use:wait_to_repalce){
            auto phi = dynamic_cast<User*>(use->_user);
            phi->setOperand(use->_value_no, end_block);
            call_parent_bb->removeUse(phi, use->_value_no);
    }
    call_parent_bb->getSuccBasicBlockList().clear();

    // call -> br
    BranchInst::createBranch(copy_func->getEntryBlock(), call_parent_bb);

    for (auto i = 0; i < copy_func->getNumArgs(); i++) {
        auto copy_arg = copy_func->getArgs().at(i);
        auto call_instr_arg = callInstr->getOperand(i + 1);
        if (copy_arg->getType()->isInt32() ||
            copy_arg->getType()->isFloatTy()) {
            copy_arg->replaceAllUse(call_instr_arg);
        } else {
            std::list<Instruction*> wait_delete_instr;
            for (auto use : copy_arg->getUseList()) {
                auto store_instr = dynamic_cast<StoreInst*>(use->_user);
                if (store_instr) {
                    auto alloca =
                        dynamic_cast<AllocaInst*>(store_instr->getOperand(1));
                    MyAssert("null ptr", alloca,DYNAMIC_CAST_NULL_PTR);
                    wait_delete_instr.push_back(store_instr);
                    wait_delete_instr.push_back(alloca);
                    // 如果有对参数的赋值会怎么样？
                    // ssa 不存在这种情况
                    for (auto allca_use : alloca->getUseList()) {
                        
                        auto load_instr = dynamic_cast<LoadInst*>(allca_use->_user);
                        if (load_instr) {
                            load_instr->replaceAllUse(call_instr_arg);
                            wait_delete_instr.push_back(load_instr);
                        }
                    }
                }
            }
            for(auto instr:wait_delete_instr){
                instr->removeUseOps();
                instr->getParent()->deleteInstr(instr);
            }
        }
    }  // end for args


    // call_parent_bb->addSuccBasicBlock(copy_func->getEntryBlock());
    // copy_func->getEntryBlock()->addPreBasicBlock(call_parent_bb);

    std::list<ReturnInst*> ret_instrs;
    for (auto bb : copy_func->getBasicBlocks()) {
            auto instr = bb->getTerminator();
            auto ret_instr = dynamic_cast<ReturnInst*>(instr);
            if (ret_instr != nullptr) {
                ret_instrs.push_back(ret_instr);

            }
    }
    for (auto ret_instr : ret_instrs) {
        // end_block->addPreBasicBlock(ret_instr->getParent());
        // ret_instr->getParent()->addSuccBasicBlock(end_block);
        FUNC_LINE_LOG("delete ret instr ret %s in block %s",
                      ret_instr->getOperand(0)->getPrintName().c_str(),
                      ret_instr->getParent()->getPrintName().c_str());
        ret_instr->getParent()->deleteInstr(ret_instr);
        BranchInst::createBranch(end_block, ret_instr->getParent());
    }
    if (copy_func->getResultType()->isFloatTy() ||
        copy_func->getResultType()->isInt32()) {
        auto ret_phi_instr =
            PhiInstr::createPhi(copy_func->getResultType(), end_block);
        callInstr->replaceAllUse(ret_phi_instr);

        for(auto ret_instr: ret_instrs){
            ret_phi_instr->setParams(ret_instr->getOperand(0), ret_instr->getParent());
            ret_instr->removeUseOps();
        }
    }
    else if(copy_func->getResultType()->isVoidTy()){
        // for(auto ret_instr: ret_instrs){
        //     end_block->addPreBasicBlock(ret_instr->getParent());
        //     ret_instr->getParent()->addSuccBasicBlock(end_block);
        //     ret_instr->removeUseOps();
        //     ret_instr->getParent()->deleteInstr(ret_instr);
        //     BranchInst::createBranch(end_block, ret_instr->getParent());
        // }

    }else {
        ERROR("error func ret type",ERROR_DEFUALT);
    }
    std::list<AllocaInst*> wait_move_allocas;
    for(auto instr:copy_func->getEntryBlock()->getInstructions()){
        auto alloca  = dynamic_cast<AllocaInst*>(instr);
        if(alloca){
            wait_move_allocas.push_back(alloca);
        }
        else {
            break;
        }
    }
    FUNC_LINE_LOG("wait move alloca size is %zu",wait_move_allocas.size());
    for(auto alloca : wait_move_allocas){
        alloca->getParent()->deleteInstr(alloca);
    }
    auto& old_func_list = call_parent_bb->getParentFunc()->getBasicBlocks();
    for(auto bb : copy_func->getBasicBlocks()){
        bb->setParentFunc(call_parent_bb->getParentFunc());
    }
    auto call_it = std::find(old_func_list.begin(),old_func_list.end(),call_parent_bb);
    call_it++;
    old_func_list.splice(call_it, copy_func->getBasicBlocks());
    caller_func->getBasicBlocks().push_back(
        end_block);
    caller_func->setAllocaEnd(nullptr);
    for(auto alloca : wait_move_allocas){
        caller_func->insertAlloca(alloca);
    }
    callInstr->removeUseOps();
#ifdef _FUNC_LINE_LOG

            FUNC_LINE_LOG("emmiting func[%s] inline result ",callInstr->getParent()->getParentFunc()->getName().c_str());
            LLVMIrPrinter fun_printer(callInstr->getParent()->getParentFunc()->getName()+"_inline_"+func_wait_for_inline->getName()+".ir",_m->getModuleName());
            fun_printer.ir_level=Module::MIR_MEM;
            callInstr->getParent()->getParentFunc()->accept(&fun_printer);
#endif // DEBUG
}
