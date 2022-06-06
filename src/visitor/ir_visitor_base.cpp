#include "ir_visitor_base.h"
#include "ir/instruction.h"
#include "utils.h"

void IrVisitorBase::visit(UnaryInst &node) {
    TRACE("visiting UnaryInst");
}
void IrVisitorBase::visit(BinaryInst &node) {
    TRACE("visiting BinaryInst");
}
void IrVisitorBase::visit(CmpInst &node) {
    TRACE("visiting CmpInst");
}
void IrVisitorBase::visit(ReturnInst &node) {
    TRACE("visiting ReturnInst");
}
void IrVisitorBase::visit(BranchInst &node) {
    TRACE("visiting BranchInst");
}
void IrVisitorBase::visit(StoreInst &node) {
    TRACE("visiting StoreInst");
}
void IrVisitorBase::visit(BinaryInst &node) {
    TRACE("visiting BinaryInst");
}
void IrVisitorBase::visit(LoadInst &node) {
    TRACE("visiting LoadInst");
}
void IrVisitorBase::visit(GetElementPtrInst &node) {
    TRACE("visiting GetElementPtrInst");
}
void IrVisitorBase::visit(CallInst &node) {
    TRACE("visiting CallInst");
}
void IrVisitorBase::visit(ZExtInst &node) {
    TRACE("visiting ZExtInst");
}
void IrVisitorBase::visit(AllocaInst &node) {
    TRACE("visiting AllocaInst");
}
