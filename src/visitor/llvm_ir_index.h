#ifndef COMPILER_LLVM_IR_INDEX_H
#define COMPILER_LLVM_IR_INDEX_H
#include "ir_visitor_base.h"
#include "ir/global_variable.h"
#include <fstream>
#include <unordered_map>
#include "passes/module.h"
// #define INDEXER_
#ifdef INDEXER_
#define PRINTER_LOG(format, ...)                                  \
    do {                                                                             \
        printf(YELLOW "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define INDEXER_LOG(format, ...)
#endif

class LLVMIrIndexer : public IrVisitorBase {
public:
    explicit LLVMIrIndexer():depth(0) {
    }
    ~LLVMIrIndexer(){
    }
    Module::IRLevel ir_level;

private:
    void visit(UnaryInst *node){};
    void visit(BinaryInst *node){};
    void visit(AllocaInst *node){};
    void visit(StoreInst *node){};
    //  ---------
    void visit(Value *node){};
    void visit(CmpInst *node){};
    void visit(ReturnInst *node){};
    void visit(BranchInst *node){};
    void visit(LoadInst *node){};
    void visit(GetElementPtrInst *node){};
    void visit(CallInst *node){};
    void visit(ZExtInst *node){};
    void visit(HIR *node){};
    void visit(Function *node) final;
    void visit(Argument *node){};
    void visit(BaseBlock *node){};
    void visit(GlobalVariable *node){};
    void visit(ConstantInt *node){};
    void visit(ConstantFloat *node){};
    void visit(ConstantArray *node){};
    void visit(PhiInstr *node) {};

    void NameValue(Value *val);
    void NameBaseBlock(BaseBlock *base_block);
    void NameInstr(Instruction *instr);

    int depth;
    std::unordered_map<Value *, int> seq;
};

#endif//COMPILER_LLVM_IR_INDEXER_H
