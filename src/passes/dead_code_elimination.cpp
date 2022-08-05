#include "dead_code_elimination.h"

#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/function.h"
#include "ir/instruction.h"
#include "passes/analysis/alias_analysis.h"
#include "passes/module.h"
#include "utils.h"

void DeadCodeElimination::run() {
    deleteDeadFunc();
    for (auto func : _m->getFunctions()) {
        if (func != _m->getMainFunction() && !func->isBuiltin()) {
            deleteDeadRet(func);
        }
    }
    for (auto func : _m->getFunctions()) {
        if (!func->isBuiltin()) {
            deleteDeadInstr(func);
            deleteDeadStore(func);
            deleteDeadInstr(func);
        }
    }
}
void DeadCodeElimination::deleteDeadFunc() {
    std::vector<Function*> wait_delete;
    for (auto func : _m->getFunctions()) {
        if (func != _m->getMainFunction()) {
            if (func->getUseList().empty() || func->getCallerSet().empty()) {
                wait_delete.push_back(func);
            }
        }
    }
    for (auto func : wait_delete) {
        _m->rmFunction(func);
    }
}

void DeadCodeElimination::deleteDeadRet(Function* func) {
    bool no_use = true;
    for (auto use : func->getUseList()) {
        if (!use->_user->getUseList().empty()) {
            no_use = false;
        }
    }
    if (no_use) {
        for (auto bb : func->getBasicBlocks()) {
            auto ret_instr = dynamic_cast<ReturnInst*>(bb->getTerminator());
            if (ret_instr && ret_instr->getOperandNumber() == 1) {
                Constant* new_return_val = nullptr;
                if (ret_instr->getOperand(0)->getType()->isFloatTy()) {
                    new_return_val = ConstantFloat::create(0);
                } else if (ret_instr->getOperand(0)->getType()->isInt32()) {
                    new_return_val = ConstantInt::create(0);
                }
                ret_instr->rmOperand(0);
                ret_instr->addOperand(new_return_val);
            }
        }
    }
}
void DeadCodeElimination::deleteDeadInstr(Function* func) {
    std::unordered_set<Instruction*> useful_inst_list;
    for (auto bb : func->getBasicBlocks()) {
        for (auto instr : bb->getInstructions()) {
            if (hasSideEffect(instr)) {
                markInstrUseful(instr, useful_inst_list);
            }
        }
    }
    for (auto bb : func->getBasicBlocks()) {
        std::unordered_set<Instruction*> useless_instrs;
        for (auto instr : bb->getInstructions()) {
            if (useful_inst_list.find(instr) == useful_inst_list.end()) {
                useless_instrs.insert(instr);
            } else {
                auto useful = false;
                if (instr->getUseList().empty()) {
                    if (instr->isRet() || instr->isCall() || instr->isBr() ||
                        instr->isStore()) {
                        useful = true;
                    } else {
                        useful = false;
                    }
                } else {
                    useful = true;
                }
                MyAssert("should delete this useless instr!", useful,
                         deleteDeadCodeError);
            }
        }
        for (auto instr : useless_instrs) {
            instr->removeUseOps();
            // instr->replaceAllUse(nullptr);
            bb->deleteInstr(instr);
        }
    }
}

bool DeadCodeElimination::hasSideEffect(Instruction* instr) {
    if (instr->isRet() || instr->isBr() || instr->isStore()) {
        return true;
    } else if (instr->isCall()) {
        auto call_instr = dynamic_cast<CallInst*>(instr);
        auto fun = call_instr->getFunction();
        return fun->hasSideEffect();
    } else {
        return false;
    }
}
void DeadCodeElimination::markInstrUseful(
    Instruction* instr, std::unordered_set<Instruction*>& work_list) {
    if (work_list.find(instr) != work_list.end()) {
        return;
    }
    work_list.insert(instr);
    for (auto oprd : instr->getOperandList()) {
        auto op_instr = dynamic_cast<Instruction*>(oprd);
        if (op_instr) {
            markInstrUseful(op_instr, work_list);
        }
    }
}
void DeadCodeElimination::deleteDeadStore(Function* func) {
    for (auto bb : func->getBasicBlocks()) {
        // TODO: 加强到支配性块里
        // only work in a basic block
        std::unordered_set<StoreInst*> pre_stores;
        std::list<Instruction*> wait_delete;
        for (auto instr : bb->getInstructions()) {
            if (instr->isStore()) {
                auto store_instr = dynamic_cast<StoreInst*>(instr);
                StoreInst* pre_useless_store = nullptr;
                for (auto pre_store : pre_stores) {
                    if (pre_store->getPtr() == store_instr->getPtr()) {
                        pre_useless_store = pre_store;
                        wait_delete.push_back(pre_useless_store);
                    }
                }
                if (pre_useless_store) {
                    pre_stores.erase(pre_useless_store);
                }
                pre_stores.insert(store_instr);
            } else if (instr->isLoad()) {
                auto load_instr = dynamic_cast<LoadInst*>(instr);
                std::list<StoreInst*> useful_stores;
                for (auto pre_store : pre_stores) {
                    auto store_array_ptr =
                        AliasAnalysis::getArrayPtr(pre_store->getPtr());
                    auto load_array_ptr =
                        AliasAnalysis::getArrayPtr(load_instr->getOperand(0));
                    if (pre_store->getPtr() == load_instr->getOperand(0)) {
                        load_instr->replaceAllUse(pre_store->getRVal());
                        load_instr->removeUseOps();
                        wait_delete.push_back(load_instr);
                        break;
                    } else if (store_array_ptr && load_array_ptr &&
                               load_array_ptr == store_array_ptr) {
                        // 保守策略，load过这个array就算没白存
                        useful_stores.push_back(pre_store);
                    }
                }
                for (auto instr : useful_stores) {
                    pre_stores.erase(instr);
                }
            } else if (instr->isCall()) {
                auto call_instr = dynamic_cast<CallInst*>(instr);
                // 保守策略，调用过有影响的函数就算store有用
                if (call_instr->getFunction()->hasSideEffect()) {
                    pre_stores.clear();
                }
            }
        }
        for (auto instr : wait_delete) {
            bb->deleteInstr(instr);
        }
    }
}
