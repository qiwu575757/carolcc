#include "hir_simplify_cfg.h"

void HighIRsimplyCFG::run() {
  for (auto fun : _m->getFunctions()) {
    _func = fun;
    MergeSingleProcessBaseBlocks();
  }
}

void HighIRsimplyCFG::MergeSingleProcessBaseBlocks() {
  for (auto basebb = _func->getBaseBlocks().begin();
       basebb != _func->getBaseBlocks().end();) {
    if ((*basebb)->isBaiscBlock()) {
      auto next_bb = ++basebb;
      basebb--;
      if (next_bb != _func->getBaseBlocks().end() &&
          MergeBasicBlock(dynamic_cast<BasicBlock *>(*basebb), *next_bb)) {
        _func->removeBaseBlock(*next_bb);
      } else {
        basebb++;
      }
    } else if ((*basebb)->isIfBlock()) {
      MergeIfBaseBlocks(dynamic_cast<IfBlock *>((*basebb)));
      basebb++;
    } else if ((*basebb)->isWhileBlock()) {
      MergeWhileBaseBlocks(dynamic_cast<WhileBlock *>((*basebb)));
      basebb++;
    }
  }
}

void HighIRsimplyCFG::MergeWhileBaseBlocks(WhileBlock *whilebb) {
  auto list1 = whilebb->getCondBaseBlockList();
  for (auto basebb = list1->begin(); basebb != list1->end();) {
    if ((*basebb)->isBaiscBlock()) {
      auto next_bb = ++basebb;
      basebb--;
      if (next_bb != list1->end() &&
          MergeBasicBlock(dynamic_cast<BasicBlock *>(*basebb), *next_bb)) {
        whilebb->removeCondBaseBlock(*next_bb);
      } else {
        basebb++;
      }
    } else if ((*basebb)->isIfBlock()) {
      MergeIfBaseBlocks(dynamic_cast<IfBlock *>((*basebb)));
      basebb++;
    } else if ((*basebb)->isWhileBlock()) {
      MergeWhileBaseBlocks(dynamic_cast<WhileBlock *>((*basebb)));
      basebb++;
    }
  }
  auto list2 = whilebb->getBodyBaseBlockList();
  for (auto basebb = list2->begin(); basebb != list2->end();) {
    if ((*basebb)->isBaiscBlock()) {
      auto next_bb = ++basebb;
      basebb--;
      if (next_bb != list2->end() &&
          MergeBasicBlock(dynamic_cast<BasicBlock *>(*basebb), *next_bb)) {
        whilebb->removeWhileBodyBaseBlock(*next_bb);
      } else {
        basebb++;
      }
    } else if ((*basebb)->isIfBlock()) {
      MergeIfBaseBlocks(dynamic_cast<IfBlock *>((*basebb)));
      basebb++;
    } else if ((*basebb)->isWhileBlock()) {
      MergeWhileBaseBlocks(dynamic_cast<WhileBlock *>((*basebb)));
      basebb++;
    }
  }
}

void HighIRsimplyCFG::MergeIfBaseBlocks(IfBlock *ifbb) {
  auto list1 = ifbb->getCondBaseBlockList();
  for (auto basebb = list1->begin(); basebb != list1->end();) {
    if ((*basebb)->isBaiscBlock()) {
      auto next_bb = ++basebb;
      basebb--;
      if (next_bb != list1->end() &&
          MergeBasicBlock(dynamic_cast<BasicBlock *>(*basebb), *next_bb)) {
        ifbb->removeCondBaseBlock(*next_bb);
      } else {
        basebb++;
      }
    } else if ((*basebb)->isIfBlock()) {
      MergeIfBaseBlocks(dynamic_cast<IfBlock *>((*basebb)));
      basebb++;
    } else if ((*basebb)->isWhileBlock()) {
      MergeWhileBaseBlocks(dynamic_cast<WhileBlock *>((*basebb)));
      basebb++;
    }
  }

  auto list2 = ifbb->getIfBodyBaseBlockList();
  for (auto basebb = list2->begin(); basebb != list2->end();) {
    if ((*basebb)->isBaiscBlock()) {
      auto next_bb = ++basebb;
      basebb--;
      if (next_bb != list2->end() &&
          MergeBasicBlock(dynamic_cast<BasicBlock *>(*basebb), *next_bb)) {
        ifbb->removeIfBodyBaseBlock(*next_bb);
      } else {
        basebb++;
      }
    } else if ((*basebb)->isIfBlock()) {
      MergeIfBaseBlocks(dynamic_cast<IfBlock *>((*basebb)));
      basebb++;
    } else if ((*basebb)->isWhileBlock()) {
      MergeWhileBaseBlocks(dynamic_cast<WhileBlock *>((*basebb)));
      basebb++;
    }
  }

  auto list3 = ifbb->getElseBodyBaseBlockList();
  for (auto basebb = list3->begin(); basebb != list3->end();) {
    if ((*basebb)->isBaiscBlock()) {
      auto next_bb = ++basebb;
      basebb--;
      if (next_bb != list3->end() &&
          MergeBasicBlock(dynamic_cast<BasicBlock *>(*basebb), *next_bb)) {
        ifbb->removeElseBodyBaseBlock(*next_bb);
      } else {
        basebb++;
      }
    } else if ((*basebb)->isIfBlock()) {
      MergeIfBaseBlocks(dynamic_cast<IfBlock *>((*basebb)));
      basebb++;
    } else if ((*basebb)->isWhileBlock()) {
      MergeWhileBaseBlocks(dynamic_cast<WhileBlock *>((*basebb)));
      basebb++;
    }
  }
}
bool HighIRsimplyCFG::MergeBasicBlock(BasicBlock *bb, BaseBlock *nextbb) {
  if (nextbb == nullptr) {
    return false;
  } else if (nextbb->isWhileBlock()) {
    return false;
  } else if (nextbb->isIfBlock()) {
    return false;
  } else if (nextbb->isBaiscBlock()) {
    for (auto instr : dynamic_cast<BasicBlock *>(nextbb)->getInstructions()) {
      bb->addInstr(instr);
    }
    return true;
  } else {
    return false;
  }
  return false;
}
