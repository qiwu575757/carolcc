#include "function.h"

#include <map>

#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "passes/module.h"
#include "type.h"
#include "user.h"
#include "utils.h"
#include "visitor/ir_visitor_base.h"

Argument::Argument(Type *type, const std::string &name, Function *parent,
                   unsigned int arg_no)
    : Value(name, type), _parent(parent), _arg_no(arg_no) {}
void Argument::accept(IrVisitorBase *v) { v->visit(this); }

Function::Function(FunctionType *type, const std::string &name, Module *parent)
    : GlobalValue(type, name), _parent(parent), _is_builtin(false) {
    parent->AddFunction(this);
    buildArgs();
}

Function::Function(FunctionType *type, const std::string &name)
    : GlobalValue(type, name), _is_builtin(false) {
    buildArgs();
}
Module *Function::getParent() const { return _parent; }

FunctionType *Function::getFunctionType() const {
    return static_cast<FunctionType *>(getType());
}

Function *Function::create(FunctionType *type, const std::string &name,
                           Module *parent) {
    return new Function(type, name, parent);
}


Type *Function::getResultType() const {
    return getFunctionType()->getResultType();
}

void Function::addBasicBlock(BasicBlock *basicblock) {
    _basic_block_list.push_back(basicblock);
}

void Function::addBasicBlockAfter(std::_List_iterator<BasicBlock *> after_pos,
                                  BasicBlock *bb) {
    after_pos++;
    _basic_block_list.insert(after_pos, bb);  // insert 默认在 after_pos之前插入
}

void Function::removeBasicBlock(BasicBlock *basicblock) {
    auto pos =
        find(_basic_block_list.begin(), _basic_block_list.end(), basicblock);
    _basic_block_list.erase(pos);
    for (auto pre : basicblock->getPreBasicBlockList()) {
        pre->removeSuccBasicBlock(basicblock);
    }
    for (auto succ : basicblock->getSuccBasicBlockList()) {
        succ->removePreBasicBlock(basicblock);
    }
}

unsigned Function::getNumArgs() const {
    return getFunctionType()->getNumArgs();
}

void Function::addBaseBlock(BaseBlock *baseblock) {
    _base_block_list.push_back(baseblock);
}

void Function::buildArgs() {
    auto *function_type = getFunctionType();
    unsigned num_args = getNumArgs();
    for (int i = 0; i < num_args; i++) {
        _args.push_back(
            new Argument(function_type->getArgType(i), "", this, i));
    }
}
void Function::accept(IrVisitorBase *v) { v->visit(this); }
void Function::addAlloca(AllocaInst *alloca) {
    this->_allocas_list.push_back(alloca);
}
void Function::movAllocaToEntry(){
    BasicBlock* entry;
    if(!_base_block_list.empty()){
        entry = dynamic_cast<BasicBlock *>(_base_block_list.front());
    }
    else {
        entry = dynamic_cast<BasicBlock *>(_basic_block_list.front());
    }
    entry->getInstructions().splice(entry->getInstructions().begin(), _allocas_list);
    
    
}
void Function::insertAlloca(AllocaInst *alloca) {
    auto entry = getEntryBlock();
    INFO("delelting alloca");
    if (_alloca_end != nullptr) {
        entry->insertAfterInstr(_alloca_end, alloca);
    } else {
        Instruction* last_alloca=nullptr;
        for(auto instr:entry->getInstructions()){
            if(!instr->isAlloca()){
                break;
            }
            last_alloca=instr;
        }
        if(last_alloca==nullptr){
            entry->getInstructions().push_front(alloca);
            alloca->setParent(entry);
        }
        else {
            entry->insertAfterInstr(last_alloca, alloca);
        }
    }
    _alloca_end = alloca;

}
void Function::setAllocaEnd(AllocaInst *alloca) {
    INFO("setting alloca");
    _alloca_end = alloca;
}
