#ifndef COMPILER_IR_TO_C_H
#define COMPILER_IR_TO_C_H
#include "visitor/ir_visitor_base.h"
class IrToC : public IrVisitorBase {

    void visit(UnaryInst &node) final;
    void visit(BinaryInst &node) final;
};

#endif//COMPILER_IR_TO_C_H
