#include "dead_code_elimination.h"

#include <unordered_map>
#include <unordered_set>

#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/function.h"
#include "ir/global_variable.h"
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
            if (!_m->isLowerIR()) {
                deleteRedundantLoad(func);
            }
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
    std::unordered_set<GlobalVariable*> changd_gv;
    for (auto bb : func->getBasicBlocks()) {
        // TODO: 加强到支配性块里
        // only work in a basic block
        std::unordered_set<StoreInst*> pre_stores;
        std::list<Instruction*> wait_delete;
        for (auto instr : bb->getInstructions()) {
            if (instr->isStore()) {
                auto store_instr = dynamic_cast<StoreInst*>(instr);
                auto store_array_ptr =
                    AliasAnalysis::getArrayPtr(store_instr->getPtr());
                if (AliasAnalysis::isGlobal(store_array_ptr)) {
                    auto gv = dynamic_cast<GlobalVariable*>(store_array_ptr);
                    changd_gv.insert(gv);
                }
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
                auto find_store = false;
                for (auto pre_store : pre_stores) {
                    auto store_array_ptr =
                        AliasAnalysis::getArrayPtr(pre_store->getPtr());
                    auto load_array_ptr =
                        AliasAnalysis::getArrayPtr(load_instr->getOperand(0));
                    if (isSamePtr(pre_store, load_instr)) {
                        find_store = true;
                        load_instr->replaceAllUse(pre_store->getRVal());
                        load_instr->removeUseOps();
                        wait_delete.push_back(load_instr);
                        break;
                    } else if (store_array_ptr && load_array_ptr &&
                               load_array_ptr == store_array_ptr) {
                        // 保守策略，load过这个array就算没白存
                        find_store = true;
                        useful_stores.push_back(pre_store);
                    }
                }
                if (!find_store) {
                    if (AliasAnalysis::isGlobal(AliasAnalysis::getArrayPtr(
                            load_instr->getOperand(0)))) {
                        auto gv = dynamic_cast<GlobalVariable*>(
                            AliasAnalysis::getArrayPtr(
                                load_instr->getOperand(0)));
                        if (changd_gv.find(gv) == changd_gv.end()) {
                            if (gv->getType()->isInt32() ||
                                gv->getType()->isFloatTy()) {
                                load_instr->replaceAllUse(gv->getInit());
                                load_instr->removeUseOps();
                                wait_delete.push_back(load_instr);
                            }
                        }
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
                if (call_instr->getFunction()->useGlobalVar()) {
                    changd_gv.insert(call_instr->getFunction()
                                         ->getStoreGlobalVarSet()
                                         .begin(),
                                     call_instr->getFunction()
                                         ->getStoreGlobalVarSet()
                                         .end());
                }
            }
        }
        for (auto instr : wait_delete) {
            bb->deleteInstr(instr);
        }
    }
}

bool DeadCodeElimination::isSamePtr(StoreInst* store, LoadInst* load) {
    if (store->getPtr() != load->getOperand(0)) {
        return false;
    } else if (store->hasOffset() && load->hasOffset()) {
        if (store->getOffset() != load->getOffset()) {
            return false;
        }
        int shift_load = 0, shift_store = 0;
        if (store->hasShift()) {
            shift_store =
                dynamic_cast<ConstantInt*>(store->getShift())->getValue();
        }
        if (load->hasShift()) {
            shift_load =
                dynamic_cast<ConstantInt*>(load->getShift())->getValue();
        }
        return shift_load == shift_store;

    } else if (store->hasOffset() || load->hasOffset()) {
        return false;
    }
    return true;
}

void DeadCodeElimination::deleteRedundantLoad(Function* func) {
    for (auto bb : func->getBasicBlocks()) {
        std::unordered_map<Value*, LoadInst*> ptr2load;
        std::list<Instruction*> wait_delete;
        for (auto instr : bb->getInstructions()) {
            if (instr->isLoad()) {
                auto load = dynamic_cast<LoadInst*>(instr);
                if (ptr2load.find(load->getOperand(0)) != ptr2load.end()) {
                    load->replaceAllUse(ptr2load[load->getOperand(0)]);
                    load->removeUseOps();
                    wait_delete.push_back(instr);
                } else {
                    ptr2load[load->getOperand(0)] = load;
                }
            } else if (instr->isStore()) {
                auto store = dynamic_cast<StoreInst*>(instr);
                std::list<Value*> killed_ptr;
                for (auto load_it : ptr2load) {
                    auto load_ptr = AliasAnalysis::getArrayPtr(load_it.first);
                    auto store_ptr =
                        AliasAnalysis::getArrayPtr(store->getPtr());
                    if (AliasAnalysis::alias(load_ptr, store_ptr)) {
                        killed_ptr.push_back(load_it.first);
                    }
                }
                for (auto val : killed_ptr) {
                    ptr2load.erase(val);
                }
            } else if (instr->isCall()) {
                auto call = dynamic_cast<CallInst*>(instr);
                auto called_func = call->getFunction();
                if (called_func->hasSideEffect()) {
                    for (auto i = 1; i < call->getOperandNumber(); i++) {
                        auto arg = call->getOperand(i);
                        if (arg->getType()->isArrayTy() || arg->getType()->isPointerTy()) {
                            std::list<Value*> killed_ptr;
                            for (auto load_it : ptr2load) {
                                auto load_ptr =
                                    AliasAnalysis::getArrayPtr(load_it.first);
                                auto def_ptr =
                                    AliasAnalysis::getArrayPtr(arg);
                                if (AliasAnalysis::alias(load_ptr, def_ptr)) {
                                    killed_ptr.push_back(load_it.first);
                                }
                            }
                            for (auto val : killed_ptr) {
                                ptr2load.erase(val);
                            }
                        }
                    }
                }
            }
        }
    }
}