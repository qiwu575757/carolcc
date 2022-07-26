#ifndef COMPILER_CONSTANT_H
#define COMPILER_CONSTANT_H

#include "type.h"
#include "user.h"
#include "value.h"
#include <iostream>
#include <sstream>
#include <string>

class ArrayType;

class Constant : public User {
public:
    explicit Constant(Type *ty, const std::string &name = "", unsigned num_ops = 0)
        : User(ty, name, num_ops) {}
    // virtual void print() override;
    void accept(IrVisitorBase *v) override;
    bool isZero();
};

class ConstantInt : public Constant {
private:
public:
    void accept(IrVisitorBase *v) override;
    ConstantInt(Type *ty, long value) : Constant(ty, "", 0), _value(value) {}

private:
    long _value;

public:

    long getValue() const;
    void setValue(int value);
    static ConstantInt *create(long val);
    std::string getPrintName()override{
        return std::to_string(_value);
    }
    static ConstantInt *get(int val);
    static ConstantInt *getBool(bool val);
};

class ConstantFloat : public Constant {
private:
    float _value;
    ConstantFloat(Type *ty, float value) : Constant(ty, "", 0), _value(value) {
        _union_number._value=value;
    }
    union format{
        double _value;
        unsigned  long _hex_number;
    }_union_number{};

public:
    void accept(IrVisitorBase *v) override;
    std::string getPrintName()override;

public:
    float getValue() const;
    void setValue(float value);
    static ConstantFloat *create(float val);
};

class ConstantArray : public Constant {
private:
    std::vector<Constant *> _const_array;

public:
    void accept(IrVisitorBase *v) override;

private:
    ConstantArray(ArrayType *ty, const std::vector<Constant *> &values);

public:
    static ConstantArray *create(ArrayType *ty, const std::vector<Constant *> &values);

    /***
     *  将线性的array_init根据bounds规定的形状变成多维的
     * @param array_bounds 指定维度
     * @param array_init  线性内容
     * @return  构造好的存有内容的多维数组
     */
    static ConstantArray *turn(Type *basic_type,
                               std::vector<int32_t> &array_bounds,
                               std::vector<Value *> &array_init);
    Constant *getElement(int index);

    unsigned getNumElements() const { return _const_array.size(); }
};

class GlobalValue : public Constant {
public:
    explicit GlobalValue(Type *ty, const std::string &name = "", unsigned num_ops = 0) : Constant(ty, name, num_ops) {}
    void accept(IrVisitorBase *v) override;
    std::string getPrintName()override{
        return "@"+getName();
    }
};

#endif