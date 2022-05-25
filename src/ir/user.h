#ifndef COMPILER_USER_H
#define COMPILER_USER_H

#include "value.h"
#include <vector>
class Value;
class Use {
private:
    Value *_user;
    Value *_value;
    unsigned _value_no; // 作为第几个参数使用
public:
    Use(Value* value,unsigned value_no,Value* user):_value(value),_user(user),_value_no(value_no){}
};
class User : public Value {
public:
    User(Type* type, const char *name,unsigned use_number);


private:
    std::vector<Value*> _use_list;
    unsigned _use_number; // 使用的变量的数量

};

#endif//COMPILER_USER_H
