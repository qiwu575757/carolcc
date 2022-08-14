#ifndef COMPILER_GLOBAL_VALUE_NUMBERING_H
#define COMPILER_GLOBAL_VALUE_NUMBERING_H

#include <iterator>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/instruction.h"
#include "pass.h"
#include "passes/analysis/dominators.h"
#include "utils.h"
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
    GlobalVariableNumbering(Module* m, const std::string& name)
        : Transform(m, name), _dom_analysis(m, "Dominator") {}
    void run() override;
    void check();
    void LVN(BasicBlock* bb);
    void SVN(BasicBlock* bb, std::queue<BasicBlock*>& work_list,
             std::set<BasicBlock*>& visited);

   private:
    std::vector<std::map<std::tuple<Value*, Value*,Instruction::OpKind>, Value*>>
        _binary_table;
    std::vector<std::map<Value*, Value*>> _gep_table;
    std::vector<std::map<Value*, Value*>> _value_table;
    Value* lookUpOrAdd(Value* val);
    Dominators _dom_analysis;
    Value* findSameInstrInTable(Instruction* instr);
    void replace(Instruction* old_val, Value* new_val);
    std::vector<Instruction*> _wait_delete;
    void pushTableScope() {
        _binary_table.emplace_back();
        _gep_table.emplace_back();
        _value_table.emplace_back();
    }
    void popTableScope() {
        _binary_table.pop_back();
        _gep_table.pop_back();
        _value_table.pop_back();
    }
    Value* findOnTable(Value* v);
    void deleteValueFromTable(Value* key_val);
    void pushValueToTable(Value* key, Value* val) {
        auto _layer_num = _value_table.size();
        auto instr = dynamic_cast<Instruction*>(key);
        if(instr ){
          if(instr->isGep()){
              _gep_table[_layer_num - 1].insert(std::make_pair(key, val));
          }
          else if(instr->isBinary() && !instr->isCmp()){
              _binary_table[_layer_num - 1].insert(
                  {{instr->getOperand(0), instr->getOperand(1),
                    instr->getInstructionKind()},
                   val});
          }
          else {
              _value_table[_layer_num - 1].insert(std::make_pair(key, val));
          }
        }
        else {
          _value_table[_layer_num - 1].insert(std::make_pair(key, val));
        }
    }
};
// #define GVN_LOG__
#ifdef GVN_LOG__
#define GVN_LOG(format, ...)                                           \
    do {                                                               \
        printf(YELLOW "[%s:%d]" format RESET "\n", __FILE__, __LINE__, \
               ##__VA_ARGS__);                                         \
        fflush(stdout);                                                \
    } while (0)
#else
#define GVN_LOG(format, ...)
#endif

#endif  // COMPILER_GLOBAL_VALUE_NUMBERING_H
