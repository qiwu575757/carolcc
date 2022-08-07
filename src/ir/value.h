#ifndef COMPILER_VALUE_H
#define COMPILER_VALUE_H
#include "helpers/type_helper.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
class IrVisitorBase;
class Use;
class User;
class Value;
class Type;
class Variable;
class Function;
class BasicBlock;
class Instruction;

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
 virtual Type *getType() const;
 void addUse(User *user, unsigned value_num);
 void replaceAllUse(Value *new_value);  //将对某个值的使用全部替换为新值
 void removeUse(Value *val, unsigned arg_no);
 std::string getName()const;
 std::list<Use *> &getUseList() { return _user_list; }
 void setName(std::string name);
 virtual void accept(IrVisitorBase *v);
 virtual std::string getPrintName() { return "%" + getName(); }
};

class ValueCloner{
public:
    Function *copyFunc(Function* old_func);
    // 仅仅负责复制basicblock里的指令
    void copyBasicBlock(BasicBlock* old_bb);
    // 复制 instr
    Instruction* copyInstr(Instruction * old_instr);
    Value* findValue(Value* old_val);

   private:
    std::map<Value *, Value *> _old2new;
    // static std::set<BasicBlock*> _visited_bb;
};
#endif  // !COMPILER_VALUE_H