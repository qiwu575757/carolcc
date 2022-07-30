#include "naming_value.h"
#include "module.h"
#include "ir/instruction.h"
#include "ir/basic_block.h"
#include "utils.h"

void NamingValue::NameValue(Value *val) {
    if (seq.find(val) == seq.end() ) {
      if(!val->getName().empty()){
        WARNNING("renaming %s",val->getName().c_str());
      }
        auto seq_num = seq.size();
        val->setName(std::to_string(seq_num));
        seq.insert({val, seq_num});
    }
}
void NamingValue::NameBasicBlock(BasicBlock *base_block) {
    NameValue(base_block);
    if (base_block->isBaiscBlock()) {
        auto basic_block = dynamic_cast<BasicBlock *>(base_block);
        INFO("HIR printer : basic block has %d instructions", basic_block->getNumOfInstr());
        for (auto &instr: basic_block->getInstructions()) {
            NameInstr(instr);
        }
    } else {
        ERROR("error base block type",EXIT_CODE_ERROR_349);
    }
}
void NamingValue::NameInstr(Instruction *instr) {
    if (seq.find(instr) == seq.end()  && !instr->getType()->isVoidTy()) {
      if(!instr->getName().empty()){
        WARNNING("renaming instr %s",instr->getName().c_str());
      }
        auto seq_num = seq.size();
        instr->setName(std::to_string(seq_num));
        seq.insert({instr, seq_num});
    }
}
void NamingValue::run() {
    for (auto f : _m->getFunctions()) {
        for (auto arg : f->getArgs()) {
            NameValue(arg);
        }
        MyAssert("this pass should work on mir", f->getBaseBlocks().empty(),EXIT_CODE_ERROR_348);
        for(auto bb :f->getBasicBlocks()){
            NameBasicBlock(bb);
        }
    }
}
