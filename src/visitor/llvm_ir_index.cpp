#include "llvm_ir_index.h"
#include "ir/function.h"
#include "ir/instruction.h"
#include "utils.h"
#include <unordered_map>
#include "passes/module.h"

void LLVMIrIndexer::NameValue(Value *val) {
    if (seq.find(val) == seq.end() ) {
      if(!val->getName().empty()){
        WARNNING("renaming %s",val->getName().c_str());
      }
        auto seq_num = seq.size();
        val->setName(std::to_string(seq_num));
        seq.insert({val, seq_num});
    }
}
void LLVMIrIndexer::NameBaseBlock(BaseBlock *base_block) {
    NameValue(base_block);
    if (base_block->isBaiscBlock()) {
        auto basic_block = dynamic_cast<BasicBlock *>(base_block);
        INFO("HIR printer : basic block has %d instructions", basic_block->getNumOfInstr());
        for (auto &instr: basic_block->getInstructions()) {
            NameInstr(instr);
        }
    } else if (base_block->isIfBlock()) {
        auto if_block = dynamic_cast<IfBlock *>(base_block);
        for (auto &cond_base_block: *(if_block->getCondBaseBlockList())) {
            NameBaseBlock(cond_base_block);
        }
        for (auto &then_block: *(if_block->getIfBodyBaseBlockList())) {
            NameBaseBlock(then_block);
        }
        for (auto &else_block: *(if_block->getElseBodyBaseBlockList())) {
            NameBaseBlock(else_block);
        }
    } else if (base_block->isWhileBlock()) {
        auto while_block = dynamic_cast<WhileBlock *>(base_block);
        for (auto &cond_block: *(while_block->getCondBaseBlockList())) {
            NameBaseBlock(cond_block);
        }
        for (auto &body_block: *(while_block->getBodyBaseBlockList())) {
            NameBaseBlock(body_block);
        }
    } else {
        ERROR("error base block type",EXIT_CODE_ERROR_435);
    }
}
void LLVMIrIndexer::NameInstr(Instruction *instr) {
    auto mov_instr = dynamic_cast<MovInstr*>(instr);
    if(!mov_instr){
        if (seq.find(instr) == seq.end() && !instr->getType()->isVoidTy()) {
            if (!instr->getName().empty()) {
                WARNNING("renaming instr %s", instr->getName().c_str());
            }
            auto seq_num = seq.size();
            instr->setName(std::to_string(seq_num));
            seq.insert({instr, seq_num});
        }
    }
    else {
        auto phi = dynamic_cast<PhiInstr*>(mov_instr->getLVal());
        if(phi_pool.count(phi)==0){
            NameValue(phi);
            phi_pool.insert(phi);
        }
    }
}
void LLVMIrIndexer::visit(Function *node) {
    phi_pool.clear();
    if(this->ir_level == Module::IRLevel::HIR){
        seq.clear();
        depth = 0;

        // 先给变量编号
        INFO("naming args");
        for (auto &arg: node->getArgs()) {
            NameValue(arg);
        }
        INFO("naming base blocks");
        for (auto &bb: node->getBaseBlocks()) {
            NameBaseBlock(bb);
        }

    }
    else if(this->ir_level == Module::IRLevel::MIR_MEM){
        seq.clear();
        depth = 0;

        // 先给变量编号
        INFO("naming args");
        for (auto &arg: node->getArgs()) {
            NameValue(arg);
        }
        INFO("naming base blocks");
        for (auto &bb: node->getBasicBlocks()) {
            NameBaseBlock(bb);
        }
    }

}
