
#ifndef COMPILER_INSTRUCTION_H
#define COMPILER_INSTRUCTION_H
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
    Instruction(Type *type, BaseBlock *parent, OpKind op_id, unsigned op_nums);

private:
    BaseBlock *_parent;
    OpKind _op_id;
};

class UnaryInst : public Instruction{
private:
    UnaryInst(Type* type,OpKind op_id,Value *v1);

public:
    static UnaryInst *createNeg(Value *v1);
    static UnaryInst *createNot(Value *v1);

};

class BinaryInst : public Instruction {
private:
    BinaryInst(Type *type, OpKind op_id, Value *v1, Value *v2);

public:
    static BinaryInst *createAdd(Value *v1, Value *v2);
    static BinaryInst *createSub(Value *v1, Value *v2);
    static BinaryInst *createMul(Value *v1, Value *v2);
    static BinaryInst *createDiv(Value *v1, Value *v2);
    static BinaryInst *createRem(Value *v1, Value *v2);
    static BinaryInst *createShl(Value *v1, Value *v2);
    static BinaryInst *createLshr(Value *v1, Value *v2);
    static BinaryInst *createAshr(Value *v1, Value *v2);
    static BinaryInst *createAnd(Value *v1, Value *v2);
    static BinaryInst *createOr(Value *v1, Value *v2);
    static BinaryInst *createXor(Value *v1, Value *v2);
};

class CmpInst : public Instruction{
public:
    enum CmpOp{
        EQ,
        NEQ,
        GT,
        GE,
        LT,
        LE
    };
private:
    CmpInst(Type* type,CmpOp op_id,Value *v1,Value *v2);

};
class RetInstr : public Instruction{
private:
    RetInstr(Type * type,Value *v);
    RetInstr(Type * type,OpKind op_id,Value *v);
public:
    static RetInstr *createRet(Value *v);
};


#endif//COMPILER_INSTRUCTION_H
