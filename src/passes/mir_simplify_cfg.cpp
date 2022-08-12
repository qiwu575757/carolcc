#include "mir_simplify_cfg.h"

#include "ir/basic_block.h"
#include "ir/function.h"
#include "ir/instruction.h"
#include "module.h"
#include "pass_manager.h"
#include "utils.h"
#include "visitor/llvm_ir_printer.h"
#include <vector>

void MirSimplifyCFG::run() {
    for (auto fun: _m->getFunctions()) {
        if (!fun->isBuiltin()) {
            _func = fun;
            SIMPLIFY_CFG_LOG("cur fun is %s", _func->getName().c_str());
            SIMPLIFY_CFG_LOG("removeNoPredecessorBasicBlocks");
            deleteCondBr(fun);
            #ifdef __MIR_SIMPLIFY_CFG_LOG
            LLVMIrPrinter a(_m->getModuleName()+".1"+_func->getName(),_m->getModuleName());
            fun->accept(&a);
            #endif // DEBUG
            removeNoPredecessorBasicBlocks();
            #ifdef __MIR_SIMPLIFY_CFG_LOG
            LLVMIrPrinter b(_m->getModuleName()+".2"+_func->getName(),_m->getModuleName());
            fun->accept(&b);
            #endif // DEBUG
            eliminateSinglePredecessorPhi();
            // SIMPLIFY_CFG_LOG("mergeSinglePredecessorBasicBlocks");
            mergeSinglePredecessorBasicBlocks();
            eliminateSingleUnCondBrBasicBlocks();
            removeSelfLoopBasicBlocks();
            removeNoPredecessorBasicBlocks();
            eliminateSinglePredecessorPhi();
        }
    }
}

void MirSimplifyCFG::removeSelfLoopBasicBlocks() {
    std::vector<BasicBlock*> wait_delete;
    for (auto bb: _func->getBasicBlocks()) {
        if (bb->getPreBasicBlocks().size() == 1) {
            if (*(bb->getPreBasicBlocks().begin()) == bb) {
                wait_delete.push_back(bb);
            }
        }
    }
    for(auto bb:wait_delete){
        _func->removeBasicBlock(bb);
    }
}

void MirSimplifyCFG::removeNoPredecessorBasicBlocks() {
    std::vector<BasicBlock *> wait_delete;
    for (auto bb: _func->getBasicBlocks()) {
        if (bb->getPreBasicBlocks().empty() && bb != _func->getEntryBlock()) {
            wait_delete.push_back(bb);
            for (auto next_bb: bb->getSuccBasicBlocks()) {
                next_bb->removePreBasicBlock(bb);
                for (auto instr: next_bb->getInstructions()) {
                    if (instr->isPhi()) {
                        for (int i = 1; i < instr->getOperandNumber(); i=i+2) {
                            if (instr->getOperand(i) == bb) {
                                instr->rmOperand(i - 1, i);
                            }
                        }
                    }
                }
            }
        }
    }
    for (auto bb: wait_delete) {
        _func->removeBasicBlock(bb);
    }
}

void MirSimplifyCFG::mergeSinglePredecessorBasicBlocks() {
    std::vector<BasicBlock *> wait_delete;
    for (auto bb: _func->getBasicBlocks()) {
        SIMPLIFY_CFG_LOG("cur basicblock number is %s", bb->getPrintName().c_str());
        if (bb->getPreBasicBlocks().size() == 1) {
            auto pre_bb = *(bb->getPreBasicBlocks().begin());
            SIMPLIFY_CFG_LOG("%s basicblock pre block is %s", bb->getPrintName().c_str(),
                             pre_bb->getPrintName().c_str());
            auto br = pre_bb->getTerminator();
            SIMPLIFY_CFG_LOG("1");
            MyAssert("null terminator", br != nullptr,EXIT_CODE_ERROR_343);
            SIMPLIFY_CFG_LOG("2");
            MyAssert("not branch ", br->isBr(),EXIT_CODE_ERROR_344);
            SIMPLIFY_CFG_LOG("3");
            // 只有这一个后继
            if (br->getOperandNumber() == 1) {
                SIMPLIFY_CFG_LOG("4");
                pre_bb->deleteInstr(br);

                // copy instr
                SIMPLIFY_CFG_LOG("bb has %zu instructions", bb->getInstructions().size());
                for (auto instr: bb->getInstructions()) {
                    SIMPLIFY_CFG_LOG("6");
                    instr->setParent(pre_bb);
                    SIMPLIFY_CFG_LOG("7");
                    pre_bb->addInstr(instr);
                    SIMPLIFY_CFG_LOG("8");
                }

                pre_bb->clearSuccBasicBlockList();
                for (auto next_bb: bb->getSuccBasicBlocks()) {
                    pre_bb->addSuccBasicBlock(next_bb);
                    next_bb->getPreBasicBlocks().remove(bb);
                    next_bb->addPreBasicBlock(pre_bb);
                }
                bb->replaceAllUse(pre_bb);
                wait_delete.push_back(bb);
            }
        }
    }
    for (auto bb: wait_delete) {
        _func->removeBasicBlock(bb);
    }
}

