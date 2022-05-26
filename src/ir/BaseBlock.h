
#ifndef COMPILER_BASEBLOCK_H
#define COMPILER_BASEBLOCK_H
#include "function.h"
#include "instruction.h"
#include "type.h"
#include "value.h"
#include <list>
#include <memory>
class Instruction;
class Function;
class Value;
class BaseBlock :public Value {
private:

    std::list<Instruction*> _instructions;
    BaseBlock *_father;
    Function *_func;
public:
    void addInstruction(Instruction *inst);
    Function* getFunction();
    void clearFather() {
        _father = nullptr;
    }
};


#endif//COMPILER_BASEBLOCK_H
