#ifndef COMPILER_HIRSIMPLIFYCFG_H
#define COMPILER_HIRSIMPLIFYCFG_H

#include "pass.h"
#include <vector>

class Function;
class BaseBlock;
class BasicBlock;
class WhileBlock;
class IfBlock;
class HighIRsimplyCFG : public Transform {
private:
  Function *_func;
  std::vector<BaseBlock *> _wait_delete;

public:
  HighIRsimplyCFG(Module *m, const std::string &name) : Transform(m,name) {}
  ~HighIRsimplyCFG() {}
  void run() override;
  void MergeSingleProcessBaseBlocks();
  bool MergeBasicBlock(BasicBlock *bb, BaseBlock *pre_bb);
  void MergeWhileBaseBlocks(WhileBlock *whilebb);
  void MergeIfBaseBlocks(IfBlock *ifbb);
};
#endif