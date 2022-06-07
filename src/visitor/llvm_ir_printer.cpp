#include "llvm_ir_printer.h"
#include "ir/instruction.h"
#include "utils.h"
int var_no=1;
const std::string head="%";
std::string getname(){
    return "%" + std::to_string(var_no++);
}
void SYSYBuilder::visit(tree_comp_unit &node) {
    INFO("line:%d",node._line_no);
    MyAssert("no function defined", node.functions.size() != 0);
    INFO(" comp_unit functions %d definitions %d 42", node.functions.size(), node.definitions.size());
    // 这种写法会允许 后置全局变量定义出现，这是因为语法设计的问题
    for (auto defs: node.definitions) {
        G_in_global_init = true;
        defs->accept(*this);
        G_in_global_init = false;
    }
    for (auto func: node.functions) {
        INFO("visiting %s", func->id.c_str());
        func->accept(*this);
    }
}