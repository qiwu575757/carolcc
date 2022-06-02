#include "ir_builder.h"

IRBuilder::IRBuilder(BasicBlock *basicb,BaseBlock *baseb) 
{ 
    this->_basic_block = basicb; 
    this->_base_block = baseb; 
};
BaseBlock *IRBuilder::GetInsertBaseBlock() 
{
    return this->_base_block; 
}
BasicBlock *IRBuilder::GetInsertBasicBlock() 
{
    return this->_basic_block; 
}
void IRBuilder::SetBasicBlockInsertPoint(BaseBlock *bb)
{
    this->_base_block = bb;
}
void IRBuilder::SetInstrInsertPoint(BasicBlock *bb) 
{
    this->_basic_block = bb;
} //
  
  UnaryInst * IRBuilder::createNeg(Value *v1)
  {
      return UnaryInst::createNeg(v1, this->_basic_block);
  }
  UnaryInst * IRBuilder::createNot(Value *v1)
  {
      return UnaryInst::createNot(v1, this->_basic_block);
  }
  BinaryInst * IRBuilder::createAdd(Value *v1, Value *v2)
  {
      return BinaryInst::createAdd(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createSub(Value *v1, Value *v2)
  {
      return BinaryInst::createSub(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createMul(Value *v1, Value *v2)
  {
      return BinaryInst::createMul(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createDiv(Value *v1, Value *v2)
  {
      return BinaryInst::createDiv(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createRem(Value *v1, Value *v2)
  {
      return BinaryInst::createRem(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createShl(Value *v1, Value *v2)
  {
      return BinaryInst::createShl(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createLshr(Value *v1, Value *v2)
  {
      return BinaryInst::createLshr(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createAshr(Value *v1, Value *v2)
  {
      return BinaryInst::createAshr(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createAnd(Value *v1, Value *v2)
  {
      return BinaryInst::createAnd(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createOr(Value *v1, Value *v2)
  {
      return BinaryInst::createOr(v1, v2, this->_basic_block);
  }
  BinaryInst * IRBuilder::createXor(Value *v1, Value *v2)
  {
      return BinaryInst::createXor(v1, v2, this->_basic_block);
  }
  CmpInst * IRBuilder::createEQ(Type *type, Value *v1, Value *v2)
  {
      return CmpInst::createEQ(type, v1, v2, this->_basic_block);
  }
  CmpInst * IRBuilder::createNEQ(Type *type, Value *v1, Value *v2)
  {
      return CmpInst::createNEQ(type, v1, v2, this->_basic_block);
  }
  CmpInst * IRBuilder::createGT(Type *type, Value *v1, Value *v2)
  {
      return CmpInst::createGT(type, v1, v2, this->_basic_block);
  }
  CmpInst * IRBuilder::createGE(Type *type, Value *v1, Value *v2)
  {
      return CmpInst::createGE(type, v1, v2, this->_basic_block);
  }
  CmpInst * IRBuilder::createLT(Type *type, Value *v1, Value *v2)
  {
      return CmpInst::createLT(type, v1, v2, this->_basic_block);
  }
  CmpInst * IRBuilder::createLE(Type *type, Value *v1, Value *v2)
  {
      return CmpInst::createLE(type, v1, v2, this->_basic_block);
  }
  ReturnInst * IRBuilder::createRet(Value *v)
  {
      return ReturnInst::createRet(v,this->_basic_block);
  }
  ReturnInst * IRBuilder::createVoidRet()
  {
      return ReturnInst::createVoidRet(this->_basic_block);
  }
  BranchInst * IRBuilder::createIf(Value *cond, BasicBlock *true_block, BasicBlock *false_block)
  {
      return BranchInst::createIf(cond, true_block, false_block, this->_basic_block);
  }
  BranchInst * IRBuilder::createWhile(Value *cond, BasicBlock *block)
  {
      return BranchInst::createWhile(cond, block, this->_basic_block);
  }
  BranchInst * IRBuilder::createBranch(BasicBlock *block)
  {
      return BranchInst::createBranch(block, this->_basic_block);
  }
  StoreInst * IRBuilder::createStore(Value *value,Value* ptr)
  {
      return StoreInst::createStore(value, ptr,this->_basic_block);
  }
  LoadInst * IRBuilder::createLoad(Value* ptr)
  {
      return LoadInst::createLoad( ptr,this->_basic_block);
  }
  GetElementPtrInst *IRBuilder::createGEP(Value *ptr, std::vector<Value *> &idxs) {
      return GetElementPtrInst::createGEP( ptr, idxs,this->_basic_block);
  }
  CallInst *IRBuilder::createCall(Function *func, std::vector<Value *> &args) {
      return CallInst::createCall( func, args,this->_basic_block);
  }
  ZExtInst *IRBuilder::creatZExtInst(Type *ty, Value *val) {
      return ZExtInst::creatZExtInst(ty,val,this->_basic_block);
  }
  AllocaInst *IRBuilder::createAlloca(Type *ty) {
      return AllocaInst::createAlloca(ty,this->_basic_block);
  }
