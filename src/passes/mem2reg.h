
#ifndef COMPILER_MEM2REG_H
#define COMPILER_MEM2REG_H
#include "pass.h"
#include <utility>
#include <vector>
class Value;
class Module;
class Function;
class BasicBlock;

#define MEM2REG_LOG_
#ifdef MEM2REG_LOG_
#define MEM2REG_LOG(format, ...)                                                            \
    do {                                                                             \
        printf(BLUE "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define MEM2REG_LOG(format, ...)
#endif

class Mem2Reg : public  Transform{
   private:
    Function* _cur_func{};
    BasicBlock* _cur_basic_block{};
   public:
    Mem2Reg(Module* m,const std::string &name): Transform(m,name){}
    void run() final ;
    void genPhi() ;
};

class RenameData{
   public :
    BasicBlock* _bb; //所属基本块
    BasicBlock* _pred; // 来自哪个前驱基本块
    std::vector<Value*> _values;// 从此前驱来的alloca的值
    RenameData(BasicBlock* bb,BasicBlock* pred,std::vector<Value*>values):_bb(bb),
    _pred(pred),_values(std::move(values)){

    }
};
#endif  // COMPILER_MEM2REG_H
