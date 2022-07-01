#include "asm/asm_builder.h"
#include "utils.h"
std::string AsmBuilder::generate_asm(std::map<Value *, int> register_mapping){
    ERROR("UNDO");
}
std::string AsmBuilder::generate_asm(){
    asm_code += generate_module_header();
    for(auto &func: this->module->getFunctions()){
        if(func->getBasicBlocks().size() != 0){
            asm_code += generate_function_code();
        }
    }
    asm_code += generate_module_tail();
    return asm_code;
}
std::string AsmBuilder::generate_module_header(){
    return "";
}
std::string AsmBuilder::generate_module_tail(){
    return "";
}
std::string AsmBuilder::generate_function_code(Function *func){
    std::string func_asm;
    func_asm += generate_function_entry_code();

    for (auto &bb : func->getBasicBlocks()) {
        func_asm += getLabelName(bb) + ":" + InstGen::newline;
        func_asm += generateBasicBlockCode(bb);
    }

    func_asm += generate_function_exit_code();

    return func_asm;
}
std::string AsmBuilder::generate_function_entry_code(){
    return "";
}
std::string AsmBuilder::generate_function_exit_code(){
    return "";
}
std::string AsmBuilder::update_value_mapping(std::list<Value>){
    return "";
}

std::string  AsmBuilder::generateBasicBlockCode(BasicBlock *bb){
    return "";
}
std::string  AsmBuilder::getLabelName(BasicBlock *bb){
    return "";
}