#ifndef COMPILER_MODULE_H
#define COMPILER_MODULE_H
#include "ir/function.h"
#include "ir/global_variable.h"
#include "ir/type.h"
#include "ir/value.h"
#include "utils.h"
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

class GlobalVariable;
class IntegerType;
class FloatType;
class PointerType;

class Module {
public:
    enum IRLevel { HIR,
                   MIR_MEM,
                   MIR_SSA,
                   LIR };
    explicit Module(const std::string &name);

    void AddFunction(Function *function);
    void RemoveFunction(Function *function) {
        _function_list.remove(function);
    }
    void RemoveGlobalVariable(GlobalVariable *v) {
        _global_variable_list.remove(v);
    }
    std::list<Function *> &getFunctions() {
        return _function_list;
    }
    std::list<GlobalVariable *> &getGlobalVariables() {
        return _global_variable_list;
    }
    std::map<std::string, Value *> &getValueSymTable() {
        return _value_sym_table;
    }
    std::string getModuleName() {
        return _module_name;
    }
    void addGlobalVariable(GlobalVariable *g);
    void HighIRprint(const std::string &file_name);
    void HIRSHOW(const std::string &file_name);
    void MIRMEMprint(const std::string &file_name);
    void MIRMEMIndex();
    void setIRLevel(IRLevel ir_level) {
        _ir_level = ir_level;
    }
    IRLevel getIRLevel() {
        return _ir_level;
    }
    bool isMIRSSALevel() {
        return _ir_level == MIR_SSA;
    }

    Function *getMainFunction() {
        for (auto func: _function_list) {
            if (func->getName() == "main") {
                return func;
            }
        }
        return nullptr;
    }
    Function *getFunction(std::string func_name) {
        for (auto func: _function_list) {
            if (func->getName() == func_name) {
                return func;
            }
        }
        ERROR("no function given %s",EXIT_CODE_ERROR_429, func_name.c_str());
    }

private:
    std::list<GlobalVariable *> _global_variable_list;//全局变量表
    std::list<Function *> _function_list;             //函数表
    std::map<std::string, Value *> _value_sym_table;  //符号表
    std::string _module_name;

    IRLevel _ir_level = HIR;

};

class Scope {
public:
    void enter();
    void exit();
    bool in_global_scope();
    /*压入名字-值 返回检查*/
    bool push(const std::string &name, Value *val);
    /*重载 压入名字-值-参数 返回检查*/
    bool push(const std::string &name, Value *val, std::vector<Value *>params);
    /*找到则返回否则返回空指针*/
    Value *find(std::string &name);
    /*重载 找到则返回否则返回空指针*/
    Value *find(std::string &name, std::vector<Value *> &params) ;
private:
    std::vector<std::map <std::string, Value *> > _inner_value;
    std::vector<std::map <std::string, std::vector<Value *> > > _array_param;
};

#endif// !COMPILER_MODULE_H