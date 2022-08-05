#ifndef COMPILER_DEAD_CODE_ELIMINATION_H
#define COMPILER_DEAD_CODE_ELIMINATION_H
#include <unordered_set>
#include "passes/pass.h"
class Module;
class Function;
class DeadCodeElimination : public pass{
   public:
    DeadCodeElimination(Module* m, const std::string& name) : pass(m, name) {}
    void run() override;

   private:
   void deleteDeadFunc();
   void detectSideEffectFunction();
   std::unordered_set<Function*> _has_side_effect_funcs;
};
#endif  // COMPILER_EMIT_IR_H