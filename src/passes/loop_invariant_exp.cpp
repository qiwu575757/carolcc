#include "loop_invariant_exp.h"

// void LoopInvariantExp::run(){

// }
// void LoopInvariantExp::invariants_find(std::vector<BasicBlock *>* loop){
//     std::set<Value *> defined_in_loop;
//     std::set<Instruction *> invariant;
//     for (auto BB : *loop){
//         for (auto inst : BB->getInstructions()){
//             if (inst->isStore()){
//                 defined_in_loop.insert(inst->get_operand(1));
//             }
//             else{
//                 defined_in_loop.insert(inst);
//             }
//         }
//     }
//     bool change = false;
//     do {
//         change = false;
//         for (auto BB : *loop){
//             invariant.clear();
//             for (auto inst : BB->get_instructions()){
//                 bool invariant_check = true;
//                 if (inst->is_call()||inst->is_alloca()||inst->is_ret()||inst->is_br()||inst->is_cmp()||inst->is_phi()||inst->is_load()){
//                     continue;
//                 }
//                 if (defined_in_loop.find(inst) == defined_in_loop.end()){
//                     continue;
//                 }
//                 for (auto operand : inst->get_operands()){
//                     if (defined_in_loop.find(operand) != defined_in_loop.end()){
//                         invariant_check = false;
//                     }
//                 }
//                 if (invariant_check){
//                     defined_in_loop.erase(inst);
//                     invariant.insert(inst);
//                     change = true;
//                 }
//             }
//             if (!invariant.empty()){
//                 invariants.push_back({BB,invariant});
//             }
//         }
//     }while (change);
// }