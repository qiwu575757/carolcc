
#ifndef COMPILER_BASE_BLOCK_H
#define COMPILER_BASE_BLOCK_H
#include "value.h"
#include "instruction.h"
#include <list>
#include <memory>
class value;
class base_block:public value {
private:
    std::list<instruction*> _instructions;
public:
};


#endif//COMPILER_BASE_BLOCK_H
