#include "asm_builder.h"
#include "asm_instr.h"
#include "utils.h"
#include <memory.h>

int key = 0;//给需要的立即数标号
std::list<Value *> variable_list;//待分配寄存器的变量列表
std::list<Value *> fp_variable_list;//待分配寄存器的变量列表
int reg_index = 0; //从 0 号寄存器开始分配
int vfpreg_index = 0; //从 0 号寄存器开始分配

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

std::string AsmBuilder::generate_module_header(std::string file_name){
    std::string module_header_code;
    module_header_code += InstGen::spaces + ".arch armv" + std::to_string(arch_version) +
              "-a" + InstGen::newline;
    module_header_code += InstGen::spaces + ".fpu vfpv3-d16" + InstGen::newline;
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
    module_tail_code += generate_use_of_global_vars();
    module_tail_code += ".data" + InstGen::newline;
    module_tail_code += AsmBuilder::generate_global_vars();
    return module_tail_code;
}

std::string AsmBuilder::generate_use_of_global_vars() {
  std::string asm_code;
  for (auto &global_var : this->module->getGlobalVariables()) {
    asm_code += ".LCPI_"+global_var->getName() + ":" + InstGen::newline;
    asm_code += InstGen::spaces+".long "+global_var->getName() + InstGen::newline;
  }
  return asm_code;
}

