#include "basic_block.h"
#include "function.h"
#include "visitor/ir_visitor_base.h"
std::list<Instruction *> &BasicBlock::getInstructions() {
    return this->_instructions;
}
BasicBlock *BasicBlock::create(const std::string &name, Function *func) {
    return new BasicBlock(name,func);
}
BasicBlock *BasicBlock::create(const std::string &name) {
    return new BasicBlock(name);
}
void BasicBlock::setParentFunc(Function *parent) {
    this->setFunction(parent);
}
Function *BasicBlock::getParentFunc() const {
    return this->getFunction();
}
Module *BasicBlock::getModule() const {
    return this->getParentFunc()->getParent();
}
const Instruction *BasicBlock::getTerminator() const {
  if (_instructions.empty()) {
    return nullptr;
  }
  switch (_instructions.back()->getInstructionKind()) {
  case Instruction::RET:
    return _instructions.back();
    break;

  case Instruction::BR:
    return _instructions.back();
    break;

  default:
    return nullptr;
    break;
  }
  return nullptr;
}
void BasicBlock::addInstr(Instruction *instr) {
    this->_instructions.push_back(instr); }
void BasicBlock::addInstrBegin(Instruction *instr) {
    this->_instructions.push_front(instr);
}
bool BasicBlock::empty() {
    return this->_instructions.empty();
}
int BasicBlock::getNumOfInstr() {
    return this->_instructions.size();
}
void BasicBlock::insertInstr(Instruction *pos, Instruction *insertInstr) {
    for (auto instr = this->_instructions.begin(); instr != this->_instructions.end(); instr++) {
        if (*instr == pos) {
        this->_instructions.insert(instr, insertInstr);
        return;
        }
    }
    WARNNING("insert instr not find");
}
void BasicBlock::insertAfterInstr(Instruction *pos, Instruction *insertInstr) {
    for (auto instr = this->_instructions.begin(); instr != this->_instructions.end(); instr++) {
        if (*instr == pos) {
            instr++;
            INFO("inserting after instr");
            this->_instructions.insert(instr, insertInstr);
            return;
        }
    }
    WARNNING("insert instr not find, inserting to head");
    this->_instructions.push_back(insertInstr);
}
void BasicBlock::deleteInstr(Instruction *instr) {
    this->_instructions.remove(instr);
}
void BasicBlock::addPreBasicBlock(BasicBlock *bb) {
    this->_pre_bbs.push_back(bb);
}
void BasicBlock::addSuccBasicBlock(BasicBlock *bb) {
    this->_succ_bbs.push_back(bb);
}
void BasicBlock::removePreBasicBlock(BasicBlock *bb) {
    this->_pre_bbs.remove(bb);
}
void BasicBlock::removeSuccBasicBlock(BasicBlock *bb) {
    this->_succ_bbs.remove(bb);
}
void BasicBlock::clearSuccBasicBlockList() {
    this->_succ_bbs.clear();
}
bool BasicBlock::hasRet() {
    return false;
}
bool BasicBlock::isEntry() {
    return this->getName() == "entry";

}
BasicBlock::BasicBlock(const std::string &name, Function *func)
    : BaseBlock(BlockType::BASIC, name, func) {
}
BasicBlock::BasicBlock(const std::string &name)
    : BaseBlock(BlockType::BASIC, name) {
}
BasicBlock::BasicBlock()
: BasicBlock("", nullptr){
}
void BasicBlock::accept(IrVisitorBase *v) {
    v->visit(this);
}
std::list<BasicBlock *> &BasicBlock::getSuccBasicBlockList() {
    return _succ_bbs;
}
std::list<BasicBlock*>& BasicBlock::getPreBasicBlockList() {
    return _pre_bbs;
}

/// IF BLOCK ///
void IfBlock::addCondBaseBlock(BaseBlock *bb) {
    this->_cond.push_back(bb);
    bb->setBaseFather(this);
}
void IfBlock::addIfBodyBaseBlock(BaseBlock *bb) {
    this->_if_body.push_back(bb);
    bb->setBaseFather(this);}
void IfBlock::addElseBodyBaseBlock(BaseBlock *bb) {
    this->_else_body.push_back(bb);
    bb->setBaseFather(this);}
