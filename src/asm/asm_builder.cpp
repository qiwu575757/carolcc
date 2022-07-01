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
    return "";
}