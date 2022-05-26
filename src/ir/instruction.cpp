
#include "function.h"
#include "instruction.h"
#include "type.h"
#include "user.h"
#include "utils.h"
#include "value.h"
Instruction::Instruction(Type *type, Instruction::OpKind op_id, unsigned int op_nums, BaseBlock *parent)
    : User(type, "", op_nums), _parent(parent), _op_id(op_id) {
    MyAssert("null parent", _parent != nullptr);
    _parent->addInstruction(this);
}
Instruction::Instruction(Type *type, Instruction::OpKind op_id, unsigned int op_nums)
    : Instruction(type, op_id, op_nums, nullptr) {
}
UnaryInst::UnaryInst(Type *type, OpKind op_id, Value *v1) : Instruction(type, op_id, 1) {
    setOperand(0, v1);
}
UnaryInst::UnaryInst(Type *type, Instruction::OpKind op_id, Value *v1, BaseBlock *parent)
    : Instruction(type, op_id, 1, parent) {
    setOperand(0, v1);
}
UnaryInst *UnaryInst::createNeg(Value *v1, BaseBlock *parent) {
    return new UnaryInst(v1->getType(), Instruction::NEG, v1, parent);
}
UnaryInst *UnaryInst::createNot(Value *v1, BaseBlock *parent) {
    return new UnaryInst(v1->getType(), Instruction::NOT, v1, parent);
}

