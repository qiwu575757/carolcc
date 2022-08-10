#include "instruction_combine.h"

#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/function.h"
#include "ir/instruction.h"
#include "passes/module.h"
#include "passes/sccp.h"
#include "utils.h"
#include "visitor/llvm_ir_printer.h"
// static int index = 0;
void InstructionCombination::run() {
    for (auto fun : _m->getFunctions()) {
        simplifyInstruction(fun);
        // LLVMIrPrinter a(_m->getModuleName() + ".a" + std::to_string(index),
        //                 _m->getModuleName());
        // fun->accept(&a);
        instructionCombine(fun);
        // LLVMIrPrinter b(_m->getModuleName() + ".a" + std::to_string(index + 1),
                        // _m->getModuleName());
        // fun->accept(&b);
        simplifyInstruction(fun);
    }
    // index += 2;
}
void InstructionCombination::simplifyInstruction(Function *func) {
    for (auto bb : func->getBasicBlocks()) {
        std::list<Instruction *> wait_delete;
        std::vector<std::pair<Instruction *, Instruction *>> insert_map;
        for (auto instr : bb->getInstructions()) {
            auto simple_instr =
                simplifyInstruction(instr, wait_delete, insert_map);
        }
        Instruction *last_pos = nullptr;
        if (insert_map.size() != 0) last_pos = insert_map.front().first;
        std::unordered_set<Instruction *> not_used_instr;
        for (auto it : insert_map) {
            if (!it.second->getUseList().empty()) {
                if (not_used_instr.count(it.first) == 0)
                    bb->insertAfterInstr(it.first, it.second);
                else {
                    bb->insertAfterInstr(last_pos, it.second);
                }
                last_pos = it.second;
            } else {
                not_used_instr.insert(it.second);
                it.second->removeUseOps();
            }
        }
        for (auto instr : wait_delete) {
            bb->deleteInstr(instr);
        }
    }
}
static std::unordered_set<Instruction *> wait_delete_set;
void InstructionCombination::instructionCombine(Function *fun) {
    _instr2value_number.clear();
    for (auto bb : fun->getBasicBlocks()) {
        wait_delete_set.clear();
        std::list<Instruction *> wait_delete;
        std::vector<std::pair<Instruction *, Instruction *>> insert_map;
        for (auto instr : bb->getInstructions()) {
            auto simple_instr =
                combineInstruction(instr, wait_delete, insert_map);
        }
        for (auto it : insert_map) {
            bb->insertAfterInstr(it.first, it.second);
        }
        for (auto instr : wait_delete) {
            bb->deleteInstr(instr);
        }
    }
}
Value *InstructionCombination::simplifyInstruction(
    Instruction *instr, std::list<Instruction *> &wait_delete,
    std::vector<std::pair<Instruction *, Instruction *>> &insert_map) {
    Value *res = nullptr;
    switch (instr->getInstructionKind()) {
        case Instruction::ADD:
            res = simplifyAdd(dynamic_cast<BinaryInst *>(instr), wait_delete,
                              insert_map, true);
            break;
        case Instruction::SUB:
            // TODO:
            break;
        case Instruction::MUL:
            res = simplifyMul(dynamic_cast<BinaryInst *>(instr), wait_delete,
                              insert_map, true);
            break;
        case Instruction::DIV:
            // TODO:
            break;
        case Instruction::REM:
            // todo
            break;
        case Instruction::CMP:
            switch (dynamic_cast<CmpInst *>(instr)->getCmpOp()) {
                case CmpInst::EQ:

                    break;

                case CmpInst::NEQ:

                    break;
                case CmpInst::GT:

                    break;
                case CmpInst::GE:

                    break;
                case CmpInst::LT:

                    break;
                case CmpInst::LE:

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
            res = instr;
            break;
        default:
            ERROR("error type", EXIT_CODE_ERROR_440);
            break;
    }
    return res;
}

Value *InstructionCombination::simplifyAdd(
    BinaryInst *binary_instr, std::list<Instruction *> &wait_delete,
    std::vector<std::pair<Instruction *, Instruction *>> &insert_map,
    bool can_recur) {
    MyAssert("error instr type", binary_instr->isAdd(), simplifyInstrTypeError);
    auto oprt1 = binary_instr->getOperand(0);
    auto oprt2 = binary_instr->getOperand(1);
    // GVN一般需要紧跟在sccp后面
    // 保证optr2 是 constant
    if (oprt1->isConstant() && oprt2->isConstant()) {
        auto new_val = SCCP::calConstantBinary(binary_instr, oprt1, oprt2);
        binary_instr->replaceAllUse(new_val);
        binary_instr->removeUseOps();
        wait_delete.push_back(binary_instr);
        return new_val;
    }
    if (oprt1->isConstant()) {
        binary_instr->removeUseOps();
        binary_instr->addOperand(oprt2);
        binary_instr->addOperand(oprt1);
        oprt1 = binary_instr->getOperand(0);
        oprt2 = binary_instr->getOperand(1);
    }
    if (oprt2->isConstant()) {
        auto oprt2_const_int = dynamic_cast<ConstantInt *>(oprt2);
        if (oprt2_const_int && oprt2_const_int->getValue() == 0) {
            binary_instr->removeUseOps();
            binary_instr->replaceAllUse(oprt1);
            wait_delete.push_back(binary_instr);
            return oprt1;
        }
        auto oprt2_const_float = dynamic_cast<ConstantFloat *>(oprt2);
        if (oprt2_const_float && oprt2_const_float->getValue() == 0) {
            binary_instr->removeUseOps();
            binary_instr->replaceAllUse(oprt1);
            wait_delete.push_back(binary_instr);
            return oprt1;
        }
    }
    if (!can_recur) {
        return binary_instr;
    }

    // (a + b(constant)  )  + c(constant )
    // = a+ (b (const)+ c(const))
    auto oprt1_binary_instr = dynamic_cast<BinaryInst *>(oprt1);
    if (oprt1_binary_instr && oprt1_binary_instr->isAdd()) {
        auto oprt1_oprt1 = oprt1_binary_instr->getOperand(0);
        auto oprt1_oprt2 = oprt1_binary_instr->getOperand(1);
        BinaryInst *temp_instr = nullptr;
        Value *simple_instr = nullptr;
        if (oprt2->isConstant() && oprt1_oprt2->isConstant()) {
            auto oprt2_int = dynamic_cast<ConstantInt *>(oprt2);
            auto oprt1_oprt2_int = dynamic_cast<ConstantInt *>(oprt1_oprt2);
            if (oprt2_int && oprt1_oprt2_int) {
                int val1 = static_cast<int>(oprt2_int->getValue());
                int val2 = static_cast<int>(oprt1_oprt2_int->getValue());
                int sum = val1 + val2;
                if (((val1 ^ sum) & (val2 ^ sum)) < 0) {
                    return binary_instr;
                }
            }
        }
        binary_instr->removeUseOps();
        auto new_oprt2_bin = BinaryInst::createAdd(oprt1_oprt2, oprt2, nullptr);
        auto new_oprt2 =
            simplifyAdd(new_oprt2_bin, wait_delete, insert_map, false);
        auto new_add = BinaryInst::createAdd(oprt1_oprt1, new_oprt2, nullptr);
        if (new_oprt2->isConstant()) {
            insert_map.push_back({binary_instr, new_add});
        } else {
            insert_map.push_back({binary_instr, new_oprt2_bin});
            insert_map.push_back({new_oprt2_bin, new_add});
        }
        binary_instr->replaceAllUse(new_add);
        wait_delete.push_back(binary_instr);
        // wait_delete.push_back(oprt1_binary_instr);
        return new_add;
    }
    return binary_instr;
}
Value *InstructionCombination::simplifyMul(
    BinaryInst *binary_instr, std::list<Instruction *> &wait_delete,
    std::vector<std::pair<Instruction *, Instruction *>> &insert_map,
    bool can_recur) {
    MyAssert("error instr type", binary_instr->isMul(), simplifyInstrTypeError);
    auto oprt1 = binary_instr->getOperand(0);
    auto oprt2 = binary_instr->getOperand(1);
    // GVN一般需要紧跟在sccp后面
    // 保证optr2 是 constant
    if (oprt1->isConstant() && oprt2->isConstant()) {
        auto new_val = SCCP::calConstantBinary(binary_instr, oprt1, oprt2);
        binary_instr->replaceAllUse(new_val);
        binary_instr->removeUseOps();
        return new_val;
    }
    if (oprt1->isConstant()) {
        binary_instr->removeUseOps();
        binary_instr->addOperand(oprt2);
        binary_instr->addOperand(oprt1);
        oprt1 = binary_instr->getOperand(0);
        oprt2 = binary_instr->getOperand(1);
    }
    if (oprt2->isConstant()) {
        auto oprt2_const_int = dynamic_cast<ConstantInt *>(oprt2);
        if (oprt2_const_int) {
            if (oprt2_const_int->getValue() == 1) {
                binary_instr->removeUseOps();
                binary_instr->replaceAllUse(oprt1);
                wait_delete.push_back(binary_instr);
                return oprt1;
            } else if (oprt2_const_int->getValue() == 0) {
                binary_instr->removeUseOps();
                auto new_val = ConstantInt::create(0);
                binary_instr->replaceAllUse(new_val);
                wait_delete.push_back(binary_instr);
                return new_val;
            }
        }
        auto oprt2_const_float = dynamic_cast<ConstantFloat *>(oprt2);
        if (oprt2_const_float) {
            if (oprt2_const_float->getValue() == 1) {
                binary_instr->removeUseOps();
                binary_instr->replaceAllUse(oprt1);
                wait_delete.push_back(binary_instr);
                return oprt1;
            } else if (oprt2_const_float->getValue() == 0) {
                binary_instr->removeUseOps();
                auto new_val = ConstantFloat::create(0);
                binary_instr->replaceAllUse(new_val);
                wait_delete.push_back(binary_instr);
                return new_val;
            }
        }
    }
    if (!can_recur) {
        return binary_instr;
    }

    // (a + b(constant)  )  + c(constant )
    // = a+ (b (const)+ c(const))
    auto oprt1_binary_instr = dynamic_cast<BinaryInst *>(oprt1);
    if (oprt1_binary_instr && oprt1_binary_instr->isMul()) {
        auto oprt1_oprt1 = oprt1_binary_instr->getOperand(0);
        auto oprt1_oprt2 = oprt1_binary_instr->getOperand(1);
        BinaryInst *temp_instr = nullptr;
        Value *simple_instr = nullptr;
        // if (oprt2->isConstant() && oprt1_oprt2->isConstant()) {
        //     auto oprt2_int = dynamic_cast<ConstantInt *>(oprt2);
        //     auto oprt1_oprt2_int = dynamic_cast<ConstantInt *>(oprt1_oprt2);
        //     // if (oprt2_int && oprt1_oprt2_int) {
        //     //     int val1 = static_cast<int>(oprt2_int->getValue());
        //     //     int val2 = static_cast<int>(oprt1_oprt2_int->getValue());
        //     //     int sum = val1 + val2;
        //     //     if (((val1 ^ sum) & (val2 ^ sum)) < 0) {
        //     //         return binary_instr;
        //     //     }
        //     // }
        // }
        binary_instr->removeUseOps();
        auto new_oprt2_bin = BinaryInst::createMul(oprt1_oprt2, oprt2, nullptr);
        auto new_oprt2 =
            simplifyMul(new_oprt2_bin, wait_delete, insert_map, false);
        auto new_mul = BinaryInst::createMul(oprt1_oprt1, new_oprt2, nullptr);
        if (new_oprt2->isConstant()) {
            insert_map.push_back({binary_instr, new_mul});
        } else {
            insert_map.push_back({binary_instr, new_oprt2_bin});
            insert_map.push_back({new_oprt2_bin, new_mul});
        }
        binary_instr->replaceAllUse(new_mul);
        wait_delete.push_back(binary_instr);
        // wait_delete.push_back(oprt1_binary_instr);
        return new_mul;
    }
    return binary_instr;
}
Value *InstructionCombination::combineInstruction(
    Instruction *instr, std::list<Instruction *> &wait_delete,
    std::vector<std::pair<Instruction *, Instruction *>> &insert_map) {
    Value *res = nullptr;
    switch (instr->getInstructionKind()) {
        case Instruction::ADD:
            res = combineAdd(dynamic_cast<BinaryInst *>(instr), wait_delete,
                             insert_map);
            break;
        case Instruction::SUB:
            // TODO:
            break;
        case Instruction::MUL:
            // TODO:
            break;
        case Instruction::DIV:
            // TODO:
            break;
        case Instruction::REM:
            // todo
            break;
        case Instruction::CMP:
            switch (dynamic_cast<CmpInst *>(instr)->getCmpOp()) {
                case CmpInst::EQ:

                    break;

                case CmpInst::NEQ:

                    break;
                case CmpInst::GT:

                    break;
                case CmpInst::GE:

                    break;
                case CmpInst::LT:

                    break;
                case CmpInst::LE:

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
            res = instr;
            break;
        default:
            ERROR("error type", EXIT_CODE_ERROR_440);
            break;
    }
    return res;
}

// a+ b(constant)
Value *InstructionCombination::combineAdd(
    BinaryInst *binary_instr, std::list<Instruction *> &wait_delete,
    std::vector<std::pair<Instruction *, Instruction *>> &insert_map) {
    auto oprd1 = binary_instr->getOperand(0);
    auto oprd2 = binary_instr->getOperand(1);
    auto oprd1_binary = dynamic_cast<BinaryInst *>(oprd1);
    auto oprd2_binary = dynamic_cast<BinaryInst *>(oprd2);
    if (oprd1 == oprd2) {
        if (oprd1_binary) {
            auto it = _instr2value_number.find(oprd1_binary);
            if (it == _instr2value_number.end()) {
                // BinaryInst *mul = nullptr;
                // if (binary_instr->getType()->isInt32())
                //     mul = BinaryInst::createMul(
                //         oprd1, ConstantInt::create(2), nullptr);
                // else if (binary_instr->getType()->isFloatTy())
                //     mul = BinaryInst::createMul(
                //         oprd1, ConstantFloat::create(2), nullptr);
                // binary_instr->removeUseOps();
                // binary_instr->replaceAllUse(mul);
                // wait_delete.push_back(binary_instr);
                // insert_map.push_back({binary_instr, {oprd1_binary,2}});
                _instr2value_number.insert({binary_instr, {oprd1_binary, 2}});
            } else {
                auto unit_instr = it->second.first;
                auto unit_instr_number = it->second.second;
                BinaryInst *mul = nullptr;
                if (binary_instr->getType()->isInt32())
                    mul = BinaryInst::createMul(
                        unit_instr, ConstantInt::create(unit_instr_number * 2),
                        nullptr);
                else if (binary_instr->getType()->isFloatTy())
                    mul = BinaryInst::createMul(
                        unit_instr,
                        ConstantFloat::create(unit_instr_number * 2), nullptr);
                binary_instr->removeUseOps();
                binary_instr->replaceAllUse(mul);
                wait_delete.push_back(binary_instr);
                insert_map.push_back({binary_instr, mul});
                _instr2value_number.insert({mul, {oprd1_binary, 2}});
            }
        } else {
            _instr2value_number.insert({binary_instr, {oprd1, 2}});
        }
    } else {
        Value *oprd1_unit = nullptr;
        int oprd1_unit_num = 0;
        Value *oprd2_unit = nullptr;
        int oprd2_unit_num = 0;
        if (oprd1_binary) {
            auto it = _instr2value_number.find(oprd1_binary);
            if (it == _instr2value_number.end()) {
                oprd1_unit = oprd1;
                oprd1_unit_num = 1;
            } else {
                oprd1_unit = it->second.first;
                oprd1_unit_num = it->second.second;
            }
        } else {
            oprd1_unit = oprd1;
            oprd1_unit_num = 1;
        }
        if (oprd2_binary) {
            auto it = _instr2value_number.find(oprd2_binary);
            if (it == _instr2value_number.end()) {
                oprd2_unit = oprd2;
                oprd2_unit_num = 1;
            } else {
                oprd2_unit = it->second.first;
                oprd2_unit_num = it->second.second;
            }
        } else {
            oprd2_unit = oprd2;
            oprd2_unit_num = 1;
        }
        if(oprd2_unit==oprd1_unit){

                BinaryInst *mul = nullptr;
                if (binary_instr->getType()->isInt32())
                    mul = BinaryInst::createMul(
                        oprd1_unit, ConstantInt::create(oprd1_unit_num+oprd2_unit_num),
                        nullptr);
                else if (binary_instr->getType()->isFloatTy())
                    mul = BinaryInst::createMul(
                        oprd1_unit,
                        ConstantFloat::create(oprd1_unit_num+oprd2_unit_num), nullptr);
                binary_instr->removeUseOps();
                binary_instr->replaceAllUse(mul);
                wait_delete.push_back(binary_instr);
                insert_map.push_back({binary_instr, mul});
                _instr2value_number.insert(
                    {mul, {oprd1_unit, oprd1_unit_num + oprd2_unit_num}});
        }
    }
    return nullptr;
}
// std::pair