void MirSimplifyCFG::eliminateSingleUnCondBrBasicBlocks() {
    std::vector<BasicBlock *> wait_delete;
    for (auto bb: _func->getBasicBlocks()) {
        if (bb->getNumOfInstr() == 1 && bb != _func->getEntryBlock()) {
            bool bb_delete = true;
            auto terminator_instr = bb->getTerminator();
            if (terminator_instr->isBr()) {
                auto branch_instr = dynamic_cast<BranchInst *>(terminator_instr);
                if (branch_instr->isBranch()) {
                    auto target_bb = branch_instr->getOperand(0);
                    for (auto pre_bb: bb->getPreBasicBlocks()) {
                        auto pre_br_instr = dynamic_cast<BranchInst *>(pre_bb->getTerminator());
                        if (pre_br_instr->isConBranch()) {
                            auto true_bb = pre_br_instr->getOperand(1);
                            auto false_bb = pre_br_instr->getOperand(2);
                            if ((target_bb == true_bb && bb == false_bb)
                                || (target_bb == false_bb && bb == true_bb)) {
                                bb_delete = false;
                                break;
                            }
                        }
//                        else {
//                            ERROR("this should be done at mergeSinglePredecessorBasicBlocks()",EXIT_CODE_ERROR_347);
//                        }
                    }
                    if (bb_delete) {
                        auto succ_bb = bb->getSuccBasicBlocks().begin();
                        for (auto instr: (*succ_bb)->getInstructions()) {
                            if (instr->isPhi()) {
                                for (int i = 0; i < instr->getOperandNumber(); i++) {
                                    if (i % 2 == 1) {
                                        if (instr->getOperand(i) == bb) {
                                            bool is_first = true;
                                            auto val = instr->getOperand(i - 1);
                                            for (auto pre_bb: bb->getPreBasicBlocks()) {
                                                if (is_first) {
                                                    instr->setOperand(i, pre_bb);
                                                    bb->removeUse(instr, i);
                                                    is_first = false;
                                                } else {
                                                    instr->addOperand(val);
                                                    instr->addOperand(pre_bb);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
//                        bb->replaceAllUse(*(bb->getSuccBasicBlocks().begin()));
                        for (auto use: bb->getUseList()) {
                            auto instr = dynamic_cast<Instruction *>(use->_user);
                            MyAssert("error type", instr->isBr(),EXIT_CODE_ERROR_345);
                            instr->setOperand(use->_value_no, *succ_bb);
                        }
                        bb->getUseList().clear();

                        (*succ_bb)->removePreBasicBlock(bb);
                        for (auto pre_bb: bb->getPreBasicBlocks()) {
                            pre_bb->removeSuccBasicBlock(bb);
                            pre_bb->addSuccBasicBlock(*succ_bb);
                            (*succ_bb)->addPreBasicBlock(pre_bb);
                        }
                        wait_delete.push_back(bb);
                    }
                }
            }
        }
    }
    for (auto b: wait_delete) {
        _func->removeBasicBlock(b);
    }
}

void MirSimplifyCFG::eliminateSinglePredecessorPhi() {
    for (auto bb: _func->getBasicBlocks()) {
        if (bb->getPreBasicBlocks().size() == 1 || bb ==
                                                   _func->getEntryBlock()) {
            std::vector<Instruction *> wait_delete;
            for (auto inst: bb->getInstructions()) {
                if (inst->isPhi()) {
                    if(inst->getOperandNumber()!=2){
                    MyAssert("error phi oprd number ", inst->getOperandNumber() == 2,EXIT_CODE_ERROR_346);
                    }
                    MyAssert("error ", inst->getOperand(1) == _func->getEntryBlock() ||
                                       (inst->getOperand(1) == *bb->getPreBasicBlockList().begin()),EXIT_CODE_ERROR_347);
                    wait_delete.push_back(inst);
                    auto var = inst->getOperand(0);

                    inst->replaceAllUse(var);
                    inst->removeUseOps();
                }
            }
            for (auto inst: wait_delete) {
                bb->deleteInstr(inst);
            }
        }
    }
}

void MirSimplifyCFG::deleteCondBr(Function *f) {
    for (auto bb: f->getBasicBlocks()) {
        if (bb->getTerminator()->isBr()) {
            auto br = bb->getTerminator();
            if (dynamic_cast<BranchInst *>(br)->isConBranch()) {
                auto cond = dynamic_cast<ConstantInt *>(br->getOperand(0));
                auto true_bb = br->getOperand(1);
                auto false_bb = br->getOperand(2);
                if (cond) {
                    if (cond->getValue() == 0) {
                        bb->deleteInstr(br);
                        for (auto succ_bb: bb->getSuccBasicBlocks()) {
                            succ_bb->removePreBasicBlock(bb);
                            if (succ_bb != false_bb) {
                                for (auto instr: succ_bb->getInstructions()) {
                                    if (instr->isPhi()) {
                                        for (int i = 0;
                                             i < instr->getOperandNumber();
                                             i++) {
                                            if (i % 2 == 1) {
                                                if (instr->getOperand(i) ==
                                                    bb) {
                                                    instr->rmOperand(i - 1, i);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        bb->getSuccBasicBlocks().clear();
                        BranchInst::createBranch(dynamic_cast<BasicBlock *>(false_bb), bb);

                    } else {
                        bb->deleteInstr(br);
                        for (auto succ_bb: bb->getSuccBasicBlocks()) {
                            succ_bb->removePreBasicBlock(bb);
                            if (succ_bb != true_bb) {
                                for (auto instr: succ_bb->getInstructions()) {
                                    if (instr->isPhi()) {
                                        for (int i = 0;
                                             i < instr->getOperandNumber();
                                             i++) {
                                            if (i % 2 == 1) {
                                                if (instr->getOperand(i) ==
                                                    bb) {
                                                    instr->rmOperand(i - 1, i);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        bb->getSuccBasicBlocks().clear();
                        BranchInst::createBranch(dynamic_cast<BasicBlock *>(true_bb), bb);
                    }
                }
            }
        }
    }
}
