#include "alias_analysis.h"

#include "ir/global_variable.h"
#include "ir/instruction.h"
Value *AliasAnalysis::getArrayPtr(Value *ptr) {
    auto gep = dynamic_cast<GetElementPtrInst *>(ptr);
    auto load = dynamic_cast<LoadInst *>(ptr);
    // 找到最根本的那个地址
    while ((gep) || (load)) {
        if (gep) {
            ptr = gep->getOperand(0);
        } else {
            ptr = load->getOperand(0);
        }
        gep = dynamic_cast<GetElementPtrInst *>(ptr);
        load = dynamic_cast<LoadInst *>(ptr);
    }

    auto alloc_instr = dynamic_cast<AllocaInst *>(ptr);
    if (alloc_instr) {
        // FIXME: may have bugs
        return alloc_instr;
    }
    auto global_var = dynamic_cast<GlobalVariable *>(ptr);
    if (global_var) {
        return global_var;
    }
    return nullptr;
}
bool AliasAnalysis::isGlobal(Value *array) {
    if (dynamic_cast<GlobalVariable *>(array)) {
        return true;
    } else {
        return false;
    }
}
bool AliasAnalysis::isParam(Value *array) {
    auto alloca = dynamic_cast<AllocaInst *>(array);
    if (alloca) {
        return alloca->getAllocaType()->isPointerTy();
    }
    return false;
}
bool AliasAnalysis::isLocal(Value *array) {
    return !isGlobal(array) && !isParam(array);
}
bool AliasAnalysis::isGlobalArray(Value *array) {
    if (isGlobalArray(array)) {
        auto global_var = dynamic_cast<GlobalVariable *>(array);
        return global_var &&
               global_var->getType()->getPointerElementType()->isArrayTy();
    }
    return false;
}

bool AliasAnalysis::alias(Value *array1, Value *array2) {
    //TODO: not finish
    if ((isGlobal(array1) && isGlobal(array2)) ||
        (isParam(array1) && isParam(array2)) ||
        (isLocal(array1) && isLocal(array2))) {
        return array2 == array2;
    }
    return false;
}
