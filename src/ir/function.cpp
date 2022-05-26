#include "type.h"
#include "function.h"
#include "user.h"

Argument::Argument(Type *type, const std::string &name, Function *parent, unsigned int arg_no) : Value(name, type), _arg_no(arg_no), _parent(parent) {
}
Function::Function(FunctionType *type, const std::string &name) : Value(type, name) {
}
