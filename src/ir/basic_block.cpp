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

/// IF BLOCK ///
void IfBlock::addCondBaseBlock(BaseBlock *bb)
{

}
void IfBlock::addIfBodyBaseBlock(BaseBlock *bb)
{

}
void IfBlock::addElseBodyBaseBlock(BaseBlock *bb)
{

}
void IfBlock::removeCondBaseBlock(BaseBlock *bb)
{

}
void IfBlock::removeIfBodyBaseBlock(BaseBlock *bb)
{

}
void IfBlock::removeElseBodyBaseBlock(BaseBlock *bb)
{

}
void IfBlock::removeBaseBlock(BaseBlock *bb)
{

}
void IfBlock::insertCondBaseBlock(std::list<BaseBlock *>::iterator it, BaseBlock *bb)
{

}
void IfBlock::insertIfBodyBaseBlock(std::list<BaseBlock *>::iterator it, BaseBlock *bb)
{

}
void IfBlock::insertElseBodyBaseBlock(std::list<BaseBlock *>::iterator it, BaseBlock *bb)
{

}
void IfBlock::clearCondBaseBlockList()
{

}
/// IF BLOCK END///


/// WHILE BLOCK ///
void WhileBlock::addCondBaseBlock(BaseBlock *bb)
{
    
}
void WhileBlock::addBodyBaseBlock(BaseBlock *bb)
{
    
}
void WhileBlock::removeCondBaseBlock(BaseBlock *bb)
{
    
}
void WhileBlock::removeWhileBodyBaseBlock(BaseBlock *bb)
{
    
}
void WhileBlock::removeBaseBlock(BaseBlock *bb)
{
    
}
void WhileBlock::insertCondBaseBlockList(std::list<BaseBlock *>::iterator it, BaseBlock *bb)
{
    
}
void WhileBlock::insertWhileBodyBaseBlockList(std::list<BaseBlock *>::iterator it, BaseBlock *bb)
{
    
}
/// WHILE BLOCK END///