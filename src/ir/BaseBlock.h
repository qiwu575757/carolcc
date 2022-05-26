
#ifndef COMPILER_BASEBLOCK_H
#define COMPILER_BASEBLOCK_H
#include "Instruction.h"
#include "value.h"
#include "type.h"
#include <list>
#include <memory>
class Instruction;
class Value;
class BaseBlock :public Value {
private:
    std::list<Instruction*> _instructions;
    BaseBlock *_father;
    Function *_func;
public:
    void clearFather() {
        _father = nullptr;
    }
};


#endif//COMPILER_BASEBLOCK_H
