#ifndef COMPILER_IR_TO_C_H
#define COMPILER_IR_TO_C_H
#include "ir_visitor_base.h"
class IrToC : public IrVisitorBase {
public:
    void visit(UnaryInst &node) final;
    void visit(BinaryInst &node) final;
    void visit(CmpInst &node) final;
    void visit(ReturnInst &node) final;
    void visit(BranchInst &node) final;
    void visit(StoreInst &node) final;
    void visit(LoadInst &node) final;
    void visit(GetElementPtrInst &node) final;
    void visit(CallInst &node) final;
    void visit(ZExtInst &node) final;
    void visit(AllocaInst &node) final;
};

#endif//COMPILER_IR_TO_C_H
