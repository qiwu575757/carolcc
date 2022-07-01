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

#include "ir/base_block.h"
#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/function.h"
#include "ir/global_variable.h"
#include "ir/instruction.h"
#include "asm/asm_instr.h"
#include "ir/user.h"
#include "ir/value.h"

const std::string global_vars_label = ".global_vars";
const int arch_version = 8;
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
  std::map<Value *, int> register_mapping;
  std::map<Value *, int> stack_mapping;
  std::set<Value *> allocated;
  std::map<Instruction *, std::set<Value *>> context_active_vars;
  int stack_size;
  bool debug;

public:
  AsmBuilder(std::shared_ptr<Module> module, bool debug = false) {
    this->module = module;
    this->debug = debug;
  }
  ~AsmBuilder() {}
  std::string generate_asm(std::map<Value *, int> register_mapping);
  std::string generate_asm();
};

#endif // SRC_ASM_BUILDER_H