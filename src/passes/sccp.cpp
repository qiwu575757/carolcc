#include "sccp.h"

#include <list>
#include <vector>

#include "ir/base_block.h"
#include "ir/basic_block.h"
#include "ir/function.h"
#include "module.h"
#include "syntax_tree.h"
#include "utils.h"
static const bool EXCUTABLE = true;
static const bool NEVER_EXCUTATED = false;
static int ffflag;
void SCCP::run() {
    for (auto f : _m->getFunctions()) {
        if (!f->isBuiltin()) {
            SCCP_LOG("run number is %d", ffflag);
            if (ffflag == 0) {
                SCCP_LOG("first pass only run constant fold");
                constantFoldAndPropagation(f);
                SCCP_LOG("finish constantFoldAndPropagation");
            } else {
                SCCP_LOG("second pass only run constant fold");
                sparseConditionalConstantPropagation(f);
                SCCP_LOG("finish sparseConditionalConstantPropagation");
            }
        }
    }
    ffflag++;
}

bool SCCP::detectBinaryConstantFold(BinaryInst *inst) {
    auto oprt1 = inst->getOperand(0);
    if (!oprt1->isConstant()) return false;
    auto oprt2 = inst->getOperand(1);
    if (!oprt2->isConstant()) return false;

    Constant *new_constant = calConstantBinary(inst, oprt1, oprt2);
    inst->replaceAllUse(new_constant);
    inst->removeUseOps();
    return true;
}

