#include "value.h"
#include "user.h"
void Value::addUse(User *user, unsigned value_num) {
    this->_user_list.push_back(new Use(this, value_num, user));
}
Value::Value(const std::string &name, Type *type) : _name(name), _type(type) {}
Value::Value(Type *type, const std::string &name):_type(type),_name(name) {
}
Type *Value::getType() {
    return _type;
}
std::string Value::getName() {
    return _name;
}
