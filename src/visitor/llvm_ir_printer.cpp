#include "llvm_ir_printer.h"
#include "ir/instruction.h"
#include "utils.h"
int var_no=1;
const std::string head="%";
std::string getname(){
    return "%" + std::to_string(var_no++);
}
void LLVMIrPrinter::visit(UnaryInst*node) {

    if(node->isNeg()){//将目的操作数的所以数据位取反加
        if(node->getType()->isIntegerTy()){
            
        }else if(node->getType()->isFloatTy()){

        }else{
            WARNNING("wrong Type");
        }
    }else if(node->isNot()){//直接加负号

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