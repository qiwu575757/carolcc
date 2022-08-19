#include "loop_invariant.h"

#include <memory>
#include <unordered_set>
#include <vector>

#include "ir/basic_block.h"
#include "ir/function.h"
#include "ir/global_variable.h"
#include "ir/instruction.h"
#include "passes/analysis/alias_analysis.h"
#include "passes/analysis/loop_analysis.h"

void LoopInvariant::run() {
    _loop_analyzer =
        std::make_unique<LoopAnalysis>(_m, "loop_analysis_for_loop_invariant");
    _loop_analyzer->run();

    for (auto loop : _loop_analyzer->getLoops()) {
        LOOP_INVARIANT("loop size is %d", loop->size());
        if (!_loop_analyzer->isOneEntryLoop(loop)) {
            continue;
        }

        do {
            _invariants.clear();
            findInvariants(loop);
#ifdef LOG_LOOP_INVARIANT
            printf("-finding loop in func %s loop(",
                   (*loop->begin())->getParentFunc()->getPrintName().c_str());
            for (auto node : *loop) {
                printf("%s ", node->getPrintName().c_str());
            }
            printf(")\n");
            printf("-invairants is ");
            for (auto var : _invariants) {
                printf("%s ", var->getPrintName().c_str());
            }
            printf("\n");
#endif  // LOG_LOOP_INVARIANT
            handleInvariants(loop);
            loop = _loop_analyzer->getOuterLoop(loop);
#ifdef LOG_LOOP_INVARIANT
            if (loop != nullptr) {
                LOOP_INVARIANT("get an outer loop");
            }

#endif  // LOG_LOOP_INVARIANT
        } while (loop != nullptr);
    }
}
void LoopInvariant::findInvariants(std::unordered_set<BasicBlock *> *loop) {
    std::unordered_set<Value *> values_changed_in_loop;
    // std::unordered_set<Value*>
    for (auto bb : *loop) {
        for (auto instr : bb->getInstructions()) {
            values_changed_in_loop.insert(instr);
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;

        for (auto bb : *loop) {
            for (auto instr : bb->getInstructions()) {
                bool is_invariant = true;
                //  todo:  call 、 ret 都可以进一步处理
                if (instr->isPhi() || instr->isCmp() || instr->isBr() ||
                    instr->isCall() || instr->isRet() || instr->isAlloca() || instr->isStore()) {
                    continue;
                } else if (values_changed_in_loop.find(instr) ==
                           values_changed_in_loop.end()) {
                    continue;
                }
                for (auto val : instr->getOperandList()) {
                    if (values_changed_in_loop.find(val) !=
                        values_changed_in_loop.end()) {
                        is_invariant = false;
                    }
                }
                if (is_invariant && instr->isLoad()) {
                    auto load = dynamic_cast<LoadInst *>(instr);
                    auto load_ptr =
                        AliasAnalysis::getArrayPtr(load->getOperand(0));
                    for (auto val2 : values_changed_in_loop) {
                        auto store_instr = dynamic_cast<StoreInst *>(val2);
                        // todo: 这里太保守了，
                        if (store_instr) {
                            auto store_ptr = AliasAnalysis::getArrayPtr(
                                store_instr->getPtr());
                            if (store_ptr == load_ptr) {
                                is_invariant = false;
                                break;
                            }
                        }
                        auto call_instr = dynamic_cast<CallInst *>(val2);
                        if (call_instr) {
                            // 保守策略，调用过有影响的函数就算store有用
                            if (call_instr->getFunction()->hasSideEffect()) {
                                is_invariant = false;
                                break;
                            }
                        }
                    }
                }

                if (is_invariant) {
                    values_changed_in_loop.erase(instr);
                    _invariants.push_back(instr);
                    changed = true;
                }
            }
        }
    }
}
void LoopInvariant::handleInvariants(std::unordered_set<BasicBlock *> *loop) {
    if (_invariants.size() == 0) {
        return;
    }
    auto loop_cond = _loop_analyzer->getLoopHeader(loop);

    auto new_bb = BasicBlock::create("");
    new_bb->setParentFunc(loop_cond->getParentFunc());
    for (auto instr : _invariants) {
        instr->getParent()->deleteInstr(instr);
        new_bb->addInstr(instr);
    }

    // pred succ relation
    BranchInst::createBranch(loop_cond, new_bb);

    std::vector<Value *> labels;
    std::vector<Value *> vals;
    std::vector<Value *> in_loop_labels;
    std::vector<Value *> in_loop_vals;
    for (auto instr : loop_cond->getInstructions()) {
        if (!instr->isPhi()) {
            break;
        }
        auto phi = dynamic_cast<PhiInstr *>(instr);
        labels.clear();
        vals.clear();
        in_loop_labels.clear();
        in_loop_vals.clear();
        int phi_oprd_index = 0;
        for (; phi_oprd_index < phi->getOperandNumber(); phi_oprd_index += 2) {
            auto pre_val = phi->getOperand(phi_oprd_index);
            auto pre_bb =
                dynamic_cast<BasicBlock *>(phi->getOperand(phi_oprd_index + 1));
            // 如果是来自循环内部的phi
            if (loop->find(pre_bb) != loop->end()) {
                in_loop_labels.push_back(pre_bb);
                in_loop_vals.push_back(pre_val);
            } else {
                labels.push_back(pre_bb);
                vals.push_back(pre_val);
            }
        }
        if (labels.size() == 0) {
            continue;
        }
        if (labels.size() == 1) {
            for (auto i = 0; i < instr->getOperandNumber(); i = i + 2) {
                if (vals[0] == instr->getOperand(i)) {
                    instr->setOperand(i + 1, new_bb);
                    labels[0]->removeUse(instr, i + 1);
                }
            }
            continue;
        }
        auto new_phi = PhiInstr::createPhi(vals[0]->getType(), new_bb);
        for (auto i = 0; i < labels.size(); i++) {
            new_phi->setParams(vals[i], labels[i]);
        }
        instr->clearOperands();
        for (auto i = 0; i < in_loop_labels.size(); i++) {
            instr->addOperand(in_loop_vals[i]);
            instr->addOperand(in_loop_labels[i]);
        }
        instr->addOperand(new_phi);
        instr->addOperand(new_bb);
    }
    std::vector<BasicBlock *> wait_move_bbs;
    for (auto pre_bb : loop_cond->getPreBasicBlockList()) {
        if (loop->find(pre_bb) != loop->end() || pre_bb == new_bb) {
            continue;
        }
        wait_move_bbs.push_back(pre_bb);
        pre_bb->removeSuccBasicBlock(loop_cond);
        auto term_instr = pre_bb->getTerminator();
        for (auto i = 0; i < term_instr->getOperandNumber(); i++) {
            if (term_instr->getOperand(i) == loop_cond) {
                term_instr->getOperand(i)->removeUse(term_instr, i);
                term_instr->setOperand(i, new_bb);
            }
        }
        pre_bb->addSuccBasicBlock(new_bb);
        new_bb->addPreBasicBlock(pre_bb);
    }
    for (auto bb : wait_move_bbs) {
        loop_cond->removePreBasicBlock(bb);
    }
    auto &bb_list = loop_cond->getParentFunc()->getBasicBlocks();
    auto it = bb_list.begin();
    for (; it != bb_list.end(); it++) {
        if (*it == loop_cond) {
            bb_list.insert(it, new_bb);
            break;
        }
    }
}