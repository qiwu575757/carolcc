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

const std::set<InstGen::Reg> caller_save_regs = {
    InstGen::Reg(0, false), InstGen::Reg(1, false),  InstGen::Reg(2, false),
    InstGen::Reg(3, false), InstGen::Reg(12, false), InstGen::Reg(14, false),
    InstGen::Reg(0, true),  InstGen::Reg(1, true),   InstGen::Reg(2, true),
    InstGen::Reg(3, true),  InstGen::Reg(4, true),   InstGen::Reg(5, true),
    InstGen::Reg(6, true),  InstGen::Reg(7, true),   InstGen::Reg(8, true),
    InstGen::Reg(9, true),  InstGen::Reg(10, true),  InstGen::Reg(11, true),
    InstGen::Reg(12, true), InstGen::Reg(13, true), InstGen::Reg(14, true),
    InstGen::Reg(15, true)};
const std::set<InstGen::Reg> callee_save_regs = {
    InstGen::Reg(4, false), InstGen::Reg(5, false), InstGen::Reg(6, false),  InstGen::Reg(7, false),
    InstGen::Reg(8, false), InstGen::Reg(9, false), InstGen::Reg(10, false), InstGen::Reg(11, false),
    InstGen::Reg(16, true), InstGen::Reg(17, true), InstGen::Reg(18, true),  InstGen::Reg(19, true),
    InstGen::Reg(20, true), InstGen::Reg(21, true), InstGen::Reg(22, true),  InstGen::Reg(23, true),
    InstGen::Reg(24, true), InstGen::Reg(25, true), InstGen::Reg(26, true),  InstGen::Reg(27, true),
    InstGen::Reg(28, true), InstGen::Reg(29, true), InstGen::Reg(30, true), InstGen::Reg(31, true)
    };

// notice
const std::set<InstGen::Reg> allocate_regs = {
    InstGen::Reg(0, false), InstGen::Reg(1, false), InstGen::Reg(2, false), InstGen::Reg(3, false),
    InstGen::Reg(4, false), InstGen::Reg(5, false), InstGen::Reg(6, false), InstGen::Reg(7, false),
    InstGen::Reg(8, false), InstGen::Reg(9, false), InstGen::Reg(10, false), InstGen::Reg(11, false),
    InstGen::Reg(12, false), InstGen::Reg(14, false) };
const std::set<InstGen::Reg> extended_regs = { InstGen::fp, InstGen::lr };

const int cache_line_bits = 7;
const int cache_line_size = 1 << cache_line_bits;
const int mt_num_threads = 4;

const int L1_cache_size = 32 * (1 << 10); // 32KiB per core
const int L2_cache_size = 1 * (1 << 20);  // 1MB shared

const int thread_id_reg = 10;

const int clone_flag = CLONE_VM | SIGCHLD;

const int int_reg_number = 14;
const int float_reg_number = 32;

enum interval_value_type{
  int_local_var,
  int_global_var,
  int_global_var_label,
  int_imm_var,
  int_arg_var,
  int_spill_var,
};

typedef struct interval{
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
};

class AsmBuilder {
private:
  std::shared_ptr<Module> module;
  std::map<std::string, int> stack_size_mapping;// func_name - stack size
  std::map<std::string, int> callee_saved_regs_size_mapping;// func_name - callee saved reg size
  int stack_cur_size = 0;
  int thread_stack_bits;
  int thread_stack_size;
  std::set<Value *> allocated;
  std::map<Instruction *, std::set<Value *>> context_active_vars;
  // int stack_size = 65536;//131072 32768 16384 根据需要，可扩充 64k（对于87号测例，需要递归开辟栈空间，若使用128k,qemu跑会崩）
  // int stack_size = 65536 - 100;//根据需要，可扩充 128k, 要求函数调用前栈保持16字节对齐
  int stack_size;
  int return_offset; // 注意维护
  bool debug;

  std::vector<interval> virtual_int_regs[500];//虚拟寄存器
  std::vector<interval> stack_map;//args spill alloc return
  std::set<int> virtual_int_reg_use[500];// 每个寄存器的使用点
  std::map<Value *,int > linear_map;//指令列表
  int op_save[4];// 栈溢出时的保存寄存器

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
  std::vector<interval> live_interval_analysis(Function *func);
  void linear_scan_reg_alloc(std::vector<interval> live_range,Function *func);
  bool value_in_reg(Value *v);
  bool force_reg_alloc(interval itv,int reg_idx);
  bool op_in_inst_is_spilled(Value *inst,Value *op);
  int give_int_reg_at(Value *inst); // 请求分配寄存器std::pair<int, bool> askForReg(Instruction *inst);
  Value * value_in_int_reg_at(Value *inst,int reg_idx);
  int get_int_value_sp_offset(Value *inst,Value *op);// 查看变量在栈上的偏移

  std::vector<InstGen::Reg> getCalleeSavedRegisters(Function *func);
  std::vector<InstGen::Reg> getCallerSavedRegisters(Function *func);
  std::vector<InstGen::Reg> getAllRegisters(Function *func);
  std::pair<int, int> getUsedRegisterNum(Function *func);
  std::string assignToTargetReg(Instruction *inst, Value *val, int target, int offset = 0);
  int getAllocaSpOffset(Value *inst);
  int acquireForReg(Value *inst, int val_pos, std::string &str);
  std::string popValue(Value *inst, int reg_idx, int val_pos);
  // 一般指令(除call/gep)无论该值在栈中还是寄存器中
  int getRegIndexOfValue(Value *inst, Value *val, bool global_label = false);
  //获得函数调用返回值变量的位置，int - reg_index/sp off, bool true - in reg/stack
  std::pair<int, bool> getValuePosition(Value *inst, Value *val);
  InstGen::CmpOp cmpConvert(CmpInst::CmpOp myCmpOp, bool reverse);


};

#endif // SRC_ASM_BUILDER_H;