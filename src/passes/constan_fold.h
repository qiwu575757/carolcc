
#ifndef COMPILER_CONSTAN_FOLD_H
#define COMPILER_CONSTAN_FOLD_H
#include "ir/constant.h"
#include "ir/instruction.h"
#include "pass.h"

class Function;
class BinaryInst;
class ConstantFold : public Transform {
   public:
    ConstantFold(Module* m,const std::string &name): Transform(m,name){}
    void run() override;
    void constantFold(Function* f);
    // 删除条件已知的br指令
    void deleteCondBr(Function*f);
    void eliminateSinglePredecessorPhi(Function* f);
    Constant* calConstantIntBinary(Instruction* pInstruction);
    Constant* calConstantFloatBinary(Instruction* instr);

   private:
//    Function* _cur_fun;
    bool detectBinaryConstantFold(BinaryInst* inst);
    bool detectCastConstantFold(UnaryInst* pInst);
    bool detectCmpConstantFold(CmpInst* pInst);
};

#endif  // COMPILER_CONSTAN_FOLD_H
