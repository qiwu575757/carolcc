#ifndef COMPILER_VALUE_H
#define COMPILER_VALUE_H
#include "helpers/type_helper.h"
#include <string>
#include <vector>
class Use;
class User;
class Value;
class Type;
class Variable;
class Value {
private:
    std::vector<Use *> _user_list;
    std::string _name;
    Type *_type;

public:
    Value(const std::string &name, Type *type);;
    Value(Type *type, const std::string &name);;
    void AddUse(User * user,unsigned value_num);
};




#endif// !COMPILER_VALUE_H