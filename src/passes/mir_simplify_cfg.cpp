#include "mir_simplify_cfg.h"
#include "ir/basic_block.h"
#include "ir/function.h"
#include "visitor/ir_builder.h"
#include "ir/instruction.h"
#include "module.h"
#include "pass_manager.h"

void MirSimplifyCFG::run() {
  for (auto fun : _m->getFunctions()) {
      if(!fun->isBuiltin()){
          _func = fun;
          RemoveNoPredecessorBaseBlocks();
          // EliminateSinglePredecessorPhi();
          //    MergeSinglePredecessorBaseBlocks();
          //    EliminateSingleUnCondBrBaseBlocks();
          //    RemoveSelfLoopBaseBlocks();
          //    RemoveNoPredecessorBaseBlocks();
          // EliminateSinglePredecessorPhi();
      }
  }
}

void MirSimplifyCFG::RemoveSelfLoopBaseBlocks() {
  for (auto bb : _func->getBasicBlocks()) {
    if (bb->getPreBasicBlocks().size() == 1) {
      if (*(bb->getPreBasicBlocks().begin()) == bb) {
        _func->removeBasicBlock(bb);
      }
    }
  }
}

void MirSimplifyCFG::RemoveNoPredecessorBaseBlocks() {
    std::vector<BasicBlock*> wait_delete;
  for (auto bb : _func->getBasicBlocks()) {
    if (bb->getPreBasicBlocks().empty() && bb != _func->getEntryBlock()) {
      wait_delete.push_back(bb);
      for (auto next_bb : bb->getSuccBasicBlocks()) {
        next_bb->removePreBasicBlock(bb);
         for (auto instr : next_bb->getInstructions()) {
           if (instr->isPhi()) {
             for (int i = 0; i < instr->getOperandNumber(); i++) {
               if (i % 2 == 1) {
                 if (instr->getOperand(i) == bb) {
                   instr->rmOperand(i - 1, i);
                 }
               }
             }
           }
         }
      }
    }
  }
  for(auto bb:wait_delete){
      _func->removeBasicBlock(bb);
  }
}

void MirSimplifyCFG::MergeSinglePredecessorBaseBlocks() {
//  for (auto bb : _func->getBasicBlocks()) {
//    if (bb->getPreBasicBlocks().size() == 1) {
//      auto pre_bb = bb->getPreBasicBlocks().begin();
//      auto br = (*pre_bb)->getTerminator();
//      // exit_ifnot(_MergeSinglePredecessorBB_SimplifyCFG, br->isBr());
//      if (br->getOperandNumber() == 1) {
//        (*pre_bb)->deleteInstr(br);
//        // merge
//        for (auto instr : bb->getInstructions()) {
//          instr->setParent(*pre_bb);
//          (*pre_bb)->addInstruction(instr);
//        }
//        (*pre_bb)->clearSuccBasicBlock();
//        for (auto next_bb : bb->getSuccBasicBlocks()) {
//          (*pre_bb)->addSuccBasicBlock(next_bb);
//          next_bb->getPreBasicBlocks().remove(bb);
//          next_bb->addPreBasicBlock(*pre_bb);
//        }
//        bb->replaceAllUseWith(*pre_bb);
//        _func->removeBasicBlock(bb);
//        // bb->replaceAllUseWith(*pre_bb);
//      }
//    }
//  }
}

