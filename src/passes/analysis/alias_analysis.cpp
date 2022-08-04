#include "alias_analysis.h"
#include "ir/instruction.h"
#include "ir/global_variable.h"
Value* AliasAnalysis::getArrayPtr(Value* ptr) {
    auto gep = dynamic_cast<GetElementPtrInst*>(ptr);
    auto load =   dynamic_cast<LoadInst*>(ptr);
    // 找到最根本的那个地址
    while((gep ) ||(load)){
        if(gep){
            ptr = gep->getOperand(0);
        }
        else {
            ptr = load->getOperand(0);
        }
        gep = dynamic_cast<GetElementPtrInst*>(ptr);
        load = dynamic_cast<LoadInst*>(ptr);
    }

    auto alloc_instr = dynamic_cast<AllocaInst*>(ptr);
    if(alloc_instr ){
    // FIXME: may have bugs
        return alloc_instr;
    }
    auto global_var = dynamic_cast<GlobalVariable*>(ptr);
    if(global_var){
        return global_var;
    }
    return nullptr;
}
bool AliasAnalysis::isGlaobal(Value *array){
    if(dynamic_cast<GlobalVariable*>(array)){
        return true;
    }
    else {
        return false;
    }
}
bool AliasAnalysis::isParam(Value *array){
    auto alloca = dynamic_cast<AllocaInst*>(array);
    if(alloca){
        return alloca->getAllocaType()->isPointerTy();
    }
    return false;
}
bool  AliasAnalysis::isLocal(Value *array){
    return !isGlaobal(array) && !isParam(array);
}
bool  AliasAnalysis::isGlobalArray(Value *array){
    if(isGlobalArray(array)){
        auto global_var = dynamic_cast<GlobalVariable *>(array);
        return global_var &&
               global_var->getType()->getPointerElementType()->isArrayTy();
    }
    return false;
}