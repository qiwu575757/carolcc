#ifndef VISITOR_IR_BUILDER_H
#define VISITOR_IR_BUILDER_H

#include "basic_block.h"
#include "instruction.h"
#include "value.h"

class IRBuilder {
private:
  BasicBlock *_basic_block;
  BaseBlock *_base_block;

public:
  IRBuilder(BasicBlock *basicb,BaseBlock *baseb);
  ~IRBuilder() = default;
  BasicBlock *GetInsertBasicBlock();
  BaseBlock *GetInsertBaseBlock();
  //在该个基本块中插入指令
  void SetInstrInsertPoint(BasicBlock *bb);
  void SetBasicBlockInsertPoint(BaseBlock *bb);

  UnaryInst *createNeg(Value *v1);
  UnaryInst *createNot(Value *v1);
  BinaryInst *createAdd(Value *v1, Value *v2);
  BinaryInst *createSub(Value *v1, Value *v2);
  BinaryInst *createMul(Value *v1, Value *v2);
  BinaryInst *createDiv(Value *v1, Value *v2);
  BinaryInst *createRem(Value *v1, Value *v2);
  BinaryInst *createShl(Value *v1, Value *v2);
  BinaryInst *createLshr(Value *v1, Value *v2);
  BinaryInst *createAshr(Value *v1, Value *v2);
  BinaryInst *createAnd(Value *v1, Value *v2);
  BinaryInst *createOr(Value *v1, Value *v2);
  BinaryInst *createXor(Value *v1, Value *v2);
  CmpInst *createEQ(Type *type, Value *v1, Value *v2);
  CmpInst *createNEQ(Type *type, Value *v1, Value *v2);
  CmpInst *createGT(Type *type, Value *v1, Value *v2);
  CmpInst *createGE(Type *type, Value *v1, Value *v2);
  CmpInst *createLT(Type *type, Value *v1, Value *v2);
  CmpInst *createLE(Type *type, Value *v1, Value *v2);
  ReturnInst *createRet(Value *v);
  ReturnInst *createVoidRet();
  BranchInst *createIf(Value *cond, BasicBlock *true_block, BasicBlock *false_block);
  BranchInst *createWhile(Value *cond, BasicBlock *block);
  BranchInst *createBranch(BasicBlock *block);
  StoreInst *createStore(Value *value,Value* ptr);
  LoadInst *createLoad(Value* ptr);
};
#endif
