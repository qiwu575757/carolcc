#ifndef SRC_ASM_BUILDER_H
#define SRC_ASM_BUILDER_H

#include <linux/sched.h>
#include <sys/wait.h>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <list>

#include "ir/base_block.h"
#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/function.h"
#include "ir/global_variable.h"
#include "ir/instruction.h"
#include "asm/asm_instr.h"
#include "ir/user.h"
#include "ir/value.h"
#include "passes/module.h"
const int reg_num = 10;
const std::string global_vars_label = ".global_vars";
const int arch_version = 7;
const bool enlarge_stack = true;
const int enlarge_stack_size = 256 * (1 << 20); // 256MB

const std::vector<InstGen::Reg> caller_save_regs = {
    InstGen::Reg(0, false), InstGen::Reg(1, false),  InstGen::Reg(2, false),
    InstGen::Reg(3, false), InstGen::Reg(12, false), InstGen::Reg(14, false),
    InstGen::Reg(0, true),  InstGen::Reg(1, true),   InstGen::Reg(2, true),
    InstGen::Reg(3, true),  InstGen::Reg(4, true),   InstGen::Reg(5, true),
    InstGen::Reg(6, true),  InstGen::Reg(7, true),   InstGen::Reg(8, true),
    InstGen::Reg(9, true),  InstGen::Reg(10, true),  InstGen::Reg(11, true),
    InstGen::Reg(12, true), InstGen::Reg(13, true), InstGen::Reg(14, true),
    InstGen::Reg(15, true)};
const std::vector<InstGen::Reg> callee_save_regs = {
    InstGen::Reg(4, false), InstGen::Reg(5, false), InstGen::Reg(6, false),  InstGen::Reg(7, false),
    InstGen::Reg(8, false), InstGen::Reg(9, false), InstGen::Reg(10, false), InstGen::Reg(11, false),
    InstGen::Reg(16, true), InstGen::Reg(17, true), InstGen::Reg(18, true),  InstGen::Reg(19, true),
    InstGen::Reg(20, true), InstGen::Reg(21, true), InstGen::Reg(22, true),  InstGen::Reg(23, true),
    InstGen::Reg(24, true), InstGen::Reg(25, true), InstGen::Reg(26, true),  InstGen::Reg(27, true),
    InstGen::Reg(28, true), InstGen::Reg(29, true), InstGen::Reg(30, true), InstGen::Reg(31, true)
    };

// notice
const std::vector<InstGen::Reg> allocate_regs = {
    InstGen::Reg(0, false), InstGen::Reg(1, false), InstGen::Reg(2, false), InstGen::Reg(3, false),
    InstGen::Reg(4, false), InstGen::Reg(5, false), InstGen::Reg(6, false), InstGen::Reg(7, false),
    InstGen::Reg(8, false), InstGen::Reg(9, false), InstGen::Reg(10, false), InstGen::Reg(11, false),
    InstGen::Reg(12, false), InstGen::Reg(14, false) };
const std::vector<InstGen::Reg> extended_regs = { InstGen::fp, InstGen::lr };

// 通过系统函数汇编推测出的其用到的浮点寄存器
const std::vector<InstGen::Reg> sysfunc_used_fpregs = {
      InstGen::Reg(0, true), InstGen::Reg(2, true),
      InstGen::Reg(14, true), InstGen::Reg(15, true)
    };


const int cache_line_bits = 7;
const int cache_line_size = 1 << cache_line_bits;
const int mt_num_threads = 4;

const int L1_cache_size = 32 * (1 << 10); // 32KiB per core
const int L2_cache_size = 1 * (1 << 20);  // 1MB shared

const int thread_id_reg = 13;
const int op_save_stack_num = 8;
const int clone_flag = CLONE_VM | SIGCHLD;

const int int_reg_number = 13;
const int float_reg_number = 32;

const int virtual_reg_max = 1200;

enum interval_value_type{
  local_var,
  global_var,
  imm_var,
  arg_var,
  spill_var,
};

struct interval{
    int st_id;
    int ed_id;
    bool def=false;
    bool spilled;
    bool is_allocated=false;
    bool is_data=false;
    std::set<int>use_id;
    Value *v;
    float use_freq;
    float weight;
    interval_value_type type;
    int specific_reg_idx;
    int offset;
    bool is_float = false;
    std::vector<std::pair<int,int>> use_def_itv; // 用于将区间进行分割，可以是在过长区间未使用时进行 中间加mov，也可能是重新def导致（目前ssa应该不会）
};

class reg_map{
  public:
  std::vector<interval> virtual_int_regs[virtual_reg_max];//虚拟寄存器
  std::vector<interval> virtual_float_regs[virtual_reg_max];//虚拟寄存器
  std::vector<interval> stack_map;//args spill alloc return
  std::set<int> virtual_int_reg_use[virtual_reg_max];// 每个整型寄存器的使用点
  std::set<int> virtual_float_reg_use[virtual_reg_max];// 每个浮点寄存器的使用点 冲突识别
  std::map<Value *,int > linear_map;//指令列表
  std::map<std::string,std::pair<int,int>> func_used_reg_map;//函数使用的寄存器数
  int op_save[4];// 栈溢出时的保存寄存器
  int stack_size;
  int return_offset; // 注意维护
};