void SCCP::constantFoldAndPropagation(Function *f) {
    for (auto bb : f->getBasicBlocks()) {
        std::vector<Instruction *> wait_delete;
        for (auto inst : bb->getInstructions()) {
            auto binary_inst = dynamic_cast<BinaryInst *>(inst);
            if (binary_inst != nullptr) {
                if (detectBinaryConstantFold(binary_inst))
                    wait_delete.push_back(binary_inst);
                continue;
            }

            auto unary_inst = dynamic_cast<UnaryInst *>(inst);
            if (unary_inst != nullptr && unary_inst->isCast()) {
                if (detectCastConstantFold(unary_inst))
                    wait_delete.push_back(unary_inst);
                continue;
            }

            auto cmp_inst = dynamic_cast<CmpInst *>(inst);
            if (cmp_inst != nullptr && cmp_inst->isCmp()) {
                if (detectCmpConstantFold(cmp_inst))
                    wait_delete.push_back(cmp_inst);
                continue;
            }

            auto zext_inst = dynamic_cast<ZExtInst *>(inst);
            if (zext_inst != nullptr) {
                if (zext_inst->getOperand(0)->isConstant()) {
                    MyAssert("error type", zext_inst->getType()->isInt32());
                    auto constant_int =
                        dynamic_cast<ConstantInt *>(zext_inst->getOperand(0));
                    if (constant_int != nullptr) {
                        auto new_constant =
                            ConstantInt::create(constant_int->getValue());
                        zext_inst->replaceAllUse(new_constant);
                        zext_inst->removeUseOps();
                        wait_delete.push_back(zext_inst);
                    }
                }
                continue;
            }
        }
        for (auto inst : wait_delete) {
            bb->deleteInstr(inst);
        }
    }
}
Constant *SCCP::calConstantIntBinary(Instruction *pInstruction,
                                     ConstantInt *oprt1_ptr,
                                     ConstantInt *oprt2_ptr) {
    MyAssert("null ptr", oprt1_ptr != nullptr && oprt2_ptr != nullptr);
    Constant *res;
    auto oprt1 = oprt1_ptr->getValue();
    auto oprt2 = oprt2_ptr->getValue();
    switch (pInstruction->getInstructionKind()) {
        case Instruction::ADD:
            res = ConstantInt::create(oprt1 + oprt2);
            break;
        case Instruction::SUB:
            res = ConstantInt::create(oprt1 - oprt2);
            break;
        case Instruction::MUL:
            res = ConstantInt::create(oprt1 * oprt2);
            break;
        case Instruction::DIV:
            res = ConstantInt::create(oprt1 / oprt2);
            break;
        case Instruction::REM:
            res = ConstantInt::create(oprt1 % oprt2);
            break;
        case Instruction::CMP:
            switch (dynamic_cast<CmpInst *>(pInstruction)->getCmpOp()) {
                case CmpInst::EQ:
                    res = ConstantInt::getBool(oprt1 == oprt2);
                    break;

                case CmpInst::NEQ:
                    res = ConstantInt::getBool(oprt1 != oprt2);
                    break;
                case CmpInst::GT:
                    res = ConstantInt::getBool(oprt1 > oprt2);
                    break;
                case CmpInst::GE:
                    res = ConstantInt::getBool(oprt1 >= oprt2);
                    break;
                case CmpInst::LT:
                    res = ConstantInt::getBool(oprt1 < oprt2);
                    break;
                case CmpInst::LE:
                    res = ConstantInt::getBool(oprt1 <= oprt2);
                    break;
            }
            break;
        case Instruction::RET:
        case Instruction::BR:
        case Instruction::NOT:
        case Instruction::SHL:
        case Instruction::LSHR:
        case Instruction::ASHR:
        case Instruction::AND:
        case Instruction::OR:
        case Instruction::XOR:
        case Instruction::ALLOCA:
        case Instruction::LOAD:
        case Instruction::STORE:
        case Instruction::PHI:
        case Instruction::GEP:
        case Instruction::CALL:
        case Instruction::ZEXT:
        case Instruction::BREAK:
        case Instruction::CONTINUE:
            ERROR("error type");
            break;
        default:
            ERROR("error type");
    }
    return res;
}
Constant *SCCP::calConstantFloatBinary(Instruction *instr,
                                       ConstantFloat *oprt1_ptr,
                                       ConstantFloat *oprt2_ptr) {
    MyAssert("null ptr", oprt1_ptr != nullptr && oprt2_ptr != nullptr);
    Constant *res;
    auto oprt1 = oprt1_ptr->getValue();
    auto oprt2 = oprt2_ptr->getValue();
    switch (instr->getInstructionKind()) {
        case Instruction::ADD:
            res = ConstantFloat::create(oprt1 + oprt2);
            break;
        case Instruction::SUB:
            res = ConstantFloat::create(oprt1 - oprt2);
            break;
        case Instruction::MUL:
            res = ConstantFloat::create(oprt1 * oprt2);
            break;
        case Instruction::DIV:
            res = ConstantFloat::create(oprt1 / oprt2);
            break;
        case Instruction::CMP:
            switch (dynamic_cast<CmpInst *>(instr)->getCmpOp()) {
                case CmpInst::EQ:
                    res = ConstantInt::getBool(oprt1 == oprt2);
                    break;
                case CmpInst::NEQ:
                    res = ConstantInt::getBool(oprt1 != oprt2);
                    break;
                case CmpInst::GT:
                    res = ConstantInt::getBool(oprt1 > oprt2);
                    break;
                case CmpInst::GE:
                    res = ConstantInt::getBool(oprt1 >= oprt2);
                    break;
                case CmpInst::LT:
                    res = ConstantInt::getBool(oprt1 < oprt2);
                    break;
                case CmpInst::LE:
                    res = ConstantInt::getBool(oprt1 <= oprt2);
                    break;
            }
            break;
        case Instruction::REM:
        case Instruction::RET:
        case Instruction::BR:
        case Instruction::NOT:
        case Instruction::SHL:
        case Instruction::LSHR:
        case Instruction::ASHR:
        case Instruction::AND:
        case Instruction::OR:
        case Instruction::XOR:
        case Instruction::ALLOCA:
        case Instruction::LOAD:
        case Instruction::STORE:
        case Instruction::PHI:
        case Instruction::GEP:
        case Instruction::CALL:
        case Instruction::ZEXT:
        case Instruction::BREAK:
        case Instruction::CONTINUE:
            ERROR("error type");
            break;
        default:
            ERROR("error type");
    }
    return res;
}
bool SCCP::detectCastConstantFold(UnaryInst *pInst) {
    MyAssert("unary inst is not cast", pInst->isCast());
    if (pInst->getOperand(0)->isConstant()) {
        Constant *new_constant;

        if (pInst->getType()->isIntegerTy() &&
            pInst->getOperand(0)->getType()->isFloatTy()) {
            auto val =
                dynamic_cast<ConstantFloat *>(pInst->getOperand(0))->getValue();
            new_constant = ConstantInt::create(static_cast<int>(val));
        } else if (pInst->getType()->isFloatTy() &&
                   pInst->getOperand(0)->getType()->isIntegerTy()) {
            auto val =
                dynamic_cast<ConstantInt *>(pInst->getOperand(0))->getValue();
            new_constant = ConstantFloat::create(static_cast<float>(val));
        } else {
            ERROR("unsupported type");
        }
        pInst->replaceAllUse(new_constant);
        pInst->removeUseOps();
        return true;
    } else {
        return false;
    }
}
bool SCCP::detectCmpConstantFold(CmpInst *pInst) {
    bool is_int = true;
    bool is_float = true;
    Constant *new_constant = nullptr;
    for (auto oprt : pInst->getOperandList()) {
        if (!oprt->isConstant()) {
            return false;
        }
        if (!oprt->getType()->isFloatTy()) is_float = false;
        if (!oprt->getType()->isIntegerTy()) is_int = false;
    }
    if (is_int) {
        new_constant = calConstantIntBinary(
            pInst, dynamic_cast<ConstantInt *>(pInst->getOperand(0)),
            dynamic_cast<ConstantInt *>(pInst->getOperand(1)));
    } else if (is_float) {
        new_constant = calConstantFloatBinary(
            pInst, dynamic_cast<ConstantFloat *>(pInst->getOperand(0)),
            dynamic_cast<ConstantFloat *>(pInst->getOperand(1)));
    } else {
        ERROR("error type");
        return false;
    }
    pInst->replaceAllUse(new_constant);
    pInst->removeUseOps();
    return true;
}