std::string AsmBuilder::generate_global_vars() {
  std::string asm_code;
  for (auto &global_var : this->module->getGlobalVariables()) {
    asm_code += global_var->getName() + ":" + InstGen::newline;
    //MyAssert("the array operand size is 0", global_var->getOperandNumber() != 0);
    if (global_var->getInit() == nullptr) {
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
    }
    if(check_array_init){
      asm_code += array_code;
    }
    else{
      asm_code += InstGen::spaces + ".zero "+std::to_string(init->getType()->getSize())+"\n";
    }
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

std::pair<int, bool> AsmBuilder::get_const_val(Value *val) {
  auto const_int_val = dynamic_cast<ConstantInt *>(val);
  auto const_float_val = dynamic_cast<ConstantFloat *>(val);
  auto inst_val = dynamic_cast<Instruction *>(val);

  if (const_int_val || const_float_val) {
    if(const_float_val){
      float float_data = const_float_val->getValue();

      // 将浮点数转化为十进制
      int long_data = *((int*)(&float_data));
      return std::make_pair(long_data, true);
    } else{
      return std::make_pair(const_int_val->getValue(), true);
    }
  } else if (inst_val && false) { //  已添加常量折叠优化，无需计算
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
    stack_cur_size = 0;
    stack_mapping.clear();
    register_mapping.clear();

    std::string func_asm;
    func_asm += func->getName() + ":" + InstGen::newline;
    func_asm += generate_function_entry_code(func);
    WARNNING("func entry %s:\n%s",func->getName().c_str(),func_asm.c_str());

    for (auto &bb : func->getBasicBlocks()) {
        func_asm += getLabelName(bb) + ":" + InstGen::newline;
        func_asm += generateBasicBlockCode(bb);
    }
    // 用于函数无return的情况
    func_asm += generate_function_exit_code();

    return func_asm;
}

std::string AsmBuilder::generate_function_entry_code(Function *func){
    std::string func_head_code;
    //func name
    func_head_code+=getLabelName(func,0) + ":" + InstGen::newline;
    //get callee save registers
    // callee_save_regs
    std::vector<InstGen::Reg> callee_reg_list;
    std::vector<InstGen::VFPReg> callee_vfpreg_list;
    for(auto reg : callee_save_regs)
        callee_reg_list.push_back(reg);
    for(auto reg : callee_save_vfpregs)
        callee_vfpreg_list.push_back(reg);

    //push (save register and lr)
    func_head_code += InstGen::push(callee_vfpreg_list);
    callee_reg_list.push_back(InstGen::lr);
    func_head_code += InstGen::push(callee_reg_list);
    //sp sub，开辟栈空间
    func_head_code += InstGen::instConst(InstGen::sub, InstGen::sp, InstGen::sp,
                                 InstGen::Constant(this->stack_size));

    int offset = 0;
    for(auto arg: func->getArgs()){
      func_head_code += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
      // func_head_code += update_value_mapping({arg});
      // int v_offset = stack_mapping[arg];
      // func_head_code += InstGen::comment("load "+(arg)->getPrintName()+" from sp+"+std::to_string(v_offset),"pop val");
      // func_head_code += InstGen::ldr(InstGen::Reg(reg_index),InstGen::Addr(InstGen::sp,v_offset));
      //register_mapping[arg] = reg_index++;

      // func_head_code += InstGen::ldr(InstGen::Reg(find_register(arg)),InstGen::Addr(InstGen::sp,offset));
      //函数参数以及被提前存储在对应位置
      stack_mapping[arg] = offset;
      offset += 4;
      stack_cur_size += 4;
    }
    return func_head_code;
}

std::string AsmBuilder::generate_function_exit_code(){
    std::string func_tail_code;
    // sp add
    func_tail_code += InstGen::instConst(InstGen::add, InstGen::sp, InstGen::sp,
                                 InstGen::Constant(stack_size));
    // pop (save register and pc)
    std::vector<InstGen::Reg> callee_reg_list;
    std::vector<InstGen::VFPReg> callee_vfpreg_list;
    for(auto reg : callee_save_regs)
        callee_reg_list.push_back(reg);
    for(auto reg : callee_save_vfpregs)
        callee_vfpreg_list.push_back(reg);

    callee_reg_list.push_back(InstGen::lr);
    func_tail_code += InstGen::pop(callee_reg_list);
    func_tail_code += InstGen::pop(callee_vfpreg_list);

    func_tail_code += InstGen::spaces+"bx  lr " + InstGen::newline;

    return func_tail_code;
}

void AsmBuilder::show_mapping(){
  int index = 0;

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
    for (auto upd_v : update_v) {
      if (stack_mapping.count(upd_v) == 1) {//栈中有该变量
        int v_offset = stack_mapping[upd_v];
        alloc_reg_asm += InstGen::comment("load "+(upd_v)->getPrintName()+" from sp+"+std::to_string(v_offset),"pop val");
        alloc_reg_asm += InstGen::load(InstGen::Reg(reg_index),InstGen::Addr(InstGen::sp,v_offset));
        register_mapping[upd_v] = reg_index++;
      } else {// 直接增加寄存器映射
        register_mapping[upd_v] = reg_index++;
      }
    }
    // show_mapping();

    return alloc_reg_asm;
}

std::string AsmBuilder::update_fpvalue_mapping(std::list<Value*> update_v){
    std::string alloc_reg_asm;

    for (auto upd_v : update_v) {
      if (stack_mapping.count(upd_v) == 1) {//栈中有该变量
        int v_offset = stack_mapping[upd_v];
        alloc_reg_asm += InstGen::comment("vload "+(upd_v)->getPrintName()+" fp value from sp+"+std::to_string(v_offset),"pop val");
        alloc_reg_asm += InstGen::vload(InstGen::VFPReg(vfpreg_index),InstGen::Addr(InstGen::sp,v_offset));
        vfpregister_mapping[upd_v] = vfpreg_index++;
      } else {// 直接增加寄存器映射
        vfpregister_mapping[upd_v] = vfpreg_index++;
      }
    }
    // show_mapping();

    return alloc_reg_asm;
}

std::string AsmBuilder::update_arg_mapping(Value *arg) {
    std::string alloc_reg_asm;

    if (stack_mapping.count(arg) == 1) {//栈中有该变量
      int v_offset = stack_mapping[arg];
      alloc_reg_asm += InstGen::comment("load "+(arg)->getPrintName()+" from sp+"+std::to_string(v_offset),"pop val");
      alloc_reg_asm += InstGen::load(InstGen::Reg(reg_index),InstGen::Addr(InstGen::sp,v_offset));
      register_mapping[arg] = reg_index++;
    } else {// 直接增加寄存器映射
      ERROR("WRONG ARG");
    }

    return alloc_reg_asm;
}

std::string AsmBuilder::flushRegs2Stack(std::list<Value*> flush_v1, std::list<Value*> flush_v2) {
    std::string flush_asm;

    for (auto flu_v : flush_v1) {
      if (stack_mapping.count(flu_v)) {
          int v_offset = stack_mapping[flu_v];
          flush_asm += InstGen::comment("store "+(flu_v)->getPrintName()+" to sp+"+std::to_string(v_offset),"push val");
          flush_asm += InstGen::store(InstGen::Reg(register_mapping[flu_v]),InstGen::Addr(InstGen::sp,v_offset));
        }
    }
    for (auto flu_v : flush_v2) {
      if (stack_mapping.count(flu_v)) {
          int v_offset = stack_mapping[flu_v];
          flush_asm += InstGen::comment("vstore "+(flu_v)->getPrintName()+" to sp+"+std::to_string(v_offset),"push val");
          flush_asm += InstGen::vstore(InstGen::VFPReg(vfpregister_mapping[flu_v]),InstGen::Addr(InstGen::sp,v_offset));
        }
    }

    return flush_asm;
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

std::string  AsmBuilder::getLabelName(BasicBlock *bb, std::string id){
    return "." + bb->getParentFunc()->getName() + "_" + id;
}

std::string AsmBuilder::getLabelName(Function *func, int type) {
  const std::vector<std::string> name_list = {"pre", "post"};
  return InstGen::spaces + "." + func->getName() + "_" + name_list.at(type);
}

int AsmBuilder::find_register(Value *v){
  auto global = dynamic_cast<GlobalVariable *>(v);

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

int AsmBuilder::find_vfpregister(Value *v, std::string &code){
  auto global = dynamic_cast<GlobalVariable *>(v);

  if(v == nullptr){
    ERROR(" value is nullptr");
  }
  v->getPrintName();

  return vfpregister_mapping[v];
}

int AsmBuilder::find_vfpregister(Value *v){
  auto global = dynamic_cast<GlobalVariable *>(v);

  if(v == nullptr){
    ERROR(" value is nullptr");
  }
  v->getPrintName();

  return vfpregister_mapping[v];
}

int AsmBuilder::find_register(Value *v, std::string &code){
  auto global = dynamic_cast<GlobalVariable *>(v);

  if(v == nullptr){
    ERROR(" value is nullptr");
  }
  if(register_mapping.count(v)){
    if(global){
      // get label
      std::list<Value *> variable_list;
      variable_list.push_back(v);

      // generate label addr reg
      const std::string reg_name_0 = "LCPI_"+std::to_string(key++);
      Value *label_reg_addr = new Value(Type::getInt32Ty(), reg_name_0);
      variable_list.push_back(label_reg_addr);

      code += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
      update_value_mapping(variable_list);

      // generate label
      InstGen::Label src_label = InstGen::Label(".LCPI_"+v->getName());

      code += InstGen::getAddress(InstGen::Reg(find_register(label_reg_addr)), src_label);
      code += InstGen::load(InstGen::Reg(find_register(v)),InstGen::Addr(InstGen::Reg(find_register(label_reg_addr)), 0));
      return register_mapping[v];
    }
    v->getPrintName();

    return register_mapping[v];
  }else{
    if(global){
      // get label
      std::list<Value *> variable_list;
      variable_list.push_back(v);

      // generate label addr reg
      const std::string reg_name_0 = "LCPI_"+std::to_string(key++);
      Value *label_reg_addr = new Value(Type::getInt32Ty(), reg_name_0);
      variable_list.push_back(label_reg_addr);

      code += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
      update_value_mapping(variable_list);

      // generate label
      InstGen::Label src_label = InstGen::Label(".LCPI_"+v->getName());

      code += InstGen::getAddress(InstGen::Reg(find_register(label_reg_addr)), src_label);
      code += InstGen::load(InstGen::Reg(find_register(v)),InstGen::Addr(InstGen::Reg(find_register(label_reg_addr)), 0));
      return register_mapping[v];
    }
    else {
      ERROR(" not find v in register! %s",v->getPrintName().c_str());
    }
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

// 用于生成二元整数运算指令的汇编
std::string AsmBuilder::generateOperInst (Instruction *inst) {
    auto &operands = inst->getOperandList();
    // std::list<Value *> variable_list;
    std::string return_asm;
    //用于接收可能的全局变量寄存器分配操作str
    std::string register_str = "";
    // 若第一个操作数为立即数，存储该立即数的value
    Value * src_value_0;//NOTICE

    auto src_op0 = operands.at(0);
    auto src_op1 = operands.at(1);

    if (inst->isAdd() || inst->isSub() || inst->isDiv()) {

      variable_list.push_back(inst);

      if (src_op0->isConstant() ) { // 第一个操作数为立即数
          // 给第一个操作数加上名字，用于分配寄存器,建立映射
          const std::string reg_name_0 = "_imm_"+std::to_string(key++);
          src_value_0 = new Value(Type::getInt32Ty(), reg_name_0);
          variable_list.push_back(src_value_0);

          if (src_op1->isConstant()) {// IMM IMM
            // 寄存器分配
            return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            return_asm += update_value_mapping(variable_list);

            // 增加asm 注释
            if (inst->isAdd()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " + "+
                          src_op1->getPrintName(), "");
            } else if (inst->isSub()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " - "+
                          src_op1->getPrintName(), "");
            } else if (inst->isDiv()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " / "+
                          src_op1->getPrintName(), "");
            }

            // find分配的寄存器
            const InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_value_0,register_str));
            return_asm += register_str;
            register_str = "";
            // 将立即数的值绑定到寄存器中
            return_asm += InstGen::setValue(src_reg_0,InstGen::Constant(atoi(src_op0->getPrintName().c_str())));

            const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
            return_asm += register_str;
            const InstGen::Constant src_const_1 = InstGen::Constant(atoi(src_op1->getPrintName().c_str()));

            if (inst->isAdd()) {
              return_asm += InstGen::add(target_reg,src_reg_0,src_const_1);
            } else if (inst->isSub()) {
              return_asm += InstGen::sub(target_reg,src_reg_0,src_const_1);
            } else if (inst->isDiv()) {
              return_asm += InstGen::divConst(target_reg,src_reg_0,src_const_1);
            }

          }
          else { // 第二个操作数是寄存器 IMM REG
            variable_list.push_back(src_op1);

            // 寄存器分配
            return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            return_asm += update_value_mapping(variable_list);

            // 增加asm 注释
            if (inst->isAdd()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " + "+
                          src_op1->getPrintName(), "");
            } else if (inst->isSub()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " - "+
                          src_op1->getPrintName(), "");
            } else if (inst->isDiv()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " / "+
                          src_op1->getPrintName(), "");
            }

            // find分配的寄存器
            const InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_value_0,register_str));
            return_asm += register_str;
            register_str = "";
            // 将立即数的值绑定到寄存器中
            return_asm += InstGen::setValue(src_reg_0,InstGen::Constant(atoi(src_op0->getPrintName().c_str())));

            const InstGen::Reg src_reg_1 = InstGen::Reg(find_register(src_op1,register_str));
            return_asm += register_str;
            register_str = "";
            const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
            return_asm += register_str;

            if (inst->isAdd()) {
              return_asm += InstGen::add(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isSub()) {
              return_asm += InstGen::sub(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isDiv()) {
              // for div imm reg, 先将IMM 保存到reg，然后调用函数计算
              return_asm += generateFunctionCall(inst,operands,"__aeabi_idiv", 0);
            }

          }
      } else { // 第一个操作数为reg型变量
          // 给寄存器分配
          variable_list.push_back(src_op0);

          if (src_op1->isConstant()) { //REG IMM
            // 寄存器分配
            return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            return_asm += update_value_mapping(variable_list);

            // 增加asm 注释
            if (inst->isAdd()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " + "+
                          src_op1->getPrintName(), "");
            } else if (inst->isSub()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " - "+
                          src_op1->getPrintName(), "");
            } else if (inst->isDiv()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " / "+
                          src_op1->getPrintName(), "");
            }

            // find分配的寄存器
            const InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_op0,register_str));
            return_asm += register_str;
            register_str = "";
            const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
            return_asm += register_str;
            // 生成constant存储立即数的值
            const InstGen::Constant src_const_1 = InstGen::Constant(atoi(src_op1->getPrintName().c_str()));

            if (inst->isAdd()) {
              return_asm += InstGen::add(target_reg,src_reg_0,src_const_1);
            } else if (inst->isSub()) {
              return_asm += InstGen::sub(target_reg,src_reg_0,src_const_1);
            } else if (inst->isDiv()) {
              return_asm += InstGen::divConst(target_reg,src_reg_0,src_const_1);
            }

          } else { // REG REG
            variable_list.push_back(src_op1);

            // 寄存器分配
            return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            return_asm += update_value_mapping(variable_list);

            // 增加asm 注释
            if (inst->isAdd()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " + "+
                          src_op1->getPrintName(), "");
            } else if (inst->isSub()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " - "+
                          src_op1->getPrintName(), "");
            } else if (inst->isDiv()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " / "+
                          src_op1->getPrintName(), "");
            }

            // find分配的寄存器
            const InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_op0,register_str));
            return_asm += register_str;
            register_str = "";
            const InstGen::Reg src_reg_1 = InstGen::Reg(find_register(src_op1,register_str));
            return_asm += register_str;
            register_str = "";
            const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
            return_asm += register_str;

            if (inst->isAdd()) {
              return_asm += InstGen::add(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isSub()) {
              return_asm += InstGen::sub(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isDiv()) {
              return_asm += generateFunctionCall(inst,operands,"__aeabi_idiv", 0);
            }
          }
      }
    } else if (inst->isMul() || inst->isRem()) {

      variable_list.push_back(inst);

      if (src_op0->isConstant() ) { // 第一个操作数为立即数
          // 给第一个操作数加上名字，用于分配寄存器,建立映射
          const std::string reg_name_0 = "_imm_"+std::to_string(key++);
          Value * src_value_0 = new Value(Type::getInt32Ty(), reg_name_0);
          variable_list.push_back(src_value_0);

          if (src_op1->isConstant()) {// IMM IMM
            // 乘法指令两个操作数必须均为reg
            // 给第一个操作数加上名字，用于分配寄存器,建立映射
            const std::string reg_name_1 = "_imm_"+std::to_string(key++);
            Value * src_value_1 = new Value(Type::getInt32Ty(), reg_name_1);
            variable_list.push_back(src_value_1);

            // 寄存器分配
            return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            return_asm += update_value_mapping(variable_list);

            if (inst->isMul()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " * "+
                          src_op1->getPrintName(), "");
            } else if (inst->isRem()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " % "+
                          src_op1->getPrintName(), "");
            }

            // find分配的寄存器
            const InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_value_0,register_str));
            return_asm += register_str;
            register_str = "";
            // 将立即数的值绑定到寄存器中
            return_asm += InstGen::setValue(src_reg_0,InstGen::Constant(atoi(src_op0->getPrintName().c_str())));

            const InstGen::Reg src_reg_1 = InstGen::Reg(find_register(src_value_1,register_str));
            return_asm += register_str;
            register_str = "";
            // 将立即数的值绑定到寄存器中
            return_asm += InstGen::setValue(src_reg_1,InstGen::Constant(atoi(src_op1->getPrintName().c_str())));

            const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
            return_asm += register_str;

            if (inst->isMul()) {
              return_asm += InstGen::mul(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isRem()) {
              return_asm += generateFunctionCall(inst,operands,"__aeabi_idivmod", 1);
            }
          }
          else { // 第二个操作数是寄存器 IMM REG
            variable_list.push_back(src_op1);

            // 寄存器分配
            return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            return_asm += update_value_mapping(variable_list);

            if (inst->isMul()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " * "+
                          src_op1->getPrintName(), "");
            } else if (inst->isRem()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " % "+
                          src_op1->getPrintName(), "");
            }

            // find分配的寄存器
            const InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_value_0,register_str));
            return_asm += register_str;
            register_str = "";
            // 将立即数的值绑定到寄存器中
            return_asm += InstGen::setValue(src_reg_0,InstGen::Constant(atoi(src_op0->getPrintName().c_str())));

            const InstGen::Reg src_reg_1 = InstGen::Reg(find_register(src_op1,register_str));
            return_asm += register_str;
            register_str = "";
            const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
            return_asm += register_str;

            if (inst->isMul()) {
              return_asm += InstGen::mul(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isRem()) {
              return_asm += generateFunctionCall(inst,operands,"__aeabi_idivmod", 1);
            }
          }
      } else { // 第一个操作数为reg型变量
          // 给寄存器分配
          variable_list.push_back(src_op0);

          if (src_op1->isConstant()) { //REG IMM
            // 乘法指令两个操作数必须均为reg
            // 给第一个操作数加上名字，用于分配寄存器,建立映射
            const std::string reg_name_1 = "_imm_"+std::to_string(key++);
            Value * src_value_1 = new Value(Type::getInt32Ty(), reg_name_1);
            variable_list.push_back(src_value_1);

            // 寄存器分配
            return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            return_asm += update_value_mapping(variable_list);

            if (inst->isMul()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " * "+
                          src_op1->getPrintName(), "");
            } else if (inst->isRem()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " % "+
                          src_op1->getPrintName(), "");
            }
            // find分配的寄存器
            const InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_op0,register_str));
            return_asm += register_str;
            register_str = "";
            const InstGen::Reg src_reg_1 = InstGen::Reg(find_register(src_value_1,register_str));
            return_asm += register_str;
            register_str = "";
            // 将立即数的值绑定到寄存器中
            return_asm += InstGen::setValue(src_reg_1,InstGen::Constant(atoi(src_op1->getPrintName().c_str())));

            const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
            return_asm += register_str;
            const InstGen::Constant src_const_1 = InstGen::Constant(atoi(src_op1->getPrintName().c_str()));

            if (inst->isMul()) {
              return_asm += InstGen::mul(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isRem()) {
              return_asm += generateFunctionCall(inst,operands,"__aeabi_idivmod", 1);
            }
          } else { // REG REG
            variable_list.push_back(src_op1);

            // 寄存器分配
            return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            return_asm += update_value_mapping(variable_list);

            if (inst->isMul()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " * "+
                          src_op1->getPrintName(), "");
            } else if (inst->isRem()) {
              return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " % "+
                          src_op1->getPrintName(), "");
            }

            // find分配的寄存器
            const InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_op0,register_str));
            return_asm += register_str;
            register_str = "";
            const InstGen::Reg src_reg_1 = InstGen::Reg(find_register(src_op1,register_str));
            return_asm += register_str;
            register_str = "";
            const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
            return_asm += register_str;

            if (inst->isMul()) {
              return_asm += InstGen::mul(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isRem()) {
              return_asm += generateFunctionCall(inst,operands,"__aeabi_idivmod", 1);
            }
          }
      }
    }

  return return_asm;
}


