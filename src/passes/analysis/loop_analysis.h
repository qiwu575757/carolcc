#ifndef LOOP_ANALYSIS_H
#define LOOP_ANALYSIS_H

#include "passes/pass.h"
#include "passes/module.h"
#include <set>
#include <list>
#include <map>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
class Module;
class Function;
class BasicBlock;
class Value;
// #define LOG_LOOP_ANALYSIS
#ifdef LOG_LOOP_ANALYSIS
#define LOOP_ANALYSIS(format, ...)                                  \
    do {                                                                             \
        printf(YELLOW "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define LOOP_ANALYSIS(format, ...)
#endif

class CFGNode {
    public:
    std::unordered_set<CFGNode*> preds;
    std::unordered_set<CFGNode*> succs;
    BasicBlock* bb;
    int index;
    int lowlink;
    bool on_stack;
};

class LoopAnalysis : public Analysis  {
   public:
    LoopAnalysis(Module* m, const std::string& name) : Analysis(m, name) {}
    void run() override;

    void buildCFG(Function* func,std::list<CFGNode*> &nodes);
    auto findStrongConnectComponents(std::list<CFGNode*>&nodes,std::unordered_set<std::unordered_set<CFGNode*>*> &result) -> bool;
    CFGNode* findLoopHeader(std::unordered_set<CFGNode*> *conn_com, std::unordered_set<CFGNode*> &already_find_headers);
    void strongConnect(CFGNode* node,std::unordered_set<std::unordered_set<CFGNode*>*> &result);
    public:
    auto& getLoops(){
        return _loops;
    }
    bool isOneEntryLoop(std::unordered_set<BasicBlock*> * loop);
    std::unordered_set<BasicBlock*>* getOuterLoop(std::unordered_set<BasicBlock*> *loop);
    std::unordered_set<BasicBlock*>* getLoopOfBasicBlock(BasicBlock* bb);
    BasicBlock* getLoopHeader(std::unordered_set<BasicBlock*> * loop);

    

   
   private:
    int _index;
    std::vector<CFGNode*> _node_stack;
    std::unordered_set<std::unordered_set<BasicBlock*>*> _loops;
    std::unordered_map<BasicBlock*,std::unordered_set<BasicBlock*>*> _header2loop;
    std::unordered_map<std::unordered_set<BasicBlock*>*,BasicBlock*> _loop2header;
    std::unordered_map<BasicBlock*,BasicBlock*> _bb2header;
};
#endif
