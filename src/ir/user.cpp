//
// Created by yonchicy on 2022/5/25.
//

#include "user.h"
User::User(Type *type, const char *name, unsigned int use_number) : Value(name,type),_use_number(use_number){
    _operands_list.resize(use_number);
}
void User::setOperand(unsigned no, Value *v) {
    this->_operands_list.at(no) = v;
    v->addUse(this,no);
}
