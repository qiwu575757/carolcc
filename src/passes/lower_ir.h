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
    void rmPhi(Function* f);
};

#endif