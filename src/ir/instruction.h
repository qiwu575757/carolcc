#ifndef COMPILER_INSTRUCTION_H
#define COMPILER_INSTRUCTION_H
#include "baseblock.h"
#include "user.h"

class BaseBlock;
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
        FADD,//float add
        SUB,
        FSUB,
        MUL,
        DIV,
        REM,
        FMUL,

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
        PHI

    };


    Instruction(Type *type, OpKind op_id, unsigned op_nums);
    Instruction(Type *type, OpKind op_id, unsigned op_nums, BaseBlock *parent);

private:
    BaseBlock *_parent;
    OpKind _op_id;
};

class UnaryInst : public Instruction {
private:
    UnaryInst(Type *type, OpKind op_id, Value *v1);
    UnaryInst(Type *type, OpKind op_id, Value *v1, BaseBlock *parent);

public:
    static UnaryInst *createNeg(Value *v1, BaseBlock *parent);
    static UnaryInst *createNot(Value *v1, BaseBlock *parent);
};

class BinaryInst : public Instruction {
private:
    BinaryInst(Type *type, OpKind op_id, Value *v1, Value *v2);
    BinaryInst(Type *type, OpKind op_id, Value *v1, Value *v2, BaseBlock *parent);

public:
    static BinaryInst *createAdd(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createSub(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createMul(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createDiv(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createRem(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createShl(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createLshr(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createAshr(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createAnd(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createOr(Value *v1, Value *v2, BaseBlock *parent);
    static BinaryInst *createXor(Value *v1, Value *v2, BaseBlock *parent);
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
    static CmpInst *createEQ(Type *type, Value *v1, Value *v2, BaseBlock *parent);
    static CmpInst *createNEQ(Type *type, Value *v1, Value *v2, BaseBlock *parent);
    static CmpInst *createGT(Type *type, Value *v1, Value *v2, BaseBlock *parent);
    static CmpInst *createGE(Type *type, Value *v1, Value *v2, BaseBlock *parent);
    static CmpInst *createLT(Type *type, Value *v1, Value *v2, BaseBlock *parent);
    static CmpInst *createLE(Type *type, Value *v1, Value *v2, BaseBlock *parent);

private:
    CmpInst(Type *type, CmpOp op_id, Value *v1, Value *v2);
    CmpInst(Type *type, CmpOp op_id, Value *v1, Value *v2, BaseBlock *parent);
};
class ReturnInst : public Instruction {
private:
    ReturnInst(Type *type ,BaseBlock*parent);
    ReturnInst(Type *type, Value *v);
    ReturnInst(Type *type, Value *v, BaseBlock *parent);
    ReturnInst(Type *type, OpKind op_id, Value *v);

public:
    static ReturnInst *createRet(Value *v,BaseBlock *parent);
    static ReturnInst *createVoidRet(BaseBlock *parent);
};

class BranchInst : public Instruction {

};


#endif//COMPILER_INSTRUCTION_H
