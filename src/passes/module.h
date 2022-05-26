#ifndef COMPILER_MODULE_H
#define COMPILER_MODULE_H
#include <list>
#include <map>
#include <string>
#include "ir/function.h"
#include<vector>
#include<unordered_map>
#include "type.h"
#include "value.h"
#include "globalvariable.h"

extern error_log module_logger;
class GlobalVariable;

class Module {
public:
    enum IRLevel { HIR, MIR_MEM, MIR_SSA, LIR };
    Module (std::string name);
    Type *getVoidTy();
    Type *getLabelTy();
    IntegerType *getInt1Ty();
    IntegerType *getInt32Ty();
    FloatType *getFloatTy();
    PointerType *getInt32PtrTy();
    PointerType *getFloatPtrTy();

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
    void HighIRprint();

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
        for (auto func : _function_list) {
            if (func->getName() == "main") {
                return func;
            }
        }
    }
    Function *getFunction(std::string func_name) {
        for (auto func : _function_list) {
            if (func->getName() == func_name) {
                return func;
            }
        }
        module_logger.println("No given function");
        exit(1);
    }

private:
    std::list<GlobalVariable *> _global_variable_list;//全局变量表
    std::list<Function *> _function_list;//函数表
    std::map<std::string, Value *> _value_sym_table;//符号表
    std::string _module_name;

    IRLevel _ir_level = HIR;

private:
  IntegerType *_int1_type;
  IntegerType *_int32_tpye;
  FloatType *_float_tpye;
  Type *_label_type;
  Type *_void_type;
  PointerType *_int32ptr_type;
};

class scope{
public:
    void enter();
    void exit();

private:
    std::vector<std::unordered_map<std::string,GlobalVariable*>> _table;
};

#endif // !COMPILER_MODULE_H