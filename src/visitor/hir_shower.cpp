#include "hir_shower.h"
#include "ir/function.h"
#include "ir/instruction.h"
#include "utils.h"
#include <unordered_map>
#include "passes/module.h"
void HIrPrinter::print_left(std::string type,std::string name){
    for(int i=0;i<depth;i++){
        print_tabs();
    }
    printf("┨ %s %s\n",type.c_str(),name.c_str());
}

void HIrPrinter::visit(AllocaInst *node) {
    print_left("AllocaInst","");
}

void HIrPrinter::visit(Function *node) {
    depth = 1;
    print_left("Function",node->getPrintName());
    for (auto &bb: node->getBaseBlocks()) {
        add_tab();
        bb->accept(this);
        delete_tab();
    }
}
void HIrPrinter::visit(Argument *node) {
    print_left("Argument","");
}
void HIrPrinter::visit(BaseBlock *node) {
    
    if (node->isBaiscBlock()) {
        print_left("BaseBlock","");
        add_tab();
        auto basic_block = dynamic_cast<BasicBlock *>(node);
        for (auto &instr: basic_block->getInstructions()) {
            instr->accept(this);
        }
        delete_tab();
    } else if (node->isIfBlock()) {
        print_left("IfBlock","");
        add_tab();
        auto if_block = dynamic_cast<IfBlock *>(node);
        print_left("IfBlock(cond)","");
        for (auto &cond_base_block:* (if_block->getCondBaseBlockList())) {
            cond_base_block->accept(this);
        }
      print_left("IfBlock(then)","");
        for (auto &then_block: *(if_block->getIfBodyBaseBlockList())) {
            then_block->accept(this);
        }
      print_left("IfBlock(else)","");
        for (auto &else_block: *(if_block->getElseBodyBaseBlockList())) {
            else_block->accept(this);
        }
        delete_tab();

    } else if (node->isWhileBlock()) {
        print_left("WhileBlock","");
        add_tab();
        auto while_block = dynamic_cast<WhileBlock *>(node);
        print_left("WhileBlock(cond)","");
        for (auto &cond_block: *(while_block->getCondBaseBlockList())) {
            cond_block->accept(this);
        }
        print_left("WhileBlock(body)","");
        for (auto &body_block: *(while_block->getBodyBaseBlockList())) {
            body_block->accept(this);
        }
        delete_tab();

    } else {
        ERROR("error base block type");
    }

}
void HIrPrinter::visit(LoadInst *node) {
    print_left("AllocaInst","");
}
void HIrPrinter::visit(ReturnInst *node) {
    print_left("ReturnInst","");
}
void HIrPrinter::visit(UnaryInst *node) {
    print_left("UnaryInst","");
}
void HIrPrinter::visit(BinaryInst *node) {
    print_left("BinaryInst","");
}
void HIrPrinter::visit(StoreInst *node) {
       print_left("StoreInst","");
}
void HIrPrinter::visit(Value *node) {
    print_left("Value","");
}
void HIrPrinter::visit(CmpInst *node) {
   print_left("CmpInst","");
}
void HIrPrinter::visit(BranchInst *node) {
    print_left("BranchInst","");
}
void HIrPrinter::visit(GetElementPtrInst *node) {
    print_left("GetElementPtrInst","");
}
void HIrPrinter::visit(CallInst *node) {
    print_left("CallInst","");
}
void HIrPrinter::visit(ZExtInst *node) {
    print_left("ZExtInst","");
}

void HIrPrinter::visit(HIR *node) {
    if(node->isBreak()){
        print_left("break","");
    }
    else if(node->isContinue()){
        print_left("continue","");
    }
    else {
        ERROR("error  type");
    }
}

void HIrPrinter::visit(ConstantInt *node) {
    print_left("ConstantInt","");
}
void HIrPrinter::visit(ConstantFloat *node) {
    print_left("ConstantFloat","");
}
void HIrPrinter::visit(ConstantArray *node) {
    print_left("ConstantArray","");
}
void HIrPrinter::print_array_init(ConstantArray *array){
    print_left("ConstantArray","");
}
void HIrPrinter::visit(GlobalVariable *node) {
    print_left("GlobalVariable","");
}
//void HIrPrinter::visit(BranchInst*node) {//WHILE,IF,BRANCH,
//    if(node->isWhile()){
//        if(node->getType()->isIntegerTy()){// %5 = load i32, i32* %2, align 4
//        //不太确定
//            printf("%s = load i32, i32* %s, align 4", getname(0), node->getOperand(0)->getName());
//        }else if(node->getType()->isFloatTy()){ // %5 = load float, float* %2, align 4
//            printf("%s = load float, float* %s, align 4", getname(0), node->getOperand(0)->getName());
//        }else if(node->getType()->isBool()){//%8 = load i8, i8* %3, align 1
//            printf("%s = load i8, i8* %s, align 1", getname(0), node->getOperand(0)->getName());
//        }else{
//            WARNNING("wrong Type");
//        }
//    }else if(node->isIf()){
//    }else if(node->isBranch()){
//        WARNNING("null BranchInst");
//    }
//}
