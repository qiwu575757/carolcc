
#ifndef COMPILER_CONSTAN_FOLD_H
#define COMPILER_CONSTAN_FOLD_H
#include "ir/instruction.h"
#include "pass.h"

class Function;
class BinaryInst;
class ConstantFold : public Transform {
   public:
    ConstantFold(Module* m,const std::string &name): Transform(m,name){}
    void run() override;
    void constantFold(Function* f);
    int calConstantIntBinary(BinaryInst* binary_instr);
    float calConstantFloatBinary(BinaryInst* binary_instr);

   private:
//    Function* _cur_fun;
    bool detectBinaryConstantFold(BinaryInst* binary_inst);
    bool detectCastConstantFold(UnaryInst* pInst);
};

#endif  // COMPILER_CONSTAN_FOLD_H