void SCCP::sparseConditionalConstantPropagation(Function *f) {
    _block_work_list.clear();
    _value_work_list.clear();
    _block_work_list.push_back(f->getEntryBlock());
    _block_excutable_table[f->getEntryBlock()] = EXCUTABLE;
    while (!_block_work_list.empty() || !_value_work_list.empty()) {
        while (!_block_work_list.empty()) {
            auto bb = _block_work_list.front();
            _block_work_list.pop_front();
            sccpOnBasicBlock(bb);
        }
        while (!_value_work_list.empty()) {
            auto instr = dynamic_cast<Instruction *>(_value_work_list.front());
            _value_work_list.pop_front();
            MyAssert("error", instr);
            sccpOnInstrution(instr);
        }
    }
    printDebugInfo(f);
    replaceConstant(f);
}
bool SCCP::isBlockExcutable(BasicBlock *bb) {
    auto it_bb = _block_excutable_table.find(bb);
    if (it_bb != _block_excutable_table.end()) {
        return it_bb->second;
    }
    return false;
}
Constant *SCCP::calConstantBinary(Instruction *inst, Value *oprt1,
                                  Value *oprt2) {
    Constant *new_constant;
    if (oprt1->getType()->isIntegerTy() && oprt2->getType()->isIntegerTy()) {
        new_constant =
            calConstantIntBinary(inst, dynamic_cast<ConstantInt *>(oprt1),
                                 dynamic_cast<ConstantInt *>(oprt2));

    } else if (oprt1->getType()->isFloatTy() && oprt2->getType()->isFloatTy()) {
        new_constant =
            calConstantFloatBinary(inst, dynamic_cast<ConstantFloat *>(oprt1),
                                   dynamic_cast<ConstantFloat *>(oprt2));

    } else {
        ERROR("unsupported type");
    }
    return new_constant;
}
bool SCCP::isValueNeverAssigned(Value *value) {
    auto it = _instr_assign_table.find(value);
    if (it != _instr_assign_table.end()) {
        return false;
    }
    return true;
}
bool SCCP::isValueHasDefiniteValue(Value *value) {
    auto it = _instr_assign_table.find(value);
    if (it != _instr_assign_table.end()) {
        if (it->second->isConstant()) {
            return true;
        }
    }
    return false;
}
bool SCCP::isValueHasMultiValue(Value *value) {
    auto it = _instr_assign_table.find(value);
    if (it != _instr_assign_table.end()) {
        if (it->second == &uncertainValue) {
            return true;
        }
    }
    return false;
}
void SCCP::sccpOnInstrution(Instruction *instr) {
    // 经过常数传播之后，这里也许都不需要判断了
    if (instr->isBinary()) {
        auto oprd1 = instr->getOperand(0);
        auto oprd2 = instr->getOperand(1);
        if (oprd1->isConstant() && isValueNeverAssigned(oprd1)) {
            _instr_assign_table[oprd1] = oprd1;
        }
        if (oprd2->isConstant() && isValueNeverAssigned(oprd2)) {
            _instr_assign_table[oprd2] = oprd2;
        }

        if (isValueHasDefiniteValue(oprd1) && isValueHasDefiniteValue(oprd2)) {
            auto new_constant = calConstantBinary(
                instr, _instr_assign_table[oprd1], _instr_assign_table[oprd2]);
            if (isValueNeverAssigned(instr)) _value_work_list.push_back(instr);
            MyAssert("new_constant is not a constant",
                     new_constant->isConstant());
            _instr_assign_table[instr] = new_constant;
        } else if (isValueHasMultiValue(oprd1) || isValueHasMultiValue(oprd2)) {
            if (isValueNeverAssigned(instr) || isValueHasDefiniteValue(instr))
                _value_work_list.push_back(instr);
            _instr_assign_table[instr] = &uncertainValue;
        } else if (isValueNeverAssigned(oprd1) || isValueNeverAssigned(oprd2)) {
            if (isValueNeverAssigned(instr) || isValueHasDefiniteValue(instr))
                _value_work_list.push_back(instr);
            _instr_assign_table[instr] = &uncertainValue;
        }

    } else if (instr->isPhi()) {
        int certain_pre_number = 0;
        Constant *a_val = nullptr;
        bool is_different_val_same = true;
        for (int i = 0; i < instr->getOperandNumber(); i = i + 2) {
            auto pre_val = instr->getOperand(i);
            auto pre_bb = dynamic_cast<BasicBlock *>(instr->getOperand(i + 1));
            MyAssert("error", pre_val != nullptr && pre_bb != nullptr);
            if (pre_val->isConstant()) {
                _instr_assign_table[pre_val] = pre_val;
            }
            if (isValueHasMultiValue(pre_val) && isBlockExcutable(pre_bb)) {
                if (isValueNeverAssigned(instr) ||
                    isValueHasDefiniteValue(instr))
                    _value_work_list.push_back(instr);
                _instr_assign_table[instr] = &uncertainValue;
                break;
            }
            if (isValueHasDefiniteValue(pre_val) && isBlockExcutable(pre_bb)) {
                certain_pre_number++;
                if (certain_pre_number == 1) {
                    a_val =
                        dynamic_cast<Constant *>(_instr_assign_table[pre_val]);
                    MyAssert("a_val is null ptr", a_val != nullptr);
                } else {
                    auto another_val =
                        dynamic_cast<Constant *>(_instr_assign_table[pre_val]);
                    if (a_val->getPrintName() != another_val->getPrintName()) {
                        if (isValueNeverAssigned(instr) ||
                            isValueHasDefiniteValue(instr))
                            _value_work_list.push_back(instr);
                        _instr_assign_table[instr] = &uncertainValue;
                        is_different_val_same=false;
                        break;
                    }
                }
            }
        }
        if (certain_pre_number == 1 || (certain_pre_number > 1 && is_different_val_same)) {
            if (isValueNeverAssigned(instr)) {
                _value_work_list.push_back(instr);
                MyAssert("a_val is not a constant", a_val->isConstant());
                _instr_assign_table[instr] = a_val;
            }
        }

    } else if (instr->isBr()) {
        auto br_inst = dynamic_cast<BranchInst *>(instr);
        if (br_inst->isBranch()) {
            // 上面已经处理过
        } else if (br_inst->isConBranch()) {
            auto cond_val = br_inst->getOperand(0);
            auto true_bb = dynamic_cast<BasicBlock *>(br_inst->getOperand(1));
            auto false_bb = dynamic_cast<BasicBlock *>(br_inst->getOperand(2));
            if (isValueHasDefiniteValue(cond_val)) {
                auto val = _instr_assign_table[cond_val];
                auto bool_res = dynamic_cast<ConstantInt *>(val)->getValue();
                if (bool_res) {
                    if (!isBlockExcutable(true_bb)) {
                        _block_excutable_table[true_bb] = EXCUTABLE;
                        _block_work_list.push_back(true_bb);
                        for (auto true_bb_suc :
                             true_bb->getSuccBasicBlockList()) {
                            if (isBlockExcutable(true_bb_suc)) {
                                _block_work_list.push_back(true_bb_suc);
                            }
                        }
                    }

                } else {
                    if (!isBlockExcutable(false_bb)) {
                        _block_excutable_table[false_bb] = EXCUTABLE;
                        _block_work_list.push_back(false_bb);
                        for (auto false_bb_suc :
                             false_bb->getSuccBasicBlockList()) {
                            if (isBlockExcutable(false_bb_suc)) {
                                _block_work_list.push_back(false_bb_suc);
                            }
                        }
                    }
                }
            } else {
                _block_excutable_table[true_bb] = EXCUTABLE;
                _block_excutable_table[false_bb] = EXCUTABLE;
            }
        }
    } else if (instr->isLoad()) {
        _instr_assign_table[instr] = &uncertainValue;
    } else if (instr->isCall()) {
        // todo
        _instr_assign_table[instr] = &uncertainValue;
    } else if (instr->isCast()) {
        auto oprd = instr->getOperand(0);
        if (oprd->isConstant()) {
            _instr_assign_table[oprd] = oprd;
        }
        if (isValueHasMultiValue(oprd) || isValueNeverAssigned(oprd)) {
            _instr_assign_table[instr] = &uncertainValue;
        } else if (isValueHasDefiniteValue(oprd)) {
            Constant *new_constant;
            if (instr->getType()->isIntegerTy() &&
                instr->getOperand(0)->getType()->isFloatTy()) {
                auto val =
                    dynamic_cast<ConstantFloat *>(_instr_assign_table[oprd])
                        ->getValue();
                new_constant = ConstantInt::create(static_cast<int>(val));
            } else if (instr->getType()->isFloatTy() &&
                       instr->getOperand(0)->getType()->isIntegerTy()) {
                auto val =
                    dynamic_cast<ConstantInt *>(_instr_assign_table[oprd])
                        ->getValue();
                new_constant = ConstantFloat::create(static_cast<float>(val));
                _instr_assign_table[instr] = new_constant;
            }
        }
    } else if (instr->isZext()) {
        auto oprd = instr->getOperand(0);
        if (oprd->isConstant()) {
            _instr_assign_table[oprd] = oprd;
        }
        if (isValueHasMultiValue(oprd) || isValueNeverAssigned(oprd)) {
            _instr_assign_table[instr] = &uncertainValue;
        } else if (isValueHasDefiniteValue(oprd)) {
            MyAssert("error type", instr->getType()->isInt32());
            auto constant_int =
                dynamic_cast<ConstantInt *>(_instr_assign_table[oprd]);
            MyAssert("error", constant_int != nullptr);
            auto new_constant = ConstantInt::create(constant_int->getValue());
            _instr_assign_table[instr] = new_constant;
        }
    }
}

