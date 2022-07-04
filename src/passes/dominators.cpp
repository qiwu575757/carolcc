#include "dominators.h"
#include "module.h"
#include "ir/basic_block.h"
#include "ir/function.h"

#define _DOMINATOR_LOG

#ifdef _DOMINATOR_LOG
#define DOMINATOR_LOG(format, ...)                                  \
    do {                                                                             \
        printf(YELLOW  format RESET , ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define DOMINATOR_LOG(format, ...)
#endif


void Dominators::createReversePostOrder(Function *f) {
    _reverse_post_order.clear();
    _post_order_id.clear();
    std::set<BasicBlock*> visited;
    postOrderVisit(f->getEntryBlock(),visited);
    _reverse_post_order.reverse();
}
void Dominators::postOrderVisit(BasicBlock *bb,
                                std::set<BasicBlock *> &visited) {
    visited.insert(bb);
    for(auto& b:bb->getSuccBasicBlockList()){
        if(visited.find(b) == visited.end()){
            postOrderVisit(b,visited);
        }
    }
    _post_order_id[bb] = _reverse_post_order.size();
    _reverse_post_order.push_back(bb);

}
void Dominators::createIDoms(Function *f) {
    for(auto bb:f->getBasicBlocks()){
        bb->setIDom(nullptr);
    }

    auto root = f->getEntryBlock();
    root->setIDom(root);

    bool changed=true;
    while (changed){
        changed = false;
        for(auto bb:_reverse_post_order){
            if(bb == root){
                continue ;
            }

            BasicBlock* pred = nullptr;
            for(auto p : bb->getPreBasicBlockList()){
                if(p->getIDom()!= nullptr){
                    pred = p;
                    break;
                }
            }

            BasicBlock * new_idom = pred;
            for(auto p : bb->getPreBasicBlockList()){
                if(p == pred)
                    continue ;

                if(p->getIDom())
                    new_idom = intersect(p,new_idom);
            }
            if(bb->getIDom()!=new_idom){
                bb->setIDom(new_idom);
                changed=true;
            }
        }
    }
}

BasicBlock * Dominators::intersect(BasicBlock* b1,BasicBlock*b2){
    while(b1!=b2){
        // post order id 越小，说明靠后
        while (_post_order_id[b1]<_post_order_id[b2]){
            b1=b1->getIDom();
        }

        while(_post_order_id[b2]<_post_order_id[b1]){
            b2=b2->getIDom();
        }
    }
    return b1;
}
void Dominators::createDominaceFrontier(Function *f) {
    for(auto bb:f->getBasicBlocks()){
        if(bb->getPreBasicBlockList().size()>=2){
            for(auto p:bb->getPreBasicBlockList()){
                auto runner = p;
                while( runner!= bb->getIDom()){
                    runner->addDomFrontier(bb);
                    runner = runner->getIDom();
                }
            }
        }

    }
}
void Dominators::createDomTree(Function *f) {
    for(auto bb:f->getBasicBlocks()){
        auto idom = bb->getIDom();
        if(idom!=bb){
            idom->addDomTreeSucc(bb);
        }
    }
}
void Dominators::run() {
    for(auto f :_m->getFunctions()){
        if(f->getBasicBlocks().empty()){
            continue ;
        }
        createReversePostOrder(f);
        createIDoms(f);
        printIDom(f);
        createDominaceFrontier(f);
        printDominaceFrontier(f);
        createDomTree(f);
        printDomTree(f);
    }
}
void Dominators::printIDom(Function *f) {
    DOMINATOR_LOG("=====[IDom]:\n");
    for(auto & bb : f->getBasicBlocks()){
        DOMINATOR_LOG("%s : ",bb->getPrintName().c_str());
        if(bb->getIDom()!= nullptr){
            DOMINATOR_LOG("%s \n",bb->getIDom()->getName().c_str());
        }
        else {
            DOMINATOR_LOG("null \n");
        }
    }
}
void Dominators::printDominaceFrontier(Function *f) {
    DOMINATOR_LOG("=====[支配边界]:\n");
    for(auto & bb : f->getBasicBlocks()){
        DOMINATOR_LOG("%s : ",bb->getPrintName().c_str());
        for(auto df : bb->getDomFrontier()) {
            DOMINATOR_LOG("%s ",df->getPrintName().c_str());
        }
        DOMINATOR_LOG("\n");

    }
}
void Dominators::printDomTree(Function *f) {
    DOMINATOR_LOG("=====[支配者树]:\n");
    for(auto & bb : f->getBasicBlocks()){
        DOMINATOR_LOG("%s : ",bb->getPrintName().c_str());
        for(auto df : bb->getDomTreeSucc()) {
            DOMINATOR_LOG("%s ",df->getPrintName().c_str());
        }
        DOMINATOR_LOG("\n");

    }
}
