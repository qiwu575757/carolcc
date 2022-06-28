#ifndef COMPILER_HIRTOMIR_H
#define COMPILER_HIRTOMIR_H

#include "pass.h"
class BasicBlock;
class BaseBlock;
class Function;

class HIRToMIR : public Transform {
public:
    HIRToMIR(const std::string &name) : Transform(name) {}
    void run();
    BasicBlock *genBasicBlock(BaseBlock *base_bb, BasicBlock *next_bb,
                           BasicBlock *while_entry, BasicBlock *while_exit,
                           Function *func);
};

#endif