BinaryInst::BinaryInst(Type *type, Instruction::OpKind op_id, Value *v1, Value *v2)
    : Instruction(type, op_id, 2) {
    setOperand(0, v1);
    setOperand(1, v2);
}
//BinaryInst *BinaryInst::createAdd(Value *v1, Value *v2) {
//    //fixme: 类型没设置好
//    return new BinaryInst(v1->getType(), Instruction::ADD, v1, v2);
//}
//BinaryInst *BinaryInst::createSub(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::SUB, v1, v2);
//}
//BinaryInst *BinaryInst::createMul(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::MUL, v1, v2);
//}
//BinaryInst *BinaryInst::createDiv(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::DIV, v1, v2);
//}
//BinaryInst *BinaryInst::createRem(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::REM, v1, v2);
//}
//BinaryInst *BinaryInst::createShl(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::SHL, v1, v2);
//}
//BinaryInst *BinaryInst::createLshr(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::LSHR, v1, v2);
//}
//BinaryInst *BinaryInst::createAshr(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::ASHR, v1, v2);
//}
//BinaryInst *BinaryInst::createAnd(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::AND, v1, v2);
//}
//BinaryInst *BinaryInst::createOr(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::OR, v1, v2);
//}
//BinaryInst *BinaryInst::createXor(Value *v1, Value *v2) {
//    return new BinaryInst(v1->getType(), Instruction::XOR, v1, v2);
//}
BinaryInst *BinaryInst::createAdd(Value *v1, Value *v2, BaseBlock *parent) {
    //fixme: 类型没设置好
    return new BinaryInst(v1->getType(), Instruction::ADD, v1, v2, parent);
}
BinaryInst *BinaryInst::createSub(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::SUB, v1, v2, parent);
}
BinaryInst *BinaryInst::createMul(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::MUL, v1, v2, parent);
}
BinaryInst *BinaryInst::createDiv(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::DIV, v1, v2, parent);
}
BinaryInst *BinaryInst::createRem(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::REM, v1, v2, parent);
}
BinaryInst *BinaryInst::createShl(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::SHL, v1, v2, parent);
}
BinaryInst *BinaryInst::createLshr(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::LSHR, v1, v2, parent);
}
BinaryInst *BinaryInst::createAshr(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::ASHR, v1, v2, parent);
}
BinaryInst *BinaryInst::createAnd(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::AND, v1, v2, parent);
}
BinaryInst *BinaryInst::createOr(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::OR, v1, v2, parent);
}
BinaryInst *BinaryInst::createXor(Value *v1, Value *v2, BaseBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::XOR, v1, v2, parent);
}
BinaryInst::BinaryInst(Type *type, Instruction::OpKind op_id, Value *v1, Value *v2, BaseBlock *parent)
    : Instruction(type, op_id, 1, parent) {
}
ReturnInst::ReturnInst(Type *type, Instruction::OpKind op_id, Value *v) : Instruction(type, op_id, 1) {
    setOperand(0, v);
}
ReturnInst::ReturnInst(Type *type, Value *v) : Instruction(type, Instruction::RET, 1) {
    setOperand(0, v);
}
ReturnInst *ReturnInst::createRet(Value *v, BaseBlock *parent) {
    return new ReturnInst(v->getType(), v, parent);
}
ReturnInst::ReturnInst(Type *type, Value *v, BaseBlock *parent)
    : Instruction(type, OpKind::RET, 1, parent) {
    setOperand(0, v);
}
ReturnInst *ReturnInst::createVoidRet(BaseBlock *parent) {
    return new ReturnInst(Type::getVoidTy(parent->getFunction()->getParent()),parent);
}
ReturnInst::ReturnInst(Type *type,BaseBlock*parent) : Instruction(type,Instruction::RET,0,parent){
}
CmpInst::CmpInst(Type *type, CmpInst::CmpOp op_id, Value *v1, Value *v2, BaseBlock *parent)
    : Instruction(type, Instruction::CMP, 2, parent) {
    setOperand(0, v1);
    setOperand(1, v2);
}
CmpInst::CmpInst(Type *type, CmpInst::CmpOp op_id, Value *v1, Value *v2)
    : Instruction(type, Instruction::CMP, 2) {
    setOperand(0, v1);
    setOperand(1, v2);
}
//CmpInst *CmpInst::createCmp(Type *type, CmpInst::CmpOp op_id, Value *v1, Value *v2) {
//    return new CmpInst(type, op_id, v1, v2);
//}
//CmpInst *CmpInst::createEQ(Type *type, Value *v1, Value *v2) {
//    return new CmpInst(type, CmpOp::EQ, v1, v2);
//}
//CmpInst *CmpInst::createNEQ(Type *type, Value *v1, Value *v2) {
//    return new CmpInst(type, CmpOp::NEQ, v1, v2);
//}
//CmpInst *CmpInst::createGT(Type *type, Value *v1, Value *v2) {
//    return new CmpInst(type, CmpOp::GT, v1, v2);
//}
//CmpInst *CmpInst::createGE(Type *type, Value *v1, Value *v2) {
//    return new CmpInst(type, CmpOp::GE, v1, v2);
//}
//CmpInst *CmpInst::createLT(Type *type, Value *v1, Value *v2) {
//    return new CmpInst(type, CmpOp::LT, v1, v2);
//}
//CmpInst *CmpInst::createLE(Type *type, Value *v1, Value *v2) {
//    return new CmpInst(type, CmpOp::LE, v1, v2);
//}
CmpInst *CmpInst::createEQ(Type *type, Value *v1, Value *v2, BaseBlock *parent) {
    return new CmpInst(type, CmpOp::EQ, v1, v2, parent);
}
CmpInst *CmpInst::createNEQ(Type *type, Value *v1, Value *v2, BaseBlock *parent) {
    return new CmpInst(type, CmpOp::NEQ, v1, v2, parent);
}
CmpInst *CmpInst::createGT(Type *type, Value *v1, Value *v2, BaseBlock *parent) {
    return new CmpInst(type, CmpOp::GT, v1, v2, parent);
}
CmpInst *CmpInst::createGE(Type *type, Value *v1, Value *v2, BaseBlock *parent) {
    return new CmpInst(type, CmpOp::GE, v1, v2, parent);
}
CmpInst *CmpInst::createLT(Type *type, Value *v1, Value *v2, BaseBlock *parent) {
    return new CmpInst(type, CmpOp::LT, v1, v2, parent);
}
CmpInst *CmpInst::createLE(Type *type, Value *v1, Value *v2, BaseBlock *parent) {
    return new CmpInst(type, CmpOp::LE, v1, v2, parent);
}
