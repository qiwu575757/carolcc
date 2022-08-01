#include "llvm_ir_printer.h"
#include "ir/function.h"
#include "ir/instruction.h"
#include "utils.h"
#include <unordered_map>
#include "passes/module.h"

static int var_no = 1;
static const std::string head = "%";
void LLVMIrPrinter::NameValue(Value *val) {
    if (seq.find(val) == seq.end() ) {
      if(!val->getName().empty()){
        WARNNING("renaming %s",val->getName().c_str());
      }
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
        ERROR("error base block type",EXIT_CODE_ERROR_356);
    }
}
void LLVMIrPrinter::NameInstr(Instruction *instr) {
    if (seq.find(instr) == seq.end()  && !instr->getType()->isVoidTy()) {
      if(!instr->getName().empty()){
        WARNNING("renaming instr %s",instr->getName().c_str());
      }
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
        ERROR("null AllocaInst",EXIT_CODE_ERROR_357);
    }
}
void LLVMIrPrinter::visit(Function *node) {
    if(this->ir_level == Module::IRLevel::HIR){
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
        if(node->getBaseBlocks().size() == 0){
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
    else if(this->ir_level == Module::IRLevel::MIR_MEM){
        seq.clear();
        depth = 0;

        // 先给变量编号
        INFO("naming args");
        for (auto &arg: node->getArgs()) {
            NameValue(arg);
        }
        INFO("naming base blocks");
        for (auto &bb: node->getBasicBlocks()) {
            NameBaseBlock(bb);
        }

        INFO("printing func type");
        if( node->getNumBasicBlocks()==0){
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
            INFO("base block size is %zu", node->getBasicBlocks().size());
            for (auto &base_block: node->getBasicBlocks()) {
                base_block->accept(this);
            }
            output_file << "}" << std::endl;
        }
    }

}
void LLVMIrPrinter::visit(Argument *node) {
    node->getType()->print(output_file);
    output_file << node->getPrintName();
}
void LLVMIrPrinter::visit(BaseBlock *node) {
    INFO(" HIR printer visiting baseblock");
    add_tab();
    if (node->isBaiscBlock()) {
        auto basic_block = dynamic_cast<BasicBlock *>(node);
        if(!node->getFunction()->getBaseBlocks().empty() && *(node->getFunction()->getBaseBlocks().cbegin())!=node){
          output_file << node->getName() << ":" << std::endl;
        }
        else if(node->getFunction()->getBaseBlocks().empty()&&*(node->getFunction()->getBasicBlocks().cbegin())!=node ){
          output_file << node->getName() << ":" ;
          if(!basic_block->getPreBasicBlockList().empty()){
              output_file<<"\t\t\t\t\t\t\t\t\t\t;preds =";
          }
          int i = 0;
          for(auto& pred : basic_block->getPreBasicBlockList()){
              output_file<<pred->getPrintName();
              i++;
              if(i!=basic_block->getPreBasicBlockList().size()){
                  output_file<<", ";
              }
          }
          output_file<<std::endl;
        }
        for (auto &instr: basic_block->getInstructions()) {
            print_tabs();
            instr->accept(this);
        }
    } else if (node->isIfBlock()) {
        auto if_block = dynamic_cast<IfBlock *>(node);
        for (auto &cond_base_block:* (if_block->getCondBaseBlockList())) {
            // print_tabs();
            cond_base_block->accept(this);
        }
        for (auto &then_block: *(if_block->getIfBodyBaseBlockList())) {
            // print_tabs();
            then_block->accept(this);
        }
        for (auto &else_block: *(if_block->getElseBodyBaseBlockList())) {
            // print_tabs();
            else_block->accept(this);
        }
    } else if (node->isWhileBlock()) {
        auto while_block = dynamic_cast<WhileBlock *>(node);
        for (auto &cond_block: *(while_block->getCondBaseBlockList())) {
            // print_tabs();
            cond_block->accept(this);
        }
        for (auto &body_block: *(while_block->getBodyBaseBlockList())) {
            // print_tabs();
            body_block->accept(this);
        }
    } else {
        ERROR("error base block type",EXIT_CODE_ERROR_358);
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
        ERROR("null LoadInst",EXIT_CODE_ERROR_359);
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

            ERROR("error type",EXIT_CODE_ERROR_360);
        }
    }
    else
        output_file<<"void";
    output_file << std::endl;
}
void LLVMIrPrinter::visit(UnaryInst *node) {
    PRINTER_LOG("visiting llvm ir printer unary inst %d",node->isCast());
    output_file<<node->getPrintName()<<" = ";
    if(node->isCast()){
        if(node->getType()->isFloatTy() && node->getOperand(0)->getType()->isIntegerTy()){
            output_file<<"sitofp ";
            node->getOperand(0)->getType()->print(output_file);
            output_file<<node->getOperand(0)->getPrintName()<<" to ";
            node->getType()->print(output_file);
        }
        else if (node->getType()->isIntegerTy() && node->getOperand(0)->getType()->isFloatTy()){
            output_file<<"fptosi ";
            node->getOperand(0)->getType()->print(output_file);
            output_file<<node->getOperand(0)->getPrintName()<<" to ";
            node->getType()->print(output_file);
        }
        else {
            ERROR("error type",EXIT_CODE_ERROR_363);
        }
    }
    output_file<<std::endl;
}
void LLVMIrPrinter::visit(BinaryInst *node) {
    output_file<<node->getPrintName()<<" = "<<node->getOperatorString()<<" ";
                node->getType()->print(output_file);
    output_file<< node->getOperand(0)->getPrintName()<<", "
    << node->getOperand(1)->getPrintName()<<std::endl;

}
void LLVMIrPrinter::visit(MlaInst *node) {
    output_file << "%" << node->getName() << " = "
                    << "mla ";
    node->getOperand(0)->getType()->print(output_file);
    output_file << node->getOperand(0)->getPrintName() << ", ";
    node->getOperand(1)->getType()->print(output_file);
    output_file << node->getOperand(1)->getPrintName() << ", ";
    node->getOperand(2)->getType()->print(output_file);
    output_file << node->getOperand(2)->getPrintName() <<std::endl;
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
    ERROR("TODO",EXIT_CODE_ERROR_361);
}
void LLVMIrPrinter::visit(CmpInst *node) {
    output_file << node->getPrintName() << " = ";
    bool is_float = false;
    if(node->getOperand(0)->getType()->isFloatTy()){
        output_file<<"fcmp ";
        is_float=true;
    }
    else if(node->getOperand(0)->getType()->isIntegerTy()){
        output_file<<"icmp ";
        is_float=false;
    }
    else {
        ERROR("error type",EXIT_CODE_ERROR_362);
    }



    if(node->isEq()){
        if(!is_float)
            output_file<<"eq";
        else
            output_file<<"oeq";
    }
    else if(node->isNeq()){
        if(!is_float)
            output_file<<"ne";
        else
            output_file<<"one";
    }
    else if(node->isGe()){
        if(!is_float)
            output_file<<"sge";
        else
            output_file<<"oge";
    }
    else if(node->isGt()){
        if(!is_float)
            output_file<<"sgt";
        else
            output_file<<"ogt";
    }
    else if(node->isLt()){
        if(!is_float)
            output_file<<"slt";
        else
            output_file<<"olt";
    }
    else if(node->isLe()){
        if(!is_float)
            output_file<<"sle";
        else
            output_file<<"ole";
    }
    output_file<<" ";
    MyAssert("icmp has no oprt",node->getOperandNumber()==2,EXIT_CODE_ERROR_353);
    node->getOperand(0)->getType()->print(output_file);
    output_file<<node->getOperand(0)->getPrintName()<<", "<<node->getOperand(1)->getPrintName()<<std::endl;
}
void LLVMIrPrinter::visit(BranchInst *node) {
    output_file<<"br ";
    if(node->getOperandNumber()==1){
        output_file<<" label "<<node->getOperand(0)->getPrintName()<<std::endl;
    }
    else if(node->getOperandNumber()==2){
        output_file<<"i1 "<<node->getOperand(0)->getPrintName()<<", "
        <<"label "<<node->getOperand(1)->getPrintName()<<std::endl;
    }
    else if(node->getOperandNumber()==3){
        output_file<<"i1 "<<node->getOperand(0)->getPrintName()<<", "
        <<"label "<<node->getOperand(1)->getPrintName()<<","
        <<"label "<<node->getOperand(2)->getPrintName()<<std::endl;
    }
    else {
        ERROR("error branch oprt number",EXIT_CODE_ERROR_364);
    }
}
void LLVMIrPrinter::visit(GetElementPtrInst *node) {
    output_file<<node->getPrintName()<<" = "<<"getelementptr inbounds ";
    node->getOperand(0)->getType()->getPointerElementType()->print(output_file);
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
    if(!node->getType()->isVoidTy()){
        WARNNING("printing call number");
        output_file<<node->getPrintName()<<" = ";
    }
    output_file<<"call ";
    node->getType()->print(output_file);
    output_file<<node->getOperand(0)->getPrintName()<<"(";
    for(int i=1;i<node->getOperandNumber();i++){
      auto oprt = node->getOperand(i);
      oprt->getType()->print(output_file);
      output_file<<" "<<oprt->getPrintName();
      if(i!=node->getOperandNumber()-1){
        output_file<<", ";
      }
    }
    output_file<<")"<<std::endl;

}
void LLVMIrPrinter::visit(ZExtInst *node) {
    output_file << node->getPrintName() << " = "
                << "zext ";
    node->getOperand(0)->getType()->print(output_file);
    output_file << node->getOperand(0)->getPrintName() << " to ";
    node->getType()->print(output_file);
    output_file<<std::endl;
}
void LLVMIrPrinter::visit(HIR *node) {
    if(node->isBreak()){
        output_file<<"break" <<std::endl;
    }
    else if(node->isContinue()){
        output_file<<"continue" <<std::endl;
    }
    else {
        ERROR("error  type",EXIT_CODE_ERROR_365);
    }

}

void LLVMIrPrinter::visit(ConstantInt *node) {
    ERROR("TODO",EXIT_CODE_ERROR_366);
}
void LLVMIrPrinter::visit(ConstantFloat *node) {
    ERROR("TODO",EXIT_CODE_ERROR_367);
}
void LLVMIrPrinter::visit(ConstantArray *node) {
    ERROR("TODO",EXIT_CODE_ERROR_368);
}
void LLVMIrPrinter::print_array_init(ConstantArray *array){
    INFO("printing array init");
    MyAssert("array is null", array != nullptr,EXIT_CODE_ERROR_355);
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
                ERROR("error type",EXIT_CODE_ERROR_369);
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
            ERROR("ERROR type",EXIT_CODE_ERROR_370);
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
            ERROR("ERROR type",EXIT_CODE_ERROR_371);
        }
    }
    output_file << ", align 4" << std::endl;
}
void LLVMIrPrinter::visit(PhiInstr *node) {
    output_file<<node->getPrintName()<<" = phi ";
    node->getType()->print(output_file);
    bool has_uncertain=false;
    for(int i=0;i<node->getOperandNumber();i++){
        output_file<<"[ ";
        auto val = node->getOperand(i++);
        if(val == Value::getUncertainValue()){
            has_uncertain = true;
            output_file<<"0";
        }
        else {
            output_file<<val->getPrintName();
        }
        output_file<< ", "
        << node->getOperand(i)->getPrintName()
        << "] ";
        if( i !=node->getOperandNumber()-1)
            output_file<<",";
    }
    if(has_uncertain){
        output_file<<"\t\t; has uncertain";
    }
    output_file<<std::endl;
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
