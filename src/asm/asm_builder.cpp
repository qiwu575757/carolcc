#include "asm_builder.h"
#include "asm_instr.h"
#include "utils.h"
std::string AsmBuilder::generate_asm(std::map<Value *, int> register_mapping){
    ERROR("UNDO");
}
std::string AsmBuilder::generate_asm(){
    asm_code += generate_module_header();
    for(auto &func: this->module->getFunctions()){
        if(func->getBasicBlocks().size() != 0){
            asm_code += generate_function_code(func);
        }
    }
    asm_code += generate_module_tail();
    return asm_code;
}
std::string AsmBuilder::generate_module_header(){
    return "";
}
std::string AsmBuilder::generate_module_tail(){
    return "";
}
std::string AsmBuilder::generate_function_code(Function *func){
    std::string func_asm;
    func_asm += generate_function_entry_code();

    for (auto &bb : func->getBasicBlocks()) {
        func_asm += getLabelName(bb) + ":" + InstGen::newline;
        func_asm += generateBasicBlockCode(bb);
    }

    func_asm += generate_function_exit_code();

    return func_asm;
}
std::string AsmBuilder::generate_function_entry_code(){
    return "";
}
std::string AsmBuilder::generate_function_exit_code(){
    return "";
}
std::string AsmBuilder::update_value_mapping(std::list<Value*> update_v){
    return "";
}

std::string  AsmBuilder::generateBasicBlockCode(BasicBlock *bb){
    std::string bb_asm;
    for (auto &inst : bb->getInstructions()) {
        bb_asm += AsmBuilder::generateInstructionCode(inst);
    }

    return bb_asm;
}
std::string  AsmBuilder::getLabelName(BasicBlock *bb){
    return "";
}

std::string AsmBuilder::generateInstructionCode(Instruction *inst) {
    std::string inst_asm;
    auto &operands = inst->getOperandList();
    std::list<Value *> variable_list;
    BasicBlock *bb_cur = inst->getParent();

    switch (inst->getInstructionKind()) {
        case Instruction::ADD:
            // 使用变量的名字
            auto src_op1 = operands.at(0);
            auto src_op2 = operands.at(1);
            variable_list.push_back(src_op1);
            variable_list.push_back(src_op2);
            variable_list.push_back(inst);
            inst_asm += update_value_mapping(variable_list);
            const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
            const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
            const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
            inst_asm += InstGen::add(target_reg,src_reg1,src_reg2);
            break;
        case Instruction::SUB:
            // 使用变量的名字
            auto src_op1 = operands.at(0);
            auto src_op2 = operands.at(1);
            variable_list.push_back(src_op1);
            variable_list.push_back(src_op2);
            variable_list.push_back(inst);
            inst_asm += update_value_mapping(variable_list);
            const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
            const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
            const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
            inst_asm += InstGen::sub(target_reg,src_reg1,src_reg2);
            break;
        case Instruction::MUL:
            // 使用变量的名字
            auto src_op1 = operands.at(0);
            auto src_op2 = operands.at(1);
            variable_list.push_back(src_op1);
            variable_list.push_back(src_op2);
            variable_list.push_back(inst);
            inst_asm += update_value_mapping(variable_list);
            const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
            const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
            const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
            inst_asm += InstGen::mul(target_reg,src_reg1,src_reg2);
            break;
        case Instruction::DIV:
            // 使用变量的名字
            auto src_op1 = operands.at(0);
            auto src_op2 = operands.at(1);
            variable_list.push_back(src_op1);
            variable_list.push_back(src_op2);
            variable_list.push_back(inst);
            inst_asm += update_value_mapping(variable_list);
            const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
            const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
            const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
            inst_asm += InstGen::sdiv(target_reg,src_reg1,src_reg2);
            break;
        case Instruction::AND:
            // 使用变量的名字
            auto src_op1 = operands.at(0);
            auto src_op2 = operands.at(1);
            variable_list.push_back(src_op1);
            variable_list.push_back(src_op2);
            variable_list.push_back(inst);
            inst_asm += update_value_mapping(variable_list);
            const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
            const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
            const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
            inst_asm += InstGen::and_(target_reg,src_reg1,src_reg2);
            break;
        case Instruction::OR:
            // 使用变量的名字
            auto src_op1 = operands.at(0);
            auto src_op2 = operands.at(1);
            variable_list.push_back(src_op1);
            variable_list.push_back(src_op2);
            variable_list.push_back(inst);
            inst_asm += update_value_mapping(variable_list);
            const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
            const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
            const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
            inst_asm += InstGen::orr(target_reg,src_reg1,src_reg2);
            break;
        case Instruction::LOAD:
            // 使用变量的名字
            auto src_op1 = operands.at(0);
            variable_list.push_back(src_op1);
            variable_list.push_back(inst);
            inst_asm += update_value_mapping(variable_list);
            const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
            const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
            inst_asm += InstGen::load(target_reg, InstGen::Addr(src_reg1,0));
            break;
        case Instruction::STORE:
        // 这里有点晕了
            // 使用变量的名字
            // auto src_op1 = operands.at(0);
            // variable_list.push_back(src_op1);
            // variable_list.push_back(inst);
            // inst_asm += update_value_mapping(variable_list);
            // const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
            // const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
            // inst_asm += InstGen::store(target_reg, InstGen::Addr(src_reg1,0));
            break;
        case Instruction::RET:
            auto src_op1 = operands.at(0);
            variable_list.push_back(src_op1);
            inst_asm += update_value_mapping(variable_list);
            const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
            inst_asm += InstGen::ret(src_reg1);
            break;
        case Instruction::CMP:
            if (operands.size() == 1){
                auto src_op1 = operands.at(0);
                switch (dynamic_cast<CmpInst *>(inst)->_cmp_op)
                {
                    case CmpInst::EQ:
                        /* code */
                        break;
                    case CmpInst::NEQ:
                        /* code */
                        break;
                    case CmpInst::GT:
                        /* code */
                        break;
                    case CmpInst::LE:
                        /* code */
                        break;
                    case CmpInst::GE:
                        /* code */
                        break;
                    case CmpInst::LT:
                        /* code */
                        break;

                    default:
                        break;
                }
            }
        case Instruction::BR:
            if (operands.size() == 1){
                auto src_op1 = operands.at(0);
                variable_list.push_back(src_op1);
                inst_asm += update_value_mapping(variable_list);
                inst_asm += InstGen::b(InstGen::Label(getLabelName(bb_cur) +
                                          "_branch_" + std::to_string(register_mapping[src_op1]),
                                      0),
                       InstGen::NOP);
            } else if (operands.size() == 2) {
                auto src_op1 = operands.at(0);
                variable_list.push_back(src_op1);
                auto src_op2 = operands.at(1);
                variable_list.push_back(src_op2);
                variable_list.push_back(inst);
                inst_asm += update_value_mapping(variable_list);
                const InstGen::Reg src_reg1 = InstGen::Reg(register_mapping[src_op1]);
                const InstGen::Reg src_reg2 = InstGen::Reg(register_mapping[src_op2]);
                const InstGen::Reg target_reg = InstGen::Reg(register_mapping[inst]);
                inst_asm += "cmp    "+ target_reg.getName() + ", " + "#0";
                inst_asm += "beq    " + InstGen::Label(getLabelName(bb_cur)).getName() +
                                          "_branch_" + std::to_string(register_mapping[src_op1]);

                inst_asm += InstGen::b(InstGen::Label(getLabelName(bb_cur) +
                                          "_branch_" + std::to_string(register_mapping[src_op2]),
                                            InstGen::NOP));
            }
            break;
    }


    return inst_asm;
}