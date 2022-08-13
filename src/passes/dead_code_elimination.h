#ifndef COMPILER_DEAD_CODE_ELIMINATION_H
#define COMPILER_DEAD_CODE_ELIMINATION_H
#include <unordered_set>
#include "ir/instruction.h"
#include "passes/pass.h"
class Module;
class Function;
// #definE __DCE_DEBUG
class DeadCodeElimination : public pass{
   public:
    DeadCodeElimination(Module* m, const std::string& name) : pass(m, name) {}
    void run() override;

   private:
   void deleteDeadFunc();
   void deleteDeadRet(Function* func);
   void deleteDeadStore(Function * func);
   void deleteRedundantLoad(Function * func);
   void deleteDeadInstr(Function* func);
   bool hasSideEffect(Instruction * instr);
   bool isSamePtr(StoreInst * store,LoadInst* load);
   void markInstrUseful(Instruction* instr,std::unordered_set<Instruction*> &work_list);
   std::unordered_set<Function*> _has_side_effect_funcs;
};
#endif  // COMPILER_EMIT_IR_H
