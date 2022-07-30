#include "constant.h"
#include "utils.h"
#include "visitor/ir_visitor_base.h"
#include <list>
#include <vector>
#include <cstring>
static bool is_constant_debug =false;
long ConstantInt::getValue() const {
    return _value;
}
void ConstantInt::setValue(int value) {
    _value = value;
}
ConstantInt *ConstantInt::create(long val) {
    return new ConstantInt(Type::getInt32Ty(), val);
}
ConstantInt *ConstantInt::get(int val) {
  return new ConstantInt(Type::getInt32Ty(), val);
}
 ConstantInt *ConstantInt::getBool(bool val){
    return new ConstantInt(Type::getInt1Ty(),val);
 }
void ConstantInt::accept(IrVisitorBase *v) {
    v->visit(this);
}
float ConstantFloat::getValue() const {
    return _value;
}
void ConstantFloat::setValue(float value) {
    _value = value;
    _union_number._value = value;
}
ConstantFloat *ConstantFloat::create(float val) {
    return new ConstantFloat(Type::getFloatTy(), val);
}
void ConstantFloat::accept(IrVisitorBase *v) {
    v->visit(this);
}
std::string ConstantFloat::getPrintName(){
    std::stringstream out;
    out << "0x" << std::hex << _union_number._hex_number;
    return out.str();
    // char buffer[200];
    // if(is_constant_debug){
    //     out<<_value;
    //     return out.str();
    // }
    // else {
        // out <<"0x"<< std::hex<<_union_number._hex_number;
    //     sprintf(buffer, "%20.6e", _value);
    //     char* B = buffer;
    //     while (*B == ' ')
    //     {
    //         B++;
    //     }
    //     char* last_base_number = B+2;
    //     bool is_all_zero = true;
    //     while(*last_base_number !='e'){
    //         if(*last_base_number != '0'){
    //             is_all_zero=false;
    //         }
    //         last_base_number++;

    //     }
    //     last_base_number--;
    //     if(*last_base_number=='0' && !is_all_zero){
    //         return B;
    //     }
    //     else {
    //         return out.str();
    //     }
    // }
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
ConstantArray *ConstantArray::turn(Type *basic_type,
                                   std::vector<int32_t> &array_bounds,
                                   std::vector<Value *> &array_init) {
    auto cur_bound = array_bounds[0];
    std::vector<int32_t> bounds;
    bounds.assign(array_bounds.begin() + 1, array_bounds.end());
    std::vector<Constant *> init_list;
    int dim_length = array_init.size() / cur_bound;
    if(dim_length==1){
        for(auto& ele:array_init)
            if(ele->getType()->eq(*basic_type))
                init_list.push_back(static_cast<Constant*>(ele));
            else {
                if(ele->getType()->isIntegerTy() && basic_type->isFloatTy()){
                    auto vall = dynamic_cast<ConstantInt*>(ele)->getValue();
                    init_list.push_back( ConstantFloat::create(static_cast<float>(vall)));
                }
                else if (ele->getType()->isFloatTy() && basic_type->isIntegerTy()){
                    auto vall = dynamic_cast<ConstantFloat*>(ele)->getValue();
                    init_list.push_back( ConstantInt::create(static_cast<long>(vall)));
                }
                else {
                    ERROR("error type",EXIT_CODE_ERROR_431);
                }
            }
        ArrayType* array_type;
        if(basic_type->isFloatTy()){
            array_type = ArrayType::get(Type::getFloatTy(),init_list.size());
        }
        else if (basic_type->isIntegerTy()){
            array_type = ArrayType::get(Type::getInt32Ty(),init_list.size());
        }
        else{
            ERROR("error type",EXIT_CODE_ERROR_432);
        }
        return ConstantArray::create(array_type,init_list);
    }
    else{
        for (int i = 0; i < array_bounds[0]; i++) {
            std::vector<Value *> array_init_temp;
            array_init_temp.assign(array_init.begin() + i * dim_length, array_init.begin() + (i + 1) * dim_length);
            auto array = turn(basic_type, bounds, array_init_temp);
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
