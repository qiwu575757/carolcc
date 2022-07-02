#include "asm_builder.h"
#include "asm_instr.h"
#include "utils.h"

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
  std::string asm_code;
  auto array_init = dynamic_cast<ConstantArray *>(init);
  if (array_init) {
    auto length =
        static_cast<ArrayType *>(array_init->getType())->getNumOfElements();
    for (int i = 0; i < length; i++) {
      asm_code +=
          AsmBuilder::generate_initializer(array_init->getElement(i));
    }
    ERROR("UNDO array init in asm!");
  } else {
    auto val = AsmBuilder::get_const_int_val(init);
    if (!val.second) {
      std::cerr << "Function generateInitializerCode exception!" << std::endl;
      abort();
    }
    asm_code += InstGen::spaces + ".long" + " " + std::to_string(val.first) +
                InstGen::newline;
  }
  return asm_code;
}

std::pair<int, bool> AsmBuilder::get_const_int_val(Value *val) { // disabled
  auto const_val = dynamic_cast<ConstantInt *>(val);
  auto inst_val = dynamic_cast<Instruction *>(val);
  if (const_val) {
    return std::make_pair(const_val->getValue(), true);
  } else if (inst_val && false) {
    auto op_list = inst_val->getOperandList();
    if (dynamic_cast<BinaryInst *>(val)) {
      auto val_0 = AsmBuilder::get_const_int_val(op_list.at(0));
      auto val_1 = AsmBuilder::get_const_int_val(op_list.at(1));
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
      auto val_0 = AsmBuilder::get_const_int_val(op_list.at(0));
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
    return func_tail_code;
}
void AsmBuilder::show_mapping(){
  int index = 0;
  for(auto v = lru_list.begin(); v != lru_list.end();v++){
    printf("\tLRU list %d: %s\n",index++,(*v)->getName().c_str());
  }
  std::map<Value*, int>::iterator iter;
  iter = register_mapping.begin();
  while(iter != register_mapping.end()) {
      std::cout << "R " << iter->second << " : " << iter->first->getPrintName() << std::endl;
      iter++;
  }
}
std::string AsmBuilder::update_value_mapping(std::list<Value*> update_v){
    std::string alloc_reg_asm;
    /****/
    for(auto upd_v : update_v){
        // check if value is in list
        bool hit_v = false;
        int lru_index = 0;
        Value *be_replaced_v;
        
        if(upd_v->getPrintName().size() > 1 && upd_v->getPrintName()[0] == '%'){
          printf("update list: %s\n",upd_v->getPrintName().c_str());
          for(auto v = lru_list.begin(); v != lru_list.end();){
              if(lru_index==reg_num){
                  be_replaced_v = *v;
              }
              if(upd_v == (*v)){
                  hit_v = true;
                  if(lru_index<=reg_num){ // don't need to split
                      lru_list.emplace_front(*v);
                      v=lru_list.erase(v++);
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
              else{
                v++;
              }
          }

          if(! hit_v){ //first time
              if(lru_list.size()<=reg_num){
                  register_mapping[upd_v]=lru_list.size();
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
    return "." + bb->getParentFunc()->getName() + "_" + bb->getName();
}

std::string AsmBuilder::getLabelName(Function *func, int type) {
  const std::vector<std::string> name_list = {"pre", "post"};
  return "." + func->getName() + "_" + name_list.at(type);
}


std::string AsmBuilder::generateInstructionCode(Instruction *inst) {
    std::string inst_asm;
    auto &operands = inst->getOperandList();
    std::list<Value *> variable_list;
    BasicBlock *bb_cur = inst->getParent();

    if (inst->isAdd()) {
      auto src_op1 = operands.at(0);
      auto src_op2 = operands.at(1);
      variable_list.push_back(src_op1);
      variable_list.push_back(src_op2);
      variable_list.push_back(inst);
      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
      const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
      const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
      inst_asm += InstGen::add(target_reg,src_reg1,src_reg2);
    } else if (inst->isSub()) {
      auto src_op1 = operands.at(0);
      auto src_op2 = operands.at(1);
      variable_list.push_back(src_op1);
      variable_list.push_back(src_op2);
      variable_list.push_back(inst);
      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
      const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
      const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
      inst_asm += InstGen::sub(target_reg,src_reg1,src_reg2);
    } else if (inst->isMul()) {
      auto src_op1 = operands.at(0);
      auto src_op2 = operands.at(1);
      variable_list.push_back(src_op1);
      variable_list.push_back(src_op2);
      variable_list.push_back(inst);
      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
      const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
      const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
      inst_asm += InstGen::mul(target_reg,src_reg1,src_reg2);
    } else if (inst->isDiv()) {
      auto src_op1 = operands.at(0);
      auto src_op2 = operands.at(1);
      variable_list.push_back(src_op1);
      variable_list.push_back(src_op2);
      variable_list.push_back(inst);
      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
      const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
      const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
      inst_asm += InstGen::sdiv(target_reg,src_reg1,src_reg2);
    } else if (inst->isAnd()) {
      auto src_op1 = operands.at(0);
      auto src_op2 = operands.at(1);
      variable_list.push_back(src_op1);
      variable_list.push_back(src_op2);
      variable_list.push_back(inst);
      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
      const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
      const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
      inst_asm += InstGen::and_(target_reg,src_reg1,src_reg2);
    } else if (inst->isOr()) {
      auto src_op1 = operands.at(0);
      auto src_op2 = operands.at(1);
      variable_list.push_back(src_op1);
      variable_list.push_back(src_op2);
      variable_list.push_back(inst);
      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
      const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
      const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
      inst_asm += InstGen::orr(target_reg,src_reg1,src_reg2);
    } else if (inst->isLoad()) {
      auto src_op1 = operands.at(0);
      variable_list.push_back(src_op1);
      variable_list.push_back(inst);
      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
      const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
      inst_asm += InstGen::load(target_reg, InstGen::Addr(src_reg1,0));
    } else if (inst->isStore()) {
        auto src = operands.at(0);
        if (src->getPrintName()[0] != '%') { //存储的值是立即数，需要先将立即数存储到reg
          ConstantInt* src_op1 =  dynamic_cast<ConstantInt *>(operands.at(0));
          const std::string reg_name = "_imm_"+std::to_string(key++);
          Value * val = new Value(Type::getInt32Ty(), reg_name);
          variable_list.push_back(val);
          auto src_op2 = operands.at(1);
          variable_list.push_back(src_op2);
          inst_asm += update_value_mapping(variable_list);
          const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[val]);
          const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
          // const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
          InstGen::CmpOp cmpop = InstGen::CmpOp(InstGen::NOP);
          inst_asm += InstGen::mov(src_reg1,InstGen::Constant(atoi(src_op1->getPrintName().c_str())),cmpop);
          inst_asm += InstGen::store(src_reg1, InstGen::Addr(src_reg2,0));
        } else{ // 需要存储的值是寄存器
          auto src_op1 = operands.at(0);
          variable_list.push_back(src_op1);
          auto src_op2 = operands.at(1);
          variable_list.push_back(src_op2);
          inst_asm += update_value_mapping(variable_list);
          const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
          const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
          inst_asm += InstGen::store(src_reg2, InstGen::Addr(src_reg1,0));
        }
    } else if (inst->isRet()) {
      auto src_op1 = operands.at(0);
      variable_list.push_back(src_op1);
      inst_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
      inst_asm += InstGen::ret(src_reg1);
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
                                    "_branch_" + std::to_string(register_mapping[src_op1]),
                                0),
                 InstGen::NOP);
      } else if (operands.size() == 2) {
          auto src_op1 = operands.at(0);
          variable_list.push_back(src_op1);
          auto src_op2 = operands.at(1);
          variable_list.push_back(src_op2);
          variable_list.push_back(inst);
          inst_asm += update_value_mapping(variable_list);
          const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
          const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
          const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
          inst_asm += "cmp    "+ target_reg.getName() + ", " + "#0";
          inst_asm += "beq    " + InstGen::Label(getLabelName(bb_cur)).getName() +
                                    "_branch_" + std::to_string(register_mapping[src_op1]);
          inst_asm += InstGen::b(InstGen::Label(getLabelName(bb_cur) +
                                    "_branch_" + std::to_string(register_mapping[src_op2]),
                                      InstGen::NOP));
      }
    } else if (inst->isCall()) {
        std::vector<InstGen::Reg> caller_reg_list;
        for(auto reg : caller_save_regs) caller_reg_list.push_back(reg);
        inst_asm += InstGen::push(caller_reg_list);
        std::string func_name = operands.at(0)->getName();
        std::vector<Value *> args(operands.begin() + 1, operands.end());
        int offset = 0;
        for(auto arg:args){
            inst_asm += InstGen::str(InstGen::sp,InstGen::Addr(InstGen::sp,offset));
            offset+=4;
        }
        inst_asm += InstGen::bl(func_name);
        inst_asm += InstGen::pop(caller_reg_list);
    }
    else {
      WARNNING("unrealized %s",inst->getPrintName().c_str());
    }
    WARNNING("coding instr %s type is %d ...\n%s",inst->getPrintName().c_str(),inst->getInstructionKind(),inst_asm.c_str());
    return inst_asm;
}
