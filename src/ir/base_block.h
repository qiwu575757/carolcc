
#ifndef COMPILER_BASEBLOCK_H
#define COMPILER_BASEBLOCK_H
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
    void accept(IrVisitorBase *v) override;

public:
    enum BlockType{
        BASIC,
        IF,
        WHILE,
    };
    void setFunction(Function *func);
    void setBaseFather(BaseBlock *father);
    BaseBlock *getBaseFather();
    Function* getFunction()const;
    void clearFather() {
        _father = nullptr;
    }

    bool isBaiscBlock();
    bool isIfBlock();
    bool isWhileBlock();

    BaseBlock(BlockType ty, const std::string &name, Function *func);
    BaseBlock(BlockType ty, const std::string &name);
private:
    BlockType _block_type;
};


#endif//COMPILER_BASEBLOCK_H
