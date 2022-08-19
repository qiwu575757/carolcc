#include "lazy_code_motion.h"
#include "ir/function.h"
#include "ir/basic_block.h"
#include "passes/analysis/dominators.h"
#include "passes/module.h"
#include "ir/instruction.h"
void LazyCodeMotion:: run(){
    for(auto func : _m->getFunctions()){
        init(func);
    }
}

void LazyCodeMotion::init(Function* func){

    // init ue_expr and expr_kill
    for(auto bb : func->getBasicBlocks()){
        auto& ue_expr = bb->getUEExprs();
        auto& expr_kill  = bb->getExprKills();
        ue_expr.clear();
        expr_kill.clear();

        bb->getAvailOuts().clear();
        for(auto instr: bb->getInstructions()){
            for(auto oprd : instr->getOperandList()){
                auto oprd_expr = dynamic_cast<Instruction*>(oprd);// 默认 指令 等价于 表达式
                if(oprd_expr){ // 如果这个操作数是一个表达式
                    if(expr_kill.find(oprd_expr) == expr_kill.end()){
                        ue_expr.insert(oprd_expr);
                    }
                }
            }
            expr_kill.insert(instr);
        }
    }
    
    Dominators dom (_m,"dom_for_lcm");
    for(auto bb :dom.getReversePostOderList(func)){
        auto& bb_avail_outs = bb->getAvailOuts();
        
    }



}