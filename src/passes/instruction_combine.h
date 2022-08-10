
#ifndef COMPILER_INSTRUCTION_COMBINE_H
#define COMPILER_INSTRUCTION_COMBINE_H

#include "ir/instruction.h"
#include "ir/value.h"
#include "pass.h"
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>
class BasicBlock;
class BaseBlock;
class Function;
class Instruction;
class BinaryInst;

class InstructionCombination : public Transform {
   public:
    InstructionCombination(Module *m, const std::string &name)
        : Transform(m, name) {}
    void run() override;

   private:
    void simplifyInstruction(Function *func);
    void instructionCombine(Function* fun);
    Value * simplifyInstruction (Instruction* instr,std::list<Instruction*>& wait_delete,std::vector<std::pair<Instruction*, Instruction*>>& insert_map);
    Value * simplifyAdd (BinaryInst*binary_instr,std::list<Instruction*>& wait_delete,std::vector<std::pair<Instruction*, Instruction*>>& insert_map,bool can_recur);
    Value * simplifyMul (BinaryInst*binary_instr,std::list<Instruction*>& wait_delete,std::vector<std::pair<Instruction*, Instruction*>>& insert_map,bool can_recur);
    Value * combineInstruction (Instruction* instr,std::list<Instruction*>& wait_delete,std::vector<std::pair<Instruction*, Instruction*>>& insert_map);
    Value * combineAdd (BinaryInst*binary_instr,std::list<Instruction*>& wait_delete,std::vector<std::pair<Instruction*, Instruction*>>& insert_map);
    private:
    std::unordered_map<BinaryInst*, std::pair<Value*, int>> _instr2value_number;

};

#endif