std::string AsmBuilder::generateFunctionCall(Instruction *inst, std::vector<Value *>operands,
            std::string func_name, int return_reg) {
    std::string return_asm;
    // 保存調用者保存寄存器
    std::vector<InstGen::Reg> caller_reg_list;
    for(auto reg : caller_save_regs)
        caller_reg_list.push_back(reg);
    std::vector<InstGen::VFPReg> caller_vfpreg_list;
    for(auto reg : caller_save_vfpregs)
        caller_vfpreg_list.push_back(reg);
    return_asm += InstGen::push(caller_reg_list);
    return_asm += InstGen::push(caller_vfpreg_list);

    // 被调用者保存的寄存器数量
    int callee_regs_size = callee_save_vfpregs.size() + callee_save_regs.size();
    int caller_regs_size = caller_save_vfpregs.size() + caller_save_regs.size();

    return_asm += InstGen::comment(" call " + func_name, "");
    if (inst->isCall()) { //若该指令直接调用函数，其第一个operand为函数名,传参时需要跳过
      std::vector<Value *> args(operands.begin()+1, operands.end());

      int offset = 0;

      for(auto arg: args){
            std::list<Value*> arg_list;
            arg_list.push_back(arg);
            // 通过栈传递函数参数
            return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),
                        "stack offset = "+std::to_string(stack_mapping[arg]));
            MyAssert("error arg", stack_mapping.count(arg) == 1);

            if (arg->isConstant()) {// 若参数直接为立即数
              if (arg->getType()->isFloatTy())  {
                return_asm += InstGen::vmov(InstGen::VFPReg(vfpreg_index),InstGen::ConstantFP(atof(arg->getPrintName().c_str())));
                return_asm += InstGen::comment("transfer fp imm args:",std::to_string(callee_regs_size+1)+" "+std::to_string(stack_size)+" "+std::to_string(offset));
                return_asm += InstGen::vstore(InstGen::VFPReg(vfpreg_index),InstGen::Addr(InstGen::sp,offset-(callee_regs_size+1)*4-stack_size));
              } else {
                return_asm += InstGen::setValue(InstGen::Reg(reg_index),InstGen::Constant(atoi(arg->getPrintName().c_str())));
                return_asm += InstGen::comment("transfer imm args:",std::to_string(callee_regs_size+1)+" "+std::to_string(stack_size)+" "+std::to_string(offset));
                return_asm += InstGen::store(InstGen::Reg(reg_index),InstGen::Addr(InstGen::sp,offset-(callee_regs_size+1)*4-stack_size));
              }
            } else { // 若参数为变量
              if (arg->getType()->isFloatTy()) {
                int v_offset = stack_mapping[arg] + caller_regs_size*4;
                return_asm += InstGen::comment("load fp arg"+(arg)->getPrintName()+" from sp+"+std::to_string(v_offset),"pop val");
                return_asm += InstGen::vload(InstGen::VFPReg(vfpreg_index),InstGen::Addr(InstGen::sp,v_offset));

                return_asm += InstGen::comment("transfer fp val args:",std::to_string(callee_regs_size+1)+" "+std::to_string(stack_size)+" "+std::to_string(offset));
                return_asm += InstGen::vstore(InstGen::VFPReg(vfpreg_index),InstGen::Addr(InstGen::sp,offset-(callee_regs_size+1)*4-stack_size));
              }else {
                // update_value_mapping(arg_list);
                int v_offset = stack_mapping[arg] + caller_regs_size*4;
                return_asm += InstGen::comment("load "+(arg)->getPrintName()+" from sp+"+std::to_string(v_offset),"pop val");
                return_asm += InstGen::load(InstGen::Reg(reg_index),InstGen::Addr(InstGen::sp,v_offset));
                // register_mapping[arg] = reg_index++;
                // update_arg_mapping(arg);
                return_asm += InstGen::comment("transfer val args:",std::to_string(callee_regs_size+1)+" "+std::to_string(stack_size)+" "+std::to_string(offset));
                return_asm += InstGen::store(InstGen::Reg(reg_index),InstGen::Addr(InstGen::sp,offset-(callee_regs_size+1)*4-stack_size));
              }
            }
            offset += 4;
        }
        return_asm += InstGen::bl(func_name);
        if (dynamic_cast<CallInst *>(inst)->getType()->isFloatTy()) {// 若有返回值
          return_asm += InstGen::vstore(InstGen::VFPReg(return_reg),InstGen::Addr(InstGen::sp,return_offset+caller_regs_size*4));
        } else {
          // stack_mapping[inst] = return_offset;
          return_asm += InstGen::store(InstGen::Reg(return_reg),InstGen::Addr(InstGen::sp,return_offset+caller_regs_size*4));
        }

        return_asm += InstGen::pop(caller_vfpreg_list);
        return_asm += InstGen::pop(caller_reg_list);

        if (dynamic_cast<CallInst *>(inst)->getType()->isFloatTy()) { // 若有返回值
          fp_variable_list.clear();
          fp_variable_list.push_back(inst);
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_fpvalue_mapping(fp_variable_list);
          return_asm += InstGen::vload(InstGen::VFPReg(find_vfpregister(inst)),InstGen::Addr(InstGen::sp,return_offset));
        } else {
          variable_list.clear();
          variable_list.push_back(inst);
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_value_mapping(variable_list);
          return_asm += InstGen::load(InstGen::Reg(find_register(inst)),InstGen::Addr(InstGen::sp,return_offset));
        }
    } else if (inst->isCast()) { // 实现 int/float 类型转化
      int v_offset = stack_mapping[operands.at(0)] + caller_regs_size*4;
      return_asm += InstGen::comment("load "+(operands.at(0))->getPrintName()+" from sp+"+std::to_string(v_offset),"pop val");
      return_asm += InstGen::load(InstGen::Reg(reg_index),InstGen::Addr(InstGen::sp,v_offset));

      return_asm += InstGen::mov(InstGen::Reg(0),InstGen::Reg(reg_index));

      return_asm += InstGen::bl(func_name);
      return_asm += InstGen::store(InstGen::Reg(return_reg),InstGen::Addr(InstGen::sp,return_offset+caller_regs_size*4));
      return_asm += InstGen::pop(caller_vfpreg_list);
      return_asm += InstGen::pop(caller_reg_list);

      if (inst->getType()->isFloatTy()) { // abi 调用，通过 r0 进行参数传递和结果返回
        return_asm += InstGen::vload(InstGen::VFPReg(find_vfpregister(inst)),InstGen::Addr(InstGen::sp,return_offset));
      } else { // 结果为int
        return_asm += InstGen::load(InstGen::Reg(find_register(inst)),InstGen::Addr(InstGen::sp,return_offset));
      }
    } else {
      std::vector<Value *> args(operands.begin(), operands.end());
      for (int i = 0; i < 2; i++ ) { //对于除法函数,取余函数，只有两个参数
        if (operands.at(i)->isConstant()) { //直接将立即数传入传参寄存器
          return_asm += InstGen::setValue(InstGen::Reg(i),InstGen::Constant(atoi(args[i]->getPrintName().c_str())));
        } else {
          return_asm += InstGen::mov(InstGen::Reg(i), InstGen::Reg(find_register(args[i])));
        }
      }
      return_asm += InstGen::bl(func_name);

      return_asm += InstGen::store(InstGen::Reg(return_reg),InstGen::Addr(InstGen::sp,return_offset+caller_regs_size*4));
      return_asm += InstGen::pop(caller_vfpreg_list);
      return_asm += InstGen::pop(caller_reg_list);
      return_asm += InstGen::load(InstGen::Reg(find_register(inst)),InstGen::Addr(InstGen::sp,return_offset));
    }

    return return_asm;
}

