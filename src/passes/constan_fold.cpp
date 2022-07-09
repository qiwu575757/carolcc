
#include "module.h"
#include "constan_fold.h"
#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/instruction.h"
void ConstantFold::run() {
    for(auto f : _m->getFunctions()){
        if(!f->isBuiltin())
            constantFold(f);
    }
}
void ConstantFold::constantFold(Function *f) {

    for(auto bb : f->getBasicBlocks()){
        std::vector<Instruction*> wait_delete;
        for(auto inst: bb->getInstructions()){
            auto binary_inst = dynamic_cast<BinaryInst*> (inst);
            if(binary_inst!= nullptr){
                auto oprt1 = binary_inst->getOperand(0);
                if(!oprt1->isConstant())
                    continue ;
                auto oprt2 = binary_inst->getOperand(1);
                if(!oprt2->isConstant())
                    continue ;

                if(oprt1->getType()->isIntegerTy() && oprt2->getType()->isIntegerTy()){
                    auto val = calConstantBinary(binary_inst);
                    auto new_constant = ConstantInt::create(val);

                    binary_inst->replaceAllUse(new_constant);
                    binary_inst->removeUseOps();
                    wait_delete.push_back(binary_inst);
                }
                else{
                    ERROR("todo");
                }

            }
        }
        for(auto inst:wait_delete){
            bb->deleteInstr(inst);
        }
    }
}
int ConstantFold::calConstantBinary(BinaryInst *binary_instr) {
    auto oprt1 = static_cast<ConstantInt*>( binary_instr->getOperand(0))->getValue();
    auto oprt2 = static_cast<ConstantInt*>( binary_instr->getOperand(1))->getValue();
    int res ;
//
//    Add(Value *v1, Value *v2, BasicBlock *parent);
//    Sub(Value *v1, Value *v2, BasicBlock *parent);
//    Mul(Value *v1, Value *v2, BasicBlock *parent);
//    Div(Value *v1, Value *v2, BasicBlock *parent);
//    Rem(Value *v1, Value *v2, BasicBlock *parent);
    switch (binary_instr->getInstructionKind()) {
        case Instruction::ADD :
            res = oprt1+oprt2;
            break;
        case Instruction::SUB:
            res = oprt1-oprt2;
            break;
        case Instruction::MUL:
            res = oprt1*oprt2;
            break;
        case Instruction::DIV:
            res = oprt1/oprt2;
            break;
        case Instruction::REM:
            res = oprt1%oprt2;
            break;
        case Instruction::RET:
        case Instruction::BR:
        case Instruction::NOT:
        case Instruction::SHL:
        case Instruction::LSHR:
        case Instruction::ASHR:
        case Instruction::AND:
        case Instruction::OR:
        case Instruction::XOR:
        case Instruction::ALLOCA:
        case Instruction::LOAD:
        case Instruction::STORE:
        case Instruction::CMP:
        case Instruction::PHI:
        case Instruction::GEP:
        case Instruction::CALL:
        case Instruction::ZEXT:
        case Instruction::BREAK:
        case Instruction::CONTINUE:
            ERROR("error type");
            break ;
        default:
            ERROR("error type");
    }
    return res;
}
