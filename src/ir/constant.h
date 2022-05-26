#ifndef COMPILER_CONSTANT_H
#define COMPILER_CONSTANT_H

#include "type.h"
#include "user.h"
#include "value.h"

class Constant : public User {
public:
    Constant(Type *ty, const char *name = "", unsigned num_ops = 0)
      : User(ty, name, num_ops) {}

    // virtual void print() override;
};

class ConstantInt : public Constant {
private:
    int _value;

public:
    ConstantInt(Type *ty, int value) :
        Constant(ty, "", 0), _value(value) {}
    int getValue() { return _value; }
    void setValue(int value) { _value = value; }
};

class ConstantArray : public Constant {
private:
    std::vector<Constant *> _const_array;

public:
    ConstantArray(ArrayType *ty, const std::vector<Constant *> &values);

    Constant *getElement(int index);

    unsigned getNumElements() const { return _const_array.size(); }
};

#endif