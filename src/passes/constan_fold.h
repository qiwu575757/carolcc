
#ifndef COMPILER_CONSTAN_FOLD_H
#define COMPILER_CONSTAN_FOLD_H
#include "pass.h"

class Function;
class BinaryInst;
class ConstantFold : public Transform {
   public:
    ConstantFold(Module* m,const std::string &name): Transform(m,name){}
    void run() override;
    void constantFold(Function* f);
    int calConstantBinary(BinaryInst* binary_instr);

   private:
//    Function* _cur_fun;
};

#endif  // COMPILER_CONSTAN_FOLD_H
