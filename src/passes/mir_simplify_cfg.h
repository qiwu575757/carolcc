#ifndef COMPILER_MIRSIMPLIFYCFG_H
#define COMPILER_MIRSIMPLIFYCFG_H

#include "ir/basic_block.h"
#include "ir/function.h"
#include "visitor/ir_builder.h"
#include "ir/instruction.h"
#include "module.h"
#include "pass_manager.h"

class MirSimplifyCFG : public Transform {
private:
  Function *_func;
  MirSimplifyCFG();

public:
  MirSimplifyCFG(Module *m, const std::string &name) : Transform(m,name) {}
  ~MirSimplifyCFG(){};
  void run() override;
  void RemoveNoPredecessorBB();
  void MergeSinglePredecessorBB();
  void EliminateSinglePredecessorPhi();
  void EliminateSingleUnCondBrBB();
  void RemoveSelfLoopBB();
};
#endif