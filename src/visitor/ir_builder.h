#ifndef VISITOR_IR_BUILDER_H
#define VISITOR_IR_BUILDER_H

#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "ir/value.h"

class IRBuilder {
private:
    BasicBlock *_basic_block; //注意这个块和父基本块不是父子关系 只是用于插入指令
    BaseBlock *_father_block;
    std::list<BaseBlock *>* _base_block_list;
public:
    IRBuilder(BasicBlock *basicb, BaseBlock *baseb);
    ~IRBuilder() = default;
    // 获得用于插入指令的基本块，注意这个块和父基本块不是父子关系
    BasicBlock *GetInsertBasicBlock();
    // 获得基本块列表的父基本快
    BaseBlock *GetBaseBlockFatherBlock();
    // 获得基本块列表
    std::list<BaseBlock *>* GetInsertBaseBlockList();
    //在该基本块中插入指令
    void SetInstrInsertPoint(BasicBlock *bb);
    //在该基本块表中插入基本块 这个是在设定父基本块时将对应的基本块表设定进来
    void SetBasicBlockInsertPoint(std::list<BaseBlock *>*);
    //设定父基本块 方便维护父子关系
    void SetBaseBlockFatherBlock(BaseBlock *bb);
    //将baseblock插入基本块列表，并同时维护父子块关系
    void pushBaseBlock(BaseBlock *bb);

//    UnaryInst *createNeg(Value *v1);
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
    StoreInst *createStore(Value *value, Value *ptr);
    LoadInst *createLoad(Value *ptr);
    GetElementPtrInst *createGEP(Value *ptr, std::vector<Value *> idxs);
    CallInst *createCall(Function *func, std::vector<Value *> &args);
    ZExtInst *creatZExtInst(Type *ty, Value *val);
    AllocaInst *createAlloca(Type *ty);
    HIR* createBreak();
    HIR* createContinue();
};
#endif
