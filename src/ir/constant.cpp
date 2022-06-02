#include "constant.h"
int ConstantInt::getValue() const {
    return _value;
}
void ConstantInt::setValue(int value) {
    _value = value;
}
ConstantInt *ConstantInt::get(int val, Module *m) {
    return new ConstantInt(Type::getInt32Ty(), val);
}
float ConstantFloat::getValue() const {
    return _value;
}
void ConstantFloat::setValue(float value) {
    _value = value;
}
ConstantFloat *ConstantFloat::get(float val, Module *m) {
    return new ConstantFloat(Type::getFloatTy(), val);
}
Constant *ConstantArray::getElement(int index) {
    return this->_const_array.at(index);
}
ConstantArray::ConstantArray(ArrayType *ty, const std::vector<Constant *> &values)
    : Constant(ty, "", values.size()) {
    for (int i = 0; i < values.size(); i++) {
        setOperand(i, values.at(i));
    }
    this->_const_array.assign(values.begin(), values.end());
}