// void MirSimplifyCFG::EliminateSinglePredecessorPhi() {
//   for (auto bb : _func->getBasicBlocks()) {
//     if (bb->getPreBasicBlocks().size() == 1 || bb == _func->getEntryBlock()) {
//       std::vector<Instruction *> wait_delete;
//       for (auto instr : bb->getInstructions()) {
//         if (instr->isPhi()) {
//           if (instr->getOperand(1) != *(bb->getPreBasicBlocks().begin()) &&
//               bb != _func->getEntryBlock()) {
//             std::cerr << "error in EliminateSinglePredecessorPhi" << std::endl;
//             std::cerr << instr->getOperand(1)->getName() << "  #"
//                       << (*(bb->getPreBasicBlocks().begin()))->getName()
//                       << std::endl;
//             exit(_EliminateSinglePredecessorPhi_SimplifyCFG);
//           }
//           // instr->replaceAllUseWith(instr->getOperand(0));
//           for (auto use : instr->getUseList()) {
//             static_cast<User *>(use.val_)->setOperand(use.arg_no_,
//                                                       instr->getOperand(0));
//             // bb->removeUse(use.val_);
//           }

//           wait_delete.push_back(instr);
//         }
//       }
//       for (auto instr : wait_delete) {
//         bb->deleteInstr(instr);
//       }
//     }
//   }
// }

void MirSimplifyCFG::EliminateSingleUnCondBrBaseBlocks() {
//  for (auto bb : _func->getBasicBlocks()) {
//    if (bb->getNumOfInstr() == 1 && bb != _func->getEntryBlock()) {
//      auto terminator = bb->getTerminator();
//      auto next_bb = bb->getSuccBasicBlocks().begin();
//      exit_ifnot(_EliminateSingleUnCondBrBB_SimplifyCFG, terminator);
//      if (terminator->isBr() && terminator->getNumOperand() == 1) {
//        bool bb_delete = true;
//        for (auto pre_bb : bb->getPreBasicBlocks()) {
//          auto br = static_cast<BranchInst *>(pre_bb->getTerminator());
//          if (br->isCondBr()) {
//            auto target1 = br->getOperand(1);
//            auto target2 = br->getOperand(2);
//            auto target3 = static_cast<BranchInst *>(terminator)->getOperand(0);
//            if ((target1 == bb && target2 == target3) ||
//                (target2 == bb && target1 == target3)) {
//              bb_delete = false;
//              break;
//            }
//          } else if (br->isCmpBr()) {
//            auto target1 = br->getOperand(2);
//            auto target2 = br->getOperand(3);
//            auto target3 = static_cast<BranchInst *>(terminator)->getOperand(0);
//            if ((target1 == bb && target2 == target3) ||
//                (target2 == bb && target1 == target3)) {
//              bb_delete = false;
//              break;
//            }
//          }
//        }
//        if (bb_delete) {
//          // ssa格式
//          for (auto instr : (*next_bb)->getInstructions()) {
//            if (instr->isPhi()) {
//              for (int i = 0; i < instr->getNumOperand(); i++) {
//                if (i % 2 == 1) {
//                  if (instr->getOperand(i) == bb) {
//                    auto idx = 0;
//                    auto val = instr->getOperand(i - 1);
//                    for (auto pre_bb : bb->getPreBasicBlocks()) {
//                      if (idx == 0) {
//                        instr->setOperand(i, pre_bb);
//                        bb->removeUse(instr, i);
//                      } else {
//                        instr->addOperand(val);
//                        instr->addOperand(pre_bb);
//                      }
//                      idx++;
//                    }
//                  }
//                }
//              }
//            }
//          }
//          // ssa格式
//          for (auto use : bb->getUseList()) {
//            auto instr = dynamic_cast<Instruction *>(use.val_);
//            if (instr) {
//              if (instr->isBr()) {
//                static_cast<User *>(use.val_)->setOperand(use.arg_no_, *next_bb);
//              }
//            }
//          }
//          (*next_bb)->removePreBasicBlock(bb);
//          for (auto pre_bb : bb->getPreBasicBlocks()) {
//            pre_bb->removeSuccBasicBlock(bb);
//            pre_bb->addSuccBasicBlock(*next_bb);
//            (*next_bb)->addPreBasicBlock(pre_bb);
//          }
//          _func->removeBasicBlock(bb);
//        }
//      }
//    }
//  }
}
void MirSimplifyCFG::EliminateSinglePredecessorPhi() { ERROR("todo");}
