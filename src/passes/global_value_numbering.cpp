#include "global_value_numbering.h"

#include <cstdlib>
#include <queue>
#include <set>
#include <vector>

#include "analysis/dominators.h"
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

    for (auto f : _m->getFunctions()) {
        if (!f->isBuiltin()) {
            _value_table.clear();
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
                ERROR("there should not have dead blocks", EXIT_CODE_ERROR_430);
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
    GVN_LOG("visiting bb %s",bb->getPrintName().c_str());
    LVN(bb);
    for (auto instr : _wait_delete) {
        bb->deleteInstr(instr);
    }

    for (auto suc_bb : bb->getSuccBasicBlockList()) {
        if (suc_bb->getPreBasicBlockList().size() == 1) {
            SVN(suc_bb, work_list, visited);
        } else if (visited.find(suc_bb) == visited.end()) {
            visited.insert(suc_bb);
            work_list.push(suc_bb);
        }
    }
    popTableScope();
}
void GlobalVariableNumbering::LVN(BasicBlock* bb) {
    for (auto instr : bb->getInstructions()) {
        if (instr->isBinary() && !instr->isCmp()) {
            auto val = lookUpOrAdd(instr);
            if (val != instr) {
                replace(instr, val);
            }
        } else if (instr->isGep()) {
            auto val = lookUpOrAdd(instr);
            if (val != instr) {
                replace(instr, val);
            }
        } else {
            // TODO:其他类型指令的优化
        }
    }
}
Value* GlobalVariableNumbering::findSameInstrInTable(Instruction* instr) {
    if (instr->isBinary() && !instr->isCmp() ) {
        auto lhs = lookUpOrAdd(instr->getOperand(0));
        auto rhs = lookUpOrAdd(instr->getOperand(1));
        for (auto map_it : _binary_table) {
            auto res1 = map_it.find(_my_tuple(lhs,rhs,instr->getInstructionKind()));
            if(res1!=map_it.end()){
                return res1->second;
            }
            if(instr->isCommutative()){
                auto res2 =
                    map_it.find(_my_tuple(lhs,rhs,instr->getInstructionKind()));
                if (res2 != map_it.end()) {
                    return res2->second;
                }
            }
        }
    } else if (instr->isGep()) {
        std::vector<Value*> oprds;
        for (auto oprd : instr->getOperandList()) {
            oprds.push_back(lookUpOrAdd(oprd));
        }
        for (auto map_it : _gep_table) {
            for (auto it : map_it) {
                auto key_instr = dynamic_cast<Instruction*>(it.first);
                auto val_instr = it.second;
                if (key_instr && key_instr->isGep()) {
                    std::vector<Value*> oprds2;
                    bool all_same = true;
                    for (auto oprd : key_instr->getOperandList()) {
                        oprds2.push_back(lookUpOrAdd(oprd));
                    }
                    if (oprds2.size() != oprds.size()) {
                        all_same = false;
                    } else {
                        for (auto i = 0; i < oprds.size(); i++) {
                            if (oprds.at(i) != oprds2.at(i)) {
                                all_same = false;
                                break;
                            }
                        }
                    }
                    if (all_same) {
                        return val_instr;
                    }
                }
            }
        }
    } else {
        // TODO: 其他种类的指令
    }
    return instr;
}
Value* GlobalVariableNumbering::lookUpOrAdd(Value* val) {
    auto vv = findOnTable(val);
    if (vv != nullptr) {
        return vv;
    }
    if (val->isConstant()) {
        auto find_val = findOnTable(val);
        if (find_val != nullptr) {
            return find_val;
        }
        pushValueToTable(val, val);
    } else if (auto instr = dynamic_cast<Instruction*>(val)) {
        auto find_instr = findSameInstrInTable(instr);
        if (find_instr != instr) {
            return find_instr;
        }
        pushValueToTable(instr, instr);
    } else {
        pushValueToTable(val, val);
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
    auto instr = dynamic_cast<Instruction*>(v);
    if (instr) {
        if (instr->isGep()) {
            for (auto s = this->_gep_table.rbegin();
                 s != this->_gep_table.rend(); s++) {
                auto iter = s->find(v);
                if (iter != s->end()) {
                    return iter->second;
                }
            }
        } else if (instr->isBinary() && !instr->isCmp()) {
            for (auto s = this->_binary_table.rbegin();
                 s != this->_binary_table.rend(); s++) {
                _my_tuple _t_tmp(instr->getOperand(0), instr->getOperand(1),
                             instr->getInstructionKind());
                auto iter1 =
                    s->find(_t_tmp);
                if (iter1 != s->end()) {
                    return iter1->second;
                } else if (instr->isCommutative() ) {
                    _my_tuple _t_tmp(instr->getOperand(1), instr->getOperand(0),
                                 instr->getInstructionKind());
                    auto iter2 =
                        s->find(_t_tmp);
                    if (iter2 != s->end()) return iter2->second;
                }
            }
        } else {
            for (auto s = this->_value_table.rbegin();
                 s != this->_value_table.rend(); s++) {
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
        }
    } else {
        for (auto s = this->_value_table.rbegin();
             s != this->_value_table.rend(); s++) {
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
    }
    return nullptr;
}
void GlobalVariableNumbering::deleteValueFromTable(Value* key_val) {
    auto instr = dynamic_cast<Instruction*>(key_val);
    if (instr) {
        if (instr->isGep()) {
            for (auto& m : _gep_table) {
                m.erase(key_val);
            }
        } else if (instr->isBinary() && !instr->isCmp()) {
            for (auto& m : _binary_table) {
                _my_tuple _t_tmp(instr->getOperand(0), instr->getOperand(1),
                             instr->getInstructionKind());
                m.erase(_t_tmp);
                if (instr->isCommutative())
                _my_tuple _t_tmp(instr->getOperand(1), instr->getOperand(0),
                             instr->getInstructionKind());
                    m.erase(_t_tmp);
            }
        } else {
            ERROR("unsupport instr type", GVNError);
        }
    } else {
        for (auto& m : _value_table) {
            m.erase(key_val);
        }
    }
}