InstGen::CmpOp cmpConvert(CmpInst::CmpOp myCmpOp, bool reverse) {
  InstGen::CmpOp asmCmpOp;

  if (!reverse) {
    switch (myCmpOp) {
    case CmpInst::CmpOp::EQ:
      asmCmpOp = InstGen::CmpOp::EQ;
      break;
    case CmpInst::CmpOp::NEQ:
      asmCmpOp = InstGen::CmpOp::NE;
      break;
    case CmpInst::CmpOp::GT:
      asmCmpOp = InstGen::CmpOp::GT;
      break;
    case CmpInst::CmpOp::GE:
      asmCmpOp = InstGen::CmpOp::GE;
      break;
    case CmpInst::CmpOp::LT:
      asmCmpOp = InstGen::CmpOp::LT;
      break;
    case CmpInst::CmpOp::LE:
      asmCmpOp = InstGen::CmpOp::LE;
      break;
    default:
      ERROR("CmpOp type not valid");
    }
  } else {
    switch (myCmpOp) {
    case CmpInst::CmpOp::EQ:
      asmCmpOp = InstGen::CmpOp::NE;
      break;
    case CmpInst::CmpOp::NEQ:
      asmCmpOp = InstGen::CmpOp::EQ;
      break;
    case CmpInst::CmpOp::GT:
      asmCmpOp = InstGen::CmpOp::LE;
      break;
    case CmpInst::CmpOp::GE:
      asmCmpOp = InstGen::CmpOp::LT;
      break;
    case CmpInst::CmpOp::LT:
      asmCmpOp = InstGen::CmpOp::GE;
      break;
    case CmpInst::CmpOp::LE:
      asmCmpOp = InstGen::CmpOp::GT;
      break;
    default:
      ERROR("CmpOp type not valid");
    }
  }

  return asmCmpOp;
}

