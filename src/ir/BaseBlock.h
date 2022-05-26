
#ifndef COMPILER_BASEBLOCK_H
#define COMPILER_BASEBLOCK_H
#include "Instruction.h"
#include "value.h"
#include <list>
#include <memory>
class Instruction;
class Function;
class Value;
class BaseBlock :public Value {
private:

    std::list<Instruction*> _instructions;
    std::string _label;

public:
    void addInstruction(Instruction* inst);

};


#endif//COMPILER_BASEBLOCK_H
