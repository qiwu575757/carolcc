#ifndef COMPILER_MIRSIMPLIFYCFG_H
#define COMPILER_MIRSIMPLIFYCFG_H


#include "pass.h"
class Function;
class MirSimplifyCFG : public Transform {
private:
  Function *_func;
public:
  MirSimplifyCFG(Module *m, const std::string &name) : Transform(m,name),_func(nullptr) {}
  void run() override;
  void RemoveNoPredecessorBaseBlocks();
  void MergeSinglePredecessorBaseBlocks();
  void EliminateSinglePredecessorPhi();
  void EliminateSingleUnCondBrBaseBlocks();
  void RemoveSelfLoopBaseBlocks();
};
#endif