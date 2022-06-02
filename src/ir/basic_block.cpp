#include "basic_block.h"
std::list<Instruction *>&BasicBlock::getInstructions()
{
    /*TODO*/
}
BasicBlock *BasicBlock::create(Module *m, const std::string &name,Function *func)
{
    /*TODO*/
}
BasicBlock *BasicBlock::create(Module *m, const std::string)
{
    /*TODO*/
}
void BasicBlock::setParentFunc(Function *parent)
{
    /*TODO*/
}
Function *BasicBlock::getParentFunc()
{
    /*TODO*/
}
Module *BasicBlock::getModule() const
{
    /*TODO*/
}
const Instruction *BasicBlock::getTerminator() const
{
    /*TODO*/
}
void BasicBlock::addInstr(Instruction *instr)
{
    /*TODO*/
}
void BasicBlock::addInstrBegin(Instruction *instr)
{
    /*TODO*/
}
bool BasicBlock::empty()
{
    /*TODO*/
}
int BasicBlock::getNumOfInstr()
{
    /*TODO*/
}
void BasicBlock::insertInstr(Instruction *pos,Instruction *insertInstr)
{
    /*TODO*/
}
void BasicBlock::deleteInstr(Instruction *instr)
{
    /*TODO*/
}
void BasicBlock::addPreBasicBlock(BasicBlock *bb)
{
    /*TODO*/
}
void BasicBlock::addSuccBasicBlock(BasicBlock *bb)
{
    /*TODO*/
}
void BasicBlock::removePreBasicBlock(BasicBlock *bb)
{
    /*TODO*/
}
void BasicBlock::removeSuccBasicBlock(BasicBlock *bb)
{
    /*TODO*/
}
void BasicBlock::clearSuccBasicBlockList()
{
    /*TODO*/
}
bool BasicBlock::hasRet()
{
    /*TODO*/
}
bool BasicBlock::isEntry()
{
    /*TODO*/
}