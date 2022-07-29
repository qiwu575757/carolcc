#include "function_inline.h"
#include <algorithm>
#include <iterator>
#include <list>
#include "ir/function.h"
#include "ir/basic_block.h"
#include "ir/value.h"
#include "module.h"
#include "ir/instruction.h"

void FunctionInline::run(){
    for(auto func:_m->getFunctions()){
        if(canFunctionInline(func)){
            inlineFunction(func);
        }
    }

}


bool FunctionInline::canFunctionInline(Function *f){
    if(!f->isBuiltin() && _m->getMainFunction() != f){
        for(auto caller : f->getCallerSet()){
            for(auto callee: f->getCalleeSet()){
                if(callee == caller){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}
void FunctionInline::inlineFunction(Function* f) {
    std::set<CallInst*> to_be_repalced_call_inst;
    for(auto caller:f->getCallerSet()){
        for(auto bb : caller->getBasicBlocks()){
            for(auto instr : bb->getInstructions()){
                auto call_instr = dynamic_cast<CallInst*>(instr);
                if(call_instr!=nullptr && call_instr->getOperand(0)->getName()== f->getName()){
                    to_be_repalced_call_inst.insert(call_instr);
                }
            }
        }
    }
    for(auto instr:to_be_repalced_call_inst){
        inlineCallInstr(instr);
    }
    for(auto caller:f->getCallerSet()){
        caller->getCalleeSet().erase(caller->getCalleeSet().find(f));
        caller->getCalleeSet().insert(f->getCalleeSet().begin(),f->getCalleeSet().end());
    }
    f->getCallerSet().clear();
}
void FunctionInline::inlineCallInstr(CallInst* callInstr) {
    auto ret_type = callInstr->getType();
    auto func_wait_for_inline = callInstr->getFunction();
    auto copy_func = ValueCloner::copyFunc(func_wait_for_inline);
    auto end_block = BasicBlock::create("",callInstr->getParent()->getParentFunc());

    auto call_parent_bb = callInstr->getParent();

    // instr after call should be moved to end_block
    std::list<Instruction*> inst_after_call;
    auto call_pos = std::find(call_parent_bb->getInstructions().begin(), call_parent_bb->getInstructions().end(),callInstr);
    call_pos++;
    end_block->getInstructions().assign(call_pos, call_parent_bb->getInstructions().end());
    call_pos--;
    inst_after_call.erase(call_pos, call_parent_bb->getInstructions().end());

    // call -> br
    BranchInst::createBranch(copy_func->getEntryBlock() ,call_parent_bb);
    
     
}

