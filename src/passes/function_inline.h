#ifndef COMPILER_FUNCTION_INLINE
#define COMPILER_FUNCTION_INLINE

#include "ir/instruction.h"
#include "pass.h"
class BasicBlock;
class Function;

// #define _FUNC_LINE_LOG
#ifdef _FUNC_LINE_LOG
#define FUNC_LINE_LOG(format, ...)                                                            \
    do {                                                                             \
        printf(BLUE "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define FUNC_LINE_LOG(format, ...)
#endif

class FunctionInline : public Transform {
public:
    FunctionInline(Module *m, const std::string &name) : Transform(m,name) {}
    void run() override;

    bool canFunctionInline(Function* f);
    void inlineFunction(Function* f);
    void inlineCallInstr(CallInst * callInstr);
};

#endif // COMPILER_FUNCTION_INLINE