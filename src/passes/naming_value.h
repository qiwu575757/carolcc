
#ifndef COMPILER_NAMING_VALUE_H
#define COMPILER_NAMING_VALUE_H
#include "pass.h"
#include <unordered_map>
#include <vector>
class Value;
class Module;
class Function;
class BasicBlock;
class Instruction;

class NamingValue:public pass{
private:
    void NameValue(Value *val);
    void NameBasicBlock(BasicBlock *base_block);
    void NameInstr(Instruction *instr);
    std::unordered_map<Value *, int> seq;
public:
    NamingValue(Module * m,const std::string &name):pass(m,name){}

    void run() final;
};
 
#endif  // COMPILER_NAMING_VALUE_H
