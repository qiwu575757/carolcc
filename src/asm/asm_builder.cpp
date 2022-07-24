#include "asm_builder.h"
#include "asm_instr.h"
#include "utils.h"
#include <memory.h>

std::string AsmBuilder::generate_asm(std::map<Value *, int> register_mapping){
    ERROR("UNDO");
}

std::string AsmBuilder::generate_asm(std::string file_name){
    // 建立函数名-栈大小映射
    for(auto &func: this->module->getFunctions()){
      live_interval_analysis(func);
      allocaStackSpace(func);
      stack_size_mapping[func->getName()] = this->stack_size;
    }

    asm_code += generate_module_header(file_name);
    for(auto &func: this->module->getFunctions()){
        if(func->getBasicBlocks().size() != 0){
            live_interval_analysis(func);
            asm_code += generate_function_code(func);
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
    if (global_var->getInit() == nullptr) { // 全局变量默认初始化
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
    for(int i=0;i<init->getType()->getSize()/4;i++){
      if(array_code.c_str()[i*12+10] != '0'){
        check_array_init = true;
        break;
      }
    }
    if(check_array_init){
      asm_code += array_code;
    } else{
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

  if(const_float_val){
    return std::make_pair(float2int(const_float_val), true);
  } else if (const_int_val){
    return std::make_pair(const_int_val->getValue(), true);
  } else {
    ERROR("error const value");
  }

  //  已添加常量折叠优化，无需计算
  std::cerr << "Function getConstIntVal exception!" << std::endl;
  abort();
}

// 将浮点数转化为十进制表示，便于操作
int AsmBuilder::float2int(ConstantFloat *val) {
  float float_data = val->getValue();

  int long_data = *((int*)(&float_data));

  return long_data;
}

std::string AsmBuilder::generate_function_code(Function *func){
    std::string func_asm;

    allocaStackSpace(func);
    func_asm += func->getName() + ":" + InstGen::newline;
    func_asm +=
        InstGen::comment("stack_size=" + std::to_string(this->stack_size), "");

    func_asm += generate_function_entry_code(func);

    for (auto &bb : func->getBasicBlocks()) {
        func_asm += getLabelName(bb) + ":" + InstGen::newline;
        func_asm += generateBasicBlockCode(bb);
    }

    return func_asm;
}

std::string AsmBuilder::generate_function_entry_code(Function *func){
    std::string func_head_code;

    func_head_code+=getLabelName(func,0) + ":" + InstGen::newline;
    auto saved_registers = getCalleeSavedRegisters(func);
    saved_registers.push_back(InstGen::lr);
    saved_registers.push_back(InstGen::fp);
    std::sort(saved_registers.begin(), saved_registers.end());

    asm_code += InstGen::comment("save callee-save registers, lr and fp", "");
    func_head_code += InstGen::push(saved_registers);
    func_head_code += InstGen::comment("alloca stack space", "");
    func_head_code += InstGen::instConst(InstGen::sub, InstGen::sp, InstGen::sp,
                                 InstGen::Constant(this->stack_size, false));

    // 参数的映射认为已经处理好了
    return func_head_code;
}

std::string AsmBuilder::generate_function_exit_code(Function *func) {
  std::string func_tail_code;

  func_tail_code += InstGen::comment("function return", "");
  func_tail_code += InstGen::instConst(InstGen::add, InstGen::sp, InstGen::sp,
                                 InstGen::Constant(this->stack_size, false));
  // restore callee-save registers and pc
  auto saved_registers = getCalleeSavedRegisters(func);
  saved_registers.push_back(InstGen::fp);
  saved_registers.push_back(InstGen::pc); // 相当与 bl lr
  std::sort(saved_registers.begin(), saved_registers.end());
  func_tail_code += InstGen::pop(saved_registers);

  return func_tail_code;
}

std::string  AsmBuilder::generateBasicBlockCode(BasicBlock *bb){
    std::string bb_asm;
    for (auto &inst : bb->getInstructions()) {
        bb_asm += AsmBuilder::generateInstructionCode(inst);
    }

    return bb_asm;
}

std::string AsmBuilder::generateInstructionCode(Instruction *inst) {
  std::string asm_code;
  std::string str;
  auto &operands = inst->getOperandList();

  if (inst->isRet()) {
    if (!operands.empty()) { // 有返回值
      asm_code += assignToTargetReg(inst, operands[0], 0);
    }
    asm_code += generate_function_exit_code(inst->getFunction());
  } else if (inst->isAlloca()) {
    int offset = getAllocaSpOffset(inst);
    asm_code += InstGen::comment("alloca val"+inst->getPrintName(), "offset = " + std::to_string(offset));
    asm_code += InstGen::instConst(InstGen::add, InstGen::Reg(getRegIndexOfValue(inst,inst),false),
                    InstGen::sp, InstGen::Constant(offset,false));
  } else if (inst->isLoad()) {
    asm_code += InstGen::load(InstGen::Reg(getRegIndexOfValue(inst,inst),inst->getType()->isFloatTy()),
            InstGen::Addr(InstGen::Reg(getRegIndexOfValue(inst,operands[0]),false),0));
  } else if (inst->isStore()) {
    bool is_fp = operands[0]->getType()->isFloatTy();
    InstGen::Reg op2_reg = InstGen::Reg(getRegIndexOfValue(inst,operands[1]),false);
    if (operands[0]->isConstant()) {
      int op1_reg_index, tmp_reg_index;
      int int_value = is_fp ? float2int(dynamic_cast<ConstantFloat *>(operands[0])) :
                atoi(operands[0]->getPrintName().c_str());
      if (is_fp) { // vmov sx, #? 类型会溢出，需要使用整型寄存器做中介
        std::string str;
        tmp_reg_index = acquireForReg(inst,str);
        asm_code += str;
      }
      op1_reg_index = is_fp ? tmp_reg_index : getRegIndexOfValue(inst,operands[0]);
      asm_code += InstGen::setValue(InstGen::Reg(op1_reg_index,false),InstGen::Constant(int_value,false));
      asm_code += InstGen::store(InstGen::Reg(op1_reg_index,false),InstGen::Addr(op2_reg,0));
      if (is_fp) {
        asm_code += popValue(inst,tmp_reg_index); // 将申请的寄存器中的值恢复
      }
    } else {
      asm_code += InstGen::store(InstGen::Reg(getRegIndexOfValue(inst,operands[0]),is_fp),
                          InstGen::Addr(op2_reg,0));
    }
  } else if (inst->isCall()) {
    std::string func_name = operands.at(0)->getName();
    std::vector<Value *> args(operands.begin()+1, operands.end());
    asm_code += generateFunctionCall(inst,args,func_name, 0);
  } else if (inst->isGep()) {
    std::vector<int> allocaed;
    std::pair<int, bool> inst_pos = getValuePosition(inst,inst);
    int target_reg = inst_pos.second ? inst_pos.first : acquireForReg(inst,str);
    asm_code += str;
    str = "";
    for (int i = 0; i < 2; i++) { // 分配寄存器供下面使用
      allocaed.push_back(acquireForReg(inst,str));
      asm_code += str;
      str = "";
    }

    asm_code += InstGen::setValue(InstGen::Reg(target_reg,false),InstGen::Constant(0,false));

    auto array_ty = operands[0]->getType()->getPointerElementType();
    for (int i = 1; i < operands.size(); i++) {
      auto element_size = array_ty->getSize();
      int index = 0;// 每次从已分配的第一个寄存器开始使用

      asm_code += InstGen::comment(" gep += "+operands[i]->getPrintName()+" * "+std::to_string(element_size),"");
      if (!operands[i]->isConstant()) { // 该操作数为变量
        asm_code += InstGen::setValue(InstGen::Reg(index++,false),InstGen::Constant(element_size,false));

        std::pair<int, bool> op_pos = getValuePosition(inst,operands[i]);
        int op_reg = op_pos.second ? op_pos.first : allocaed[index];
        if (!op_pos.second)
          asm_code += InstGen::load(InstGen::Reg(op_reg,false),
                          InstGen::Addr(InstGen::sp,op_pos.first));

        asm_code += InstGen::mla(InstGen::Reg(target_reg,false), InstGen::Reg(allocaed[0],false),
                                InstGen::Reg(op_reg,false),InstGen::Reg(target_reg,false));
      } else if (static_cast<ConstantInt*>(operands[i])->getValue() != 0) {
        int offset = static_cast<ConstantInt*>(operands[i])->getValue() * element_size;
        asm_code += InstGen::instConst(InstGen::add,InstGen::Reg(target_reg,false),
                          InstGen::Reg(target_reg,false), InstGen::Constant(offset,false));
      }
      array_ty = static_cast<ArrayType*>(array_ty)->getElementType();
    }
    std::pair<int, bool> op_pos = getValuePosition(inst,operands[0]);
    int op_reg = op_pos.second ? op_pos.first : allocaed[0];
    if (!op_pos.second)
      asm_code += InstGen::load(InstGen::Reg(op_reg,false),
                      InstGen::Addr(InstGen::sp,op_pos.first));

    asm_code += InstGen::add(InstGen::Reg(target_reg,false),InstGen::Reg(target_reg,false),
                    InstGen::Reg(op_reg,false));
    if (!inst_pos.second) { // 若结果值应保存回栈中
      asm_code += InstGen::store(InstGen::Reg(target_reg,false),
                                InstGen::Addr(InstGen::sp,inst_pos.first));
      asm_code += popValue(inst,target_reg);
    }
    // 将申请的寄存器恢复
    for (int i = 0; i < 2; i++) { // 分配寄存器供下面使用
      asm_code += popValue(inst,allocaed[i]);
    }
  } else if (
      inst->isAdd() || inst->isSub() || inst->isMul() ||
      inst->isDiv() || inst->isRem() || inst->isCmp() ||
      inst->isZext()) {
      int target = getRegIndexOfValue(inst,inst);
      int op0 = getRegIndexOfValue(inst,operands[0]);
      bool is_fp = inst->getType()->isFloatTy() || operands[0]->getType()->isFloatTy();
      int op1 = is_fp ? getRegIndexOfValue(inst,operands[1]) : -1;
      int tmp; // 作为向浮点寄存器传值的中介

      if (inst->isAdd() || inst->isSub() || inst->isDiv() || inst->isCmp()) {
        bool need_reg = is_fp && operands[1]->isConstant();
        if (!is_fp && operands[1]->isConstant()){
          if (inst->isAdd()) {
            asm_code += InstGen::add(InstGen::Reg(target,false),InstGen::Reg(op0,false),
                          InstGen::Constant(atoi((operands[1])->getPrintName().c_str()),false));
          } else if (inst->isSub()){
            asm_code += InstGen::sub(InstGen::Reg(target,false),InstGen::Reg(op0,false),
                          InstGen::Constant(atoi((operands[1])->getPrintName().c_str()),false));
          } else if (inst->isDiv()) {
            asm_code += InstGen::divConst(InstGen::Reg(target,false),InstGen::Reg(op0,false),
                          InstGen::Constant(atoi((operands[1])->getPrintName().c_str()),false));
          } else if (inst->isCmp()) {
            asm_code += InstGen::instConst(InstGen::cmp, InstGen::Reg(op0,false),
                          InstGen::Constant(atoi((operands[1])->getPrintName().c_str()),false));
          }
        } else {
          if (need_reg) { // 默认浮点的值有没有存到对应寄存器？
            tmp = acquireForReg(inst,str);
            asm_code += str;
            str = "";
            asm_code += InstGen::setValue(InstGen::Reg(tmp,false),
                  InstGen::Constant(float2int(dynamic_cast<ConstantFloat *>(operands[1])),false));
            asm_code += InstGen::mov(InstGen::Reg(op1,is_fp),InstGen::Reg(tmp,false));
          }
          if (inst->isAdd()) {
            asm_code += InstGen::add(InstGen::Reg(target,is_fp),InstGen::Reg(op0,is_fp),InstGen::Reg(op1,is_fp));
          } else if (inst->isSub()){
            asm_code += InstGen::sub(InstGen::Reg(target,is_fp),InstGen::Reg(op0,is_fp),InstGen::Reg(op1,is_fp));
          } else if (inst->isDiv()) {
            if (is_fp)
              asm_code += InstGen::sdiv(InstGen::Reg(target,is_fp),InstGen::Reg(op0,is_fp),InstGen::Reg(op1,is_fp));
            else
              asm_code += generateFunctionCall(inst,operands,"__aeabi_idiv", 0);
          } else if (inst->isCmp()) { // 注意命令
            asm_code += InstGen::cmp(InstGen::Reg(op0,is_fp),InstGen::Reg(op1,is_fp));
          }

          if (need_reg)
            asm_code += popValue(inst,tmp);
        }

        if (inst->isCmp()) {
          InstGen::CmpOp cmp_op = cmpConvert(static_cast<CmpInst *>(inst)->_cmp_op, false);
          // 将比较结果存入目标寄存器， 0 --> false, 1 --> true;
          asm_code += InstGen::mov(InstGen::Reg(target,false),InstGen::Constant(0,false),InstGen::NOP);
          asm_code += InstGen::mov(InstGen::Reg(target,false),InstGen::Constant(1,false),cmp_op);
        }
      } else if (inst->isMul()) {
          asm_code += InstGen::mul(InstGen::Reg(target,is_fp),InstGen::Reg(op0,is_fp),InstGen::Reg(op1,is_fp));
      } else if (inst->isRem()) {
          asm_code += generateFunctionCall(inst,operands,"__aeabi_idivmod", 1);
      } else if (inst->isZext()) { //默认0号操作数已经在寄存器中
          asm_code += InstGen::mov(InstGen::Reg(target,false),InstGen::Reg(op0,false));
      }
  } else if (inst->isCast()) { //
    bool is_fp = inst->getType()->isFloatTy();
    if (is_fp)
      asm_code += generateFunctionCall(inst,operands,"__aeabi_i2f", 0); // sitofp
    else
      asm_code += generateFunctionCall(inst,operands,"__aeabi_f2iz", 0); // fptosi
  } else if (inst->isBr()) {
    if (operands.size() == 1) {
      asm_code += InstGen::b(InstGen::Label(getLabelName(inst->getParent(),operands[0]->getName())),
                            InstGen::NOP);
    } else if (operands.size() == 3) { 
      if (operands[0]->isConstant()) {
          if (atoi(operands[0]->getPrintName().c_str()) > 0 ) {
          asm_code += InstGen::b(InstGen::Label(getLabelName(inst->getParent(),operands[1]->getName())),
                                    InstGen::NOP);
          } else {
          asm_code += InstGen::b(InstGen::Label(getLabelName(inst->getParent(),operands[2]->getName())),
                                    InstGen::NOP);
          }
      } else {
        asm_code += InstGen::cmp(InstGen::Reg(getRegIndexOfValue(inst,operands[0]),false),
                                  InstGen::Constant(1,false));
        asm_code += InstGen::b(InstGen::Label(getLabelName(inst->getParent(),operands[1]->getName())),
                                  InstGen::EQ);
        asm_code += InstGen::b(InstGen::Label(getLabelName(inst->getParent(),operands[2]->getName())),
                                  InstGen::NOP);
      }
    } else {
      ERROR("TO many args in br instruction");
    }
  } else {
    ERROR("Unknown ir instrucion.");
  }
}

std::string AsmBuilder::generateFunctionCall(Instruction *inst, std::vector<Value *>args,
            std::string func_name, int return_reg) {
  std::string func_asm;
  auto func = inst->getFunction();
  int callee_saved_regs_size = 4*callee_saved_regs_size_mapping[func->getName().c_str()];
  int callee_stack_size = stack_size_mapping[func->getName().c_str()];
  auto saved_registers = getCallerSavedRegisters(func);

  std::pair<int, bool> inst_pos = getValuePosition(inst,inst);
  bool has_return_value = (return_reg >= 0) && (inst->getType()->getSize() > 0);
  if (has_return_value && inst_pos.second) {
    auto returned_reg = InstGen::Reg(inst_pos.first, inst->getType()->isFloatTy());
    decltype(saved_registers) new_save_registers;
    for (auto &reg : saved_registers) {
      if (reg.getID() != returned_reg.getID()) {
        new_save_registers.push_back(reg);
      }
    }
    saved_registers = new_save_registers;
  }
  saved_registers.push_back(InstGen::Reg(11,false)); // used as tmp reg
  std::sort(saved_registers.begin(), saved_registers.end());

  if (!saved_registers.empty()) {
    func_asm += InstGen::push(saved_registers);
  }
  func_asm += InstGen::comment(" call " + func_name, "");
  {
    // FOR args 0 - 3
    {
      for (int i = 0; i < std::min(args.size(), (size_t)4); i++) {
        bool is_fp = args[i]->getType()->isFloatTy();
        if (args[i]->isConstant()) {
          if (is_fp) {
            func_asm += InstGen::setValue(InstGen::Reg(11,false),InstGen::Constant(float2int(dynamic_cast<ConstantFloat *>(args[i])),false));
            // 对于类型转化abi函数，传参只用 r0
            func_asm += InstGen::mov(InstGen::Reg(i,!inst->isCast()),InstGen::Reg(11,false));
          } else {
            func_asm += InstGen::setValue(InstGen::Reg(i,false),InstGen::Constant(atoi(args[i]->getPrintName().c_str()),false));
          }
        } else {
          std::pair<int, bool> arg_pos = getValuePosition(inst,args[i]);
          if (arg_pos.second)// 参数在寄存器中
            func_asm += InstGen::mov(InstGen::Reg(i,is_fp&!inst->isCast()),InstGen::Reg(arg_pos.first,is_fp));
          else
            func_asm += InstGen::load(InstGen::Reg(11,false),
                  InstGen::Addr(InstGen::sp,arg_pos.first));
            func_asm += InstGen::mov(InstGen::Reg(i,is_fp&!inst->isCast()),InstGen::Reg(11,false));
        }
      }
    }
    // for args >= 4
    {
      int offset = 0;
      for (int i = 4; i < args.size(); i++) {
        bool is_fp = args[i]->getType()->isFloatTy();
        if (args[i]->isConstant()) {
          int int_value = is_fp ? float2int(dynamic_cast<ConstantFloat *>(args[i])) :
                    atoi((args[i])->getPrintName().c_str());
          func_asm += InstGen::setValue(InstGen::Reg(11,false),InstGen::Constant(int_value,false));
          func_asm += InstGen::store(InstGen::Reg(11,false),//这里 -4 不理解
                InstGen::Addr(InstGen::sp,offset-callee_saved_regs_size-4-callee_stack_size));
        } else {
          std::pair<int, bool> arg_pos = getValuePosition(inst,args[i]);
          if (arg_pos.second)// 参数在寄存器中
            func_asm += InstGen::store(InstGen::Reg(arg_pos.first,is_fp),
                InstGen::Addr(InstGen::sp,offset-callee_saved_regs_size-4-callee_stack_size));
          else // 将浮点数与整型数统一处理，有问题吗----可以注意 vload sx
            func_asm += InstGen::load(InstGen::Reg(11,false),
                  InstGen::Addr(InstGen::sp,arg_pos.first));
            func_asm += InstGen::store(InstGen::Reg(11,false),
                InstGen::Addr(InstGen::sp,offset-callee_saved_regs_size-4-callee_stack_size));
        }
        offset += 4;
      }
    }
  }
  func_asm += InstGen::bl(func_name);
  if (has_return_value) {
    bool is_fp = dynamic_cast<CallInst *>(inst)->getType()->isFloatTy();
    if (inst_pos.second) { // inst value in reg
      func_asm += InstGen::mov(InstGen::Reg(inst_pos.first,is_fp), InstGen::Reg(return_reg,is_fp));
    } else { // inst value in stack
      func_asm += InstGen::store(InstGen::Reg(return_reg,is_fp),
              InstGen::Addr(InstGen::sp,inst_pos.first+saved_registers.size()*4));
    }
  }
  if (!saved_registers.empty()) {
    asm_code += InstGen::pop(saved_registers);
  }

  return func_asm;
}

// std::string AsmBuilder::getFromSpecificReg(Instruction *inst, );
std::string AsmBuilder::assignToTargetReg(Instruction *inst, Value *val, int target, int offset) {
  std::string assign_code;
  auto val_int_const = dynamic_cast<ConstantInt *>(val);
  auto val_fp_const = dynamic_cast<ConstantFloat *>(val);
  auto val_global = dynamic_cast<GlobalVariable *>(val);
  if (val_int_const) {
    int imm = val_int_const->getValue();
    assign_code += InstGen::setValue(InstGen::Reg(target, false), InstGen::Constant(imm, false));
  } else if (val_fp_const) {
    float imm = float2int(val_fp_const);
    // notice
    assign_code += InstGen::setValue(InstGen::Reg(11, false), InstGen::Constant(imm, false));
    assign_code += InstGen::mov(InstGen::Reg(target,false), InstGen::Reg(11,false));
  } else if (val_global) {
    int label_reg = getRegIndexOfValue(inst,val,true);
    assign_code += InstGen::getAddress(InstGen::Reg(label_reg,false),
                      InstGen::Label(".LCPI_"+val->getName()));
    assign_code += InstGen::load(InstGen::Reg(getRegIndexOfValue(inst,val),val->getType()->isFloatTy()),
                      InstGen::Addr(InstGen::Reg(label_reg, false), 0));
  }

  return assign_code;
}

std::vector<InstGen::Reg> AsmBuilder::getCallerSavedRegisters(Function *func) {
  std::set<InstGen::Reg> registers;
  for (auto &reg : getAllRegisters(func)) {
    if (caller_save_regs.count(reg)) {
      registers.insert(reg);
    }
  }
  return std::vector<InstGen::Reg>(registers.begin(), registers.end());
}

std::vector<InstGen::Reg> AsmBuilder::getCalleeSavedRegisters(Function *func) {
  std::set<InstGen::Reg> registers;
  for (auto &reg : AsmBuilder::getAllRegisters(func)) {
    if (callee_save_regs.count(reg)) {
      registers.insert(reg);
    }
  }
  return std::vector<InstGen::Reg>(registers.begin(), registers.end());
}

// notice
std::vector<InstGen::Reg> AsmBuilder::getAllRegisters(Function *func) {
  std::set<InstGen::Reg> registers;

  std::pair<int, int> n = getUsedRegisterNum(func);// 返回整型寄存器、浮点寄存器各用多少个
  if (n.first > 13)
    registers.insert(InstGen::lr);
    n.first -= 1;
  for (int i = 0; i < n.first; i++ ) {
    registers.insert(InstGen::Reg(i,false));
  }
  for (int i = 0; i < n.second; i++ ) {
    registers.insert(InstGen::Reg(i,true));
  }

  return std::vector<InstGen::Reg>(registers.begin(), registers.end());
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