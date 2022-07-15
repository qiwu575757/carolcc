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

const int op_reg_0 = 12;
const int op_reg_1 = 14; // lr
const int op_reg_2 = vinst_temp_reg.getID();

const std::set<InstGen::Reg> caller_save_regs = {
    InstGen::Reg(0), InstGen::Reg(1),  InstGen::Reg(2),
    InstGen::Reg(3), InstGen::Reg(12), InstGen::Reg(14)};
const std::set<InstGen::Reg> callee_save_regs = {
    InstGen::Reg(4), InstGen::Reg(5), InstGen::Reg(6),  InstGen::Reg(7),
    InstGen::Reg(8), InstGen::Reg(9), InstGen::Reg(10), InstGen::Reg(11)};
const std::set<InstGen::VFPReg> caller_save_vfpregs = {
    InstGen::VFPReg(0), InstGen::VFPReg(1), InstGen::VFPReg(2), InstGen::VFPReg(3),
    InstGen::VFPReg(4), InstGen::VFPReg(5), InstGen::VFPReg(6),  InstGen::VFPReg(7),
    InstGen::VFPReg(8), InstGen::VFPReg(9), InstGen::VFPReg(10), InstGen::VFPReg(11),
    InstGen::VFPReg(12), InstGen::VFPReg(13), InstGen::VFPReg(14), InstGen::VFPReg(15)
    };
const std::set<InstGen::VFPReg> callee_save_vfpregs = {
    InstGen::VFPReg(16), InstGen::VFPReg(17), InstGen::VFPReg(18), InstGen::VFPReg(19),
    InstGen::VFPReg(20), InstGen::VFPReg(21), InstGen::VFPReg(22), InstGen::VFPReg(23),
    InstGen::VFPReg(24), InstGen::VFPReg(25), InstGen::VFPReg(26), InstGen::VFPReg(27),
    InstGen::VFPReg(28), InstGen::VFPReg(29), InstGen::VFPReg(30), InstGen::VFPReg(31)
    };

const std::set<InstGen::Reg> allocate_regs = {
    InstGen::Reg(0), InstGen::Reg(1), InstGen::Reg(2), InstGen::Reg(3),
    InstGen::Reg(4), InstGen::Reg(5), InstGen::Reg(6), InstGen::Reg(7),
    InstGen::Reg(8), InstGen::Reg(9), InstGen::Reg(10)};
const std::set<InstGen::Reg> temp_regs = {
    InstGen::Reg(op_reg_0), InstGen::Reg(op_reg_1), InstGen::Reg(op_reg_2)};

const int cache_line_bits = 7;
const int cache_line_size = 1 << cache_line_bits;
const int mt_num_threads = 4;

const int L1_cache_size = 32 * (1 << 10); // 32KiB per core
const int L2_cache_size = 1 * (1 << 20);  // 1MB shared

const int thread_id_reg = 10;

const int clone_flag = CLONE_VM | SIGCHLD;

class AsmBuilder {
private:
  std::shared_ptr<Module> module;
  std::map<Value *, int> register_mapping; // value - id
  std::map<Value *, int> vfpregister_mapping; // float value - id
  std::map<Value *, int> stack_mapping; // value - offset
  int stack_cur_size=0;
  std::set<Value *> allocated;
  std::map<Instruction *, std::set<Value *>> context_active_vars;
  // int stack_size = 65536;//根据需要，可扩充 64k（对于87号测例，需要递归开辟栈空间，若使用128k,qemu跑会崩）
  int stack_size = 131072;//根据需要，可扩充 128k
  int return_offset = stack_size - 4;
  bool debug;
  std::string asm_code;

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
  std::string generate_function_exit_code();

  /*LRU list update interval by function code; insert ldr str instr*/
  std::string update_value_mapping(std::list<Value *> update_v);
  std::string update_arg_mapping(Value *arg);
  std::string erase_value_mapping(std::list<Value*>& erase_v);
  std::string flushRegs2Stack(std::list<Value *>flush_v);
  std::string generateBasicBlockCode(BasicBlock *bb);
  std::string getLabelName(BasicBlock *bb);
  std::string getLabelName(Function *func, int type);
  std::string getLabelName(BasicBlock *bb, std::string id);
  std::string generate_global_vars();
  std::string generate_use_of_global_vars();
  std::string generate_initializer(Constant *init);
  std::pair<int, bool> get_const_val(Value *val);
  std::string generateInstructionCode(Instruction *inst);
  void show_mapping();
  void erase_register_map(Value * v);
  std::string generateOperInst (Instruction *inst);
  std::string generateFunctionCall(Instruction *inst, std::vector<Value *> operands,
          std::string func_name, int return_reg);
  std::string generateCmpInst(Instruction *inst, Value *imm_0);
};

#endif // SRC_ASM_BUILDER_H