#include "dead_code_elimination.h"
#include "ir/function.h"
#include "module.h"
#include <vector>



void DeadCodeElimination::run(){
    deleteDeadFunc();
}
void DeadCodeElimination::deleteDeadFunc(){
    std::vector<Function*> wait_delete;
    for(auto func : _m->getFunctions()){
        if(func->getUseList().empty()){
            wait_delete.push_back(func);
        }
    }
    for(auto func: wait_delete){
        _m->rmFunction(func);
    }
}
void DeadCodeElimination::detectSideEffectFunction(){
    std::vector<Function*> wait_delete;
    for(auto func : _m->getFunctions()){
        // if(func.)
    }
    
}