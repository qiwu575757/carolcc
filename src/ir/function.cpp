#include "function.h"
#include "passes/module.h"
#include "type.h"
#include "user.h"

Argument::Argument(Type *type, const std::string &name, Function *parent, unsigned int arg_no)
    : Value(name, type), _arg_no(arg_no), _parent(parent) {
}
Function::Function(FunctionType *type, const std::string &name, Module *parent)
    : Value(type, name), _parent(parent) {
    parent->AddFunction(this);
}
Module *Function::getParent() const {
    return _parent;
}
FunctionType *Function::getFunctionType() const {
    ERROR("TODO");
    return nullptr;
}
Function *Function::create(FunctionType *type, const std::string &name, Module *parent) {
    ERROR("TODO");
    return nullptr;
}
Type *Function::getResultType() const {
    ERROR("TODO");
    return nullptr;
}
void Function::addBasicBlock(BasicBlock *basicblock) {
    ERROR("TODO");
}
void Function::addBasicBlockAfter(std::vector<BasicBlock *>::iterator after_pos, BasicBlock *bb) {
    ERROR("TODO");
}
void Function::removeBasicBlock(BasicBlock *basicblock) {
    ERROR("TODO");
}
unsigned Function::getNumArgs() const {
    ERROR("TODO");
    return 0;
}
void Function::addBaseBlock(BaseBlock *baseblock) {
    ERROR("TODO");
}
void Function::buildArgs() {
    ERROR("TODO");
}
