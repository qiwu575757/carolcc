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

void Value::replaceAllUse(Value *new_val) {
    for (auto use : _user_list) {
        auto user = dynamic_cast<User *>(use->_user);
        user->setOperand(use->_value_no, new_val);
    }
    this->_user_list.clear();
}

void Value::removeUse(Value *val, unsigned value_no) {
    Use *remove_use;
    for (auto use : _user_list) {
        // 这里需要检查
        if (use->_user == val)
        {
            remove_use = use;
            break;
        }
    }
    auto iter = std::find(_user_list.begin(), _user_list.end(), remove_use);

    _user_list.erase(iter);
}
bool Value::isConstant(){
        auto constant = dynamic_cast<Constant*>(this);
        return constant != nullptr;
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
