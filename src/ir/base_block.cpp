#include "function.h"
#include "base_block.h"
#include "visitor/ir_visitor_base.h"
Function *BaseBlock::getFunction() const {
    return this->_func;
}
BaseBlock *BaseBlock::getBaseFather() {
    return this->_father;
}
void BaseBlock::setBaseFather(BaseBlock *father) {
    this->_father = father;
    this->setFunction(father->getFunction());
}
void BaseBlock::setFunction(Function *func) {
    this->_func = func;
}
BaseBlock::BaseBlock(BlockType ty, const std::string &name, Function *func)
    : Value(Type::getLabelTy(), name), _father(nullptr), _func(func), _block_type(ty) {
    func->addBaseBlock(this);
}
BaseBlock::BaseBlock(BlockType ty, const std::string &name)
    : Value(Type::getLabelTy(), name), _father(nullptr), _func(nullptr), _block_type(ty) {
}
void BaseBlock::accept(IrVisitorBase *v) {
    v->visit(this);
}
bool BaseBlock::isBaiscBlock() {
    return _block_type == BaseBlock::BASIC;
}
bool BaseBlock::isIfBlock() {
    return _block_type == BaseBlock::IF;
}
bool BaseBlock::isWhileBlock() {
    return _block_type == BaseBlock::WHILE;
}
