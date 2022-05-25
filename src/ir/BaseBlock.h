
#ifndef COMPILER_BASEBLOCK_H
#define COMPILER_BASEBLOCK_H
#include "Instruction.h"
#include "value.h"
#include <list>
#include <memory>
class Instruction;
class Value;
class BaseBlock :public Value {
private:
    std::list<Instruction*> _instructions;
public:
};


#endif//COMPILER_BASEBLOCK_H
