#ifndef COMPILER_VALUE_H
#define COMPILER_VALUE_H
#include "helpers/type_helper.h"
#include <string>
#include <vector>
#include <list>
class IrVisitorBase;
class Use;
class User;
class Value;
class Type;
class Variable;

class Value {
private:
    std::list<Use *> _user_list;
    std::string _name;
    Type *_type;

public:
 static Value *getUncertainValue();
 Value(const std::string &name, Type *type);
 Value(Type *type, const std::string &name);
 bool isConstant();
 Type *getType() const;
 void addUse(User *user, unsigned value_num);
 void replaceAllUse(Value *new_value);  //将对某个值的使用全部替换为新值
 void removeUse(Value *val, unsigned arg_no);
 std::string getName();
 std::list<Use *> &getUseList() { return _user_list; }
 void setName(std::string name);
 virtual void accept(IrVisitorBase *v);
 virtual std::string getPrintName() { return "%" + getName(); }
};

#endif  // !COMPILER_VALUE_H