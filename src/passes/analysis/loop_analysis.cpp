#include "loop_analysis.h"

#include <algorithm>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "ir/base_block.h"
#include "ir/basic_block.h"
#include "ir/function.h"
#include "ir/instruction.h"
void LoopAnalysis::run() {
    std::list<CFGNode *> nodes;
    std::unordered_set<CFGNode *> already_find_headers;
    std::unordered_set<std::unordered_set<CFGNode *> *> components;
    for (auto func : _m->getFunctions()) {
        LOOP_ANALYSIS("cur func is %s",func->getPrintName().c_str());
        if (func->isBuiltin() || func->getBasicBlocks().empty()) {
            continue;
        }
        buildCFG(func, nodes);
        LOOP_ANALYSIS("nodes size is %d",nodes.size());

        while (findStrongConnectComponents(nodes, components)) {
        LOOP_ANALYSIS("components size is %d",components.size());
            for (auto com : components) {
                auto header = findLoopHeader(com, already_find_headers);
                auto loop = new std::unordered_set<BasicBlock *>;
                for (auto n : *com) {
                    loop->insert(n->bb);
                }
                #ifdef LOG_LOOP_ANALYSIS
                printf("loop has :");
                for(auto bb : *loop){
                    printf("%s ",bb->getPrintName().c_str());
                }
                printf("\n");
                #endif // LOG_LOOP_ANALYSIS
                _loops.insert(loop);
                _header2loop[header->bb] = loop;
                _loop2header[loop] = header->bb;

                for (auto bb : *loop) {
                    _bb2header[bb] = header->bb;
                }
                already_find_headers.insert(header);
                nodes.remove(header);

                for (auto suc_bb : header->succs) {
                    suc_bb->preds.erase(header);
                }
                for (auto pre_bb : header->preds) {
                    pre_bb->succs.erase(header);
                }
            }
            for (auto com : components) {
                delete com;
            }
            components.clear();

            for (auto n : nodes) {
                n->index = n->lowlink = -1;
                n->on_stack = false;
            }
        }
        // for mem clear
        for (auto n : already_find_headers) {
            nodes.push_back(n);
            for (auto pred : n->preds) {
                pred->succs.insert(n);
            }
        }
        already_find_headers.clear();
        for (auto n : nodes) {
            delete n;
        }
        nodes.clear();
    }
}

void LoopAnalysis::buildCFG(Function *func,
                            std::list<CFGNode *> &nodes) {
    std::unordered_map<BasicBlock *, CFGNode *> bb2node;
    bb2node.clear();
    for (auto bb : func->getBasicBlocks()) {
        auto node = new CFGNode;
        node->bb = bb;
        node->index = node->lowlink = -1;
        node->on_stack = false;
        bb2node[bb] = node;
        nodes.push_back(node);
    }

    for (auto bb : func->getBasicBlocks()) {
        auto node = bb2node[bb];
        for (auto pred_bb : bb->getPreBasicBlocks()) {
            node->preds.insert(bb2node[pred_bb]);
        }
        for (auto suc_bb : bb->getSuccBasicBlockList()) {
            node->succs.insert(bb2node[suc_bb]);
        }
    }
}

bool LoopAnalysis::findStrongConnectComponents(
    std::list<CFGNode *> &nodes,
    std::unordered_set<std::unordered_set<CFGNode *> *> &result) {
    _index = 0;
    _node_stack.clear();
    for (auto n : nodes) {
        if (n->index == -1) {
            strongConnect(n, result);
        }
    }
    return !result.empty();
}

void LoopAnalysis::strongConnect(
    CFGNode *node,
    std::unordered_set<std::unordered_set<CFGNode *> *> &result) {
    node->index = _index;
    node->lowlink = _index;
    _index++;
    _node_stack.push_back(node);
    node->on_stack = true;
    for (auto suc_bb : node->succs) {
        if (suc_bb->index == -1) {
            strongConnect(suc_bb, result);
            node->lowlink = std::min(node->lowlink, suc_bb->lowlink);
        } else if (suc_bb->on_stack) {
            node->lowlink = std::min(node->lowlink, suc_bb->lowlink);
        }
    }

    if (node->lowlink == node->index) {
        auto res_set = new std::unordered_set<CFGNode *>;
        CFGNode *tmp = nullptr;
        do {
            tmp = _node_stack.back();
            _node_stack.pop_back();
            tmp->on_stack = false;
            res_set->insert(tmp);
        } while (node != tmp);
        if (res_set->size() == 1) {
            delete res_set;
        } else {
            result.insert(res_set);
        }
    }
}
CFGNode *LoopAnalysis::findLoopHeader(
    std::unordered_set<CFGNode *> *conn_com,
    std::unordered_set<CFGNode *> &already_find_headers) {
    CFGNode *header = nullptr;

    for (auto n : *conn_com) {
        for (auto pre_bb : n->preds) {
            if (conn_com->find(pre_bb) == conn_com->end()) {
                header = n;
            }
        }
    }
    if (header != nullptr) {
        return header;
    }

    for (auto out_loop_header : already_find_headers) {
        for (auto suc_bb : out_loop_header->succs) {
            if (conn_com->find(suc_bb) != conn_com->end()) {
                header = suc_bb;
            }
        }
    }
    return header;
}

bool LoopAnalysis::isOneEntryLoop(std::unordered_set<BasicBlock *> *loop) {
    auto header = _loop2header[loop];
    for (auto node : *loop) {
        if (node == header) {
            continue;
        }
        if (_header2loop.find(node) != _header2loop.end()) {
            return false;
        }
    }
    return true;
}

std::unordered_set<BasicBlock*>* LoopAnalysis::getOuterLoop(std::unordered_set<BasicBlock *> *loop) {
    auto header = _loop2header[loop];

    for(auto pred : header->getPreBasicBlocks()){
        if(loop->find(pred) !=loop->end()){
            continue;
        }

        auto pre_loop = getLoopOfBasicBlock(pred);
        // 不是嵌套的loop
        if(pre_loop==nullptr || pre_loop->find(header) == pre_loop->end()){
            return nullptr;
        }
        else {
            return pre_loop;
        }
    }
    return nullptr;
}
std::unordered_set<BasicBlock*>* LoopAnalysis::getLoopOfBasicBlock(BasicBlock *bb) {
    if(_bb2header.find(bb) == _bb2header.end()){
        return nullptr;
    }
    return _header2loop[_bb2header[bb]];
}
BasicBlock *LoopAnalysis::getLoopHeader(
    std::unordered_set<BasicBlock *> *loop) {
        return _loop2header[loop];
    }