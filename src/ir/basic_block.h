#ifndef SRC_IR_BASIC_BLOCK_H
#define SRC_IR_BASIC_BLOCK_H

#include "base_block.h"
#include "value.h"
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
class Function;
class Instruction;
class Module;

class BasicBlock : public BaseBlock {

    BasicBlock(const std::string &name, Function *func);
    BasicBlock(const std::string &name);
    BasicBlock();

public:
    void accept(IrVisitorBase *v) override;


public:
    std::list<BasicBlock *> _pre_bbs;
    std::list<BasicBlock *> _succ_bbs;
    std::set<BasicBlock *> _doms;// dominance set

    std::set<BasicBlock *> _dom_tree_succ_blocks;// dominance set
    std::set<BasicBlock *> _dom_frontier;// dominance set

    void addDomFrontier(BasicBlock* bb){_dom_frontier.insert(bb);}
    std::set<BasicBlock*>& getDomFrontier(){return _dom_frontier;}

    void addDomTreeSucc(BasicBlock* bb){_dom_tree_succ_blocks.insert(bb);}
    std::set<BasicBlock*>& getDomTreeSucc(){return _dom_tree_succ_blocks;}

    void addDom(BasicBlock* bb){_doms.insert(bb);}
    std::set<BasicBlock*>& getDoms(){return _doms;}

    BasicBlock *_idom = nullptr;
    void setIDom(BasicBlock* idom){_idom = idom;}
    BasicBlock* getIDom(){return _idom;}
    std::list<Instruction *> _instructions;
    
    std::unordered_set<Value *> _ue_exprs, _avail_out,_expr_kills ;
    auto& getUEExprs(){return _ue_exprs;}
    auto& getAvailOuts(){return _avail_out;}
    auto& getExprKills(){return _expr_kills;}
    static BasicBlock *create(const std::string &name, Function *func);
    static BasicBlock *create(const std::string &name);
    std::list<Instruction *> &getInstructions();
    std::list<BasicBlock *> &getPreBasicBlocks();
    std::list<BasicBlock *> &getSuccBasicBlocks();
    void setParentFunc(Function *parent);
    Function *getParentFunc() const;
    Module *getModule() const;
    Instruction *getTerminator() const;
    void addInstr(Instruction *instr);
    void addInstrBegin(Instruction *instr);
    bool empty();
    int getNumOfInstr();
    void insertInstr(Instruction *pos, Instruction *insertInstr);
    void deleteInstr(Instruction *instr);
    void addPreBasicBlock(BasicBlock *bb);
    std::list<BasicBlock*>& getPreBasicBlockList();
    std::list<BasicBlock*>& getSuccBasicBlockList();
    void addSuccBasicBlock(BasicBlock *bb);
    void removePreBasicBlock(BasicBlock *bb);
    void removeSuccBasicBlock(BasicBlock *bb);
    void clearSuccBasicBlockList();
    
    bool hasRet();
    bool isEntry();
    void insertAfterInstr(Instruction *pos, Instruction *insertInstr);
};

class IfBlock : public BaseBlock {
private:
    IfBlock(const std::string &name, Function *func);

public:
    void accept(IrVisitorBase *v) override;

private:
    IfBlock(const std::string &name);

public:
    // this will auto add this to function's baseblock list
    // don't use this in nested structure
    static IfBlock *create(const std::string &name, Function *func);
    static IfBlock *create(const std::string &name);
    void addCondBaseBlock(BaseBlock *bb);
    void addIfBodyBaseBlock(BaseBlock *bb);
    void addElseBodyBaseBlock(BaseBlock *bb);

    std::list<BaseBlock *> *getCondBaseBlockList();
    std::list<BaseBlock *> *getIfBodyBaseBlockList();
    std::list<BaseBlock *> *getElseBodyBaseBlockList();

    void removeCondBaseBlock(BaseBlock *bb);
    void removeIfBodyBaseBlock(BaseBlock *bb);
    void removeElseBodyBaseBlock(BaseBlock *bb);
    void removeBaseBlock(BaseBlock *bb);
    void insertCondBaseBlock(std::list<BaseBlock *>::iterator it, BaseBlock *bb);
    void insertIfBodyBaseBlock(std::list<BaseBlock *>::iterator it, BaseBlock *bb);
    void insertElseBodyBaseBlock(std::list<BaseBlock *>::iterator it, BaseBlock *bb);

    void clearCondBaseBlockList();

private:
    std::list<BaseBlock *> _cond; /**/
    std::list<BaseBlock *> _if_body;
    std::list<BaseBlock *> _else_body;
};

class WhileBlock : public BaseBlock {
   private:
   public:
    void accept(IrVisitorBase *v) override;

   private:
    WhileBlock(const std::string &name, Function *func);
    WhileBlock(const std::string &name);

   public:
    // this will auto add this to function's baseblock list
    // don't use this in nested structure
    static WhileBlock *create(const std::string &name, Function *func);
    static WhileBlock *create(const std::string &name);

    void addCondBaseBlock(BaseBlock *bb);
    void addBodyBaseBlock(BaseBlock *bb);

    std::list<BaseBlock *> *getCondBaseBlockList();
    std::list<BaseBlock *> *getBodyBaseBlockList();

    void removeCondBaseBlock(BaseBlock *bb);
    void removeWhileBodyBaseBlock(BaseBlock *bb);
    void removeBaseBlock(BaseBlock *bb);
    void insertCondBaseBlockList(std::list<BaseBlock *>::iterator it, BaseBlock *bb);
    void insertWhileBodyBaseBlockList(std::list<BaseBlock *>::iterator it, BaseBlock *bb);


   private:
    std::list<BaseBlock *> _cond;
    std::list<BaseBlock *> _body;
};

#endif  // SRC_IR_BASIC_BLOCK_H