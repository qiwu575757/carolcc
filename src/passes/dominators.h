#ifndef COMPILER_DOMINATORS_H
#define COMPILER_DOMINATORS_H

#include "pass.h"
#include "module.h"
#include <set>
#include <list>
#include <map>
class Module;
class Function;
class BasicBlock;

class Dominators : public Analysis {
   public:
    Dominators(Module* m, const std::string& name) : Analysis(m, name) {}
    void run() override;
    void createReversePostOrder(Function* f);
    void postOrderVisit(BasicBlock* bb,std::set<BasicBlock*> & visited);
    void createIDoms(Function* f);
    static void createDominaceFrontier(Function* f);
    static void createDomTree(Function* f);
    std::list<BasicBlock*>& getReversePostOderList(Function *f){
        createReversePostOrder(f);
        return _reverse_post_order;
    }

   private:
    static void printIDom(Function *f);
    void printDominaceFrontier(Function *f);
    void printDomTree(Function *f);
    BasicBlock * intersect(BasicBlock* b1,BasicBlock*b2);
    std::list<BasicBlock*> _reverse_post_order;
    std::map<BasicBlock*,int> _post_order_id;
    static void init(Function* pFunction);
};

#endif  // COMPILER_DOMINATORS_H
