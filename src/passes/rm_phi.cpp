#include "rm_phi.h"
#include <list>
#include <map>
#include <utility>

#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "module.h"
#include "../utils.h"

void RmPhi::run() {
    for (auto func : _m->getFunctions()) {
        // rmPhiCycle(func);
        splitCriticalEdge(func);
        // rmPhi(func);
    }

}

void RmPhi::rmPhi(Function *func){
    std::list<PhiInstr*>phis;
    for(auto bb : func->getBasicBlocks()){
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
void RmPhi::splitCriticalEdge(Function* func) {
    std::list<std::pair<BasicBlock*, BasicBlock*>> insert_map;
    std::list<PhiInstr*> wait_delete;
    for(auto bb :   func->getBasicBlocks()){
        std::map<BasicBlock*, BasicBlock*> bb_map;
      for(auto instr : bb->getInstructions()){
        if(instr->isPhi()){
            auto phi = dynamic_cast<PhiInstr*>(instr);
            wait_delete.push_back(phi);
            for(auto i = 0;i<phi->getOperandNumber();i=i+2){
                auto pre_val = phi->getOperand(i);
                auto pre_bb =  dynamic_cast<BasicBlock*>(phi->getOperand(i+1)) ;
                if(bb_map.find(pre_bb) !=bb_map.end()){
                    pre_bb = bb_map[pre_bb];
                }
                if(bb->getPreBasicBlockList().size() > 1 && pre_bb->getSuccBasicBlockList().size() > 1){
                    auto new_bb =  BasicBlock::create("");
                    insert_map.push_back(std::make_pair(pre_bb, new_bb));
                    bb->getPreBasicBlockList().remove(pre_bb);
                    pre_bb->getSuccBasicBlockList().remove(bb);
                    auto pre_bb_br_instr = dynamic_cast<BranchInst*>(pre_bb->getTerminator()) ;
                    for(auto br_instr_index =0;br_instr_index< pre_bb_br_instr->getOperandNumber() ; br_instr_index ++){
                        if(pre_bb_br_instr->getOperand(br_instr_index) == bb){
                            pre_bb_br_instr->changeOperand(br_instr_index, new_bb);
                            pre_bb->addSuccBasicBlock(new_bb);
                            new_bb->addPreBasicBlock(pre_bb);
                            break;
                        }
                    }
                    BranchInst::createBranch(bb, new_bb);

                    auto instr_mov = MovInstr::createMov(phi,pre_val, new_bb);
                    phi->changeOperand(i, instr_mov);
                    phi->changeOperand(i+1, new_bb);
                    bb_map.insert({pre_bb, new_bb});
                }
                else if(bb->getPreBasicBlockList().size() > 1 && pre_bb->getSuccBasicBlockList().size() == 1){
                    auto instr_mov = MovInstr::createMov(phi,pre_val, pre_bb);
                    phi->changeOperand(i, instr_mov);
                }
            }
        }
      }
    }
    for(auto ppair: insert_map){
        auto key_bb = ppair.first;
        for(auto bb  = func->getBasicBlocks().begin();bb !=func->getBasicBlocks().end();bb ++){
            if (*bb == key_bb) {
                bb ++ ;
                func->getBasicBlocks().insert(bb, ppair.second);
                ppair.second->setParentFunc(func);
                break;
            }
        }
    }
    for(auto phi : wait_delete){
        phi->getParent()->deleteInstr(phi);
    }
}

void RmPhi::rmPhiCycle(Function *func){
    // std::list<PhiInstr*>phis;
    // for(auto bb : func->getBasicBlocks()){
    //     for(auto instr: bb->getInstructions())  {
    //         auto phi = dynamic_cast<PhiInstr*>(instr);
    //         if(phi){
    //             phis.push_back(phi);
    //             unsigned i = 0;
    //             for(;i<phi->getOperandNumber();i=i+2){
    //                 auto pre_val  =phi->getOperand(i);
    //                 auto pre_bb  = dynamic_cast<BasicBlock*>(phi->getOperand(i+1)) ;
    //                 MovInstr::createMov(phi, pre_val, pre_bb);
    //             }

    //         }else {
    //             break;
    //         }
    //     }
    // }
    // for(auto phi:phis){
    //     phi->getParent()->deleteInstr(phi);
    // }
}