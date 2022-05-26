
#include "Instruction.h"
#include "user.h"
#include "value.h"
Instruction::Instruction(Type *type, BaseBlock *parent, Instruction::OpKind op_id, unsigned int op_nums)
    : User(type, "", op_nums), _parent(parent), _op_id(op_id) {
}
Instruction::Instruction(Type *type, Instruction::OpKind op_id, unsigned int op_nums) : Instruction(type, nullptr, op_id, op_nums) {}
UnaryInst::UnaryInst(Type *type, OpKind op_id, Value *v1) : Instruction(type, op_id, 1) {
    setOperand(0, v1);
}
UnaryInst *UnaryInst::createNeg(Value *v1) {
    return new UnaryInst(v1->getType(), Instruction::NEG, v1);
}
UnaryInst *UnaryInst::createNot(Value *v1) {
    return new UnaryInst(v1->getType(), Instruction::NOT, v1);
}

BinaryInst::BinaryInst(Type *type, Instruction::OpKind op_id, Value *v1, Value *v2)
    : Instruction(type, op_id, 2) {
    setOperand(0, v1);
    setOperand(1, v2);
}
BinaryInst *BinaryInst::createAdd(Value *v1, Value *v2) {
    //fixme: 类型没设置好
    return new BinaryInst(v1->getType(), Instruction::ADD, v1, v2);
}
BinaryInst *BinaryInst::createSub(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::SUB, v1, v2);
}
BinaryInst *BinaryInst::createMul(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::MUL, v1, v2);
}
BinaryInst *BinaryInst::createDiv(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::DIV, v1, v2);
}
BinaryInst *BinaryInst::createRem(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::REM, v1, v2);
}
BinaryInst *BinaryInst::createShl(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::SHL, v1, v2);
}
BinaryInst *BinaryInst::createLshr(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::LSHR, v1, v2);
}
BinaryInst *BinaryInst::createAshr(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::ASHR, v1, v2);
}
BinaryInst *BinaryInst::createAnd(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::AND, v1, v2);
}
BinaryInst *BinaryInst::createOr(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::OR, v1, v2);
}
BinaryInst *BinaryInst::createXor(Value *v1, Value *v2) {
    return new BinaryInst(v1->getType(), Instruction::XOR, v1, v2);
}
RetInstr::RetInstr(Type *type, Instruction::OpKind op_id, Value *v) : Instruction(type, op_id, 1) {
    setOperand(0, v);
}
RetInstr::RetInstr(Type *type, Value *v) : Instruction(type, Instruction::RET, 1) {
    setOperand(0, v);
}
RetInstr *RetInstr::createRet(Value *v) {
    return new RetInstr(v->getType(), Instruction::RET, v);
}
CmpInst::CmpInst(Type *type, CmpInst::CmpOp op_id, Value *v1, Value *v2) : Instruction(type, Instruction::CMP, ) {
}
