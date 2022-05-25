
#ifndef COMPILER_INSTRUCTION_H
#define COMPILER_INSTRUCTION_H
#include "user.h"

class BaseBlock;
class User;
class Instruction : public User {
public:
    enum OpKind{
        ADD,
        SUB,
    };

    Instruction(Type* type,BaseBlock *parent,OpKind op_id,unsigned op_nums);
private:
    BaseBlock *_parent;
    OpKind _op_id;

};



#endif//COMPILER_INSTRUCTION_H
