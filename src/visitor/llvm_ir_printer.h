#ifndef COMPILER_LLVM_IR_PRINTER_H
#define COMPILER_LLVM_IR_PRINTER_H
#include "ir_visitor_base.h"
#include "ir/global_variable.h"
#include <fstream>
#include <unordered_map>
#include "passes/module.h"
// #define PRINTER_
#ifdef PRINTER_
#define PRINTER_LOG(format, ...)                                  \
    do {                                                                             \
        printf(YELLOW "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define PRINTER_LOG(format, ...)
#endif

class LLVMIrPrinter : public IrVisitorBase {
public:
    explicit LLVMIrPrinter(const std::string &name,const std::string &module_name):depth(0) {
        output_file.open(name);
        output_file<<"; ModuleID = \'"<<module_name<<"\'\n";
    }
    ~LLVMIrPrinter(){
        output_file.close();
    }
    Module::IRLevel ir_level;

private:
    void visit(UnaryInst *node) final;
    void visit(BinaryInst *node) final;
    void visit(MlaInst *node) final;
    void visit(AllocaInst *node) final;
    void visit(StoreInst *node) final;
    //  ---------
    void visit(Value *node) final;
    void visit(CmpInst *node) final;
    void visit(ReturnInst *node) final;
    void visit(BranchInst *node) final;
    void visit(LoadInst *node) final;
    void visit(GetElementPtrInst *node) final;
    void visit(CallInst *node) final;
    void visit(ZExtInst *node) final;
    void visit(HIR *node) final;
    void visit(Function *node) final;
    void visit(Argument *node) final;
    void visit(BaseBlock *node) final;
    void visit(GlobalVariable *node) final;
    void visit(ConstantInt *node) final;
    void visit(ConstantFloat *node) final;
    void visit(ConstantArray *node) final;
    void visit(PhiInstr *node) final;

    void NameValue(Value *val);
    void NameBaseBlock(BaseBlock *base_block);
    void NameInstr(Instruction *instr);
    void print_array_init(ConstantArray *array);
    void print_tabs(){
//      if(ir_level ==Module::HIR)
        output_file<<std::string(depth,' ')  ;
    }
    void add_tab(){
        depth+=4;
    }
    void delete_tab(){
        if(depth>=4)
            depth-=4;
        else
            WARNNING("too many right shift tabs");
    }

    int depth;
    std::ofstream output_file;
    std::unordered_map<Value *, int> seq;
};

#endif//COMPILER_LLVM_IR_PRINTER_H
