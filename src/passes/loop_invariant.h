
#ifndef _LOOP_INVARIANT_H
#define _LOOP_INVARIANT_H

#include <list>
#include <memory>
#include <unordered_set>
#include <utility>
#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "pass.h"
#include "passes/analysis/loop_analysis.h"
class Module;
class Function;

#define LOG_LOOP_INVARIANT
#ifdef LOG_LOOP_INVARIANT
#define LOOP_INVARIANT(format, ...)                                  \
    do {                                                                             \
        printf(YELLOW "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define LOOP_INVARIANT(format, ...)
#endif

class LoopInvariant : public Transform {
    public:
    LoopInvariant(Module *m, const std::string &name) : Transform(m,name) {}
    void run() override;
    private:
    std::unique_ptr<LoopAnalysis> _loop_analyzer;
    std::vector<Instruction*> _invariants;
    void findInvariants(std::unordered_set<BasicBlock*>* loop);
    void handleInvariants(std::unordered_set<BasicBlock*>* loop);
};
#endif // !_LOOP_INVARIANT_H