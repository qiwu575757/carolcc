
#include "constan_fold.h"

#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/instruction.h"
#include "module.h"
void ConstantFold::run() {
    for (auto f : _m->getFunctions()) {
        if (!f->isBuiltin()) constantFold(f);
    }
}

bool ConstantFold::detectBinaryConstantFold(BinaryInst *binary_inst) {
    auto oprt1 = binary_inst->getOperand(0);
    if (!oprt1->isConstant()) return false;
    auto oprt2 = binary_inst->getOperand(1);
    if (!oprt2->isConstant()) return false;

    Constant *new_constant;
    if (oprt1->getType()->isIntegerTy() && oprt2->getType()->isIntegerTy()) {
        auto val = calConstantIntBinary(binary_inst);
        new_constant = ConstantInt::create(val);

    } else if (oprt1->getType()->isFloatTy() && oprt2->getType()->isFloatTy()) {
        auto val = calConstantFloatBinary(binary_inst);
        new_constant = ConstantFloat::create(val);
    } else {
        ERROR("unsupported type");
    }
    binary_inst->replaceAllUse(new_constant);
    binary_inst->removeUseOps();
    return true;
}

void ConstantFold::constantFold(Function *f) {
    for (auto bb : f->getBasicBlocks()) {
        std::vector<Instruction *> wait_delete;
        for (auto inst : bb->getInstructions()) {
            auto binary_inst = dynamic_cast<BinaryInst *>(inst);
            if (binary_inst != nullptr) {
                if (detectBinaryConstantFold(binary_inst))
                    wait_delete.push_back(binary_inst);
                continue ;
            }

            auto unary_inst = dynamic_cast<UnaryInst *>(inst);
            if (unary_inst != nullptr && unary_inst->isCast()) {
                if(detectCastConstantFold(unary_inst))
                    wait_delete.push_back(unary_inst);
                continue ;
            }
        }
        for (auto inst : wait_delete) {
            bb->deleteInstr(inst);
        }
    }
}
int ConstantFold::calConstantIntBinary(BinaryInst *binary_instr) {
    auto oprt1_ptr = dynamic_cast<ConstantInt *>(binary_instr->getOperand(0));
    auto oprt2_ptr = dynamic_cast<ConstantInt *>(binary_instr->getOperand(1));
    MyAssert("null ptr", oprt1_ptr != nullptr && oprt2_ptr != nullptr);
    int res;
    auto oprt1 = oprt1_ptr->getValue();
    auto oprt2 = oprt2_ptr->getValue();
    switch (binary_instr->getInstructionKind()) {
        case Instruction::ADD:
            res = oprt1 + oprt2;
            break;
        case Instruction::SUB:
            res = oprt1 - oprt2;
            break;
        case Instruction::MUL:
            res = oprt1 * oprt2;
            break;
        case Instruction::DIV:
            res = oprt1 / oprt2;
            break;
        case Instruction::REM:
            res = oprt1 % oprt2;
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
        case Instruction::CMP:
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
float ConstantFold::calConstantFloatBinary(BinaryInst *binary_instr) {
    auto oprt1_ptr = dynamic_cast<ConstantFloat *>(binary_instr->getOperand(0));
    auto oprt2_ptr = dynamic_cast<ConstantFloat *>(binary_instr->getOperand(1));
    MyAssert("null ptr", oprt1_ptr != nullptr && oprt2_ptr != nullptr);
    float res;
    auto oprt1 = oprt1_ptr->getValue();
    auto oprt2 = oprt2_ptr->getValue();
    switch (binary_instr->getInstructionKind()) {
        case Instruction::ADD:
            res = oprt1 + oprt2;
            break;
        case Instruction::SUB:
            res = oprt1 - oprt2;
            break;
        case Instruction::MUL:
            res = oprt1 * oprt2;
            break;
        case Instruction::DIV:
            res = oprt1 / oprt2;
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
        case Instruction::CMP:
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
