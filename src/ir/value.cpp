#include "value.h"
#include "user.h"
#include "utils.h"
#include "visitor/ir_visitor_base.h"
void Value::addUse(User *user, unsigned value_num) {
    this->_user_list.push_back(new Use(this, value_num, user));
}
Value::Value(const std::string &name, Type *type) : _name(name), _type(type) {}
Value::Value(Type *type, const std::string &name):_type(type),_name(name) {
}
Type *Value::getType() const{
    return _type;
}
std::string Value::getName() {
    return _name;
}
void Value::accept(IrVisitorBase *v) {
    v->visit(this);
}
void Value::setName(std::string name) {
    if(!_name.empty()){
        WARNNING("rename the value:\"%s\" -> \"%s\"",_name.c_str(),name.c_str());
    }
    _name= name;
}