void IfBlock::removeCondBaseBlock(BaseBlock *bb) {
    this->_cond.remove(bb);
}
void IfBlock::removeIfBodyBaseBlock(BaseBlock *bb) {
    this->_if_body.remove(bb);
}
void IfBlock::removeElseBodyBaseBlock(BaseBlock *bb) {
    this->_else_body.remove(bb);
}
void IfBlock::removeBaseBlock(BaseBlock *bb) {
    this->removeCondBaseBlock(bb);
    this->removeIfBodyBaseBlock(bb);
    this->removeElseBodyBaseBlock(bb);
}
void IfBlock::insertCondBaseBlock(std::list<BaseBlock *>::iterator it, BaseBlock *bb) {
    this->_cond.insert(it, bb);
    bb->setBaseFather(this);
}
void IfBlock::insertIfBodyBaseBlock(std::list<BaseBlock *>::iterator it, BaseBlock *bb) {
    this->_if_body.insert(it, bb);
    bb->setBaseFather(this);
}
void IfBlock::insertElseBodyBaseBlock(std::list<BaseBlock *>::iterator it, BaseBlock *bb) {
    this->_else_body.insert(it, bb);
    bb->setBaseFather(this);
}
void IfBlock::clearCondBaseBlockList() {
    this->_cond.clear();
}
IfBlock::IfBlock(const std::string &name, Function *func)
    : BaseBlock(BlockType::IF, name, func) {
}
IfBlock::IfBlock(const std::string &name)
    : BaseBlock(BlockType::IF, name) {
}
IfBlock *IfBlock::create(const std::string &name, Function *func) {
    return new IfBlock(name, func);
}
IfBlock *IfBlock::create(const std::string &name) {
    return new IfBlock(name);
}
std::list<BaseBlock *> *IfBlock::getCondBaseBlockList() {
    return &this->_cond;
}
std::list<BaseBlock *> *IfBlock::getIfBodyBaseBlockList() {
    return &this->_if_body;
}
std::list<BaseBlock *> *IfBlock::getElseBodyBaseBlockList() {
    return &this->_else_body;
}
void IfBlock::accept(IrVisitorBase *v) {

    v->visit(this);

}
/// IF BLOCK END///


/// WHILE BLOCK ///
void WhileBlock::addCondBaseBlock(BaseBlock *bb) {
    this->_cond.push_back(bb);
    bb->setBaseFather(this);
}
void WhileBlock::addBodyBaseBlock(BaseBlock *bb) {
    this->_body.push_back(bb);
    bb->setBaseFather(this);}
void WhileBlock::removeCondBaseBlock(BaseBlock *bb) {
    this->_cond.remove(bb);
}
void WhileBlock::removeWhileBodyBaseBlock(BaseBlock *bb) {
    this->_body.remove(bb);
}
void WhileBlock::removeBaseBlock(BaseBlock *bb) {
    this->removeCondBaseBlock(bb);
    this->removeWhileBodyBaseBlock(bb);
}
void WhileBlock::insertCondBaseBlockList(std::list<BaseBlock *>::iterator it, BaseBlock *bb) {
    this->_cond.insert(it,bb);
    bb->setBaseFather(this);
}
void WhileBlock::insertWhileBodyBaseBlockList(std::list<BaseBlock *>::iterator it, BaseBlock *bb) {
     this->_body.insert(it,bb);
    bb->setBaseFather(this);
}
WhileBlock *WhileBlock::create(const std::string &name, Function *func) {
    return new WhileBlock(name, func);
}
WhileBlock *WhileBlock::create(const std::string &name) {
    return new WhileBlock(name);
}
std::list<BaseBlock *> *WhileBlock::getCondBaseBlockList() {
    return &this->_cond;
}
std::list<BaseBlock *> *WhileBlock::getBodyBaseBlockList() {
    return &this->_body;
}
WhileBlock::WhileBlock(const std::string &name, Function *func)
    : BaseBlock(BlockType::WHILE, name, func) {
}
WhileBlock::WhileBlock(const std::string &name)
    : BaseBlock(BlockType::WHILE, name) {
}
void WhileBlock::accept(IrVisitorBase *v) {

    v->visit(this);

}
/// WHILE BLOCK END///