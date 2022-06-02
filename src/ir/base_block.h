
#ifndef COMPILER_BASEBLOCK_H
#define COMPILER_BASEBLOCK_H
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

    std::list<Instruction*> _instructions; /*指令线性表*/
    BaseBlock *_father; /**/
    Function *_func;
public:
    enum class BlockType{
        BASIC,
        IF,
        WHILE,
    };
    void addInstruction(Instruction *inst);
    BaseBlock *getBaseFather();
    Function* getFunction();
    void clearFather() {
        _father = nullptr;
    }
    
    std::string getName();

    BaseBlock(BlockType ty, Module *m, const std::string &name,
                     Function *func);
    
    BlockType _block_type;
};


#endif//COMPILER_BASEBLOCK_H