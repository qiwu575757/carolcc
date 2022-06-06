#include "utils.h"
#include "ir_to_c.h"
#include "ir/instruction.h"
int var_no=0;
void IrToC::visit(UnaryInst &node) {
    if (node._op_id == Instruction::NEG) {
        printf("");
    }else if (node._op_id == Instruction::NOT) {
        printf("");
    }
}
void IrToC::visit(BinaryInst &node) {
    node.getOperand(0);
    node.getOperand(1);
    if (node.isAdd() == Instruction::ADD) {
        printf("int ");
    }else if (node._op_id == Instruction::SUB) {
        printf("");
    }else if (node._op_id == Instruction::MUL) {
        printf("");
    }else if (node._op_id == Instruction::DIV) {
        printf("");
    }else if (node._op_id == Instruction::REM) {
        printf("");
    }else if (node._op_id == Instruction::SHL) {
        printf("");
    }else if (node._op_id == Instruction::LSHR) {
        printf("");
    }else if (node._op_id == Instruction::ASHR) {
        printf("");
    }else if (node._op_id == Instruction::AND) {
        printf("");
    }else if (node._op_id == Instruction::OR) {
        printf("");
    }else if (node._op_id == Instruction::XOR) {
        printf("");
    }
}
void IrToC::visit(CmpInst &node) {
    if (node._op_id == Instruction::CMP) {
        printf("");
    }else if (node._op_id == CmpInst::EQ) {
        printf("");
    }else if (node._op_id == CmpInst::NEQ) {
        printf("");
    }else if (node._op_id == CmpInst::GT) {
        printf("");
    }else if (node._op_id == CmpInst::GE) {
        printf("");
    }else if (node._op_id == CmpInst::LT) {
        printf("");
    }else if (node._op_id == CmpInst::LE) {
        printf("");
    }
}
void IrToC::visit(ReturnInst &node) {
    if (node._op_id == Instruction::RET) {
        printf("");
    }
}
void IrToC::visit(BranchInst &node) {
    if (node._op_id == Instruction::BR) {
        printf("");
    }else if (node._op_id == BranchInst::IF) {
        printf("");
    }else if (node._op_id == BranchInst::WHILE) {
        printf("");
    }else if (node._op_id == BranchInst::BRANCH) {
        printf("");
    }
}
void IrToC::visit(StoreInst &node) {
    if (node._op_id == Instruction::STORE) {
        printf("");
    }
}
void IrToC::visit(LoadInst &node) {
    if (node._op_id == Instruction::LOAD) {
        printf("");
    }
}
void IrToC::visit(GetElementPtrInst &node) {
    if (node._op_id == Instruction::GEP) {
        printf("");
    }
}
void IrToC::visit(CallInst &node) {
    if (node._op_id == Instruction::Call) {
        printf("");
    }
}
void IrToC::visit(ZExtInst &node) {
    if (node._op_id == Instruction::ZExt) {
        printf("");
    }
}
void IrToC::visit(AllocaInst &node) {
    if (node._op_id == Instruction::ALLOCA) {
        printf("");
    }
}
