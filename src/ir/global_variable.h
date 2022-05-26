#ifndef COMPILER_GLOBAL_VARIABLE_H
#define COMPILER_GLOBAL_VARIABLE_H
#include "constant.h"
#include "passes/module.h"
#include "user.h"

class Module;
class GlobalVariable : public User {
private:
    bool _is_const : true;
    Constant *_init_value;

public:
    GlobalVariable(const std::string &name, Module *module, Type *ty, bool is_const, Constant *init_val);

    Constant *getInit() { return _init_value; }
};

#endif