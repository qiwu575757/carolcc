#ifndef COMPILER_LLVM_IR_PRINTER_H
#define COMPILER_LLVM_IR_PRINTER_H
#include "ir_visitor_base.h"
class LLVMIrPrinter : public IrVisitorBase {
public:
private:
    virtual void visit(UnaryInst*node) final;
    virtual void visit(BinaryInst*node) final;
    virtual void visit(AllocaInst*node) final;
    virtual void visit(StoreInst*node) final;
    //  ---------
    virtual void visit(Value*node) final;
    virtual void visit(CmpInst*node) final;
    virtual void visit(ReturnInst*node) final;
    virtual void visit(BranchInst*node) final;
    virtual void visit(LoadInst*node) final;
    virtual void visit(GetElementPtrInst*node) final;
    virtual void visit(CallInst*node) final;
    virtual void visit(ZExtInst*node) final;
    virtual void visit(HIR*node) final;
};

#endif//COMPILER_LLVM_IR_PRINTER_H
