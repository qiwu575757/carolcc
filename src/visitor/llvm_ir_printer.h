#ifndef COMPILER_LLVM_IR_PRINTER_H
#define COMPILER_LLVM_IR_PRINTER_H
#include "ir_visitor_base.h"
#include <fstream>
#include <unordered_map>
class LLVMIrPrinter : public IrVisitorBase {
public:
    explicit LLVMIrPrinter(std::string &name) {
        output_file.open(name);
    }

private:
    void visit(UnaryInst *node) final;
    void visit(BinaryInst *node) final;
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
    void visit(GlobalValue *node) final;
    void visit(ConstantInt *node) final;
    void visit(ConstantFloat *node) final;
    void visit(ConstantArray *node) final;

    void NameValue(Value *val);
    void NameBaseBlock(BaseBlock *base_block);
    void NameInstr(Instruction *instr);

    std::ofstream output_file;
    std::unordered_map<Value *, int> seq;
};

#endif//COMPILER_LLVM_IR_PRINTER_H
