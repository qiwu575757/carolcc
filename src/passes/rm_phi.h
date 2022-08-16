#ifndef COMPILER_RM_PHI_H
#define COMPILER_RM_PHI_H

#include "ir/function.h"
#include "pass.h"
class BasicBlock;

class RmPhi : public Transform {
public:
    RmPhi(Module *m, const std::string &name) : Transform(m,name) {}
    void run() override;
    void splitCriticalEdge(Function* func);
    void rmPhi(Function* func);
    void rmPhiCycle(Function *func);
    //  移除多余的mov指令
};

#endif