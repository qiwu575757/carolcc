#include "global_value_numbering.h"

#include <cstdlib>
#include <queue>
#include <set>
#include <vector>

#include "dominators.h"
#include "ir/constant.h"
#include "ir/function.h"
#include "ir/instruction.h"
#include "module.h"
#include "naming_value.h"
#include "utils.h"

void GlobalVariableNumbering::run() {
    check();

    // rename for GVN
    // NamingValue name_pass(_m,"NamingValue");
    // name_pass.run();
    _dom_analysis.run();

    for (auto f : _m->getFunctions()) {
        if (!f->isBuiltin()) {
            std::queue<BasicBlock*> work_list;
            std::set<BasicBlock*> is_visited;
            work_list.push(f->getEntryBlock());
            while (!work_list.empty()) {
                auto bb = work_list.front();
                work_list.pop();
                SVN(bb, work_list, is_visited);
            }
        }
    }
}
void GlobalVariableNumbering::check() {
    for (auto f : _m->getFunctions()) {
        for (auto bb : f->getBasicBlocks()) {
            if (bb != f->getEntryBlock() && bb->getPreBasicBlocks().empty()) {
                ERROR("there should not have dead blocks",EXIT_CODE_ERROR_430);
            }
        }
    }
}
void GlobalVariableNumbering::SVN(BasicBlock* bb,
                                  std::queue<BasicBlock*>& work_list,
                                  std::set<BasicBlock*>& visited) {
    visited.insert(bb);
    pushTableScope();
    _wait_delete.clear();
    LVN(bb);
    for (auto instr : _wait_delete) {
        bb->deleteInstr(instr);
    }

    for (auto suc_bb : bb->getSuccBasicBlockList()) {
        if (suc_bb->getPreBasicBlockList().size() == 1) {
            SVN(suc_bb, work_list, visited);
        } else if (visited.find(suc_bb) == visited.end()) {
            work_list.push(suc_bb);
        }
    }
    popTableScope();
}
void GlobalVariableNumbering::LVN(BasicBlock* bb) {
    for (auto instr : bb->getInstructions()) {
        if (instr->isPhi()) {
            // TODO
        } else if (instr->isBinary()) {
            auto val = lookUpOrAdd(instr);
            if (val != instr) {
                replace(instr, val);
            }
        } /*else if(instr->isGep()) {
            auto val = lookUpOrAdd(instr);
            if (val != instr) {
                replace(instr, val);
            }
        } else {
            // TODO:其他类型指令的优化
        }*/
    }
}
Value* GlobalVariableNumbering::findSameInstrInTable(Instruction* instr) {
    if (instr->isBinary()) {
        auto lhs = lookUpOrAdd(instr->getOperand(0));
        auto rhs = lookUpOrAdd(instr->getOperand(1));
        for (auto map_it : _value_table) {
            for (auto it : map_it) {
                auto key_instr = dynamic_cast<Instruction*>(it.first);
                auto val_instr = it.second;
                if (key_instr && key_instr->isBinary()) {
                    auto lhs2 = lookUpOrAdd(key_instr->getOperand(0));
                    auto rhs2 = lookUpOrAdd(key_instr->getOperand(1));
                    bool same_op = false;
                    if ((key_instr->getInstructionKind() ==
                         instr->getInstructionKind()) &&
                        !key_instr->isCmp()) {
                        same_op = true;
                    } else if (key_instr->isCmp() && instr->isCmp()) {
                        auto cmp1 = dynamic_cast<CmpInst*>(key_instr);
                        auto cmp2 = dynamic_cast<CmpInst*>(instr);
                        if(cmp1->getCmpOp() == cmp2->getCmpOp()){
                            same_op=true;
                        }
                    }
                    bool same_oprd = (lhs == lhs2 && rhs == rhs2) ||
                                     (key_instr->isCommutative() &&
                                      (lhs == rhs2 && rhs == lhs2));

                    if (same_op && same_oprd) {
                        return val_instr;
                    }
                }
            }
        }
    } /*else if(instr->isGep()){
        std::vector<Value*> oprds ;
        for(auto oprd : instr->getOperandList()){
            oprds.push_back(lookUpOrAdd(oprd));
        }
        for (auto map_it : _value_table) {
            for (auto it : map_it) {
                auto key_instr = dynamic_cast<Instruction*>(it.first);
                auto val_instr = it.second;
                if (key_instr && key_instr->isGep()) {
                    std::list<Value*> oprds2;
                    bool all_same=true;
                    int i =0;
                    for(auto oprd : key_instr->getOperandList()){
                        if(oprds.at(i) != lookUpOrAdd(oprd) ){
                            all_same=false;
                            break;
                        }
                        i++;
                        if(i == oprds.size()){
                            all_same=false;
                            break;
                        }
                    }
                    if (i ==oprds.size() && all_same ) {
                        return val_instr;
                    }
                }
            }
        }
    } else {
        // TODO: 其他种类的指令
    }*/
    return instr;
}
Value* GlobalVariableNumbering::lookUpOrAdd(Value* val) {
    auto vv = findOnTable(val);
    if (vv != nullptr) {
        return vv;
    }
    if (val->isConstant()) {
        pushValueToTable(val, val);
    } else if (auto instr = dynamic_cast<Instruction*>(val)) {
        pushValueToTable(val, findSameInstrInTable(instr));
    }
    // } else {
    //     ERROR("error type");
    // }
    return findOnTable(val);
}
void GlobalVariableNumbering::replace(Instruction* old_val, Value* new_val) {
    if (old_val == new_val) {
        return;
    }
    deleteValueFromTable(old_val);
    old_val->replaceAllUse(new_val);
    old_val->removeUseOps();
    _wait_delete.push_back(old_val);
}

Value* GlobalVariableNumbering::findOnTable(Value* v) {
    for (auto s = this->_value_table.rbegin(); s != this->_value_table.rend();
         s++) {
        auto iter = s->find(v);
        if (iter != s->end()) {
            return iter->second;
        }
        if (v->isConstant()) {
            for (auto it : *s) {
                if (it.first->isConstant()) {
                    if (it.first->getPrintName() == v->getPrintName()) {
                        return it.second;
                    }
                }
            }
        }
    }
    return nullptr;
}
void GlobalVariableNumbering::deleteValueFromTable(Value* key_val) {
    for (auto& m : _value_table) {
        m.erase(key_val);
    }
}
