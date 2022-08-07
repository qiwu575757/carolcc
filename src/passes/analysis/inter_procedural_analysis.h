#include <list>
#include <map>
#include <set>

#include "passes/pass.h"
class Function;
class BasicBlock;
class Value;

class InterProceduralAnalysis : public Analysis {
   public:
    InterProceduralAnalysis(Module* m, const std::string& name)
        : Analysis(m, name) {}

    void run() override;

   private:
    void setCallerSideEffect(Function* f);
    void setCallerUseGobalVar(Function* f);
};
