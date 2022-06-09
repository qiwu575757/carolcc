#include "llvm_ir_printer.h"
#include "ir/instruction.h"
#include "ir/function.h"
#include "utils.h"
int var_no=1;
const std::string head="%";
void LLVMIrPrinter::NameValue(Value *val) {
    if(seq.find(val) == seq.end() && val->getName().empty()){
        auto seq_num = seq.size() ;
        val->setName(std::to_string(seq_num));
        seq.insert({val,seq_num});
    }
}
void LLVMIrPrinter::NameBaseBlock(BaseBlock *base_block) {
    NameValue(base_block);
    if(base_block->isBaiscBlock()){
        auto basic_block = dynamic_cast<BasicBlock*> (base_block);
        for(auto& instr : basic_block->getInstructions()){
            NameInstr(instr);
        }
    }
    else if(base_block->isIfBlock()){
        auto if_block = dynamic_cast<IfBlock*> (base_block);
        for(auto& cond_base_block : *(if_block->getCondBaseBlockList())){
            NameBaseBlock(cond_base_block);
        }
        for(auto & then_block : *(if_block->getIfBodyBaseBlockList())){
            NameBaseBlock(then_block);
        }
        for(auto & else_block : *(if_block->getElseBodyBaseBlockList())){
            NameBaseBlock(else_block);
        }
    }
    else if (base_block->isWhileBlock()){
        auto while_block = dynamic_cast<WhileBlock*> (base_block);
        for(auto & cond_block : *(while_block->getCondBaseBlockList())){
            NameBaseBlock(cond_block);
        }
        for(auto & body_block : *(while_block->getBodyBaseBlockList())){
            NameBaseBlock(body_block);
        }
    }
    else {
        ERROR("error base block type");
    }

}
void LLVMIrPrinter::NameInstr(Instruction *instr) {
    if(seq.find(instr) == seq.end() && instr->getName().empty() && !instr->getType()->isVoidTy()){
        auto seq_num = seq.size() ;
        instr->setName(std::to_string(seq_num));
        seq.insert({instr,seq_num});
    }
}
std::string getname( int n ){
    if(!n){
        return "%" + std::to_string(var_no++);
    }else{
        return "%" + std::to_string( n + var_no);
    }
}
//void LLVMIrPrinter::visit(UnaryInst*node) {
//    if(node->isNot()){//直接加负号
//        if(node->getType()->isIntegerTy()){//%6 = sub i32 0, %5
//            printf("%s = sub i32 0, %s", getname(0), node->getOperand(0)->getName());
//        }else if(node->getType()->isFloatTy()){ // %5 = fsub float -0.000000e+00, %4
//            printf("%s = fsub float -0.000000e+00, %s", getname(0), node->getOperand(0)->getName());
//        }else if(node->getType()->isBool()){   //%4 = load i8, i8* %2, align 1
//                                               //%5 = trunc i8 %4 to i1
//                                               //%6 = zext i1 %5 to i32
//                                               //%7 = sub nsw i32 0, %6
//            printf("%s = load i8, i8* %s, align 1", getname(0), node->getOperand(0)->getName());
//            printf("%s = trunc i8 %s to i1", getname(0), getname(-1));
//            printf("%s = zext i1 %s to i32", getname(0), getname(-1));
//            printf("%s = sub nsw i32 0, %s", getname(0), getname(-1));
//        }else{
//            WARNNING("wrong Type");
//        }
//    }else if(node->isNeg()){//!a
//        if(node->getType()->isIntegerTy()){//%5 = icmp ne i32 %4, 0
//                                           //%6 = xor i1 %5, true
//                                           //%7 = zext i1 %6 to i32
//            printf("%s = icmp ne i32 %s, 0", getname(0), node->getOperand(0)->getName());
//            printf("%s = xor i1 %s, true", getname(0), getname(-1));
//            printf("%s = zext i1 %s to i32", getname(0), getname(-1));
//        }else if(node->getType()->isFloatTy()){ // %5 = fcmp une float %4, 0.000000e+00
//                                                // %6 = xor i1 %5, true
//                                                // %7 = zext i1 %6 to i32
//                                                // %8 = sitofp i32 %7 to float
//            printf("%s = fcmp une float %s, 0.000000e+00", getname(0), node->getOperand(0)->getName());
//            printf("%s = xor i1 %s, true", getname(0), getname(-1));
//            printf("%s = zext i1 %s to i32", getname(0), getname(-1));
//            printf("%s = sitofp i32 %s to float", getname(0), getname(-1));
//        }else if(node->getType()->isBool()){   //%4 = load i8, i8* %2, align 1
//                                               //%5 = trunc i8 %4 to i1
//                                               //%6 = xor i1 %5, true
//                                               //%7 = zext i1 %6 to i8
//            printf("%s = load i8, i8* %s, align 1", getname(0), node->getOperand(0)->getName());
//            printf("%s = trunc i8 %s to i1", getname(0), getname(-1));
//            printf("%s = xor i1 %s, true", getname(0), getname(-1));
//            printf("%s = zext i1 %s to i8", getname(0), getname(-1));
//        }else{
//            WARNNING("wrong Type");
//        }
//    }else{
//        WARNNING("null UnaryInst");
//    }
//}
//void LLVMIrPrinter::visit(BinaryInst*node) {
//    if(node->isAdd()){
//        if(node->getType()->isIntegerTy()){//%7 = add nsw i32 %5, %6
//            printf("%s = add nsw i32 %s, %s", getname(0), node->getOperand(0)->getName(),node->getOperand(1)->getName());
//        }else if(node->getType()->isFloatTy()){ // %7 = fadd float %5, %6
//            printf("%s = fadd float %s, %s", getname(0),  node->getOperand(0)->getName(),node->getOperand(1)->getName());
//        }else if(node->getType()->isBool()){   //bool 加法会出现吗？我看llvm里是全部转成int然后加
//
//        }else{
//            WARNNING("wrong Type");
//        }
//    }else if(node->isSub()){
//        if(node->getType()->isIntegerTy()){//%7 = sub nsw i32 %5, %6
//            printf("%s = sub nsw i32 %s, %s", getname(0), node->getOperand(0)->getName(),node->getOperand(1)->getName());
//        }else if(node->getType()->isFloatTy()){ // %7 = fsub float %5, %6
//            printf("%s = fsub float %s, %s", getname(0), node->getOperand(0)->getName(),node->getOperand(1)->getName());
//        }else if(node->getType()->isBool()){//同上
//        }else{
//            WARNNING("wrong Type");
//        }
//    }else if(node->isMul()){
//        if(node->getType()->isIntegerTy()){//%7 = mul nsw i32 %5, %6
//            printf("%s = mul nsw i32 %s, %s", getname(0), node->getOperand(0)->getName(),node->getOperand(1)->getName());
//        }else if(node->getType()->isFloatTy()){ // %7 = fmul float %5, %6
//            printf("%s = fmul float %s, %s", getname(0), node->getOperand(0)->getName(),node->getOperand(1)->getName());
//        }else if(node->getType()->isBool()){//同上
//        }else{
//            WARNNING("wrong Type");
//        }
//    }else if(node->isDiv()){
//        if(node->getType()->isIntegerTy()){//%7 = sdiv i32 %5, %6
//            printf("%s = sdiv i32 %s, %s", getname(0), node->getOperand(0)->getName(),node->getOperand(1)->getName());
//        }else if(node->getType()->isFloatTy()){ // %7 = fdiv float %5, %6
//            printf("%s = fdiv float %s, %s", getname(0), node->getOperand(0)->getName(),node->getOperand(1)->getName());
//        }else if(node->getType()->isBool()){//同上
//        }else{
//            WARNNING("wrong Type");
//        }
//    }else if(node->isRem()){
//    }else{
//        WARNNING("null BinaryInst");
//    }
//}
void LLVMIrPrinter::visit(AllocaInst*node) {
    if(node->isAlloca()){
        
    }else{
        WARNNING("null AllocaInst");
    }
}
void LLVMIrPrinter::visit(StoreInst*node) {
    if(node->isStore()){
        
    }else{
        WARNNING("null StoreInst");
    }
}
void LLVMIrPrinter::visit(Function *node) {
    for(auto &bb : node->getBaseBlocks()){
        NameBaseBlock(bb);
    }

    output_file<< "define  dso_local ";
    node->getResultType()->print(output_file);
    output_file<<"@"<<node->getName()<<"(";
    for(auto & arg : node->getArgs()){
        arg->accept(this);
    }

}
void LLVMIrPrinter::visit(Argument *node) {
    NameValue(node);
    if(node->getType()->isPointerTy()){
        for(int i=1;i<node->getArrayBound().size();i++){
            output_file<<"[ ";
            auto bound =  static_cast<ConstantInt*> (node->getArrayBound().at(i));
            output_file<<bound->getValue()<<" x ";
        }
        node->getType()->getPointerElementType()->print(output_file);
        for(int i=1;i<node->getArrayBound().size();i++){
            output_file<<"]";
        }
        output_file<<"* ";
    }
    else {
        output_file<<node->getType()<<" "<<node->getName();
    }
    output_file<<node->getName();
}
