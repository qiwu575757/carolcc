#ifndef COMPILER_MIRSIMPLIFYCFG_H
#define COMPILER_MIRSIMPLIFYCFG_H


#include "pass.h"

#define __MIR_SIMPLIFY_CFG_LOG
#ifdef __MIR_SIMPLIFY_CFG_LOG
#define SIMPLIFY_CFG_LOG(format, ...)                                                            \
    do {                                                                             \
        printf(BLUE "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define SIMPLIFY_CFG_LOG(format, ...)
#endif


class Function;
class MirSimplifyCFG : public Transform {
private:
  Function *_func;
public:
  MirSimplifyCFG(Module *m, const std::string &name) : Transform(m,name),_func(nullptr) {}
  void run() override;
  void RemoveNoPredecessorBaseBlocks();
  void MergeSinglePredecessorBaseBlocks();
  void EliminateSinglePredecessorPhi();
  void EliminateSingleUnCondBrBaseBlocks();
  void RemoveSelfLoopBaseBlocks();
};
#endif