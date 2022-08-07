#ifndef COMPILER_LOOPINVARIANTEXP_H
#define COMPILER_LOOPINVARIANTEXP_H
// #include "ir/instruction.h"
#include "module.h"
#include "pass.h"
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <memory>
#include "analysis/cfg_analysis.h"

class LoopInvariantExp : public pass
{
private:
    /* data */
    std::unique_ptr<CfgAnalysis> CfgAnalyser;
    std::vector<std::pair<BasicBlock *, std::set<Instruction *>>> invariants;

public:
    LoopInvariantExp(Module* m,const std::string &name): pass(m,name){}
    void run() final;
    void invariantsFind(std::vector<BasicBlock *>* );
    // const std::string get_name() const override {return name;}
};

#endif