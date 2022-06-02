
#include "instruction.h"
#include "function.h"
#include "passes/module.h"
#include "type.h"
#include "user.h"
#include "utils.h"
#include "value.h"
Instruction::Instruction(Type *type, Instruction::OpKind op_id, unsigned int op_nums, BasicBlock *parent)
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
UnaryInst::UnaryInst(Type *type, Instruction::OpKind op_id, Value *v1, BasicBlock *parent)
    : Instruction(type, op_id, 1, parent) {
    setOperand(0, v1);
}
UnaryInst *UnaryInst::createNeg(Value *v1, BasicBlock *parent) {
    return new UnaryInst(v1->getType(), Instruction::NEG, v1, parent);
}
UnaryInst *UnaryInst::createNot(Value *v1, BasicBlock *parent) {
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
BinaryInst *BinaryInst::createAdd(Value *v1, Value *v2, BasicBlock *parent) {
    //fixme: 类型没设置好
    return new BinaryInst(v1->getType(), Instruction::ADD, v1, v2, parent);
}
BinaryInst *BinaryInst::createSub(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::SUB, v1, v2, parent);
}
BinaryInst *BinaryInst::createMul(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::MUL, v1, v2, parent);
}
BinaryInst *BinaryInst::createDiv(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::DIV, v1, v2, parent);
}
BinaryInst *BinaryInst::createRem(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::REM, v1, v2, parent);
}
BinaryInst *BinaryInst::createShl(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::SHL, v1, v2, parent);
}
BinaryInst *BinaryInst::createLshr(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::LSHR, v1, v2, parent);
}
BinaryInst *BinaryInst::createAshr(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::ASHR, v1, v2, parent);
}
BinaryInst *BinaryInst::createAnd(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::AND, v1, v2, parent);
}
BinaryInst *BinaryInst::createOr(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::OR, v1, v2, parent);
}
BinaryInst *BinaryInst::createXor(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::XOR, v1, v2, parent);
}
BinaryInst::BinaryInst(Type *type, Instruction::OpKind op_id, Value *v1, Value *v2, BasicBlock *parent)
    : Instruction(type, op_id, 1, parent) {
}
ReturnInst::ReturnInst(Type *type, Instruction::OpKind op_id, Value *v) : Instruction(type, op_id, 1) {
    setOperand(0, v);
}
ReturnInst::ReturnInst(Type *type, Value *v) : Instruction(type, Instruction::RET, 1) {
    setOperand(0, v);
}
ReturnInst *ReturnInst::createRet(Value *v, BasicBlock *parent) {
    return new ReturnInst(v->getType(), v, parent);
}
ReturnInst::ReturnInst(Type *type, Value *v, BasicBlock *parent)
    : Instruction(type, OpKind::RET, 1, parent) {
    setOperand(0, v);
}
ReturnInst *ReturnInst::createVoidRet(BasicBlock *parent) {
    return new ReturnInst(Type::getVoidTy(), parent);
}
ReturnInst::ReturnInst(Type *type, BasicBlock *parent) : Instruction(type, Instruction::RET, 0, parent) {
}
CmpInst::CmpInst(Type *type, CmpInst::CmpOp op_id, Value *v1, Value *v2, BasicBlock *parent)
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
CmpInst *CmpInst::createEQ(Type *type, Value *v1, Value *v2, BasicBlock *parent) {
    return new CmpInst(type, CmpOp::EQ, v1, v2, parent);
}
CmpInst *CmpInst::createNEQ(Type *type, Value *v1, Value *v2, BasicBlock *parent) {
    return new CmpInst(type, CmpOp::NEQ, v1, v2, parent);
}
CmpInst *CmpInst::createGT(Type *type, Value *v1, Value *v2, BasicBlock *parent) {
    return new CmpInst(type, CmpOp::GT, v1, v2, parent);
}
CmpInst *CmpInst::createGE(Type *type, Value *v1, Value *v2, BasicBlock *parent) {
    return new CmpInst(type, CmpOp::GE, v1, v2, parent);
}
CmpInst *CmpInst::createLT(Type *type, Value *v1, Value *v2, BasicBlock *parent) {
    return new CmpInst(type, CmpOp::LT, v1, v2, parent);
}
CmpInst *CmpInst::createLE(Type *type, Value *v1, Value *v2, BasicBlock *parent) {
    return new CmpInst(type, CmpOp::LE, v1, v2, parent);
}
BranchInst::BranchInst(BranchInst::BrOp br_op, Value *cond, Value *true_block, Value *false_block, BasicBlock *parent)
    : Instruction(parent->getFunction()->getParent()->getInt1Ty(),
                  Instruction::BR, 3, parent),
      _br_kind(br_op) {
    setOperand(0, cond);
    setOperand(1, true_block);
    setOperand(2, false_block);
}
BranchInst::BranchInst(BranchInst::BrOp br_op, Value *cond, Value *block, BasicBlock *parent)
    : Instruction(parent->getFunction()->getParent()->getInt1Ty(),
                  Instruction::BR, 2, parent),
      _br_kind(br_op) {
    setOperand(0, cond);
    setOperand(1, block);
}
BranchInst::BranchInst(BranchInst::BrOp br_op, Value *block, BasicBlock *parent)
    : Instruction(parent->getFunction()->getParent()->getVoidTy(), Instruction::BR, 1, parent), _br_kind(br_op) {
    setOperand(0, block);
}
BranchInst *BranchInst::createIf(Value *cond, BasicBlock *true_block, BasicBlock *false_block, BasicBlock *parent) {
    return new BranchInst(BranchInst::IF, cond, true_block, false_block, parent);
}
BranchInst *BranchInst::createWhile(Value *cond, BasicBlock *block, BasicBlock *parent) {
    return new BranchInst(BranchInst::WHILE, cond, block, parent);
}
BranchInst *BranchInst::createBranch(BasicBlock *block, BasicBlock *parent) {
    return new BranchInst(BranchInst::BRANCH, block, parent);
}

StoreInst::StoreInst(Value *value, Value *ptr, BasicBlock *parent)
: Instruction(Type::getInt32PtrTy(),Instruction::STORE,2,parent){
    WARNNING("还没处理浮点类型");
    setOperand(0,value);
    setOperand(1,ptr);
}
StoreInst *StoreInst::createStore(Value *value,Value* ptr,BasicBlock *parent) {
    return  new StoreInst(value,ptr,parent);
}
LoadInst::LoadInst(Value *ptr, BasicBlock *parent)
:Instruction(Type::getInt32Ty(),Instruction::LOAD,1,parent){
    WARNNING("还没处理浮点类型");

}
LoadInst *LoadInst::createLoad(Value *ptr, BasicBlock *parent) {
    return new LoadInst(ptr,parent);
}
