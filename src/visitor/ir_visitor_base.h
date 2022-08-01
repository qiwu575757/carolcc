#ifndef COMPILER_IR_VISITOR_BASE_H
#define COMPILER_IR_VISITOR_BASE_H
#include "ir/constant.h"
#include "ir/global_variable.h"
#include "ir/instruction.h"
#include "ir/function.h"
class IrVisitorBase {
public:
    virtual void visit(UnaryInst *node) = 0;
    virtual void visit(BinaryInst *node) = 0;
    virtual void visit(MlaInst *node) = 0;
    virtual void visit(AllocaInst *node) = 0;
    virtual void visit(StoreInst *node) = 0;
    //  ---------
    virtual void visit(Value *node) = 0;
    virtual void visit(CmpInst *node) = 0;
    virtual void visit(ReturnInst *node) = 0;
    virtual void visit(BranchInst *node) = 0;
    virtual void visit(LoadInst *node) = 0;
    virtual void visit(GetElementPtrInst *node) = 0;
    virtual void visit(CallInst *node) = 0;
    virtual void visit(ZExtInst *node) = 0;
    virtual void visit(HIR *node) = 0;
    // ---------
    virtual void visit(Function *node) = 0;
    virtual void visit(Argument *node) = 0;
    virtual void visit(BaseBlock *node) = 0;
    virtual void visit(GlobalVariable *node) = 0;
    virtual void visit(ConstantInt *node) = 0;
    virtual void visit(ConstantFloat *node) = 0;
    virtual void visit(ConstantArray *node) = 0;
    virtual void visit(PhiInstr *node) = 0;
};
#endif//COMPILER_IR_VISITOR_BASE_H
