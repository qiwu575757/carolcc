
#include "base_block.h"
void BaseBlock::addInstruction(Instruction *inst) {
    this->_instructions.push_back(inst);
}
Function *BaseBlock::getFunction()const {
    return this->_func;
}
BaseBlock *BaseBlock::getBaseFather() {
    return this->_father;
}
std::string BaseBlock::getName(){
    std::string _name = "<baseblock>";
    return _name;
}
void BaseBlock::setBaseFather(BaseBlock *father) {
    this->_father = father;
    this->setFunction(father->getFunction());
}
void BaseBlock::setFunction(Function *func)
{ 
     this->_func = func; 
}
BaseBlock::BaseBlock(BlockType ty, const std::string &name, Function *func)
    :Value(Type::getLabelTy(),name),_func(func),_block_type(ty),_father(nullptr){
}
