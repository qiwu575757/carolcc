#include "lower_ir.h"
#include <list>

#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "module.h"

void LowerIR::run() {
    for (auto func : _m->getFunctions()) {
        rmPhi(func);
        // for (auto bb : func->getBasicBlocks()) {
        //     splitGEP(bb);
        // }
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
            ERROR("GEP have more than 2 operands",EXIT_CODE_ERROR_432);
        } else {
            iter++;
        }
    }
}
void LowerIR::rmPhi(Function *f){
    std::list<PhiInstr*>phis;
    for(auto bb : f->getBasicBlocks()){
        for(auto instr: bb->getInstructions())  {
            auto phi = dynamic_cast<PhiInstr*>(instr);
            if(phi){
                phis.push_back(phi);
                unsigned i = 0;
                for(;i<phi->getOperandNumber();i=i+2){
                    auto pre_val  =phi->getOperand(i);
                    auto pre_bb  = dynamic_cast<BasicBlock*>(phi->getOperand(i+1)) ;
                    MovInstr::createMov(phi, pre_val, pre_bb);
                }

            }else {
                break;
            }
        }
    }
    for(auto phi:phis){
        phi->getParent()->deleteInstr(phi);
    }
}