// 用于生成二元浮点加减乘除指令的汇编,其操作数必须均为寄存器
std::string AsmBuilder::generateFPOperInst(Instruction *inst) {
    std::string return_asm;
    auto &operands = inst->getOperandList();
    std::string register_str = "";

    auto src_op0 = operands.at(0);
    auto src_op1 = operands.at(1);

    Value *src_value_0, *src_value_1;//NOTICE
    std::string reg_name_0, reg_name_1;

    if (inst->isCmp()) {
      return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " cmp "+
                            src_op1->getPrintName(), "");
      variable_list.push_back(inst); // 对于cmp, fcmp结果均为int型

      return_asm += update_value_mapping(variable_list); // 结果保存在int值寄存器中
    } else {
      if (inst->isMul()) {
        return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " * "+
                    src_op1->getPrintName() + " fp mul", "");
      } else if (inst->isAdd()) {
        return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " + "+
                    src_op1->getPrintName()+ " fp add", "");
      } else if (inst->isSub()) {
        return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " - "+
                    src_op1->getPrintName()+" fp sub", "");
      } else if (inst->isDiv()) {
        return_asm +=  InstGen::comment(inst->getPrintName()+" = "+src_op0->getPrintName() + " / "+
                    src_op1->getPrintName()+" fp div", "");
      }

      fp_variable_list.push_back(inst);
    }

    if (inst->getType()->isFloatTy() || src_op0->getType()->isFloatTy()) {
      if (src_op0->isConstant()) {
        // 给第一个操作数加上名字，用于分配寄存器,建立映射
        std::string reg_name_0 = "_fpimm_"+std::to_string(key++);
        src_value_0 = new Value(Type::getFloatTy(), reg_name_0);
        fp_variable_list.push_back(src_value_0);

        if (src_op1->isConstant()) { // vcmp 指令操作数均为reg
          std::string reg_name_1 = "_fpimm_"+std::to_string(key++);
          src_value_1 = new Value(Type::getFloatTy(), reg_name_1);
          fp_variable_list.push_back(src_value_1);

          // 寄存器分配
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_fpvalue_mapping(fp_variable_list);

          // find分配的寄存器
          const InstGen::VFPReg src_reg_0 = InstGen::VFPReg(find_vfpregister(src_value_0,register_str));
          // 将立即数的值绑定到寄存器中
          return_asm += InstGen::vmov(src_reg_0,InstGen::ConstantFP(atof(src_op0->getPrintName().c_str())));

          const InstGen::VFPReg src_reg_1 = InstGen::VFPReg(find_vfpregister(src_value_1,register_str));
          // 将立即数的值绑定到寄存器中
          return_asm += InstGen::vmov(src_reg_1,InstGen::ConstantFP(atof(src_op1->getPrintName().c_str())));

          if (inst->isCmp()) {
            return_asm += InstGen::vcmp(src_reg_0, src_reg_1);
            return_asm += InstGen::vmrs();
          } else {
            const InstGen::VFPReg target_reg = InstGen::VFPReg(find_vfpregister(inst,register_str));
            return_asm += register_str;

            if (inst->isMul()) {
              return_asm += InstGen::vmul(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isAdd()) {
              return_asm += InstGen::vadd(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isSub()) {
               return_asm += InstGen::vsub(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isDiv()) {
               return_asm += InstGen::vdiv(target_reg,src_reg_0,src_reg_1);
            }
          }
        } else { // imm reg
          fp_variable_list.push_back(src_op1);

          // 寄存器分配
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_fpvalue_mapping(fp_variable_list);

          // find分配的寄存器
          const InstGen::VFPReg src_reg_0 = InstGen::VFPReg(find_vfpregister(src_value_0,register_str));
          // 将立即数的值绑定到寄存器中
          return_asm += InstGen::vmov(src_reg_0,InstGen::ConstantFP(atof(src_op0->getPrintName().c_str())));
          const InstGen::VFPReg src_reg_1 = InstGen::VFPReg(find_vfpregister(src_op1,register_str));
          return_asm += register_str;
          register_str = "";

          if (inst->isCmp()) {
            return_asm += InstGen::vcmp(src_reg_0, src_reg_1);
            return_asm += InstGen::vmrs();
          } else {
            const InstGen::VFPReg target_reg = InstGen::VFPReg(find_vfpregister(inst,register_str));
            return_asm += register_str;

            if (inst->isMul()) {
              return_asm += InstGen::vmul(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isAdd()) {
              return_asm += InstGen::vadd(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isSub()) {
               return_asm += InstGen::vsub(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isDiv()) {
               return_asm += InstGen::vdiv(target_reg,src_reg_0,src_reg_1);
            }
          }
        }
      } else {
        fp_variable_list.push_back(src_op0);

        if (src_op1->isConstant()) { // reg imm
          std::string reg_name_1 = "_fpimm_"+std::to_string(key++);
          src_value_1 = new Value(Type::getFloatTy(), reg_name_1);
          fp_variable_list.push_back(src_value_1);

          // 寄存器分配
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_fpvalue_mapping(fp_variable_list);

          const InstGen::VFPReg src_reg_0 = InstGen::VFPReg(find_vfpregister(src_op0,register_str));
          return_asm += register_str;
          register_str = "";
          const InstGen::VFPReg src_reg_1 = InstGen::VFPReg(find_vfpregister(src_value_1,register_str));
          // 将立即数的值绑定到寄存器中
          return_asm += InstGen::vmov(src_reg_1,InstGen::ConstantFP(atof(src_op1->getPrintName().c_str())));

          if (inst->isCmp()) {
            return_asm += InstGen::vcmp(src_reg_0, src_reg_1);
            return_asm += InstGen::vmrs();
          } else {
            const InstGen::VFPReg target_reg = InstGen::VFPReg(find_vfpregister(inst,register_str));
            return_asm += register_str;

            if (inst->isMul()) {
              return_asm += InstGen::vmul(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isAdd()) {
              return_asm += InstGen::vadd(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isSub()) {
               return_asm += InstGen::vsub(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isDiv()) {
               return_asm += InstGen::vdiv(target_reg,src_reg_0,src_reg_1);
            }
          }
        } else { // reg reg
          fp_variable_list.push_back(src_op1);

          // 寄存器分配
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_fpvalue_mapping(fp_variable_list);

          const InstGen::VFPReg src_reg_0 = InstGen::VFPReg(find_vfpregister(src_op0,register_str));
          return_asm += register_str;
          register_str = "";
          const InstGen::VFPReg src_reg_1 = InstGen::VFPReg(find_vfpregister(src_op1,register_str));
          return_asm += register_str;
          register_str = "";

          if (inst->isCmp()) {
            return_asm += InstGen::vcmp(src_reg_0, src_reg_1);
            return_asm += InstGen::vmrs();
          } else {
            const InstGen::VFPReg target_reg = InstGen::VFPReg(find_vfpregister(inst,register_str));
            return_asm += register_str;

            if (inst->isMul()) {
              return_asm += InstGen::vmul(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isAdd()) {
              return_asm += InstGen::vadd(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isSub()) {
               return_asm += InstGen::vsub(target_reg,src_reg_0,src_reg_1);
            } else if (inst->isDiv()) {
               return_asm += InstGen::vdiv(target_reg,src_reg_0,src_reg_1);
            }
          }
        }
      }
    } else { // for icmp
      if (src_op0->isConstant()) {
        // 给第一个操作数加上名字，用于分配寄存器,建立映射
        std::string reg_name_0 = "_imm_"+std::to_string(key++);
        src_value_0 = new Value(Type::getInt32Ty(), reg_name_0);
        variable_list.push_back(src_value_0);

        if (src_op1->isConstant()) { // IMM IMM
          // 寄存器分配
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_value_mapping(variable_list);

          // 查找存储立即数的寄存器
          InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_value_0));
          // 生成constant存储立即数的值
          const InstGen::Constant src_const_1 = InstGen::Constant(atoi(src_op1->getPrintName().c_str()));

          return_asm += InstGen::instConst(InstGen::cmp, src_reg_0,src_const_1);
        } else { // IMM REG
          variable_list.push_back(src_op1);

          // 寄存器分配
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_value_mapping(variable_list);

          // 查找存储立即数的寄存器
          InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_value_0));
          const InstGen::Reg src_reg_1 = InstGen::Reg(find_register(src_op1,register_str));
          return_asm += register_str;
          register_str = "";
          return_asm += InstGen::cmp(src_reg_0,src_reg_1);
        }
      } else {
        // 给寄存器分配
        variable_list.push_back(src_op0);

        if (src_op1->isConstant()) { // REG IMM
          // 寄存器分配
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_value_mapping(variable_list);

          InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_op0));

          // 生成constant存储立即数的值
          const InstGen::Constant src_const_1 = InstGen::Constant(atoi(src_op1->getPrintName().c_str()));
          return_asm += InstGen::instConst(InstGen::cmp, src_reg_0,src_const_1);
        }
        else { // REG REG
          variable_list.push_back(src_op1);
          // 寄存器分配
          return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          return_asm += update_value_mapping(variable_list);

          InstGen::Reg src_reg_0 = InstGen::Reg(find_register(src_op0, register_str));
          return_asm += register_str;
          register_str = "";
          const InstGen::Reg src_reg_1 = InstGen::Reg(find_register(src_op1,register_str));
          return_asm += register_str;
          register_str = "";

          return_asm += InstGen::cmp(src_reg_0,src_reg_1);
        }
      }
    }

    if (inst->isCmp()) {
      InstGen::CmpOp cmp_op = cmpConvert(static_cast<CmpInst *>(inst)->_cmp_op, false);
      // 将比较结果存入目标寄存器， 0 --> false, 1 --> true;
      return_asm += InstGen::mov(InstGen::Reg(find_register(inst)),InstGen::Constant(0),InstGen::NOP);
      return_asm += InstGen::mov(InstGen::Reg(find_register(inst)),InstGen::Constant(1),cmp_op);
    }

    return return_asm;
}

std::string AsmBuilder::generateLoadInst(Instruction *inst) {
    std::string return_asm;
    std::string register_str = "";
    auto &operands = inst->getOperandList();

    auto src_op0 = operands.at(0);
    if (inst->getType()->isIntegerTy()) {
      variable_list.push_back(src_op0);
      variable_list.push_back(inst);

      return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
      return_asm += update_value_mapping(variable_list);
      const InstGen::Reg src_reg0 = InstGen::Reg(find_register(src_op0,register_str));
      return_asm += register_str;
      register_str = "";
      const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
      return_asm += register_str;
      register_str = "";

      return_asm +=  InstGen::comment(target_reg.getName()+" load from "+src_op0->getPrintName().c_str(), "");
      return_asm += InstGen::load(target_reg, InstGen::Addr(src_reg0,0));
    } else { // load fp element
      variable_list.push_back(src_op0);
      fp_variable_list.push_back(inst);

      return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
      return_asm += update_value_mapping(variable_list);
      return_asm += update_fpvalue_mapping(fp_variable_list);

      const InstGen::Reg src_reg0 = InstGen::Reg(find_register(src_op0,register_str));
      return_asm += register_str;
      register_str = "";
      const InstGen::VFPReg target_reg = InstGen::VFPReg(find_vfpregister(inst,register_str));
      return_asm += register_str;
      register_str = "";

      return_asm +=  InstGen::comment(target_reg.getName()+" load from "+src_op0->getPrintName().c_str(), "fp load");
      return_asm += InstGen::vload(target_reg, InstGen::Addr(src_reg0,0));
    }

    return return_asm;
}

std::string AsmBuilder::generateStoreInst(Instruction *inst) {
    std::string return_asm;
    std::string register_str = "";
    auto &operands = inst->getOperandList();

    Value *src_value_0, *src_value_1;//NOTICE
    std::string reg_name_0, reg_name_1;

    auto src = operands.at(0);
    auto src_op2 = operands.at(1);
    variable_list.push_back(src_op2);

    if (src->getType()->isFloatTy()) {
      if (src->isConstant()) { //存储的值是立即数，需要先将立即数存储到reg
        ConstantFloat* src_op1 =  dynamic_cast<ConstantFloat *>(operands.at(0));
        std::string reg_name_0 = "_fpimm_"+std::to_string(key++);
        src_value_0 = new Value(Type::getFloatTy(), reg_name_0);
        fp_variable_list.push_back(src_value_0);

        return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
        return_asm += update_fpvalue_mapping(fp_variable_list);
        return_asm += update_value_mapping(variable_list);

        const InstGen::VFPReg src_reg1 = InstGen::VFPReg(find_vfpregister(src_value_0,register_str));
        return_asm += register_str;
        register_str = "";
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2,register_str));
        return_asm += register_str;
        // const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));

        return_asm +=  InstGen::comment(src->getPrintName()+" store to "+src_reg2.getName(), "fp store");
        InstGen::CmpOp cmpop = InstGen::CmpOp(InstGen::NOP);
        return_asm += InstGen::vmov(src_reg1,InstGen::ConstantFP(atof(src_op1->getPrintName().c_str())));
        return_asm += InstGen::vstore(src_reg1, InstGen::Addr(src_reg2,0));

        // return_asm += erase_value_mapping(variable_list);

      } else{ // 需要存储的值是寄存器
        auto src_op1 = operands.at(0);
        fp_variable_list.push_back(src_op1);

        return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
        return_asm += update_fpvalue_mapping(fp_variable_list);
        return_asm += update_value_mapping(variable_list);

        const InstGen::VFPReg src_reg1 = InstGen::VFPReg(find_vfpregister(src_op1,register_str));
        return_asm += register_str;
        register_str = "";
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2,register_str));
        return_asm += register_str;

        return_asm +=  InstGen::comment(src->getPrintName()+" store to "+src_reg2.getName(), "fp store");
        return_asm += InstGen::vstore(src_reg1, InstGen::Addr(src_reg2,0));
      }
    } else {
      if (src->isConstant()) { //存储的值是立即数，需要先将立即数存储到reg
        ConstantInt* src_op1 =  dynamic_cast<ConstantInt *>(operands.at(0));
        const std::string reg_name = "_imm_"+std::to_string(key++);
        Value * val = new Value(Type::getInt32Ty(), reg_name);
        variable_list.push_back(val);
        return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
        return_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(val,register_str));
        return_asm += register_str;
        register_str = "";
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2,register_str));
        return_asm += register_str;
        // const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));

        return_asm +=  InstGen::comment(src->getPrintName()+" store to "+src_reg2.getName(), "");
        InstGen::CmpOp cmpop = InstGen::CmpOp(InstGen::NOP);
        return_asm += InstGen::setValue(src_reg1,InstGen::Constant(atoi(src_op1->getPrintName().c_str())));
        return_asm += InstGen::store(src_reg1, InstGen::Addr(src_reg2,0));

        // return_asm += erase_value_mapping(variable_list);

      } else{ // 需要存储的值是寄存器
        auto src_op1 = operands.at(0);
        variable_list.push_back(src_op1);
        return_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
        return_asm += update_value_mapping(variable_list);
        const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1,register_str));
        return_asm += register_str;
        register_str = "";
        const InstGen::Reg src_reg2 = InstGen::Reg(find_register(src_op2,register_str));
        return_asm += register_str;

        return_asm +=  InstGen::comment(src->getPrintName()+" store to "+src_reg2.getName(), "");
        return_asm += InstGen::store(src_reg1, InstGen::Addr(src_reg2,0));
      }
    }

    return return_asm;
}

