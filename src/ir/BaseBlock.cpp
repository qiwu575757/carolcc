
#include "BaseBlock.h"
void BaseBlock::addInstruction(Instruction *inst) {
    this->_instructions.push_back(inst);
}
