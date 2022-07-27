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
    explicit LLVMIrIndexer(){
    }
    ~LLVMIrIndexer(){
    }
    Module::IRLevel ir_level;

private:
    //  ---------
    void visit(Function *node) final;

    void NameValue(Value *val);
    void NameBaseBlock(BaseBlock *base_block);
    void NameInstr(Instruction *instr);
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
    std::unordered_map<Value *, int> seq;
};

#endif//COMPILER_LLVM_IR_INDEXER_H
