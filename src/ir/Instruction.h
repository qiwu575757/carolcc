
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
class UnaryInst : public Instruction {
};


#endif//COMPILER_INSTRUCTION_H
