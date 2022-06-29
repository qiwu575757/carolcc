#ifndef COMPILER_LOWERIR_H
#define COMPILER_LOWERIR_H

#include "pass.h"
class BasicBlock;

class LowerIR : public Transform {
public:
    LowerIR(Module *m, const std::string &name) : Transform(m,name) {}
    void run() override;
    void splitGEP(BasicBlock *bb);
};

#endif