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
    ConstantInt(Type *ty, int value) : Constant(ty, "", 0), _value(value) {}

public:

    int getValue() const;
    void setValue(int value);
    static ConstantInt *create(int val);
};

class ConstantFloat : public Constant {
private:
    float _value;
    ConstantFloat(Type *ty, float value) : Constant(ty, "", 0), _value(value) {}

public:
    float getValue() const;
    void setValue(float value);
    static ConstantFloat *create(float val);
};

class ConstantArray : public Constant {
private:
    std::vector<Constant *> _const_array;
    ConstantArray(ArrayType *ty, const std::vector<Constant *> &values);

public:
    static ConstantArray *create(ArrayType *ty, const std::vector<Constant *> &values);

    /***
     *  将线性的array_init根据bounds规定的形状变成多维的
     * @param array_bounds 指定维度
     * @param array_init  线性内容
     * @return  构造好的存有内容的多维数组
     */
    static ConstantArray *turn(std::vector<int> &array_bounds,std::vector<Value*> &array_init);
    Constant *getElement(int index);

    unsigned getNumElements() const { return _const_array.size(); }
};

class GlobalValue : public Constant {
public:
    explicit GlobalValue(Type *ty, const std::string &name = "", unsigned num_ops = 0) : Constant(ty, name, num_ops) {}
};

#endif