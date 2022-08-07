#include "loop_invariant_exp.h"

void LoopInvariantExp::run(){
    CfgAnalyser = std::make_unique<CfgAnalysis>(_m,"");
    CfgAnalyser->run();

    auto loops = CfgAnalyser->getLoops();
    for (auto loop : *loops){
        bool inner = true;
        for (auto bb : *loop){//具体干什么不详
            if (CfgAnalyser->findBbLoop(bb) != loop){
                inner = false;
                break;
            }
        }
        if (inner == false){
            continue;
        }

        while (loop != nullptr){
            invariants.clear();
            invariantsFind(loop);
            if (!invariants.empty()){
                auto first_BB = CfgAnalyser->findLoopEntry(loop);
                BasicBlock* new_BB = BasicBlock::create("");

                //find pre_BBs
                std::vector<BasicBlock*> pre_BBs;
                for (auto pre_BB : first_BB->getPreBasicBlocks()){
                    if (CfgAnalyser->findBbLoop(pre_BB) != loop){
                        pre_BBs.push_back(pre_BB);
                    }
                }

                //phi in loop entry
                for (auto iter_inst = first_BB->getInstructions().begin(); iter_inst != first_BB->getInstructions().end(); iter_inst++){
                    auto inst = *iter_inst;
                    if (inst->isPhi() == false){
                        break;
                    }
                    auto inst_phi = dynamic_cast<PhiInstr *>(inst);
                    std::vector<std::pair<Value*,BasicBlock*>> ops_in_loop;
                    std::vector<std::pair<Value*,BasicBlock*>> ops_out_loop;
                    for (auto i = 0; i < inst->getOperandNumber(); i=i+2){
                        auto val = inst->getOperand(i);
                        auto from_BB = dynamic_cast<BasicBlock*>(inst->getOperand(i+1));
                        val->removeUse(inst,i);
                        from_BB->removeUse(inst,i);

                        if (CfgAnalyser->findBbLoop(from_BB) == loop){
                            ops_in_loop.push_back({val,from_BB});
                        }
                        else{
                            ops_out_loop.push_back({val,from_BB});
                        }
                    }

                    if (ops_out_loop.size()>1){
                        //more than 1 value out of loop
                        //need phi in new_BB
                        auto new_phi = PhiInstr::createPhi((*ops_out_loop.begin()).first->getType(),new_BB);
                        new_BB->addInstr(new_phi);
                        for (auto pair : ops_out_loop){
                            new_phi->setParams(pair.first,pair.second);
                        }
                        
                        inst_phi->rmOperand(0, inst->getOperandNumber()-1);
                        for (auto pair : ops_in_loop){
                            inst_phi->setParams(pair.first,pair.second);
                        }
                        inst_phi->setParams(new_phi, new_BB);
                    }
                    else{
                        //not need a new phi
                        inst_phi->rmOperand(0, inst->getOperandNumber()-1);
                        for (auto pair : ops_out_loop){
                            inst_phi->setParams(pair.first,new_BB);
                        }
                        for (auto pair : ops_in_loop){
                            inst_phi->setParams(pair.first,pair.second);
                        }
                    }
                }

                //invariant move
                for (auto pair : invariants){
                    for (auto inst : pair.second){
                        pair.first->getInstructions().remove(inst);
                        new_BB->addInstr(inst);
                        inst->setParent(new_BB);
                    }
                }
                //bb br change
                BranchInst::createBranch(first_BB, new_BB);
                for (auto pre_BB : pre_BBs){
                    auto terminator = pre_BB->getTerminator();
                    if (terminator->getOperandNumber() == 1){
                        terminator->setOperand(0, new_BB);
                    }
                    else{
                        if (dynamic_cast<BasicBlock*>(terminator->getOperand(1)) == first_BB){
                            terminator->setOperand(1, new_BB);
                        }
                        else{
                            terminator->setOperand(2, new_BB);
                        }
                    }
                    for (int i = 0; i < terminator->getOperandNumber(); i++) {
                        first_BB->removeUse(terminator,i);
                    }
                    first_BB->removePreBasicBlock(pre_BB);
                    pre_BB->removeSuccBasicBlock(first_BB);
                    new_BB->addPreBasicBlock(pre_BB);
                    pre_BB->addSuccBasicBlock(new_BB);
                }
            }
            loop = CfgAnalyser->findOuterLoop(loop);
        }
    }
}
void LoopInvariantExp::invariantsFind(std::vector<BasicBlock *>* loop){
    std::set<Value *> defined_in_loop;
    std::set<Instruction *> invariant;
    for (auto bb : *loop){
        for (auto inst : bb->getInstructions()){
            if (inst->isStore()){
                defined_in_loop.insert(inst->getOperand(1));
            }
            else{
                defined_in_loop.insert(inst);
            }
        }
    }
    bool change = false;
    do {
        change = false;
        for (auto bb : *loop){
            invariant.clear();
            for (auto inst : bb->getInstructions()){
                bool invariant_check = true;
                if (inst->isCall()||inst->isAlloca()||inst->isRet()||inst->isBr()||inst->isCmp()||inst->isPhi()||inst->isLoad()){
                    continue;
                }
                if (defined_in_loop.find(inst) == defined_in_loop.end()){
                    continue;
                }
                for (auto operand : inst->getOperandList()){
                    if (defined_in_loop.find(operand) != defined_in_loop.end()){
                        invariant_check = false;
                    }
                }
                if (invariant_check){
                    defined_in_loop.erase(inst);
                    invariant.insert(inst);
                    change = true;
                }
            }
            if (!invariant.empty()){
                invariants.push_back({bb,invariant});
            }
        }
    }while (change);
}