#include "cfg_analysis.h"

void CfgAnalysis::run(){
    for (auto func : _m->getFunctions()){
        if (func->getBasicBlocks().size()==0) continue;
        //reset
        // for (auto bb : func->getBasicBlocks()){
        //     bb->incoming_reset();
        //     bb->loop_depth_reset();
        // }
        //analyse
        // incoming_find(func);
        loopFind(func);
    }
}

// void CfgAnalysis::incoming_find(Function* func){
//     color.clear();
//     auto bb = func->getEntryBlock();
//     color[bb] = 1;
//     incoming_DFS(bb);
// }

// void CfgAnalysis::incoming_DFS(BasicBlock* bb){
//     auto succs = bb->getSuccBasicBlockList();
//     for (auto succ_BB : succs){
//         if (color[succ_BB] == 0){
//             succ_BB->incoming_add();
//             color[succ_BB] = 1;
//             incoming_DFS(succ_BB);
//         }
//         else if (color[succ_BB] == 2){
//             succ_BB->incoming_add();
//         }
//     }
//     color[bb] = 2;
// }

int DFN,LOW;
std::map<BasicBlock*,int> BB_DFN;
std::map<BasicBlock*,int> BB_LOW;
std::stack<BasicBlock*> BB_Stack;
std::vector<BasicBlock*>* BBs;
std::stack<std::vector<BasicBlock*>*> loop_stack;

void CfgAnalysis::loopFind(Function *func){
    DFN = 0;
    BB_DFN.clear();
    BB_LOW.clear();
    color.clear();
    tarjanDfs(func->getEntryBlock());
    while(!loop_stack.empty()){
        auto loop = loop_stack.top();
        loop_stack.pop();
        if (bb_loop[findLoopEntry(loop)]!=nullptr){
            outer_loop[loop] = bb_loop[findLoopEntry(loop)];
        }
        for (auto bb : *loop){
            // bb->loop_depth_add();
            bb_loop[bb] = loop;
        }
        color[findLoopEntry(loop)] = 3;
        DFN = 0;
        BB_DFN.clear();
        BB_LOW.clear();
        for (auto succ : findLoopEntry(loop)->getSuccBasicBlockList()){
            if (bb_loop[succ] == loop && color[succ]!=3){
                tarjanDfs(succ);
            }
        }
    }
}


void CfgAnalysis::tarjanDfs(BasicBlock *bb){
    DFN++;
    BB_DFN[bb] = DFN;
    BB_LOW[bb] = DFN;
    BB_Stack.push(bb);
    color[bb] = 1;
    for (auto succ : bb->getSuccBasicBlockList()){
        if (color[succ] != 3){
            if (BB_DFN[succ] == 0){
                tarjanDfs(succ);
                if (BB_LOW[succ] < BB_LOW[bb]){
                    BB_LOW[bb] = BB_LOW[succ];
                }
            }
            else if (BB_LOW[succ] < BB_LOW[bb] && color[succ] == 1){
                BB_LOW[bb] = BB_LOW[succ];
            }
        }
    }
    if (BB_DFN[bb] == BB_LOW[bb]){
        BBs = new std::vector<BasicBlock*>;
        auto bb_instack = BB_Stack.top();
        while (bb_instack != bb){
            BB_Stack.pop();
            color[bb_instack] = 2;
            BBs->push_back(bb_instack);
            bb_instack = BB_Stack.top();
        }
        BB_Stack.pop();
        color[bb_instack] = 2;
        BBs->push_back(bb_instack);
        if (BBs->size()>1){
            loops.push_back(BBs);
            loop_stack.push(BBs);
        }
    }
}