void SCCP::sccpOnBasicBlock(BasicBlock *bb) {
    if (isBlockExcutable(bb) && bb->getSuccBasicBlocks().size() == 1) {
        auto suc_bb = bb->getSuccBasicBlocks().front();
        if (!isBlockExcutable(suc_bb)) {
            _block_excutable_table[suc_bb] = EXCUTABLE;
            _block_work_list.push_back(suc_bb);
            for (auto suc_suc_bb : suc_bb->getSuccBasicBlocks()) {
                if (isBlockExcutable(suc_suc_bb)) {
                    _block_work_list.push_back(suc_suc_bb);
                }
            }
        }
    }
    for (auto instr : bb->getInstructions()) {
        sccpOnInstrution(instr);
    }
}

void SCCP::printDebugInfo(Function *f) {
    SCCP_LOG("[var table]:");
    for (auto vv : _instr_assign_table) {
        if (vv.second == &uncertainValue) {
            SCCP_LOG(" %s -> uncertain", vv.first->getPrintName().c_str());
        } else {
            SCCP_LOG(" %s -> %s", vv.first->getPrintName().c_str(),
                     vv.second->getPrintName().c_str());
        }
    }
    SCCP_LOG("[block table]:");
    for (auto bb : f->getBasicBlocks()) {
        if (isBlockExcutable(bb)) {
            SCCP_LOG("%s : excutable", bb->getPrintName().c_str());
        } else {
            SCCP_LOG("%s : not excutable", bb->getPrintName().c_str());
        }
    }
}
void SCCP::replaceConstant(Function *f) {
    std::map<BasicBlock *, std::list<Instruction *> > wait_delete;
    // delete instruction
    for (auto vv : _instr_assign_table) {
        auto key_instr = dynamic_cast<Instruction *>(vv.first);
        auto val_instr = vv.second;
        if (key_instr && isValueHasDefiniteValue(key_instr)) {
            SCCP_LOG("replacing instr(%s)",key_instr->getPrintName().c_str());
            key_instr->replaceAllUse(val_instr);
            key_instr->removeUseOps();
            if (key_instr->getParent() == nullptr) {
                ERROR("key_instr(%s) should have parent!",
                      key_instr->getPrintName().c_str());
            }
            wait_delete[key_instr->getParent()].push_back(key_instr);
        }
    }
    for (auto bb_map : wait_delete) {
        for (auto instr : bb_map.second) {
            bb_map.first->deleteInstr(instr);
        }
    }

    // delete blocks
}