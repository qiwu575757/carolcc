#include "rmphi.h"
#include <list>

#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "module.h"
#include "../utils.h"

void RmPhi::run() {
    for (auto func : _m->getFunctions()) {
        rmPhi(func);
    }

}

void RmPhi::rmPhi(Function *f){
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