std::string AsmBuilder::generateInstructionCode(Instruction *inst) {
    std::string inst_asm;
    auto &operands = inst->getOperandList();

    BasicBlock *bb_cur = inst->getParent();

    int type_size = 0;
    //用于接收可能的全局变量寄存器分配操作str
    std::string register_str = "";
    if(!inst->isAlloca()){
      //维护变量的栈分配
      if (inst->isGep()) {
        type_size = 4;
        stack_mapping[inst] = stack_cur_size;
      } else {
        type_size = inst->getType()->getSize();
        stack_mapping[inst] = stack_cur_size;
      }
    }

    if (inst->isAdd() || inst->isSub() || inst->isMul() ||
        inst->isDiv() || inst->isRem()) { //算术运算指令
      if (inst->getType()->isFloatTy()) {
        inst_asm += generateFPOperInst(inst);
      } else {
        inst_asm += generateOperInst(inst);
      }
    } else if (inst->isCmp()) {
      inst_asm += generateFPOperInst(inst);
    } else if (inst->isLoad()) {
      inst_asm += generateLoadInst(inst);
    } else if (inst->isStore()) {
        inst_asm += generateStoreInst(inst);
    } else if (inst->isRet()) {
      if (inst->getOperandNumber() != 0) { // 处理有返回值的情况
        auto src = operands.at(0);
        if (src->isConstant()) { //存储的值是立即数，需要先将立即数存储到reg
          if (src->getType()->isFloatTy()) {
            inst_asm += InstGen::vret(InstGen::ConstantFP(atof(src->getPrintName().c_str())));
          } else {
            inst_asm += InstGen::ret(InstGen::Constant(atoi(src->getPrintName().c_str())));
          }
        } else {
          if (src->getType()->isFloatTy()) { // ret float %15
            auto src_op1 = operands.at(0);
            fp_variable_list.push_back(src_op1);
            inst_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            inst_asm += update_fpvalue_mapping(fp_variable_list);

            if (find_vfpregister(src_op1,register_str) != 0) {//如果源寄存器就是s0，就无需mov
              inst_asm += register_str;
              register_str = "";
              const InstGen::VFPReg src_reg1 = InstGen::VFPReg(find_register(src_op1,register_str));
              inst_asm += register_str;
              inst_asm += InstGen::vret(src_reg1);
            }
          } else {
            auto src_op1 = operands.at(0);
            variable_list.push_back(src_op1);
            inst_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
            inst_asm += update_value_mapping(variable_list);
            if (find_register(src_op1,register_str) != 0) {//如果源寄存器就是r0，就无需mov
              inst_asm += register_str;
              register_str = "";
              const InstGen::Reg src_reg1 = InstGen::Reg(find_register(src_op1,register_str));
              inst_asm += register_str;
              inst_asm += InstGen::ret(src_reg1);
            }
          }
        }
      }

      // 生成函数退栈操作代码,无论函数返回类型
      inst_asm += generate_function_exit_code();

    } else if (inst->isBr()) {
      if (operands.size() == 1){
          auto src_op0 = operands.at(0)->getName();
          inst_asm += InstGen::b(InstGen::Label(getLabelName(bb_cur,src_op0)),InstGen::NOP);
      } else if (operands.size() == 2) {
          WARNNING("不太理解 br i1 %1, label %2 形式的跳转指令");
      } else if (operands.size() == 3) {
          auto src_op0 = operands.at(0);
          variable_list.push_back(src_op0);

          inst_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
          inst_asm += update_value_mapping(variable_list);
          auto src_op1_id = operands.at(1)->getName();
          auto src_op2_id = operands.at(2)->getName();

          // 根据reg0指令为0还是1判断跳转条件是否成立, 1 ==> 条件成立
          const InstGen::Reg src_reg0 = InstGen::Reg(find_register(src_op0,register_str));
          inst_asm += register_str;
          register_str = "";
          inst_asm += InstGen::cmp(src_reg0, InstGen::Constant(1));

          inst_asm += InstGen::b(InstGen::Label(getLabelName(bb_cur,src_op1_id)),InstGen::EQ);
          inst_asm += InstGen::b(InstGen::Label(getLabelName(bb_cur,src_op2_id)),InstGen::NOP);

      } else {
        ERROR("TO many args in br instruction");
      }
    } else if (inst->isCall()) {
      std::string func_name = operands.at(0)->getName();
      inst_asm += generateFunctionCall(inst,operands,func_name, 0);
    } else if (inst->isAlloca()) {
      type_size = inst->getType()->getSize();
      MyAssert("type size", type_size != 1);
      stack_mapping[inst] = stack_cur_size + type_size;

      type_size += 4;

      variable_list.push_back(inst); // inst 对应栈位置存储的值是地址
      inst_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
      inst_asm += update_value_mapping(variable_list);

      const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
      inst_asm += InstGen::comment(" alloc "+inst->getPrintName()+" to sp + " + std::to_string(stack_cur_size),
      " size: "+std::to_string(type_size));
      inst_asm += register_str;
      inst_asm += InstGen::instConst(InstGen::add, target_reg, InstGen::sp,
                                 InstGen::Constant(stack_cur_size));
    } else if (inst->isGep()) { //将gep指令转化为mul, add %1 =
      // new register variable_list.push_back(inst);
      variable_list.push_back(inst);
      auto src_op1 = operands.at(0);
      variable_list.push_back(src_op1);

      const std::string reg_name1 = "_imm_"+std::to_string(key++);
      Value * val1 = new Value(Type::getInt32Ty(), reg_name1);
      variable_list.push_back(val1);

      // 若操作数是寄存器，需要增加寄存器映射
      for(int i = 1; i < operands.size();i++) {
        auto src_op = operands.at(i);
        if (!src_op->isConstant()) {//不是立即数
          variable_list.push_back(src_op);
        }
      }

      // update register
      inst_asm += InstGen::comment(__FILE__+std::to_string(__LINE__),"");
      inst_asm += update_value_mapping(variable_list);
      inst_asm += InstGen::comment(" gep "+inst->getPrintName()," from " + src_op1->getPrintName());

      // init result reg 0
      const InstGen::Reg target_reg = InstGen::Reg(find_register(inst,register_str));
      inst_asm += register_str;
      inst_asm += InstGen::setValue(target_reg,InstGen::Constant(0));
      // get operand length
      //
      auto array_ty = src_op1->getType()->getPointerElementType();
      for(int i = 1; i < operands.size();i++){
          auto src_op = operands.at(i);
          auto element_size = array_ty->getSize();

          //  element_size = src_op->getType()->getPointerElementType()->getSize();
          if (!src_op->isConstant()) {//不是立即数

            inst_asm += InstGen::comment(" gep += "+src_op->getPrintName()+" * "+std::to_string(element_size),"");

            InstGen::Reg dst_reg = InstGen::Reg(find_register(val1,register_str));
            inst_asm += register_str;
            register_str = "";
            inst_asm += InstGen::setValue(dst_reg,InstGen::Constant(element_size));
            // inst_asm += InstGen::mul(InstGen::Reg(find_register(val1)),InstGen::Reg(find_register(val1)), InstGen::Reg(find_register(val2)));

            InstGen::Reg inst1_reg = InstGen::Reg(find_register(inst,register_str));
            inst_asm += register_str;
            register_str = "";
            InstGen::Reg val1_reg = InstGen::Reg(find_register(val1,register_str));
            inst_asm += register_str;
            register_str = "";
            InstGen::Reg val2_reg =  InstGen::Reg(find_register(src_op,register_str));
            inst_asm += register_str;
            register_str = "";
            InstGen::Reg inst2_reg =  InstGen::Reg(find_register(inst,register_str));
            inst_asm += register_str;

            inst_asm += InstGen::mla(inst1_reg,val1_reg,val2_reg,inst2_reg);

          } else if(static_cast<ConstantInt*>(src_op)->getValue() != 0){
            // 邪法
            inst_asm += InstGen::comment(" gep += "+src_op->getPrintName()+" * "+std::to_string(element_size),"");

            WARNNING("SRC OP name is: ",src_op->getPrintName().c_str());
            int offset = static_cast<ConstantInt*>(src_op)->getValue() * element_size;
            InstGen::Reg inst1_reg = InstGen::Reg(find_register(inst,register_str));
            inst_asm += register_str;
            register_str = "";
            InstGen::Reg inst2_reg = InstGen::Reg(find_register(inst,register_str));
            inst_asm += register_str;
            register_str = "";
            inst_asm += InstGen::instConst(InstGen::add, inst1_reg, inst2_reg,
                                 InstGen::Constant(offset));
            // variable_list.push_back(src_op2);
          }
          // variable_list.push_back(inst);
          array_ty = static_cast<ArrayType*>(array_ty)->getElementType();
      }
      InstGen::Reg inst1_reg = InstGen::Reg(find_register(inst,register_str));
      inst_asm += register_str;
      register_str = "";
      InstGen::Reg inst2_reg = InstGen::Reg(find_register(inst,register_str));
      inst_asm += register_str;
      register_str = "";
      InstGen::Reg inst3_reg =  InstGen::Reg(find_register(src_op1,register_str));
      inst_asm += register_str;
      register_str = "";
      inst_asm += InstGen::add(inst1_reg,inst2_reg,inst3_reg);
    } else if(inst->isCast()){
      if (inst->getType()->isFloatTy()) { // int to float 类型转换
        // 将inst结果存到栈中
        fp_variable_list.push_back(inst);
        inst_asm += update_fpvalue_mapping(fp_variable_list);

        inst_asm += generateFunctionCall(inst,operands,"__aeabi_i2f", 0); // sitofp
      } else {
        variable_list.push_back(inst);
        inst_asm += update_value_mapping(variable_list);
        inst_asm += generateFunctionCall(inst,operands,"__aeabi_f2iz", 0); // fptosi
      }

    } else {
      WARNNING("unrealized %s",inst->getPrintName().c_str());
    }

    if (type_size < 4) {
      type_size = 4;//保证对于bool型变量也分配4byte空间
    }

    // 将寄存器的值刷到栈里，退化为栈分配
    inst_asm += flushRegs2Stack(variable_list, fp_variable_list);

    stack_cur_size += type_size;
    variable_list.clear();//清空列表
    fp_variable_list.clear();//清空列表
    register_mapping.clear();//清空寄存器映射
    vfpregister_mapping.clear();//清空寄存器映射
    reg_index = 0;
    vfpreg_index = 0;

    WARNNING("coding instr %s type is %d ...\n%s",inst->getPrintName().c_str(),inst->getInstructionKind(),inst_asm.c_str());
    return inst_asm;
}
