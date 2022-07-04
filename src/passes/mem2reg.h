
#ifndef COMPILER_MEM2REG_H
#define COMPILER_MEM2REG_H
#include "pass.h"
#include <utility>
#include <vector>
class Value;
class Module;
class Function;
class BasicBlock;
class Mem2Reg : public  Transform{
   private:
    Function* _cur_func;
    BasicBlock* _cur_basic_block;
   public:
    Mem2Reg(Module* m,const std::string &name): Transform(m,name){}
    void run() final ;
    void genPhi() ;
    void reName(BasicBlock * bb) ;
};

class RenameData{
   public :
    BasicBlock* _bb; //所属基本块
    BasicBlock* _pred;
    std::vector<Value*> _values;// 当前基本块末尾的alloca对应的value
    RenameData(BasicBlock* bb,BasicBlock* pred,std::vector<Value*>values):_bb(bb),
    _pred(pred),_values(std::move(values)){

    }
};
#endif  // COMPILER_MEM2REG_H
