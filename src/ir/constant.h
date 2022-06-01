#ifndef COMPILER_CONSTANT_H
#define COMPILER_CONSTANT_H

#include "type.h"
#include "user.h"
#include "value.h"

class ArrayType;

class Constant : public User {
public:
    explicit Constant(Type *ty, const std::string &name = "", unsigned num_ops = 0)
        : User(ty, name, num_ops) {}

    // virtual void print() override;
};

class ConstantInt : public Constant {
private:
    int _value;

public:
    ConstantInt(Type *ty, int value) : Constant(ty, "", 0), _value(value) {}
    int getValue() const;
    void setValue(int value);
};

class ConstantFloat : public Constant {
private:
    float _value;

public:
    ConstantFloat(Type *ty, float value) : Constant(ty, "", 0), _value(value) {}
    float getValue() const;
    void setValue(float value);
};

class ConstantArray : public Constant {
private:
    std::vector<Constant *> _const_array;

public:
    ConstantArray(ArrayType *ty, const std::vector<Constant *> &values);

    Constant *getElement(int index);

    unsigned getNumElements() const { return _const_array.size(); }
};

class GlobalValue : public Constant {
public:
    explicit GlobalValue(Type *ty, const std::string &name = "", unsigned num_ops = 0) :
        Constant(ty,name,num_ops) {}
};

#endif