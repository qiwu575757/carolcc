#ifndef COMPILER_IR_VISITOR_BASE_H
#define COMPILER_IR_VISITOR_BASE_H
#include "ir/instruction.h"
class IrVisitorBase {
    virtual void visit(UnaryInst &node) = 0;
    virtual void visit(BinaryInst &node) = 0;
};
#endif//COMPILER_IR_VISITOR_BASE_H
