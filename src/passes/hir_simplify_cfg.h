#ifndef COMPILER_HIRSIMPLIFYCFG_H
#define COMPILER_HIRSIMPLIFYCFG_H
#include "ir/basic_block.h"
#include "ir/function.h"
#include "visitor/ir_builder.h"
#include "ir/instruction.h"
#include "module.h"
#include "pass_manager.h"
#include "pass.h"

class HighIRsimplyCFG : public Transform {
private:
  Function *_func;
  std::vector<BaseBlock *> _wait_delete;

public:
  HighIRsimplyCFG(Module *m) : Transform(m) {}
  ~HighIRsimplyCFG() {}
  void run() override;
  void MergeSingleProcessBaseBlocks();
  bool MergeBasicBlock(BasicBlock *bb, BaseBlock *pre_bb);
  void MergeWhileBaseBlocks(WhileBlock *whilebb);
  void MergeIfBaseBlocks(IfBlock *ifbb);
};
