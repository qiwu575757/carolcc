#ifndef COMPILER_GLOBALVARIABLE_H
#define COMPILER_GLOBALVARIABLE_H
#include "constant.h"
#include "module.h"
#include "user.h"

class GlobalVariable : public User {
private:
    bool _is_const : true;
    Constant *_init_value;

public:
    GlobalVariable(std::string name, Module *module, Type *ty, bool is_const, Constant *init_val);

    Constant *getInit() { return _init_value; }
};

#endif