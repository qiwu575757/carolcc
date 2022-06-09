
#include "module.h"
#include "ir/type.h"
#include "visitor/llvm_ir_printer.h"
/// SCOPE DEFINE ///
void Scope::enter() {
    this->_inner_value.push_back({});
    this->_array_param.push_back({});
}
void Scope::exit() {
    this->_inner_value.pop_back();
    this->_array_param.pop_back();
}
bool Scope::in_global_scope() {
    return this->_inner_value.size() == 1;
}
bool Scope::push(const std::string &name, Value *val) {
    auto _layer_num = this->_inner_value.size();
    auto res = this->_inner_value[_layer_num - 1].insert({name, val});
    return res.second;
}
bool Scope::push(const std::string &name, Value *val, std::vector<Value *> params) {
    auto _layer_num = this->_inner_value.size();
    auto res = this->_array_param[_layer_num - 1].insert({name, params});
    return res.second;
}
Value *Scope::find(std::string &name) {
    for (auto s = this->_inner_value.rbegin(); s != this->_inner_value.rend(); s++) {
        auto iter = s->find(name);
        if (iter != s->end()) {
            return iter->second;
        }
    }
    WARNNING("%s not found", name.c_str());
    return nullptr;
}

Value *Scope::find(std::string &name, std::vector<Value *> &params) {
    for (auto s = this->_array_param.rbegin(); s != this->_array_param.rend(); s++) {
        auto iter = s->find(name);
        if (iter != s->end()) {
            params.assign(iter->second.begin(), iter->second.end());
            return iter->second[0];
        }
    }
    WARNNING("%s array not found", name.c_str());
    return nullptr;
}
/// SCOPE END DEFINE ///

Module::Module(const std::string &name) : _module_name(name) {
}
void Module::AddFunction(Function *function) {
    this->_function_list.push_back(function);
}
void Module::addGlobalVariable(GlobalVariable *g) {
    this->_global_variable_list.push_back(g);
}
void Module::HighIRprint() {
    LLVMIrPrinter printer;
    for(auto global_var:_global_variable_list){
        global_var->accept(&printer);
    }
    for(auto func:_function_list){
        func->accept(&printer);
    }
}
