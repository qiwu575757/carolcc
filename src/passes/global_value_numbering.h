#ifndef COMPILER_GLOBAL_VALUE_NUMBERING_H
#define COMPILER_GLOBAL_VALUE_NUMBERING_H

#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/instruction.h"
#include "pass.h"
#include "passes/analysis/dominators.h"

#include <map>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
class Function;
class BinaryInst;
//*
// * 全局值编号，同时完成 代数简化等操作
//
// * 目前只完成了lvn
//
//
class GlobalVariableNumbering : public Transform {
public:
    GlobalVariableNumbering(Module* m,const std::string &name): Transform(m,name),_dom_analysis(m, "Dominator"){
        
    }
    void run() override;
    void check();
    void LVN(BasicBlock* bb);
    void SVN(BasicBlock* bb,std::queue<BasicBlock*>& work_list,std::set<BasicBlock*> &visited);
    

private:
    std::vector<std::unordered_map<Value*,Value*>> _value_table;
    Value* lookUpOrAdd(Value* val);
    Dominators _dom_analysis;
    Value* findSameInstrInTable(Instruction* instr);
    void replace(Instruction* old_val,Value* new_val);
    std::vector<Instruction*> _wait_delete;
    void pushTableScope(){_value_table.push_back({});}
    void popTableScope() {_value_table.pop_back();}
    Value* findOnTable(Value* v);
    void deleteValueFromTable(Value* key_val);
    void pushValueToTable(Value* key,Value* val){
        auto _layer_num = _value_table.size();
        _value_table[_layer_num-1].insert(std::make_pair(key,val));
    }
};



#ifdef GVN_LOG__
#define GVN_LOG(format, ...)                                  \
    do {                                                                             \
        printf(YELLOW "[%s:%d]" format RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout);                                                              \
    } while (0)
#else
#define GVN_LOG(format, ...)
#endif


#endif //COMPILER_GLOBAL_VALUE_NUMBERING_H
