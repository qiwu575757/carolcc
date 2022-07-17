#ifndef COMPILER_GLOBAL_VALUE_NUMBERING_H
#define COMPILER_GLOBAL_VALUE_NUMBERING_H

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
    Constant* calConstantIntBinary(Instruction* pInstruction);
    Constant* calConstantFloatBinary(Instruction* instr);

private:
//    Function* _cur_fun;
    bool detectBinaryConstantFold(BinaryInst* inst);
    bool detectCastConstantFold(UnaryInst* pInst);
    bool detectCmpConstantFold(CmpInst* pInst);
};

#endif //COMPILER_GLOBAL_VALUE_NUMBERING_H
