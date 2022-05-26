
#include "BaseBlock.h"
void BaseBlock::addInstruction(Instruction *inst) {
    this->_instructions.push_back(inst);
}
Function *BaseBlock::getFunction() {
    return this->_func;
}
