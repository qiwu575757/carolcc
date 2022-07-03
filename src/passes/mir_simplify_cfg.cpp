#include "mir_simplify_cfg.h"

void MirSimplifyCFG::run() {
  for (auto fun : _m->getFunctions()) {
    _func = fun;
    RemoveNoPredecessorBaseBlocks();
    EliminateSinglePredecessorPhi();
    MergeSinglePredecessorBaseBlocks();
    EliminateSingleUnCondBrBaseBlocks();
    RemoveSelfLoopBaseBlocks();
    RemoveNoPredecessorBaseBlocks();
    EliminateSinglePredecessorPhi();
  }
}

void MirSimplifyCFG::RemoveSelfLoopBaseBlocks() {
  std::vector<BasicBlock *> wait_delete;
  for (auto bb : _func->getBasicBlocks()) {
    if (bb->getPreBasicBlocks().size() == 1) {
      if (*(bb->getPreBasicBlocks().begin()) == bb) {
        wait_delete.push_back(bb);
      }
    }
  }
  for (auto bb : wait_delete) {
    _func->removeBasicBlock(bb);
  }
}