
#ifndef COMPILER_EMITHIR_H
#define COMPILER_EMITHIR_H
#include "pass.h"
class Module;
class EmitHir : public pass{
   public:
    EmitHir(Module* m,const std::string &name): pass(m,name){}
    void run() override;

};
class EmitIR : public pass{
   public:
    EmitIR(Module* m,const std::string &name): pass(m,name){}
    void run() override;

};

class EmitPadGraph : public pass{
   public:
    EmitPadGraph(Module* m,const std::string &name): pass(m,name){}
    void run() override;

};

#endif  // COMPILER_EMITHIR_H
