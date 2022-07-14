
#include "constan_fold.h"

#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/instruction.h"
#include "module.h"
void ConstantFold::run() {
    for (auto f : _m->getFunctions()) {
        if (!f->isBuiltin()) {
            constantFold(f);
            deleteCondBr(f);
        }
    }
}

bool ConstantFold::detectBinaryConstantFold(BinaryInst *inst) {
    auto oprt1 = inst->getOperand(0);
    if (!oprt1->isConstant()) return false;
    auto oprt2 = inst->getOperand(1);
    if (!oprt2->isConstant()) return false;

    Constant *new_constant;
    if (oprt1->getType()->isIntegerTy() && oprt2->getType()->isIntegerTy()) {
        new_constant = calConstantIntBinary(inst);

    } else if (oprt1->getType()->isFloatTy() && oprt2->getType()->isFloatTy()) {
        new_constant = calConstantFloatBinary(inst);

    } else {
        ERROR("unsupported type");
    }
    inst->replaceAllUse(new_constant);
    inst->removeUseOps();
    return true;
}

void ConstantFold::deleteCondBr(Function *f) {
    for (auto bb : f->getBasicBlocks()) {
        if (bb->getTerminator()->isBr()) {
            auto br = bb->getTerminator();
            if (dynamic_cast<BranchInst *>(br)->isConBranch()) {
                auto cond = dynamic_cast<ConstantInt *>(br->getOperand(0));
                auto true_bb = br->getOperand(1);
                auto false_bb = br->getOperand(2);
                if (cond) {
                    if (cond->getValue() == 0) {
                        bb->deleteInstr(br);
                        for (auto succ_bb : bb->getSuccBasicBlocks()) {
                            succ_bb->removePreBasicBlock(bb);
                            if (succ_bb != false_bb) {
                                for (auto instr : succ_bb->getInstructions()) {
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
                        BranchInst::createBranch(dynamic_cast<BasicBlock*>(false_bb),bb);

                    } else {
                        bb->deleteInstr(br);
                        for (auto succ_bb : bb->getSuccBasicBlocks()) {
                            succ_bb->removePreBasicBlock(bb);
                            if (succ_bb != true_bb) {
                                for (auto instr : succ_bb->getInstructions()) {
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
                        BranchInst::createBranch(dynamic_cast<BasicBlock*>(true_bb),bb);
                    }
                }
            }
        }
    }
}

void ConstantFold::constantFold(Function *f) {
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
        }
        for (auto inst : wait_delete) {
            bb->deleteInstr(inst);
        }
    }
}
Constant *ConstantFold::calConstantIntBinary(Instruction *pInstruction) {
    auto oprt1_ptr = dynamic_cast<ConstantInt *>(pInstruction->getOperand(0));
    auto oprt2_ptr = dynamic_cast<ConstantInt *>(pInstruction->getOperand(1));
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
Constant *ConstantFold::calConstantFloatBinary(Instruction *instr) {
    auto oprt1_ptr = dynamic_cast<ConstantFloat *>(instr->getOperand(0));
    auto oprt2_ptr = dynamic_cast<ConstantFloat *>(instr->getOperand(1));
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
bool ConstantFold::detectCastConstantFold(UnaryInst *pInst) {
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
bool ConstantFold::detectCmpConstantFold(CmpInst *pInst) {
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
        new_constant = calConstantIntBinary(pInst);
    } else if (is_float) {
        new_constant = calConstantFloatBinary(pInst);
    } else {
        ERROR("error type");
        return false;
    }
    pInst->replaceAllUse(new_constant);
    pInst->removeUseOps();
    return true;
}
