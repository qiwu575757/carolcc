#ifndef COMPILER_INSTRUCTION_H
#define COMPILER_INSTRUCTION_H
#include "basicblock.h"
#include "user.h"

class BasicBlock;
class User;
class Instruction : public User {
public:
    enum OpKind {
        // terminator instructions
        RET,
        BR,

        // unary instructions
        NEG,
        NOT,
        // binary instructions
        ADD,
//        FADD,//float add
        SUB,
//        FSUB,
        MUL,
        DIV,
        REM,
//        FMUL,

        // bitwise binary instructions
        SHL,
        LSHR,
        ASHR,
        AND,
        OR,
        XOR,


        // memory access nad addressing operations
        ALLOCA,
        LOAD,
        STORE,

        // other instructions
        CMP,
        PHI,
        GEP, // get element ptr

    };


    Instruction(Type *type, OpKind op_id, unsigned op_nums);
    Instruction(Type *type, OpKind op_id, unsigned op_nums, BasicBlock *parent);

private:
    BasicBlock *_parent;
    OpKind _op_id;
};

class UnaryInst : public Instruction {
private:
    UnaryInst(Type *type, OpKind op_id, Value *v1);
    UnaryInst(Type *type, OpKind op_id, Value *v1, BasicBlock *parent);

public:
    static UnaryInst *createNeg(Value *v1, BasicBlock *parent);
    static UnaryInst *createNot(Value *v1, BasicBlock *parent);
};

class BinaryInst : public Instruction {
private:
    BinaryInst(Type *type, OpKind op_id, Value *v1, Value *v2);
    BinaryInst(Type *type, OpKind op_id, Value *v1, Value *v2, BasicBlock *parent);

public:
    static BinaryInst *createAdd(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createSub(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createMul(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createDiv(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createRem(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createShl(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createLshr(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createAshr(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createAnd(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createOr(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createXor(Value *v1, Value *v2, BasicBlock *parent);
};

class CmpInst : public Instruction {
public:
    enum CmpOp {
        EQ,
        NEQ,
        GT,
        GE,
        LT,
        LE
    };
    //    static CmpInst *createCmp(Type *type,CmpOp op_id,Value *v1,Value *v2);
    static CmpInst *createEQ(Type *type, Value *v1, Value *v2, BasicBlock *parent);
    static CmpInst *createNEQ(Type *type, Value *v1, Value *v2, BasicBlock *parent);
    static CmpInst *createGT(Type *type, Value *v1, Value *v2, BasicBlock *parent);
    static CmpInst *createGE(Type *type, Value *v1, Value *v2, BasicBlock *parent);
    static CmpInst *createLT(Type *type, Value *v1, Value *v2, BasicBlock *parent);
    static CmpInst *createLE(Type *type, Value *v1, Value *v2, BasicBlock *parent);

private:
    CmpInst(Type *type, CmpOp op_id, Value *v1, Value *v2);
    CmpInst(Type *type, CmpOp op_id, Value *v1, Value *v2, BasicBlock *parent);
};
class ReturnInst : public Instruction {
private:
    ReturnInst(Type *type ,BasicBlock*parent);
    ReturnInst(Type *type, Value *v);
    ReturnInst(Type *type, Value *v, BasicBlock *parent);
    ReturnInst(Type *type, OpKind op_id, Value *v);

public:
    static ReturnInst *createRet(Value *v,BasicBlock *parent);
    static ReturnInst *createVoidRet(BasicBlock *parent);
};

class BranchInst : public Instruction {
public:
    enum BrOp {
        WHILE,
        IF,
        BRANCH,
    };

private:
    BrOp _br_kind;
    BranchInst(BrOp br_op, Value *cond, Value *true_block, Value *false_block, BasicBlock *parent);
    BranchInst(BrOp br_op, Value *cond, Value *block, BasicBlock *parent);
    BranchInst(BrOp br_op, Value *block, BasicBlock *parent);

public:
    static BranchInst *createIf(Value *cond, BasicBlock *true_block, BasicBlock *false_block, BasicBlock *parent);
    static BranchInst *createWhile(Value *cond, BasicBlock *block, BasicBlock *parent);
    static BranchInst *createBranch(BasicBlock *block, BasicBlock *parent);
};

class StoreInst : public Instruction{
private:
    StoreInst(Value *value,Value* ptr,BasicBlock *parent);
public:
    static StoreInst *createStore(Value *value,Value* ptr,BasicBlock *parent);
};
class LoadInst:public Instruction{
private:
    LoadInst(Value* ptr,BasicBlock *parent);
public:
    static LoadInst *createLoad(Value* ptr,BasicBlock *parent);
};
//class GetElementPtrInst:public Instruction{
//private:
//    GetElementPtrInst(Type* ty,Value* array_ptr,)
//};

#endif//COMPILER_INSTRUCTION_H
