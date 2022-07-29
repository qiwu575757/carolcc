#ifndef COMPILER_FUNCTION_INLINE
#define COMPILER_FUNCTION_INLINE

#include "ir/instruction.h"
#include "pass.h"
class BasicBlock;
class Function;

class FunctionInline : public Transform {
public:
    FunctionInline(Module *m, const std::string &name) : Transform(m,name) {}
    void run() override;

    bool canFunctionInline(Function* f);
    void inlineFunction(Function* f);
    void inlineCallInstr(CallInst * callInstr);
};

#endif // COMPILER_FUNCTION_INLINE