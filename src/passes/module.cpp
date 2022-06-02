
#include "module.h"
#include "ir/type.h"
/// SCOPE DEFINE ///
void Scope::enter() {
    this->_inner_value.push_back({});
    this->_array_param.push_back({});
}
void Scope::exit() {
    this->_inner_value.pop_back();
    this->_array_param.pop_back();
}
bool Scope::in_global_scope(){
    return this->_inner_value.size()== 1;
}
bool Scope::push(std::string name, Value *val){
    int _layer_num = this->_inner_value.size();
    auto res = this->_inner_value[_layer_num-1].insert({name,val});
    return res.second;
}
bool Scope::push(std::string name, Value *val, std::vector<Value *>params){
    int _layer_num = this->_inner_value.size();
    auto res = this->_array_param[_layer_num-1].insert({name,params});
    return res.second;
}
Value * Scope::find(std::string name) {
    for (auto s = this->_inner_value.rbegin(); s !=  this->_inner_value.rend(); s++) {
        auto iter = s->find(name);
        if (iter != s->end()) {
            return iter->second;
        }
    }
    return nullptr;
}

Value * Scope::find(std::string name, std::vector<Value *> &params) {
    for (auto s =  this->_array_param.rbegin(); s != this->_array_param..rend(); s++) {
        auto iter = s->find(name);
        if (iter != s->end()) {
            params.assign(iter->second.begin(), iter->second.end());
            return iter->second[0];
        }
    }
    return nullptr;
}
/// SCOPE END DEFINE ///

Module::Module(const std::string &name) : _module_name(name) {
    _int1_type = IntegerType::get(1);
    _int32_type = IntegerType::get(32);
    _float_type = FloatType::get();
    _label_type = new Type(Type::LabelTyID);
    _void_type = new Type(Type::VoidTyID);
    _int32ptr_type = PointerType::get(_int32_type);
    _floatptr_type = PointerType::get(_float_type);
}
Type *Module::getVoidTy() {
    return _void_type;
}
Type *Module::getLabelTy() {
    return _label_type;
}
IntegerType *Module::getInt1Ty() {
    return _int1_type;
}
IntegerType *Module::getInt32Ty() {
    return _int32_type;
}
FloatType *Module::getFloatTy() {
    return _float_type;
}
PointerType *Module::getInt32PtrTy() {
    return _int32ptr_type;
}
PointerType *Module::getFloatPtrTy() {
    return _floatptr_type;
}
void Module::AddFunction(Function *function) {
    this->_function_list.push_back(function);
}
void Module::addGlobalVariable(GlobalVariable *g) {
    this->_global_variable_list.push_back(g);
}
void Module::HighIRprint() {
    ERROR("TODO");
}
