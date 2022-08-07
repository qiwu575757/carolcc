#ifndef COMPILER_LOOPINVARIANTEXP_H
#define COMPILER_LOOPINVARIANTEXP_H

#include "module.h"
#include "pass.h"
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <memory>

class LoopInvariantExp : public pass
{
// private:
    /* data */
    // std::unique_ptr<CFG_analyse> CFG_analyser;
    // std::vector<std::pair<BasicBlock *, std::set<Instruction *>>> invariants;
    // std::string name = "LoopInvariant";

public:
    LoopInvariantExp(Module* m,const std::string &name): pass(m,name){}
    void run() final;
    void invariants_find(std::vector<BasicBlock *>* );
    // const std::string get_name() const override {return name;}
};

#endif