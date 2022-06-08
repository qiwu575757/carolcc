#include "llvm_ir_printer.h"
#include "ir/instruction.h"
#include "utils.h"
int var_no=1;
const std::string head="%";
std::string getname( int n ){
    if(!n){
        return "%" + std::to_string(var_no++);
    }else{
        return "%" + std::to_string( n + var_no);
    }
}
void LLVMIrPrinter::visit(UnaryInst*node) {
    if(node->isNot()){//直接加负号
        if(node->getType()->isIntegerTy()){//%6 = sub i32 0, %5
            printf("%s = sub i32 0, %s", getname(0), node->getOperand(0)->getName());
        }else if(node->getType()->isFloatTy()){ // %5 = fsub float -0.000000e+00, %4
            printf("%s = fsub float -0.000000e+00, %s", getname(0), node->getOperand(0)->getName());
        }else if(node->getType()->isBool()){   //%4 = load i8, i8* %2, align 1
                                               //%5 = trunc i8 %4 to i1
                                               //%6 = zext i1 %5 to i32
                                               //%7 = sub nsw i32 0, %6
            printf("%s = load i8, i8* %s, align 1", getname(0), node->getOperand(0)->getName());
            printf("%s = trunc i8 %s to i1", getname(0), getname(-1));
            printf("%s = zext i1 %s to i32", getname(0), getname(-1));
            printf("%s = sub nsw i32 0, %s", getname(0), getname(-1));
        }else{
            WARNNING("wrong Type");
        }
    }else if(node->isNeg()){//!a 

    }else{
        WARNNING("null UnaryInst");
    }

}
void LLVMIrPrinter::visit(BinaryInst*node) {
    
}
void LLVMIrPrinter::visit(AllocaInst*node) {
    
}
void LLVMIrPrinter::visit(StoreInst*node) {
    
}