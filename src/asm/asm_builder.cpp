#include "asm/asm_builder.h"
#include "utils.h"
std::string AsmBuilder::generate_asm(std::map<Value *, int> register_mapping){
    ERROR("UNDO");
}
std::string AsmBuilder::generate_asm(){
    asm += generate_module_header();
    for(auto &func: this->module->getFunction()){
        if(func->get){
            asm += generate_function_code();
        }
    }
    asm += generate_module_tail();
    return asm;
}
std::string AsmBuilder::generate_module_header(){
    return "";
}
std::string AsmBuilder::generate_module_tail(){
    return "";
}
std::string AsmBuilder::generate_function_code(){
    std::string func_asm;
    func_asm += generate_function_entry_code();
    '''
    '''


    '''
    '''
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