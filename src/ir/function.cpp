#include "function.h"
#include "type.h"
#include "user.h"

Argument::Argument(Type *type, const std::string &name, Function *parent, unsigned int arg_no)
    : Value(name, type), _arg_no(arg_no), _parent(parent) {
}
Function::Function(FunctionType *type, const std::string &name, Module *parent)
    : Value(type, name) {
    parent->AddFunction(this);
}
