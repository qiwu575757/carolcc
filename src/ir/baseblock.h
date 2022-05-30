
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
enum class BlockType;
class BaseBlock :public Value {
private:

    std::list<Instruction*> _instructions; /*指令线性表*/
    BaseBlock *_father; /**/
    Function *_func;
public:
    void addInstruction(Instruction *inst);
    BaseBlock *getBaseFather();
    Function* getFunction();
    void clearFather() {
        _father = nullptr;
    }
    BlockType _block_type;
    
    std::string getName();

    BaseBlock(BlockType ty, Module *m, const std::string &name,
                     Function *func);
};
enum class BlockType{
    BASIC,
    IF,
    WHILE
};

#endif//COMPILER_BASEBLOCK_H
