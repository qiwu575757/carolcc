#ifndef SRC_IR_BASICBLOCK_H
#define SRC_IR_BASICBLOCK_H

#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "baseblock.h"
#include "value.h"
class Function;
class Instruction;
class Module;

class BasicBlock:public BaseBlock{
    public:
    std::list<BasicBlock *> _pre_bbs;
    std::list<BasicBlock *> _succ_bbs;
    std::set<BasicBlock *> _doms;        // dominance set
    BaseBlock *_idom = nullptr;
    std::set<BaseBlock *> _dom_frontier;
    std::set<BaseBlock *>  _dom_tree_succ_blocks;
    std::list<Instruction *> _instructions;
    std::unordered_set<Value *> _active_in, _active_out, _defined_vals; /*活跃变量进入，活跃变量输出，本模块定义的变量*/
    std::unordered_map<Value *, BasicBlock *> _inherited_vals; /*继承莫个基本块的变量*/

    std::list<Instruction *>&getInstructions();
    BasicBlock *create(Module *m, const std::string &name,Function *func);
    BasicBlock *create(Module *m, const std::string);
    void setParentOfFunc(Function *parent);
    Function *getParentOfFunc();
    Module *getModule() const;
    const Instruction *getTerminator() const;
    void addInstr(Instruction *instr);
    void addInstrBegin(Instruction *instr);
    bool empty();
    int getNumOfInstr();
    void insertInstr(Instruction *pos,Instruction *insertInstr);
    void deleteInstr(Instruction *instr);
    void addPreBasicBlock(BasicBlock *bb);
    void addSuccBasicBlock(BasicBlock *bb);
    void removePreBasicBlock(BasicBlock *bb);
    void removeSuccBasicBlock(BasicBlock *bb);
    void clearSuccBasicBlockList();

    bool hasRet();
    bool isEntry() { return getName() == "entry"; }
    auto &getActiveIns();
    auto &getActiveOuts();
    auto &getDefinedVals();
    auto &getInheritedVals();
    void addActiveIn(Value *val);
    void addActiveOut(Value *val);
    void initAuxilliaryValsOfActiveSet(
      std::unordered_set<Value *> *,
      std::unordered_map<BasicBlock *, std::unordered_set<Value *>> *);
  void initDefinedVals(std::unordered_set<Value *> *);
};

#endif