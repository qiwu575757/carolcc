#include "ir/function.h"
#include "hir_to_mir.h"
#include "ir/base_block.h"
#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "module.h"

#define CONST_INT(num) ConstantInt::create(num)
#define CONST_FLOAT(num) ConstantFloat::create(num)

void HIRToMIR::run() {
    for (auto func : _m->getFunctions()) {
        auto base_bbs = func->getBaseBlocks();
        BasicBlock *next_bb = nullptr;
        for (auto b = base_bbs.rbegin(); b != base_bbs.rend(); b++) {
            auto base_bb = *b;
            next_bb = genBasicBlock(base_bb, next_bb, nullptr, nullptr, func);
        }
        // mir 不在需要 baseblocks
        func->getBaseBlocks().clear();
        // WARNNING("func BasicBlocks num:%d",func->getNumBasicBlocks());
    }

    _m->setIRLevel(Module::MIR_MEM);
}

BasicBlock *HIRToMIR::genBasicBlock(BaseBlock *base_bb, BasicBlock *next_bb,
                           BasicBlock *while_entry, BasicBlock *while_exit,
                           Function *func) {
    auto &basic_bbs = func->getBasicBlocks();

    if (base_bb->isBaiscBlock()) {
        BasicBlock *this_bb = dynamic_cast<BasicBlock *>(base_bb);
        if (next_bb == nullptr && this_bb->getTerminator()==nullptr && basic_bbs.empty()) {
            if (func->getResultType()->isInt32()) {
                 ReturnInst::createRet(CONST_INT(0), this_bb);
            } else if (func->getResultType()->isFloatTy()) {
                 ReturnInst::createRet(CONST_FLOAT(0.0), this_bb);
            } else {
                 ReturnInst::createVoidRet(this_bb);
            }
        } else if (this_bb->getTerminator() == nullptr) {
            auto inst = this_bb->getInstructions().back();
            BasicBlock *target = next_bb;
            if (inst != nullptr) {
                if (inst->isBreak()) {
                    target = while_exit;
                    this_bb->deleteInstr(inst);
                } else if (inst->isContinue()) {
                    target = while_entry;
                    this_bb->deleteInstr(inst);
                }
            }
             BranchInst::createBranch(target, this_bb);
        }
        basic_bbs.push_front(this_bb);
        return this_bb;
    } else if (base_bb->isIfBlock()) {
        auto if_block = dynamic_cast<IfBlock *>(base_bb);
        BasicBlock *if_true_block = nullptr;
        BasicBlock *if_false_block = nullptr;
        auto if_cond = if_block->getCondBaseBlockList();
        auto then_body = if_block->getIfBodyBaseBlockList();
        auto else_body = if_block->getElseBodyBaseBlockList();

        if (next_bb == nullptr) {//代表函数结尾，添加ret指令
            auto bb = BasicBlock::create("");
            // 维护父子关系
            bb->setFunction(func);
            func->addBasicBlock(bb);
            if (func->getResultType()->isInt32()) {
                 ReturnInst::createRet(CONST_INT(0), bb);
            } else if (func->getResultType()->isFloatTy()) {
                 ReturnInst::createRet(CONST_FLOAT(0.0), bb);
            } else {
                 ReturnInst::createVoidRet(bb);
            }
            next_bb = bb;
        }

        // 在 then body 和 else body 内部递归生成 mir
        BasicBlock *cur_next = next_bb;
        for (auto iter = (*then_body).rbegin(); iter != (*then_body).rend(); iter++) {
            cur_next = genBasicBlock(*iter, cur_next, while_entry, while_exit, func);
        }
        if_true_block = cur_next;

        cur_next = next_bb;
        for (auto iter = (*else_body).rbegin(); iter != (*else_body).rend(); iter++) {
            cur_next = genBasicBlock(*iter, cur_next, while_entry, while_exit, func);
        }
        if_false_block = cur_next;

        // if_cond 中的list只有一个baseblock
        auto cond_bb = dynamic_cast<BasicBlock *>((*if_cond).back());
        auto cond = cond_bb->getInstructions().back();
         BranchInst::createCondBr(cond, if_true_block, if_false_block, cond_bb);
        basic_bbs.push_front(cond_bb);
        if(if_cond->size()==1){
            return cond_bb;
        }
        else if(if_cond->size()>1) {
            auto iter =  if_cond->rbegin() ;
            for(auto &i : *if_cond){
                MyAssert("error type", i->isBaiscBlock());
            }
            cur_next = cond_bb;
            iter ++;
            for(;iter!=if_cond->rend();iter++){
                cur_next = genBasicBlock(*iter, cur_next , while_entry, while_exit, func);
            }
            return cur_next;
        }
        // cond_bb 的最后一条指令存储了该cond 的运行结果
    } else if (base_bb->isWhileBlock()) {
        auto while_block = dynamic_cast<WhileBlock *>(base_bb);
        auto while_cond = while_block->getCondBaseBlockList();
        auto while_body = while_block->getBodyBaseBlockList();

        if (next_bb == nullptr) {
            auto bb = BasicBlock::create("");
            bb->setFunction(func);
            func->addBasicBlock(bb);
            if (func->getResultType()->isInt32()) {
                 ReturnInst::createRet(CONST_INT(0), bb);
            } else if (func->getResultType()->isFloatTy()) {
                 ReturnInst::createRet(CONST_FLOAT(0.0), bb);
            } else {
                 ReturnInst::createVoidRet(bb);
            }
            next_bb = bb;
        }

        BasicBlock *if_true_block = nullptr;
        BasicBlock *if_false_block = next_bb;
        auto first_cond_bb = dynamic_cast<BasicBlock *>((*while_cond).front());
        BasicBlock *cur_next = first_cond_bb;
        for (auto iter = (*while_body).rbegin(); iter != (*while_body).rend(); iter++) {
            cur_next = genBasicBlock(*iter, cur_next, first_cond_bb, next_bb, func);
        }
        if_true_block = cur_next;

        auto last_cond_bb = dynamic_cast<BasicBlock *>((*while_cond).back());
        auto cond = last_cond_bb->getInstructions().back();
         BranchInst::createCondBr(cond, if_true_block, if_false_block, last_cond_bb);
        basic_bbs.push_front(last_cond_bb);
        if(while_cond->size()==1){
            return last_cond_bb;
        }
        else if(while_cond->size()>1) {
            auto iter =  while_cond->rbegin() ;
            for(auto &i : *while_cond){
                MyAssert("error type", i->isBaiscBlock());
            }
            cur_next = last_cond_bb;
            iter ++;
            for(;iter!=while_cond->rend();iter++){
                cur_next = genBasicBlock(*iter, cur_next , while_entry, while_exit, func);
            }
            return cur_next;
        }
    } else {
        ERROR("unknown baseblock");
    }

    return nullptr;
}