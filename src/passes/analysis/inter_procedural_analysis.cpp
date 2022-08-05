#include "passes/analysis/inter_procedural_analysis.h"
#include "ir/global_variable.h"
#include "passes/analysis/alias_analysis.h"
#include "passes/module.h"
#include "ir/function.h"
#include "ir/basic_block.h"
#include "ir/instruction.h"

void InterProceduralAnalysis::run() {
    for (auto func : _m->getFunctions()) {
        func->setSideEffect(func->isBuiltin());
        func->setUseGlobalVar(func->isBuiltin());
    }

    for(auto func : _m->getFunctions() ){
        for(auto bb : func->getBasicBlocks()){
            for( auto instr : bb->getInstructions()){
                if(instr->isLoad()){
                    auto load = dynamic_cast<LoadInst*>(instr);
                    auto load_ptr = instr->getOperand(0);
                    auto alloca  = dynamic_cast<AllocaInst*>(load_ptr);
                    if(alloca){
                        if(alloca->getAllocaType()->isInt32() || alloca->getAllocaType()->isFloatTy()){
                            continue;
                        }
                    }
                    auto ptr = AliasAnalysis::getArrayPtr(load_ptr);
                    if(AliasAnalysis::isGlobal(ptr)){
                        func->setUseGlobalVar(true);
                        auto gv = dynamic_cast<GlobalVariable*>(ptr);
                        func->getUsedGlobalVarSet().insert(gv);
                    }
                }
                else if(instr->isStore()){
                    auto store = dynamic_cast<StoreInst*>(instr);
                    auto store_ptr = store->getPtr();
                    auto alloca = dynamic_cast<AllocaInst*>(store_ptr);
                    if(alloca){
                        if(alloca->getAllocaType()->isInt32() || alloca->getAllocaType()->isFloatTy()){
                            continue;
                        }
                    }
                    auto ptr = AliasAnalysis::getArrayPtr(store_ptr);
                    if(AliasAnalysis::isGlobal(ptr) || AliasAnalysis::isParam(ptr)){
                        func->setSideEffect(true);
                        if(AliasAnalysis::isGlobal(ptr)){
                            func->setUseGlobalVar(true);
                            auto gv = dynamic_cast<GlobalVariable*>(ptr);
                            func->getUsedGlobalVarSet().insert(gv);
                        }
                    }

                }
                
            }
        }
    }
    for(auto func : _m->getFunctions()){
        if(func->hasSideEffect()){
            setCallerSideEffect(func);
        }
        if(func->useGlobalVar()){
            setCallerUseGobalVar(func);
        }
    }
}
void InterProceduralAnalysis:: setCallerSideEffect(Function* f){
    for(auto caller_func : f->getCallerSet()){
        if(!caller_func->hasSideEffect()){
            caller_func->setSideEffect(true);
            setCallerSideEffect(caller_func);
        }
    }
}
void  InterProceduralAnalysis::setCallerUseGobalVar(Function* f){

    for(auto caller_func : f->getCallerSet()){
        if(!caller_func->useGlobalVar()){
            caller_func->setUseGlobalVar(true);
            setCallerUseGobalVar(caller_func);
        }
    }
}
