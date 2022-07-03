#include "asm_builder.h"
#include "asm_instr.h"
#include "utils.h"
#include <memory.h>

int key = 0;//给需要的立即数标号
std::string AsmBuilder::generate_asm(std::map<Value *, int> register_mapping){
    ERROR("UNDO");
}
std::string AsmBuilder::generate_asm(std::string file_name){
    asm_code += generate_module_header(file_name);
    WARNNING("module header:\n%s",asm_code.c_str());
    for(auto &func: this->module->getFunctions()){
        if(func->getBasicBlocks().size() != 0){
            WARNNING("coding func %s ...",func->getName().c_str());
            asm_code += generate_function_code(func);
            WARNNING("func %s:\n%s",func->getName().c_str(),asm_code.c_str());
        }
    }
    asm_code += generate_module_tail();
    return asm_code;
}

/*
*/

std::string AsmBuilder::generate_module_header(std::string file_name){
    std::string module_header_code;
    module_header_code += InstGen::spaces + ".arch armv" + std::to_string(arch_version) +
              "-a" + InstGen::newline;
    module_header_code += InstGen::spaces + ".file" + " " + "\"" +
                file_name+"\"" + InstGen::newline;
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
std::string AsmBuilder::generate_module_tail(){
    std::string module_tail_code;
    module_tail_code += InstGen::spaces + ".data" + InstGen::newline;
    module_tail_code += AsmBuilder::generate_global_vars();
    return module_tail_code;
}

std::string AsmBuilder::generate_global_vars() {
  std::string asm_code;
  for (auto &global_var : this->module->getGlobalVariables()) {
    asm_code += global_var->getName() + ":" + InstGen::newline;
    if (!global_var->getType()->getPointerElementType()->eq(
            *global_var->getOperandList().at(0)->getType())) {
      asm_code += InstGen::spaces + ".zero" + " " +
                  std::to_string(global_var->getType()->getSize()) +
                  InstGen::newline;
    } else {
      asm_code += AsmBuilder::generate_initializer(
          static_cast<Constant *>(global_var->getOperandList().at(0)));
    }
  }
  return asm_code;
}

std::string AsmBuilder::generate_initializer(Constant *init) {
  std::string asm_code,array_code;
  auto array_init = dynamic_cast<ConstantArray *>(init);
  if (array_init) {
    auto length =
        static_cast<ArrayType *>(array_init->getType())->getNumOfElements();
    for (int i = 0; i < length; i++) {
      array_code +=
          AsmBuilder::generate_initializer(array_init->getElement(i));
    }

    bool check_array_init = false;
    printf("array_size : %d\n",init->getType()->getSize());
    for(int i=0;i<init->getType()->getSize()/4;i++){
      if(array_code.c_str()[i*12+10]!='0'){
        check_array_init=true;
        break;
      }
      // printf(" |%c| ",array_code.c_str()[i*12+10]);
    }
    if(check_array_init){
      asm_code += array_code;
    }
    else{
      asm_code += InstGen::spaces + ".zero "+std::to_string(init->getType()->getSize())+"\n";
    }
    // ERROR("UNDO array init in asm!");
  } else {
    auto val = AsmBuilder::get_const_val(init);
    if (!val.second) {
      std::cerr << "Function generateInitializerCode exception!" << std::endl;
      abort();
    }
    asm_code += InstGen::spaces + ".long" + " " + std::to_string(val.first) +
                InstGen::newline;
  }
  return asm_code;
}

std::pair<int, bool> AsmBuilder::get_const_val(Value *val) { // disabled
  auto const_int_val = dynamic_cast<ConstantInt *>(val);
  auto const_float_val = dynamic_cast<ConstantFloat *>(val);
  auto inst_val = dynamic_cast<Instruction *>(val);
  //#### UNDO ####
  //  NOT SUPPORT FLOAT
  //#### UNDO ####

  if (const_int_val || const_float_val) {
    if(const_float_val){
      ERROR("UNDO init float! in asm");
      ///??????
      float data = const_float_val->getValue();
      int float_data = *((int*)(&data));
      return std::make_pair(float_data, true);
    }
    else{
      return std::make_pair(const_int_val->getValue(), true);
    }
  } else if (inst_val && false) {
    auto op_list = inst_val->getOperandList();
    if (dynamic_cast<BinaryInst *>(val)) {
      auto val_0 = AsmBuilder::get_const_val(op_list.at(0));
      auto val_1 = AsmBuilder::get_const_val(op_list.at(1));
      if (val_0.second && val_1.second) {
        int ret = 0;
        bool flag = true;
        switch (inst_val->getInstructionKind()) {
        case Instruction::ADD:
          ret = val_0.first + val_1.first;
          break;
        case Instruction::SUB:
          ret = val_0.first - val_1.first;
          break;
        case Instruction::AND:
          ret = val_0.first & val_1.first;
          break;
        case Instruction::OR :
          ret = val_0.first | val_1.first;
          break;
        case Instruction::MUL:
          ret = val_0.first * val_1.first;
          break;
        case Instruction::DIV:
          ret = val_0.first / val_1.first;
          break;
        case Instruction::REM:
          ret = val_0.first % val_1.first;
          break;
        default:
          flag = false;
          break;
        }
        return std::make_pair(ret, flag);
      } else {
        return std::make_pair(0, false);
      }
    }
    if (dynamic_cast<UnaryInst *>(val)) {
      auto val_0 = AsmBuilder::get_const_val(op_list.at(0));
      if (val_0.second) {
        int ret = 0;
        bool flag = true;
        switch (inst_val->getInstructionKind()) {
        case Instruction::NOT:
          ret = !val_0.first;
          break;
        default:
          flag = false;
          break;
        }
        return std::make_pair(ret, flag);
      } else {
        return std::make_pair(0, false);
      }
    }
  }
  std::cerr << "Function getConstIntVal exception!" << std::endl;
  abort();
}


std::string AsmBuilder::generate_function_code(Function *func){
    std::string func_asm;
    func_asm += func->getName() + ":" + InstGen::newline;
    func_asm += generate_function_entry_code(func);
    WARNNING("func entry %s:\n%s",func->getName().c_str(),func_asm.c_str());
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
    callee_reg_list.push_back(InstGen::lr);
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
    std::vector<InstGen::Reg> callee_reg_list;
    for(auto reg : callee_save_regs) callee_reg_list.push_back(reg);
    callee_reg_list.push_back(InstGen::pc);
    func_tail_code += InstGen::pop(callee_reg_list);
    func_tail_code += InstGen::spaces+"bx  lr " + InstGen::newline;

    return func_tail_code;
}
void AsmBuilder::show_mapping(){
  int index = 0;
  for(auto v = lru_list.begin(); v != lru_list.end();v++){
    printf("LRU list %d: %s\n",index++,(*v)->getName().c_str());
  }
  for(auto& iter:register_mapping){
      std::cout << "REGISTER " << iter.second << " : " << iter.first->getPrintName() << std::endl;
  }
  std::map<Value*, int>::iterator iter2;
  iter2 = stack_mapping.begin();
  while(iter2 != stack_mapping.end()) {
      std::cout << "STACK sp+" << iter2->second << " : " << iter2->first->getPrintName() << std::endl;
      iter2++;
  }
}

std::string AsmBuilder::erase_value_mapping(std::list<Value*>& erase_v){
    std::string alloc_reg_asm;
    /****/
    for(auto ers_v : erase_v){
        printf("[X] erase: %s\n",ers_v->getPrintName().c_str());
        int reg_index = find_register(ers_v);
        for(auto v = lru_list.begin(); v != lru_list.end();)
        {
          if((*v)==ers_v){
            ////
              lru_list.erase(v++);
            break;
          }
          else{
            v++;
          }
        }

        if(lru_list.size()>reg_num-1){
          int index=0;
          Value * replace_v ;

          for(auto v_edge = lru_list.begin(); v_edge != lru_list.end();v_edge++){
            if(index==reg_num-1){
                replace_v = (*v_edge);
                break;
            }
            index++;
          }
          if(register_mapping.count(replace_v)==0){

            int replace_v_offset = stack_mapping[replace_v];
            MyAssert("nullptr",replace_v!=nullptr);
            set_register(replace_v,reg_index,true);
            // data update
            alloc_reg_asm += InstGen::comment("erase: load edge value"+ replace_v->getPrintName()+ " to reg:"+ std::to_string(reg_index),"");
            alloc_reg_asm += InstGen::ldr(InstGen::Reg(reg_index),InstGen::Addr(InstGen::sp,replace_v_offset));

          }
        }

        // check if value is in list
        if(register_mapping.count(ers_v)==1){
          auto iter = register_mapping.find(ers_v);
          register_mapping.erase(iter);
        }
    }
    return alloc_reg_asm;
}

std::string AsmBuilder::update_value_mapping(std::list<Value*> update_v){
    std::string alloc_reg_asm;
    /****/
    for(auto upd_v : update_v){
        // check if value is in list
        bool hit_v = false;
        int lru_index = 0;
        Value *be_replaced_v;

        if(!upd_v->isConstant()){
          printf("--update list: %s\n",upd_v->getPrintName().c_str());
          for(auto v = lru_list.begin(); v != lru_list.end();){
              if(lru_index==reg_num){
                  be_replaced_v = *v;
              }
              if(upd_v == (*v)){
                printf("HIT\n");
                  hit_v = true;
                  if(lru_index<=reg_num){ // don't need to split
                      lru_list.push_front(*v);
                      v=lru_list.erase(v++);
                  }
                  else{ // split
                      int be_replaced_v_src = find_register(be_replaced_v);// reg
                      if(be_replaced_v->getName()[0] == '_'){
                        int used_v_offset = stack_mapping[(*v)];
                        int used_v_dst = be_replaced_v_src;//reg

                        lru_list.push_front(*v);

                        // data update
                        alloc_reg_asm += InstGen::comment("load "+(*v)->getPrintName()+" from sp+"+std::to_string(used_v_offset),"");
                        // alloc_reg_asm += InstGen::str(InstGen::Reg(be_replaced_v_src),InstGen::Addr(InstGen::sp,used_v_offset));
                        alloc_reg_asm += InstGen::ldr(InstGen::Reg(used_v_dst),InstGen::Addr(InstGen::sp,used_v_offset));

                        
                      }
                      else{
                        int be_replaced_v_offset = stack_mapping[be_replaced_v];
                        int used_v_offset = stack_mapping[(*v)];
                        int used_v_dst = be_replaced_v_src;//reg

                        lru_list.push_front(*v);

                        // data update
                        alloc_reg_asm += InstGen::comment("store "+be_replaced_v->getPrintName()+" to "+std::to_string(be_replaced_v_src),"load "+(*v)->getPrintName()+" from sp+"+std::to_string(used_v_offset));
                        alloc_reg_asm += InstGen::str(InstGen::Reg(be_replaced_v_src),InstGen::Addr(InstGen::sp,be_replaced_v_offset));
                        alloc_reg_asm += InstGen::ldr(InstGen::Reg(used_v_dst),InstGen::Addr(InstGen::sp,used_v_offset));
                      }
                      //map update
                      auto iter = register_mapping.find(be_replaced_v);
                      register_mapping.erase(iter);
                      MyAssert("nullptr",*(v));
                      set_register((*v),be_replaced_v_src,true);
                      lru_list.erase(v++);
                  }

                  break;
              }
              else{
                v++;
              }
              lru_index+=1;
          }

          if(! hit_v){ //first time

              if(register_mapping.size()<=reg_num){
                  int register_bits[reg_num+1];
                  memset(register_bits,0,sizeof(register_bits));
                  std::map<Value*, int>::iterator iter;
                  iter = register_mapping.begin();
                  while(iter != register_mapping.end()) {
                    register_bits[ iter->second] +=1;
                    iter++;
                  }
                  for(int i=0;i<=reg_num;i++){
                    if(register_bits[i]==0){
                      MyAssert("nullptr",upd_v);

                       set_register(upd_v,i,true);
                       alloc_reg_asm += InstGen::comment("alloc "+upd_v->getPrintName()+" to reg "+std::to_string(i),"");
                        lru_list.push_front(upd_v);
                        break;
                    }
                    else if (register_bits[i]>1)
                    {
                      ERROR("wrong reg alloc : %d was allocated for %d times \n",i,register_bits[i]);
                    }

                  }
              }
              else  if(!be_replaced_v->isConstant()){
                  printf(" value %s update value %s\n",upd_v->getPrintName().c_str(),be_replaced_v->getPrintName().c_str());
                  int be_replaced_v_src = find_register(be_replaced_v);// reg
                  int be_replaced_v_dst = stack_mapping[be_replaced_v];
                  lru_list.push_front(upd_v);

                  alloc_reg_asm += InstGen::comment("store "+be_replaced_v->getPrintName()+" from reg "+std::to_string(be_replaced_v_src),"to sp+"+std::to_string(be_replaced_v_dst));
                  // data update
                  alloc_reg_asm += InstGen::str(InstGen::Reg(be_replaced_v_src),InstGen::Addr(InstGen::sp,be_replaced_v_dst));
                  //map update
                  auto iter = register_mapping.find(be_replaced_v);
                  register_mapping.erase(be_replaced_v);
                  MyAssert("nullptr",upd_v);
                  set_register(upd_v,be_replaced_v_src,true);

              }
              else{
                  ERROR(" wrong in reg alloc !");
              }
          }
          show_mapping();
        }
        else{
          WARNNING("[WARNNING] wrong name '%s' which can not be identified",upd_v->getPrintName().c_str());
        }



    }
    return alloc_reg_asm;
}

std::string  AsmBuilder::generateBasicBlockCode(BasicBlock *bb){
    std::string bb_asm;
    for (auto &inst : bb->getInstructions()) {
        bb_asm += AsmBuilder::generateInstructionCode(inst);
    // WARNNING("coding blocks %s ...",bb_asm.c_str());
    }

    return bb_asm;
}
std::string  AsmBuilder::getLabelName(BasicBlock *bb){
    return InstGen::spaces + "." + bb->getParentFunc()->getName() + "_" + bb->getName();
}

std::string AsmBuilder::getLabelName(Function *func, int type) {
  const std::vector<std::string> name_list = {"pre", "post"};
  return InstGen::spaces + "." + func->getName() + "_" + name_list.at(type);
}

int AsmBuilder::find_register(Value *v){
  if(v==nullptr){
    ERROR(" value is nullptr");
  }
  if(register_mapping.count(v)){
    v->getPrintName();
    return register_mapping[v];
  }else{
    ERROR(" not find v in register! %s",v->getPrintName().c_str());
  }
}

void AsmBuilder::set_register(Value *v,int data,bool init){
  if(v==nullptr){
    ERROR(" value is nullptr");
  }
  if(register_mapping.count(v)){
    v->getPrintName();
    register_mapping[v]=data;
  }else if(init){
    register_mapping[v]=data;
  }
  else{
    ERROR(" not init and not find v in register! %s",v->getPrintName().c_str());
  }
}

std::string AsmBuilder::generateInstructionCode(Instruction *inst) {
    std::string inst_asm;
    auto &operands = inst->getOperandList();
    std::list<Value *> variable_list;
    BasicBlock *bb_cur = inst->getParent();

    int type_size=0;
    if(!inst->isAlloca()){
      //维护变量的栈分配
      type_size = inst->getType()->getSize();
      stack_mapping[inst] = stack_cur_size;
    }
    if (inst->isAdd()) {
      auto src = operands.at(0);
      if (src->isConstant()) {
        auto src_op1 = operands.at(0);
        variable_list.push_back(src_op1);
        auto src_op2 = operands.at(1);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);

        inst_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op1->getPrintName() + " + "+src_op2->getPrintName(), "");

        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::add(target_reg,src_reg1,InstGen::Constant(atoi(src_op2->getPrintName().c_str())));
      } else {
        auto src_op1 = operands.at(0);
        auto src_op2 = operands.at(1);
        variable_list.push_back(src_op1);
        variable_list.push_back(src_op2);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);

        inst_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op1->getPrintName() + " + "+src_op2->getPrintName(), "");

        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::add(target_reg,src_reg1,src_reg2);
      }
    } else if (inst->isSub()) {
      auto src = operands.at(0);
      if (src->isConstant()) {
        auto src_op1 = operands.at(0);
        variable_list.push_back(src_op1);
        auto src_op2 = operands.at(1);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::sub(target_reg,src_reg1,InstGen::Constant(atoi(src_op2->getPrintName().c_str())));
      } else {
        auto src_op1 = operands.at(0);
        auto src_op2 = operands.at(1);
        variable_list.push_back(src_op1);
        variable_list.push_back(src_op2);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::sub(target_reg,src_reg1,src_reg2);
      }
    } else if (inst->isMul()) {
        auto src_op1 = operands.at(0);
        auto src_op2 = operands.at(1);
        variable_list.push_back(src_op1);
        variable_list.push_back(src_op2);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::mul(target_reg,src_reg1,src_reg2);
    } else if (inst->isDiv()) {
        auto src_op1 = operands.at(0);
        auto src_op2 = operands.at(1);
        variable_list.push_back(src_op1);
        variable_list.push_back(src_op2);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::sdiv(target_reg,src_reg1,src_reg2);
    } else if (inst->isAnd()) {
      auto src = operands.at(0);
      if (src->isConstant()) {
        auto src_op1 = operands.at(0);
        variable_list.push_back(src_op1);
        auto src_op2 = operands.at(1);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::and_(target_reg,src_reg1,InstGen::Constant(atoi(src_op2->getPrintName().c_str())));
      } else {
        auto src_op1 = operands.at(0);
        auto src_op2 = operands.at(1);
        variable_list.push_back(src_op1);
        variable_list.push_back(src_op2);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::and_(target_reg,src_reg1,src_reg2);
      }
    } else if (inst->isOr()) {
      auto src = operands.at(0);
      if (src->isConstant()) {
        auto src_op1 = operands.at(0);
        variable_list.push_back(src_op1);
        auto src_op2 = operands.at(1);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::orr(target_reg,src_reg1,InstGen::Constant(atoi(src_op2->getPrintName().c_str())));
      } else {
        auto src_op1 = operands.at(0);
        auto src_op2 = operands.at(1);
        variable_list.push_back(src_op1);
        variable_list.push_back(src_op2);
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2));
        const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
        inst_asm += InstGen::orr(target_reg,src_reg1,src_reg2);
      }
    } else if (inst->isLoad()) {
      auto src_op1 = operands.at(0);
      variable_list.push_back(src_op1);
      variable_list.push_back(inst);

      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
      const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));

      inst_asm +=  InstGen::comment(target_reg.getName()+" load from "+src_op1->getPrintName().c_str(), "");
      inst_asm += InstGen::load(target_reg, InstGen::Addr(src_reg1,0));
    } else if (inst->isStore()) {
        auto src = operands.at(0);
        if (src->isConstant()) { //存储的值是立即数，需要先将立即数存储到reg
          ConstantInt* src_op1 =  dynamic_cast<ConstantInt *>(operands.at(0));
          const std::string reg_name = "_imm_"+std::to_string(key++);
          Value * val = new Value(Type::getInt32Ty(), reg_name);
          variable_list.push_back(val);
          auto src_op2 = operands.at(1);
          variable_list.push_back(src_op2);
          inst_asm += update_value_mapping(variable_list);
          const InstGen::Reg src_reg1 = InstGen::Reg(find_register(val));
          const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2));
          // const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));


          inst_asm +=  InstGen::comment(src->getPrintName()+" store to "+src_reg2.getName(), "");
          InstGen::CmpOp cmpop = InstGen::CmpOp(InstGen::NOP);
          inst_asm += InstGen::setValue(src_reg1,InstGen::Constant(atoi(src_op1->getPrintName().c_str())));
          inst_asm += InstGen::store(src_reg1, InstGen::Addr(src_reg2,0));

          variable_list.clear();
          variable_list.push_back(val);
          inst_asm += erase_value_mapping(variable_list);

        } else{ // 需要存储的值是寄存器
          auto src_op1 = operands.at(0);
          variable_list.push_back(src_op1);
          auto src_op2 = operands.at(1);
          variable_list.push_back(src_op2);
          inst_asm += update_value_mapping(variable_list);
          const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
          const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2));

          inst_asm +=  InstGen::comment(src->getPrintName()+" store to "+src_reg2.getName(), "");
          inst_asm += InstGen::store(src_reg1, InstGen::Addr(src_reg2,0));
        }
    } else if (inst->isRet()) {
          auto src = operands.at(0);
          if (src->isConstant()) { //存储的值是立即数，需要先将立即数存储到reg
            inst_asm += InstGen::ret(InstGen::Constant(atoi(src->getPrintName().c_str())));
          } else {
            auto src_op1 = operands.at(0);
            variable_list.push_back(src_op1);
            inst_asm += update_value_mapping(variable_list);
            if (find_register(src_op1) != 0) {//如果源寄存器就是r0，就无需mov
              const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
              inst_asm += InstGen::ret(src_reg1);
            }
          }
    } else if (inst->isCmp()) {
        if (operands.size() == 1){
            auto src_op1 = operands.at(0);
            switch (dynamic_cast<CmpInst *>(inst)->_cmp_op)
            {
                case CmpInst::EQ:
                    /* code */
                    break;
                case CmpInst::NEQ:
                    /* code */
                    break;
                case CmpInst::GT:
                    /* code */
                    break;
                case CmpInst::LE:
                    /* code */
                    break;
                case CmpInst::GE:
                    /* code */
                    break;
                case CmpInst::LT:
                    /* code */
                    break;

                default:
                    break;
            }
        }
    WARNNING("unrealized CMP");
    } else if (inst->isBr()) {
      if (operands.size() == 1){
          auto src_op1 = operands.at(0);
          variable_list.push_back(src_op1);
          inst_asm += update_value_mapping(variable_list);
          inst_asm += InstGen::b(InstGen::Label(getLabelName(bb_cur) +
                                    "_branch_" + std::to_string(find_register(src_op1)),
                                0),
                 InstGen::NOP);
      } else if (operands.size() == 2) {
          auto src_op1 = operands.at(0);
          variable_list.push_back(src_op1);
          auto src_op2 = operands.at(1);
          variable_list.push_back(src_op2);
          variable_list.push_back(inst);
          inst_asm += update_value_mapping(variable_list);
          const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1));
          const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2));
          const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
          inst_asm += InstGen::spaces +"cmp "+ target_reg.getName() + ", " + "#0";
          inst_asm += InstGen::spaces + "beq " + InstGen::Label(getLabelName(bb_cur)).getName() +
                                    "_branch_" + std::to_string(find_register(src_op1));
          inst_asm += InstGen::b(InstGen::Label(getLabelName(bb_cur) +
                                    "_branch_" + std::to_string(find_register(src_op2)),
                                      InstGen::NOP));
      }
    } else if (inst->isCall()) {
        std::vector<InstGen::Reg> caller_reg_list;
        for(auto reg : caller_save_regs) caller_reg_list.push_back(reg);
        inst_asm += InstGen::push(caller_reg_list);
        std::string func_name = operands.at(0)->getName();
        std::vector<Value *> args(operands.begin() + 1, operands.end());
        int offset = 0;

        inst_asm += InstGen::comment(" call "+func_name, "");
        for(auto arg:args){
            inst_asm += InstGen::str(InstGen::sp,InstGen::Addr(InstGen::sp,offset));
            offset+=4;
        }
        inst_asm += InstGen::bl(func_name);
        inst_asm += InstGen::pop(caller_reg_list);
    } else if (inst->isAlloca()) {
      type_size = inst->getType()->getSize();
      stack_mapping[inst] = stack_cur_size+type_size;

      inst_asm += InstGen::comment(" alloc "+inst->getPrintName()+" to sp+" + std::to_string(stack_cur_size),
      " size: "+std::to_string(type_size));

     type_size+=4;

      variable_list.push_back(inst);
      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));
      inst_asm += InstGen::add(target_reg,InstGen::sp,InstGen::Constant(stack_cur_size));
    } else if (inst->isGep()) { //将gep指令转化为mul, add %1 =
      // new register variable_list.push_back(inst);
      variable_list.push_back(inst);
      auto src_op1 = operands.at(0);
      variable_list.push_back(src_op1);

      const std::string reg_name1 = "_imm_"+std::to_string(key++);
      Value * val1 = new Value(Type::getInt32Ty(), reg_name1);
      variable_list.push_back(val1);

      const std::string reg_name2 = "_imm_"+std::to_string(key++);
      Value * val2 = new Value(Type::getInt32Ty(), reg_name2);
      variable_list.push_back(val2);

      // update register
      inst_asm += update_value_mapping(variable_list);
      inst_asm += InstGen::comment(" gep "+inst->getPrintName()," from " + src_op1->getPrintName());

      // init result reg 0
      inst_asm += InstGen::setValue(InstGen::Reg(find_register(inst)),InstGen::Constant(0));
      // get operand length
      //
      auto array_ty = src_op1->getType()->getPointerElementType();
      for(int i = 1; i < operands.size();i++){
          auto src_op = operands.at(i);
          auto element_size = array_ty->getSize();


          //  element_size = src_op->getType()->getPointerElementType()->getSize();
          if (!src_op->isConstant()) {//不是立即数

            inst_asm += InstGen::comment(" gep += "+src_op->getPrintName()+" * "+std::to_string(element_size),"");

            inst_asm += InstGen::setValue(InstGen::Reg(find_register(val1)),InstGen::Constant(element_size));
            // inst_asm += InstGen::mul(InstGen::Reg(find_register(val1)),InstGen::Reg(find_register(val1)), InstGen::Reg(find_register(val2)));

            inst_asm += InstGen::mla(InstGen::Reg(find_register(inst)),InstGen::Reg(find_register(val1)),
            InstGen::Reg(find_register(val2)), InstGen::Reg(find_register(inst)));
          } else if(static_cast<ConstantInt*>(src_op)->getValue()!=0){
            // 邪法
            inst_asm += InstGen::comment(" gep += "+src_op->getPrintName()+" * "+std::to_string(element_size),"");

            WARNNING("SRC OP name is: ",src_op->getPrintName().c_str());
            int offset = static_cast<ConstantInt*>(src_op)->getValue() * element_size;
            inst_asm += InstGen::add(InstGen::Reg(find_register(inst)),InstGen::Reg(find_register(inst)),
            InstGen::Constant(offset));
            // variable_list.push_back(src_op2);
          }
          // variable_list.push_back(inst);
          array_ty = static_cast<ArrayType*>(array_ty)->getElementType();
      }
      inst_asm += InstGen::add(InstGen::Reg(find_register(inst)),InstGen::Reg(find_register(inst)),
          InstGen::Reg(find_register(src_op1)));
      variable_list.clear();
      variable_list.push_back(val1);
      variable_list.push_back(val2);
      inst_asm +=  erase_value_mapping(variable_list);

      // const InstGen::Reg size_reg = InstGen::Reg(find_register(val1));
      // int offset = src_op1->getType()->getPointerElementType()->getSize();
      // inst_asm += InstGen::setValue(size_reg,InstGen::Constant(offset));
      // const InstGen::Reg base_reg = InstGen::Reg(find_register(src_op1));
      // if (src_op2->isConstant()) {//立即数
      //   const InstGen::Reg dem_reg = InstGen::Reg(find_register(val2));
      //   const InstGen::Reg target_reg = InstGen::Reg(find_register(inst));

      //   inst_asm += InstGen::mla(target_reg,size_reg,dem_reg, base_reg);
      // } else {
      //   const InstGen::Reg dem_reg = InstGen::Reg(find_register(src_op2));
      //   const InstGen::Reg target_reg = InstGen::Reg(register_mapping(inst));

      //   inst_asm += InstGen::mla(target_reg,size_reg,dem_reg, base_reg);
      }
     else {
      WARNNING("unrealized %s",inst->getPrintName().c_str());
    }

    stack_cur_size += type_size;

    WARNNING("coding instr %s type is %d ...\n%s",inst->getPrintName().c_str(),inst->getInstructionKind(),inst_asm.c_str());
    return inst_asm;
}
