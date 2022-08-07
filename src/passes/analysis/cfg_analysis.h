#ifndef COMPILER_CFG_ANALYSIS_H
#define COMPILER_CFG_ANALYSIS_H
#include "ir/instruction.h"
#include "passes/module.h"
#include "passes/pass.h"
#include <vector>
#include <map>
#include <stack>

/***************searching incoming forwarding branches****************/
/***************searching loops****************/
/***************on mir****************/


class CfgAnalysis : public pass{
private:
    std::vector<std::vector<BasicBlock *>*> loops;

    std::map<BasicBlock *,std::vector<BasicBlock *>*> bb_loop;
    std::map<std::vector<BasicBlock *>*,std::vector<BasicBlock *>*> outer_loop;

    std::map<BasicBlock*,int>color;
    
public:
    CfgAnalysis(Module* m,const std::string &name): pass(m,name){}
    void run() override;
    // void incoming_find(Function* f);
    // void incoming_DFS(BasicBlock* bb);
    void loopFind(Function* f);
    void tarjanDfs(BasicBlock* bb);

    BasicBlock* findLoopEntry(std::vector<BasicBlock *>* loop){return *(*loop).rbegin();};
    std::vector<BasicBlock *>* findBbLoop(BasicBlock* bb){return bb_loop[bb];};
    std::vector<BasicBlock *>* findOuterLoop(std::vector<BasicBlock *>* loop){return outer_loop[loop];};
    std::vector<std::vector<BasicBlock *>*>* getLoops(){return &loops;};
};


#endif