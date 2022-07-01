#ifndef HIR_PRINTER_H
#define HIR_PRINTER_H
#include "ir_visitor_base.h"
#include "passes/module.h"
#include <fstream>
#include <unordered_map>


class HIrPrinter : public IrVisitorBase {
public:
    explicit HIrPrinter(const std::string &name):depth(0) {
            printf("%s",name.c_str());
            printf("###########################################\n");
        }
    ~HIrPrinter(){
            printf("###########################################\n");
    }
    Module::IRLevel ir_level;

private:
    void print_left(std::string type,std::string name);
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
    void visit(GlobalVariable *node) final;
    void visit(ConstantInt *node) final;
    void visit(ConstantFloat *node) final;
    void visit(ConstantArray *node) final;

    void NameValue(Value *val);
    void NameBaseBlock(BaseBlock *base_block);
    void NameInstr(Instruction *instr);
    void print_array_init(ConstantArray *array);
    void print_tabs(){
        printf("━━━━━━━━");
    }
    void add_tab(){
        depth+=1;
    }
    void delete_tab(){
        if(depth>=0)
            depth-=1;
        else
            WARNNING("too many right shift tabs");
    }

    int depth;
};

#endif//HIR_PRINTER_H