class AsmBuilder {
private:
  std::shared_ptr<Module> module;
  std::map<std::string, int> stack_size_mapping;// func_name - stack size
  std::map<std::string, int> callee_saved_regs_size_mapping;// func_name - callee saved reg size
  std::map<std::string, Function*> name_func_mapping;// func_name - func
  int thread_stack_bits;
  int thread_stack_size;
  std::set<Value *> allocated;
  std::map<Instruction *, std::set<Value *>> context_active_vars;
  std::string getType(interval_value_type t);
 // int stack_size;
  //int return_offset; // 注意维护
  bool debug;
  std::map<std::string, reg_map> func_reg_map;
  std::string cur_func_name;
  // std::vector<interval> virtual_int_regs[virtual_reg_max];//虚拟寄存器
  // std::vector<interval> virtual_float_regs[virtual_reg_max];//虚拟寄存器
  // std::vector<interval> stack_map;//args spill alloc return
  // std::set<int> virtual_int_reg_use[virtual_reg_max];// 每个整型寄存器的使用点
  // std::set<int> virtual_float_reg_use[virtual_reg_max];// 每个浮点寄存器的使用点 冲突识别
  // std::map<Value *,int > linear_map;//指令列表
  // std::map<std::string,std::pair<int,int>> func_used_reg_map;//函数使用的寄存器数
  // int op_save[4];// 栈溢出时的保存寄存器


public:
  AsmBuilder(std::shared_ptr<Module> module, bool debug = false) {
    this->module = module;
    this->debug = debug;
  }
  ~AsmBuilder() {}
  int find_register(Value *v);
  int find_register(Value *v,std::string &code);
  void set_register(Value *v,int data,bool init);
  std::string generate_asm(std::map<Value *, int> register_mapping);
  std::string generate_asm(std::string file_name);
  std::string generate_module_header(std::string file_name);
  std::string generate_module_tail();
  std::string generate_function_code(Function *func);
  std::string generate_function_entry_code(Function *func);
  std::string generate_function_exit_code(Function *func);

  std::string generateBasicBlockCode(BasicBlock *bb);
  std::string getLabelName(BasicBlock *bb);
  std::string getLabelName(Function *func, int type);
  std::string getLabelName(BasicBlock *bb, std::string id);
  std::string generate_global_vars();
  std::string generate_use_of_global_vars();
  std::string generate_initializer(Constant *init);
  std::pair<int, bool> get_const_val(Value *val);
  std::string generateInstructionCode(Instruction *inst);
  std::string generateOperInst (Instruction *inst);
  std::string generateFunctionCall(Instruction *inst, std::vector<Value *> args,
          std::string func_name, int return_reg);

  // void allocaStackSpace(Function *func);

  // 浮点运算函数
  int float2int(ConstantFloat *val);
  // LSRA
  void live_interval_analysis(Function *fun,bool insert = false);
  void linear_scan_reg_alloc(std::vector<interval> live_range,Function *func, bool insert = false);
  bool value_in_reg(Value *v);
  bool force_reg_alloc(interval itv,int reg_idx);
  bool op_in_inst_is_spilled(Value *inst,Value *op);
  int give_reg_at(Value *inst); // 请求分配寄存器std::pair<int, bool> askForReg(Instruction *inst);
  int give_used_reg_at(Value *inst);
  Value * value_in_reg_at(Value *inst,int reg_idx,bool is_fp);
  int get_value_sp_offset(Value *inst,Value *op);// 查看变量在栈上的偏移

  std::vector<InstGen::Reg> getCalleeSavedRegisters(Function *func);
  std::vector<InstGen::Reg> getSysFuncCallerSavedRegisters(std::string func_name);
  std::vector<InstGen::Reg> getCallerSavedRegisters(Function *func);
  std::vector<InstGen::Reg> getAllRegisters(Function *func);
  std::pair<int, int> getUsedRegisterNum(Function *func);
  std::string assignToTargetReg(Instruction *inst, Value *val, int target, int offset = 0);
  int getAllocaSpOffset(Value *inst);
  int acquireForReg(Value *inst, int val_pos, std::string &str); // 外部函数
  std::string popValue(Value *inst, int reg_idx, int val_pos); // 外部函数
  // 一般指令(除call/gep)无论该值在栈中还是寄存器中
  int getRegIndexOfValue(Value *inst, Value *val, bool global_label = false); // 外部函数
  //获得函数调用返回值变量的位置，int - reg_index/sp off, bool true - in reg/stack
  std::pair<int, bool> getValuePosition(Value *inst, Value *val); // 外部函数
  InstGen::CmpOp cmpConvert(CmpInst::CmpOp myCmpOp, bool reverse);
  std::string getGlobalValAddress(int op_reg, Value *val);
  std::string passFunctionArgs(Instruction *inst,std::vector<Value *>args,
          std::string func_name,std::vector<InstGen::Reg> saved_registers);


};

const InstGen::Reg temp_reg = InstGen::Reg(11, false);
#endif // SRC_ASM_BUILDER_H;