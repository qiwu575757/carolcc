#ifndef COMPILER_HIRTOMIR_H
#define COMPILER_HIRTOMIR_H

#include "pass.h"

class HIRToMIR : public Transform {
public:
    HIRToMIR(std::string &name, Module *m) : Transform(name,m) {}
    void run() override;
    BasicBlock *genBasicBlock(BaseBlock *base_bb, BasicBlock *next_bb,
                           BasicBlock *while_entry, BasicBlock *while_exit,
                           Function *func);
};

#endif