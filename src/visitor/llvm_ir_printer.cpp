#include "llvm_ir_printer.h"
#include "ir/function.h"
#include "ir/instruction.h"
#include "utils.h"
#include <unordered_map>


int var_no = 1;
const std::string head = "%";
void LLVMIrPrinter::NameValue(Value *val) {
    if (seq.find(val) == seq.end() && val->getName().empty()) {
        auto seq_num = seq.size();
        val->setName(std::to_string(seq_num));
        seq.insert({val, seq_num});
    }
}
void LLVMIrPrinter::NameBaseBlock(BaseBlock *base_block) {
    NameValue(base_block);
    if (base_block->isBaiscBlock()) {
        auto basic_block = dynamic_cast<BasicBlock *>(base_block);
        INFO("HIR printer : basic block has %d instructions", basic_block->getNumOfInstr());
        for (auto &instr: basic_block->getInstructions()) {
            NameInstr(instr);
        }
    } else if (base_block->isIfBlock()) {
        auto if_block = dynamic_cast<IfBlock *>(base_block);
        for (auto &cond_base_block: *(if_block->getCondBaseBlockList())) {
            NameBaseBlock(cond_base_block);
        }
        for (auto &then_block: *(if_block->getIfBodyBaseBlockList())) {
            NameBaseBlock(then_block);
        }
        for (auto &else_block: *(if_block->getElseBodyBaseBlockList())) {
            NameBaseBlock(else_block);
        }
    } else if (base_block->isWhileBlock()) {
        auto while_block = dynamic_cast<WhileBlock *>(base_block);
        for (auto &cond_block: *(while_block->getCondBaseBlockList())) {
            NameBaseBlock(cond_block);
        }
        for (auto &body_block: *(while_block->getBodyBaseBlockList())) {
            NameBaseBlock(body_block);
        }
    } else {
        ERROR("error base block type");
    }
}
void LLVMIrPrinter::NameInstr(Instruction *instr) {
    if (seq.find(instr) == seq.end() && instr->getName().empty() && !instr->getType()->isVoidTy()) {
        auto seq_num = seq.size();
        instr->setName(std::to_string(seq_num));
        seq.insert({instr, seq_num});
    }
}
std::string getname(int n) {
    if (!n) {
        return "%" + std::to_string(var_no++);
    } else {
        return "%" + std::to_string(n + var_no);
    }
}
void LLVMIrPrinter::visit(AllocaInst *node) {
    if (node->isAlloca()) {
        output_file << "%" << node->getName() << " = "
                    << "alloca ";
        node->getType()->getPointerElementType()->print(output_file);
        output_file << ", align 4" << std::endl;
    } else {
        ERROR("null AllocaInst");
    }
}
void LLVMIrPrinter::visit(Function *node) {
    seq.clear();
    depth = 0;

    // 先给变量编号
    INFO("naming args");
    for (auto &arg: node->getArgs()) {
        NameValue(arg);
    }
    INFO("naming base blocks");
    for (auto &bb: node->getBaseBlocks()) {
        NameBaseBlock(bb);
    }

    INFO("printing func type");
    if(node->getBaseBlocks().size() == 0 || node->getBaseBlocks().size()==0){
        // declare
        output_file<<"declare ";
        node->getResultType()->print(output_file);
        output_file << node->getPrintName() << "(";
        for (int i = 0; i < node->getNumArgs(); i++) {
            auto &arg = node->getArgs().at(i);
            if (i != 0) {
                output_file << ",";
            }
            arg->getType()->print(output_file);
        }
        output_file << ")" << std::endl;
    }
    else {
        output_file << "define  dso_local ";
        INFO("printing func type");
        node->getResultType()->print(output_file);
        INFO("print func name");
        output_file << node->getPrintName() << "(";
        INFO("func arg number is %d", node->getNumArgs());
        for (int i = 0; i < node->getNumArgs(); i++) {
            auto &arg = node->getArgs().at(i);
            if (i != 0) {
                output_file << ",";
            }
            arg->accept(this);
        }
        output_file << ") {" << std::endl;
        INFO("base block size is %zu", node->getBaseBlocks().size());
        for (auto &base_block: node->getBaseBlocks()) {
            base_block->accept(this);
        }
        output_file << "}" << std::endl;
    }
}
void LLVMIrPrinter::visit(Argument *node) {
    node->getType()->print(output_file);
    output_file << node->getPrintName();
}
void LLVMIrPrinter::visit(BaseBlock *node) {
    INFO(" HIR printer visiting baseblock");
    if(stoi(node->getName())!=0){
    output_file << node->getName() << ":" << std::endl;

    }
    add_tab();
    if (node->isBaiscBlock()) {
        auto basic_block = dynamic_cast<BasicBlock *>(node);
        for (auto &instr: basic_block->getInstructions()) {
            print_tabs();
            instr->accept(this);
        }
    } else if (node->isIfBlock()) {
        auto if_block = dynamic_cast<IfBlock *>(node);
        for (auto &cond_base_block: *(if_block->getCondBaseBlockList())) {
            print_tabs();
            cond_base_block->accept(this);
        }
        for (auto &then_block: *(if_block->getIfBodyBaseBlockList())) {
            print_tabs();
            then_block->accept(this);
        }
        for (auto &else_block: *(if_block->getElseBodyBaseBlockList())) {
            print_tabs();
            else_block->accept(this);
        }
    } else if (node->isWhileBlock()) {
        auto while_block = dynamic_cast<WhileBlock *>(node);
        for (auto &cond_block: *(while_block->getCondBaseBlockList())) {
            print_tabs();
            cond_block->accept(this);
        }
        for (auto &body_block: *(while_block->getBodyBaseBlockList())) {
            print_tabs();
            body_block->accept(this);
        }
    } else {
        ERROR("error base block type");
    }
    delete_tab();
}
void LLVMIrPrinter::visit(LoadInst *node) {

    if (node->isLoad()) {
        output_file << "%" << node->getName() << " = "
                    << "load ";
        // 输出自己的类型
        node->getType()->print(output_file);
        output_file << ", ";
        // 输出第一个操作数
        node->getOperand(0)->getType()->print(output_file);
        output_file << node->getOperand(0)->getPrintName() << ", "
                    << "align 4" << std::endl;
    } else {
        ERROR("null LoadInst");
    }
}
void LLVMIrPrinter::visit(ReturnInst *node) {
    INFO("HIR printer visiting return stmt");
    output_file << "ret ";
    // node->getType()->print(output_file);
    if (node->getOperandNumber()!=0) {
        node->getOperand(0)->getType()->print(output_file);
        if (node->getOperand(0)->getType()->isIntegerTy() || node->getOperand(0)->getType()->isFloatTy()) {
            output_file << node->getOperand(0)->getPrintName();
        } else {
            ERROR("error type");
        }
    }
    output_file << std::endl;
}
void LLVMIrPrinter::visit(UnaryInst *node) {
    ERROR("todo");
}
void LLVMIrPrinter::visit(BinaryInst *node) {
    output_file<<node->getPrintName()<<" = "<<node->getOperatorString()<<" ";
                node->getType()->print(output_file);
    output_file<< node->getOperand(0)->getPrintName()<<", "
    << node->getOperand(1)->getPrintName()<<std::endl;

}
void LLVMIrPrinter::visit(StoreInst *node) {
    output_file << "store ";
    node->getOperand(0)->getType()->print(output_file);
    output_file << node->getOperand(0)->getPrintName() << ", ";
    node->getOperand(1)->getType()->print(output_file);
    output_file << node->getOperand(1)->getPrintName() << ", "
                << "align 4" << std::endl;
}
void LLVMIrPrinter::visit(Value *node) {
}
void LLVMIrPrinter::visit(CmpInst *node) {
}
void LLVMIrPrinter::visit(BranchInst *node) {
}
void LLVMIrPrinter::visit(GetElementPtrInst *node) {
    output_file<<node->getPrintName()<<" = "<<"getelementptr inbounds ";
    node->getElementType()->print(output_file);
    output_file<<", ";
    for(int i=0;i<node->getOperandNumber();i++){
        auto oprt = node->getOperand(i);
        if(i!=0){
            output_file<<", ";
        }
        oprt->getType()->print(output_file);
        output_file<<" "<<oprt->getPrintName();
    }
    output_file<<std::endl;
}
void LLVMIrPrinter::visit(CallInst *node) {
}
void LLVMIrPrinter::visit(ZExtInst *node) {
}
void LLVMIrPrinter::visit(HIR *node) {
}
void LLVMIrPrinter::visit(ConstantInt *node) {
}
void LLVMIrPrinter::visit(ConstantFloat *node) {
}
void LLVMIrPrinter::visit(ConstantArray *node) {
}
void LLVMIrPrinter::print_array_init(ConstantArray *array){
    INFO("printing array init");
    MyAssert("array is null", array != nullptr);
    //type
    array->getType()->print(output_file);
    auto array_type = static_cast<ArrayType *>(array->getType());

    // init val
    if(array_type->getElementType()->isInt32()||array_type->getElementType()->isFloatTy()){
        bool isZeroInitializer=true;
        for (size_t i = 0; i < array->getNumElements(); i++)
        {
            if(array_type->getElementType()->isInt32()){
                auto constant_int = static_cast<ConstantInt*>(array->getElement(i));
                if(constant_int->getValue()!=0){
                    isZeroInitializer=false;
                }
            }
            else if (array_type->getElementType()->isFloatTy()) {
                auto constant_float = static_cast<ConstantFloat*>(array->getElement(i));
                if(constant_float->getValue()!=0.0){
                    isZeroInitializer=false;
                }
            }
            else {
                ERROR("error type");
            }
        }
        if(isZeroInitializer){
            output_file<<"zeroinitializer ";
        }
        else {
            output_file<<"[";
            for (size_t i = 0; i < array->getNumElements(); i++)
            {
                array_type->getElementType()->print(output_file);
                output_file<<" "<<array->getElement(i)->getPrintName();
                if(i != array->getNumElements()-1){
                    output_file<<", ";
                }
            }
            output_file<<"]";
        }
    }
    else if(array_type->getElementType()->isArrayTy()){ // element type is a
        output_file<<"[ ";
        for (size_t i = 0; i < array->getNumElements(); i++)
        {
            print_array_init(dynamic_cast<ConstantArray*>(array->getElement(i)));
            if(i!= array->getNumElements()-1){
                output_file<<", ";
            }
        }
        output_file<<"] ";
        
    }


}
void LLVMIrPrinter::visit(GlobalVariable *node) {
    INFO("printing global var");
    output_file << node->getPrintName() << "= dso_local ";
    if (node->isConstant()) {
        output_file << "constant ";
    } else {
        output_file << "global ";
    }
    if(!node->getType()->getPointerElementType()->isArrayTy()){
        node->getType()->getPointerElementType()->print(output_file);
    }
    if (node->getInit() != nullptr) {
        INFO("get init ");
        if (node->getType()->getPointerElementType()->isFloatTy() || node->getType()->getPointerElementType()->isIntegerTy()) {
            output_file << node->getInit()->getPrintName();
        } else if (node->getType()->getPointerElementType()->isArrayTy() ) {
            print_array_init(static_cast<ConstantArray *>(node->getInit()));
        } else {
            ERROR("ERROR type");
        }
    } else {
        INFO("null init ");
        if (node->getType()->getPointerElementType()->isFloatTy() || node->getType()->getPointerElementType()->isIntegerTy()) {
            output_file << " 0";
        } else if(node->getType()->getPointerElementType()->isArrayTy() ) {
            
            node->getType()->getPointerElementType()->print(output_file);
            output_file<<"zeroinitializer";
        }
        else {
            ERROR("ERROR type");    
        }
    }
    output_file << ", align 4" << std::endl;
}
//void LLVMIrPrinter::visit(BranchInst*node) {//WHILE,IF,BRANCH,
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
