
#include "user.h"
#include "utils.h"
User::User(Type *type, const char *name, unsigned int use_number) : Value(name,type),_use_number(use_number){
    _operands_list.resize(use_number);
}
void User::setOperand(unsigned no, Value *v) {
    MyAssert("out of index",no>=0&&no<_use_number);
    this->_operands_list.at(no) = v;
    v->addUse(this,no);
}