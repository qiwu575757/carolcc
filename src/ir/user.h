#ifndef COMPILER_USER_H
#define COMPILER_USER_H

#include "value.h"
#include <vector>
class Value;
class Use {
public:
    Value *_user;
    Value *_value;
    unsigned _value_no;// 作为第几个参数使用
    Use(Value *value, unsigned value_no, Value *user) : _user(user), _value(value), _value_no(value_no) {}
};
class User : public Value {
public:
    void accept(IrVisitorBase *v) override;
    User(Type *type, const std::string &name, unsigned use_number);
    /**
     *  设置User类的参数,同时维护 v的use链
     * @param no  是第几个参数
     * @param v  具体的参数
     */
    void setOperand(unsigned no, Value *v);
    void addOperand( Value *v);
    void rmOperand(unsigned  i1,unsigned  i2) ;
    void rmOperand(unsigned  index);
    Value *getOperand(unsigned no) const;
    std::vector<Value*>& getOperandList(){return _operands_list;}
    unsigned getOperandNumber() const { return _use_number; };
/**
 * 删掉 自己参数被使用的记录
 */
    void removeUseOps();

private:
    std::vector<Value *> _operands_list;
    unsigned _use_number;// 使用的变量的数量
};

#endif//COMPILER_USER_H
