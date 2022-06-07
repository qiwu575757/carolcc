
#include "user.h"
#include "visitor/ir_visitor_base.h"
#include "utils.h"
User::User(Type *type, const std::string &name, unsigned int use_number) : Value(name,type),_use_number(use_number){
    _operands_list.resize(use_number);
}
void User::setOperand(unsigned no, Value *v) {
    
    MyAssert("out of index",no>=0&&no<_use_number);
    // MyAssert("null ptr",v!=nullptr);
    this->_operands_list.at(no) = v;
    v->addUse(this,no);
}
Value *User::getOperand(unsigned int no)const {
    MyAssert("out of index",no>=0&&no<_use_number);
    return _operands_list.at(no);
}
void User::accept(IrVisitorBase *v) {
    v->visit(this);
}
