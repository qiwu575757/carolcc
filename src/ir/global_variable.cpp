#include "global_variable.h"
GlobalVariable::GlobalVariable(const std::string &name, Module *module, Type *ty, bool is_const, Constant *init_val)
    : User(ty, name, init_val == nullptr ? 0u : 1u) {
    module->addGlobalVariable(this);
    if (init_val != nullptr) {
        setOperand(0, init_val);
    }
}
