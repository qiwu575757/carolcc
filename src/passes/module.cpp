
#include "module.h"
#include "ir/type.h"

void scope::enter() {
    this->_table.emplace({});
}
void scope::exit() {
    this->_table.pop_back();
}
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
