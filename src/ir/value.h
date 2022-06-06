#ifndef COMPILER_VALUE_H
#define COMPILER_VALUE_H
#include "helpers/type_helper.h"
#include <string>
#include <vector>
class IrVisitorBase;
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
    Value(const std::string &name, Type *type);
    Value(Type *type, const std::string &name);
    Type* getType() const;
    void addUse(User * user,unsigned value_num);
    std::string getName();
    virtual void accept(IrVisitorBase *v);
};




#endif// !COMPILER_VALUE_H