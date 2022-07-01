#include "asm/asm_builder.h"
#include "asm/asm_instr.h"
#include "utils.h"
std::string AsmBuilder::generate_asm(std::map<Value *, int> register_mapping){
    ERROR("UNDO");
}
std::string AsmBuilder::generate_asm(){
    asm_code += generate_module_header();
    for(auto &func: this->module->getFunctions()){
        if(func->getBasicBlocks().size() != 0){
            asm_code += generate_function_code(func);
        }
    }
    asm_code += generate_module_tail();
    return asm_code;
}
std::string AsmBuilder::generate_module_header(){
    std::string module_header_code;
    module_header_code += InstGen::spaces + ".arch armv" + std::to_string(arch_version) +
              "-a" + InstGen::newline;
    module_header_code += InstGen::spaces + ".file" + " " + "\"" +
                "_FILE_NAME_\"" + InstGen::newline;
    module_header_code += InstGen::spaces + ".text" + InstGen::newline;
    for (auto &func : this->module->getFunctions()) {
        if (func->getBasicBlocks().size()) {
            module_header_code += InstGen::spaces + ".global" + " " 
            + func->getName() + InstGen::newline;
        }
    }
    module_header_code += InstGen::spaces + ".arm" + InstGen::newline;
    return module_header_code;
}
// std::string AsmBuilder::generate_module_tail(){
//     std::string module_tail_code;
//     module_tail_code += InstGen::spaces + ".data" + InstGen::newline;
//     module_tail_code += CodeGen::generateGlobalVarsCode();
//     return module_tail_code;
// }
std::string AsmBuilder::generate_function_code(Function *func){
    std::string func_asm;
    func_asm += generate_function_entry_code(func);

    for (auto &bb : func->getBasicBlocks()) {
        func_asm += getLabelName(bb) + ":" + InstGen::newline;
        func_asm += generateBasicBlockCode(bb);
    }

    func_asm += generate_function_exit_code(func);

    return func_asm;
}
std::string AsmBuilder::generate_function_entry_code(Function *func){
    std::string func_head_code;
    //func name
    func_head_code+=getLabelName(func,0) + ":" + InstGen::newline;
    //get callee save registers
    // callee_save_regs
    std::vector<InstGen::Reg> callee_reg_list;
    for(auto reg : callee_save_regs) callee_reg_list.push_back(reg);
    //push (save register and lr)
    func_head_code += InstGen::push(callee_reg_list);
    //sp sub
    func_head_code += InstGen::instConst(InstGen::sub, InstGen::sp, InstGen::sp,
                                 InstGen::Constant(this->stack_size));
    //arg in reg or stack
    // for (auto &arg : func->getArgs()) {
    //     bool extended = false;
    //     auto sizeof_val = arg->getType()->getSize(extended);
    //     sizeof_val = ((sizeof_val + 3) / 4) * 4;
        
    //     this->stack_mapping.erase(dummy);
    //     source.push_back(dummy);
    //     target.push_back(arg);
    // }
    return func_head_code;
}
std::string AsmBuilder::generate_function_exit_code(Function *func){
    std::string func_tail_code;
    // sp add
    func_tail_code += InstGen::instConst(InstGen::add, InstGen::sp, InstGen::sp,
                                 InstGen::Constant(this->stack_size));
    // pop (save register and pc)
    std::vector<InstGen::Reg> caller_reg_list;
    for(auto reg : caller_save_regs) caller_reg_list.push_back(reg);
    func_tail_code += InstGen::pop(caller_reg_list);
    return func_tail_code;
}
std::string AsmBuilder::update_value_mapping(std::list<Value*> update_v){
    std::string alloc_reg_asm;
    /****/
    for(auto upd_v : update_v){

        // check if value is in list
        bool hit_v = false;
        int lru_index = 0;
        Value *be_replaced_v;

        for(auto v = lru_list.begin(); v != lru_list.end();){
            lru_index+=1;
            if(lru_index==reg_num){
                be_replaced_v = *v;
            }
            if(upd_v->getName() == (*v)->getName()){
                hit_v = true;
                if(lru_index<=reg_num){ // don't need to split
                    lru_list.emplace_front(*v);
                    lru_list.erase(v++);
                }
                else{ // split
                    int be_replaced_v_src = register_mapping[be_replaced_v];// reg
                    int be_replaced_v_dst = stoi(be_replaced_v->getName());//stack
                    int used_v_src = stoi((*v)->getName());//stack
                    int used_v_dst = be_replaced_v_src;//reg

                    lru_list.emplace_front(*v);

                    // data update
                    alloc_reg_asm += InstGen::str(InstGen::Reg(be_replaced_v_src),InstGen::Addr(InstGen::sp,be_replaced_v_dst<<2));
                    alloc_reg_asm += InstGen::ldr(InstGen::Reg(used_v_dst),InstGen::Addr(InstGen::sp,used_v_src<<2));
                    //map update
                    register_mapping.erase(be_replaced_v);
                    register_mapping[(*v)]=be_replaced_v_src;
                    lru_list.erase(v++);
                }

                break;
            }
        }

        if(! hit_v){ //first time 
            if(lru_list.size()<=reg_num){
                lru_list.emplace_front(upd_v);
            }
            else{
                int be_replaced_v_src = register_mapping[be_replaced_v];// reg
                int be_replaced_v_dst = stoi(be_replaced_v->getName());//stack

                lru_list.emplace_front(upd_v);

                // data update
                alloc_reg_asm += InstGen::str(InstGen::Reg(be_replaced_v_src),InstGen::Addr(InstGen::sp,be_replaced_v_dst<<2));
                //map update
                register_mapping.erase(be_replaced_v);
                register_mapping[upd_v]=be_replaced_v_src;
            }
        }



    }
    return alloc_reg_asm;
}

std::string  AsmBuilder::generateBasicBlockCode(BasicBlock *bb){
    return "";
}
std::string  AsmBuilder::getLabelName(BasicBlock *bb){
    return "." + bb->getParentFunc()->getName() + "_" + bb->getName();
}

std::string AsmBuilder::getLabelName(Function *func, int type) {
  const std::vector<std::string> name_list = {"pre", "post"};
  return "." + func->getName() + "_" + name_list.at(type);
}