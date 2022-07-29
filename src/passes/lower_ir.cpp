#include "lower_ir.h"

#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "module.h"

void LowerIR::run() {
    for (auto func : _m->getFunctions()) {
        for (auto bb : func->getBasicBlocks()) {
            splitGEP(bb);
        }
    }

    _m->setIRLevel(Module::LIR);
}

void LowerIR::splitGEP(BasicBlock *bb) {
    auto &insts = bb->getInstructions();
    for (auto iter = insts.begin(); iter != insts.end(); ) {
        auto inst = *iter;
        if (inst->isGep() && inst->getOperandNumber() == 3) {
            auto size = inst->getType()->getPointerElementType()->getSize();
            auto offset = BinaryInst::createMul(inst->getOperand(1),ConstantInt::get(size),bb);
            auto addaddr = BinaryInst::createAdd(inst->getOperand(0),offset,bb);
            insts.insert(iter, offset);
            insts.insert(iter, addaddr);
            inst->replaceAllUse(addaddr);
            inst->removeUseOps();
            iter = insts.erase(iter);
        } else if (inst->isGep()) {
            ERROR("GEP have more than 2 operands");
        } else {
            iter++;
        }
    }
}