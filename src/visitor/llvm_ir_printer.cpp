#include "llvm_ir_printer.h"
#include "ir/instruction.h"
#include "utils.h"
int var_no=1;
const std::string head="%";
std::string getname(){
    return "%" + std::to_string(var_no++);
}