#ifndef COMPILER_LOWER_IR_H
#define COMPILER_LOWER_IR_H

#include "ir/function.h"
#include "pass.h"
class BasicBlock;

class LowerIR : public Transform {
public:
    LowerIR(Module *m, const std::string &name) : Transform(m,name) {}
    void run() override;
    void splitGEP(BasicBlock *bb);
    void convertRem2And(BasicBlock *bb);
    void splitRem(BasicBlock *bb);
    bool isPowerOfTwo(int x);
    void convertMlaLoad2LoadOffset(BasicBlock *bb);
    void deleteMla(BasicBlock *bb);
};

#endif