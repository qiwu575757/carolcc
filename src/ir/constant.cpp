#include "constant.h"
#include "utils.h"
#include "visitor/ir_visitor_base.h"
#include <list>
#include <vector>
int ConstantInt::getValue() const {
    return _value;
}
void ConstantInt::setValue(int value) {
    _value = value;
}
ConstantInt *ConstantInt::create(int val) {
    return new ConstantInt(Type::getInt32Ty(), val);
}
void ConstantInt::accept(IrVisitorBase *v) {
    v->visit(this);
}
float ConstantFloat::getValue() const {
    return _value;
}
void ConstantFloat::setValue(float value) {
    _value = value;
}
ConstantFloat *ConstantFloat::create(float val) {
    return new ConstantFloat(Type::getFloatTy(), val);
}
void ConstantFloat::accept(IrVisitorBase *v) {
    v->visit(this);
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
ConstantArray *ConstantArray::create(ArrayType *ty, const std::vector<Constant *> &values) {
    return new ConstantArray(ty, values);
}
ConstantArray *ConstantArray::turn(std::vector<int32_t> &array_bounds, std::vector<Value *> &array_init) {
    auto cur_bound = array_bounds[0];
    std::vector<int32_t> bounds;
    bounds.assign(array_bounds.begin() + 1, array_bounds.end());
    std::vector<Constant *> init_list;
    int dim_length = array_init.size() / cur_bound;
    if(dim_length==1){
        for(auto& ele:array_init)
            init_list.push_back(static_cast<Constant*>(ele));
        ArrayType* array_type;
        if(array_init.at(0)->getType()->isFloatTy()){
            array_type = ArrayType::get(Type::getFloatTy(),init_list.size());
        }
        else if (array_init.at(0)->getType()->isIntegerTy()){
            array_type = ArrayType::get(Type::getInt32Ty(),init_list.size());
        }
        else{
            ERROR("error type");
        }
        return ConstantArray::create(array_type,init_list);
    }
    else{
        for (int i = 0; i < array_bounds[0]; i++) {
            std::vector<Value *> array_init_temp;
            array_init_temp.assign(array_init.begin() + i * dim_length, array_init.begin() + (i + 1) * dim_length);
            auto array = turn(bounds, array_init_temp);
            init_list.push_back(array);
        }
        auto ty = ArrayType::get(static_cast<ArrayType*>(init_list[0]->getType()),init_list.size());
        return ConstantArray::create(ty,init_list);
    }

}
void ConstantArray::accept(IrVisitorBase *v) {
    v->visit(this);
}
void Constant::accept(IrVisitorBase *v) {
    v->visit(this);
}
void GlobalValue::accept(IrVisitorBase *v) {
    v->visit(this);
}
