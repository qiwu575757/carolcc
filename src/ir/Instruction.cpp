
#include "Instruction.h"
#include "user.h"
Instruction::Instruction(Type *type, BaseBlock *parent, Instruction::OpKind op_id, unsigned int op_nums)
    : User(type, "", op_nums), _parent(parent), _op_id(op_id) {
}
