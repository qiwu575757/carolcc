#include <memory.h>

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "../ir/basic_block.h"
#include "asm_builder.h"
#include "asm_instr.h"
#include "utils.h"

std::pair<int, int> AsmBuilder::getUsedRegisterNum(Function *func) {
    for (auto it = func_used_reg_map.begin(); it != func_used_reg_map.end(); it++) {
        LSRA_WARNNING("#####%s %d %d",it->first.c_str(),it->second.first,it->second.second);
    }
    LSRA_WARNNING("#####%s",func->getPrintName().c_str());
    if(func->getBasicBlocks().empty()){
        return std::pair<int,int>(0,0);
    }
    // MyAssert("can not find func",func_used_reg_map.count(func->getPrintName()),255);
    return func_used_reg_map[func->getPrintName()];
}

int AsmBuilder::getAllocaSpOffset(Value *inst) {  // 值的栈偏移
    if(dynamic_cast<MovInstr *>(inst))inst = dynamic_cast<MovInstr *>(inst)->getLVal();//MOV转化
    for (int i = 0; i < func_reg_map[cur_func_name].stack_map.size(); i++) {
        if (func_reg_map[cur_func_name].stack_map[i].v == inst && func_reg_map[cur_func_name].stack_map[i].is_data) {
            return func_reg_map[cur_func_name].stack_map[i].offset;
        }
    }
    ERROR("can't give any reg because all the reg is using",EXIT_CODE_ERROR_302);
    return -1;
}

int vir2real(int reg_idx){
    if(reg_idx == 11){
        // LSRA_WARNNING("@@@@@@@@@@@@@@@@@ use r12");
        return 12;
    }
    if(reg_idx == 12){
        // LSRA_WARNNING("@@@@@@@@@@@@@@@@@ use r14");
        return 14;
    }
    return reg_idx;
}
int real2vir(int reg_idx){
    if(reg_idx == 12) return 11;
    if(reg_idx == 14) return 12;
    return reg_idx;
}

int AsmBuilder::acquireForReg(Value *inst, int val_pos, std::string &str) {
    LSRA_WARNNING("ask for value %s",inst->getPrintName().c_str());
    if (val_pos >= op_save_stack_num) {
        ERROR("overwrite op_idx",EXIT_CODE_ERROR_303);
    }
    int reg_get = give_reg_at(inst);  // LVAL REG NUM
    if (reg_get == -1) {
        // debug
        LSRA_WARNNING("-- int reg set --");
        #ifdef __LSRA_SHOW
        for (int i = 0; i < int_reg_number; i++) {
            LSRA_SHOW("[reg %02d]", i);
            for (auto &reg : func_reg_map[cur_func_name].virtual_int_reg_use[i]) {
                LSRA_SHOW(" %02d ", reg);
            }
            LSRA_SHOW("[reg %02d]\n", i);
        }
        #endif // DEBUG
        LSRA_WARNNING("-- float reg set --");
        #ifdef __LSRA_SHOW
        for (int i = 0; i < float_reg_number; i++) {
            LSRA_SHOW("[reg %02d]", i);
            for (auto &reg : func_reg_map[cur_func_name].virtual_float_reg_use[i]) {
                LSRA_SHOW(" %02d ", reg);
            }
            LSRA_SHOW("[reg %02d]\n", i);
        }
        #endif // DEBUG
        ERROR("can't give any reg because all the reg is using at inst %d %s"
        ,EXIT_CODE_ERROR_304,func_reg_map[cur_func_name].linear_map[inst],inst->getPrintName().c_str());
    }
    Value *reg_v = value_in_reg_at(inst, reg_get, inst->getType()->isFloatTy());
    if (reg_v != nullptr) {  // 说明占用了寄存器
        //!! 插入一条冲突寄存器分配到func_reg_map[cur_func_name]->virtual_int_regs
        //!! 做好冲突维护
        // store reg_get to reg_save
        str += InstGen::comment("insert str", "");
        if(inst->getType()->isFloatTy()){
            str += InstGen::store(InstGen::Reg((reg_get), false),
                              InstGen::Addr(InstGen::sp, func_reg_map[cur_func_name].op_save[val_pos]));
            return reg_get;
        }
        else{
            str += InstGen::store(InstGen::Reg(vir2real(reg_get), false),
                              InstGen::Addr(InstGen::sp, func_reg_map[cur_func_name].op_save[val_pos]));
            return vir2real(reg_get);
        }
    }
    else{ // 没占用寄存器
        if(inst->getType()->isFloatTy()){
            return reg_get;
        }
        else{
            return vir2real(reg_get);
        }
    }
}
std::string AsmBuilder::popValue(Value *inst, int reg_idx, int val_pos) {
    std::string insert_inst = "";
    if (val_pos >= op_save_stack_num) {
        ERROR("overwrite op_idx",EXIT_CODE_ERROR_305);
    }
    Value *reg_v = nullptr;
    if(inst->getType()->isFloatTy()){
        reg_v = value_in_reg_at(inst, (reg_idx),inst->getType()->isFloatTy());
    }
    else{
        reg_v = value_in_reg_at(inst, real2vir(reg_idx),inst->getType()->isFloatTy());
    }
    if (reg_v != nullptr) {  // 说明占用了寄存器
        insert_inst += InstGen::comment("insert ldr", "");
        insert_inst +=
            InstGen::load(InstGen::Reg(reg_idx, false),
                          InstGen::Addr(InstGen::sp, func_reg_map[cur_func_name].op_save[val_pos]));
    }
    return insert_inst;
}
// 一般指令(除call/gep)无论该值在栈中还是寄存器中
int AsmBuilder::getRegIndexOfValue(Value *inst, Value *val, bool global_label) {

    if(dynamic_cast<MovInstr *>(val))val = dynamic_cast<MovInstr *>(val)->getLVal();//MOV转化
    int tag = func_reg_map[cur_func_name].linear_map[inst];

    // if(func_reg_map[cur_func_name].v2reg[val].size() < 100){

        for(auto itv: func_reg_map[cur_func_name].v2reg[val]){
            LSRA_WARNNING("%s value %d %d ", val->getPrintName().c_str(), itv.st_id,itv.ed_id);
            if (tag >= itv.st_id &&
                tag <= itv.ed_id) {

                if(val->getType()->isFloatTy()){
                    if(itv.conflict_reg<float_reg_number)
                        return itv.conflict_reg;
                }
                else{
                    if(itv.conflict_reg<int_reg_number)
                        return vir2real(itv.conflict_reg);
                }
            }
        }
    // }
    // else{
    //     if(val->getType()->isFloatTy()){
    //     // LSRA_WARNNING("inst %s fval %s idx %d",inst->getPrintName().c_str(),
    //     // val->getPrintName().c_str(),tag);
    //         for (int i = 0; i < float_reg_number; i++) {
    //             for (int j = 0; j < func_reg_map[cur_func_name].virtual_float_regs[i].size(); j++) {
    //                 // LSRA_WARNNING("check %s same ? %d name_same ? %d start %d end %d reg %d",
    //                 // func_reg_map[cur_func_name].virtual_float_regs[i][j].v->getPrintName().c_str(),
    //                 // func_reg_map[cur_func_name].virtual_float_regs[i][j].v == val,
    //                 // func_reg_map[cur_func_name].virtual_float_regs[i][j].v->getPrintName() == val->getPrintName(),
    //                 // func_reg_map[cur_func_name].virtual_float_regs[i][j].st_id,
    //                 // func_reg_map[cur_func_name].virtual_float_regs[i][j].ed_id,i);
    //                 if (func_reg_map[cur_func_name].virtual_float_regs[i][j].v == val &&
    //                     tag >= func_reg_map[cur_func_name].virtual_float_regs[i][j].st_id &&
    //                     tag <= func_reg_map[cur_func_name].virtual_float_regs[i][j].ed_id) {
    //                     // LSRA_WARNNING("%s value give reg %d",
    //                                 //   val->getPrintName().c_str(), i);
    //                     return i;
    //                 }
    //             }
    //         }
    //     }
    //     else{
    //     // LSRA_WARNNING("inst %s ival %s idx %d",inst->getPrintName().c_str(),
    //     // val->getPrintName().c_str(),tag);
    //         for (int i = 0; i < int_reg_number; i++) {
    //             if(func_reg_map[cur_func_name].virtual_int_regs[i].size()<4){
    //                 for (int j = 0; j < func_reg_map[cur_func_name].virtual_int_regs[i].size(); j++) {
    //                     // LSRA_WARNNING("check %s same ? %d name_same ? %d start %d end %d reg %d",
    //                     // func_reg_map[cur_func_name].virtual_int_regs[i][j].v->getPrintName().c_str(),
    //                     // func_reg_map[cur_func_name].virtual_int_regs[i][j].v == val,
    //                     // func_reg_map[cur_func_name].virtual_int_regs[i][j].v->getPrintName() == val->getPrintName(),
    //                     // func_reg_map[cur_func_name].virtual_int_regs[i][j].st_id,
    //                     // func_reg_map[cur_func_name].virtual_int_regs[i][j].ed_id,i);
    //                     if (func_reg_map[cur_func_name].virtual_int_regs[i][j].v == val &&
    //                         tag >= func_reg_map[cur_func_name].virtual_int_regs[i][j].st_id &&
    //                         tag <= func_reg_map[cur_func_name].virtual_int_regs[i][j].ed_id) {
    //                         // LSRA_WARNNING("%s value give reg %d",
    //                                     //   val->getPrintName().c_str(), i);
    //                         return vir2real(i);
    //                     }
    //                 }
    //             }
    //             else
    //             {
    //                 int l = 0, r = func_reg_map[cur_func_name].virtual_int_regs[i].size()-1;
    //                 while(r!=l){
    //                     float mid = (l+r)/2.0;
    //                     float mid_l = func_reg_map[cur_func_name].virtual_int_regs[i][int(mid+0.5)].st_id;
    //                     if(tag<mid_l){
    //                         r = int(mid);
    //                     }else{
    //                         l = int(mid+0.5);
    //                     }
    //                 }
    //                 if (func_reg_map[cur_func_name].virtual_int_regs[i][l].v == val &&
    //                     func_reg_map[cur_func_name].virtual_int_regs[i][l].st_id <= tag &&
    //                     func_reg_map[cur_func_name].virtual_int_regs[i][l].ed_id >= tag) {  // j 在 itv 后面 并且是第一个
    //                     return vir2real(i);
    //                 }


    //             }
    //         }
    //     }
    // }
    LSRA_WARNNING("inst %s val %s idx %d",inst->getPrintName().c_str(),
    val->getPrintName().c_str(),tag);
    LSRA_WARNNING("cant find this value in reg alloc!");
    return -1;
}
//获得函数调用返回值变量的位置，int - reg_index/sp off, bool true - in reg/stack
std::pair<int, bool> AsmBuilder::getValuePosition(Value *inst,
                                                  Value *val) {  // 全局变量？
    if(dynamic_cast<Instruction*>(inst)->isMOV())
        LSRA_WARNNING("mov inst lval is %s, ask pos of value %s",dynamic_cast<MovInstr*>(inst)->getLVal()->getPrintName().c_str(), val->getPrintName().c_str());
    else
        LSRA_WARNNING("type: %d tag: %d inst lval is %s, ask pos of value %s",dynamic_cast<Instruction*>(inst)->getInstructionKind(),func_reg_map[cur_func_name].linear_map[inst],inst->getPrintName().c_str(), val->getPrintName().c_str());
    int get_reg = getRegIndexOfValue(inst, val);

    if(get_reg == -1 && (dynamic_cast<GlobalVariable *>(val) || dynamic_cast<Constant *>(val))){
        LSRA_WARNNING("global value or constant value split which is not solved here return with offset -1");
        return std::pair<int, bool>(-1, false);
    }
    else if (get_reg == -1) {
        int offset = get_value_sp_offset(inst, val);
        return std::pair<int, bool>(offset, false);
    }
    return std::pair<int, bool>(get_reg, true);
}

bool cmp(interval lhs, interval rhs)  //升序
{
    return lhs.weight > rhs.weight;
}

bool AsmBuilder::value_in_reg(Value *v) {  //计算栈空间
    if(dynamic_cast<MovInstr *>(v))v = dynamic_cast<MovInstr *>(v)->getLVal();//MOV转化


    if(v->getType()->isFloatTy()){
        for (int i = 0; i < float_reg_number; i++) {
            for (int j = 0; j < func_reg_map[cur_func_name].virtual_float_regs[i].size(); j++) {
                if (func_reg_map[cur_func_name].virtual_float_regs[i][j].v == v) {
                    return true;
                }
            }
        }
    }
    else{
        for (int i = 0; i < int_reg_number; i++) {
            for (int j = 0; j < func_reg_map[cur_func_name].virtual_int_regs[i].size(); j++) {
                if (func_reg_map[cur_func_name].virtual_int_regs[i][j].v == v) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool AsmBuilder::force_reg_alloc(
    interval itv,
    int reg_idx) {  // 当出现绝对优先分配，并且强制寄存器分配序号的情况
    // 返回True代表分配成功
    int index = 0;
    bool conflict = false;
    if(itv.is_float){
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_float_regs[reg_idx].size(); j++) {
        conflict = true;
        if (func_reg_map[cur_func_name].virtual_float_regs[reg_idx][j].ed_id < itv.st_id &&
            j == func_reg_map[cur_func_name].virtual_float_regs[reg_idx].size() -
                     1) {  // j 在 itv 前面 并且是最后一个
            conflict = false;
            index = j + 1;
            break;
        } else if (func_reg_map[cur_func_name].virtual_float_regs[reg_idx][j].st_id > itv.ed_id &&
                   j == 0) {  // j 在 itv 后面 并且是第一个
            conflict = false;
            index = j;
            break;
        } else if (j != func_reg_map[cur_func_name].virtual_float_regs[reg_idx].size() - 1 &&
                   func_reg_map[cur_func_name].virtual_float_regs[reg_idx][j + 1].st_id > itv.ed_id &&
                   func_reg_map[cur_func_name].virtual_float_regs[reg_idx][j].ed_id <
                       itv.st_id) {  // j 在 itv 后面 并且是第一个
            conflict = false;
            index = j + 1;
            break;
        }
        }
        if (!conflict) {
            func_reg_map[cur_func_name].virtual_float_regs[reg_idx].insert(
                func_reg_map[cur_func_name].virtual_float_regs[reg_idx].begin() + index, itv);
            itv.conflict_reg = reg_idx;
            func_reg_map[cur_func_name].v2reg[itv.v].push_back(itv);
            return true;
        }
    }
    else{
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_int_regs[reg_idx].size(); j++) {
        conflict = true;
        if (func_reg_map[cur_func_name].virtual_int_regs[reg_idx][j].ed_id < itv.st_id &&
            j == func_reg_map[cur_func_name].virtual_int_regs[reg_idx].size() -
                     1) {  // j 在 itv 前面 并且是最后一个
            conflict = false;
            index = j + 1;
            break;
        } else if (func_reg_map[cur_func_name].virtual_int_regs[reg_idx][j].st_id > itv.ed_id &&
                   j == 0) {  // j 在 itv 后面 并且是第一个
            conflict = false;
            index = j;
            break;
        } else if (j != func_reg_map[cur_func_name].virtual_int_regs[reg_idx].size() - 1 &&
                   func_reg_map[cur_func_name].virtual_int_regs[reg_idx][j + 1].st_id > itv.ed_id &&
                   func_reg_map[cur_func_name].virtual_int_regs[reg_idx][j].ed_id <
                       itv.st_id) {  // j 在 itv 后面 并且是第一个
            conflict = false;
            index = j + 1;
            break;
        }
        }
        if (!conflict) {
            func_reg_map[cur_func_name].virtual_int_regs[reg_idx].insert(
            func_reg_map[cur_func_name].virtual_int_regs[reg_idx].begin() + index, itv);
            itv.conflict_reg = reg_idx;
            func_reg_map[cur_func_name].v2reg[itv.v].push_back(itv);
            return true;
        }
    }

    return false;
}

void AsmBuilder::linear_scan_reg_alloc(std::vector<interval> live_range,
                                       Function *func, bool insert) {
    LSRA_WARNNING("[linear_scan_reg_alloc]");
    // reset
    func_reg_map[cur_func_name].stack_size = 0;
    //
    // 优先处理入口参数
    for (auto p : live_range) {
        if (p.type == interval_value_type::arg_var && !p.spilled) {
            if (!force_reg_alloc(p, p.specific_reg_idx)) {
                ERROR("can't use force reg alloc when conflict exist!",EXIT_CODE_ERROR_307);
            }
        }
    }
    // cal weight
    //栈溢出处理 检查
    // 对于浮点和整型共同处理
    for (auto &p : live_range) {
        if(p.v->isConstant()){
            p.weight = 1e-4;
            continue;
        }
        int total_use_num = p.use_id.size();
        int sum_range = p.ed_id - p.st_id;
        p.use_freq = (total_use_num + 1e-4) / (sum_range + 1);
        p.weight += p.use_freq;  // 暂时考虑按这个
    }

// sort by weight
    LSRA_WARNNING("[sort by weight]");

    sort(live_range.begin(), live_range.end(), cmp);
    for (auto p : live_range) {
        LSRA_WARNNING(
                "[%d,%d] v:%s w:%f type:%s",p.st_id, p.ed_id,
                p.v->getPrintName().c_str(),
                p.weight, getType(p.type).c_str());
    }
    for (auto &itv : live_range) {
        if(itv.type == interval_value_type::call_val){ // 特殊占位
            if(itv.is_float){
                func_reg_map[cur_func_name].virtual_float_regs[itv.specific_reg_idx].push_back(itv);
            }
            else{
                func_reg_map[cur_func_name].virtual_int_regs[itv.specific_reg_idx].push_back(itv);
            }
            continue;
        }
        if (itv.type == interval_value_type::arg_var) { //#
            // 已经处理过arg，不再计算
            continue;
        }
        float ave = (itv.st_id+itv.ed_id)/2.0;
        if(itv.is_float){
            for (int i = 0; i < virtual_reg_max; i++) {
                bool conflict = false;
                int index = 0;
                if(func_reg_map[cur_func_name].virtual_float_regs[i].empty()){
                    index = 0;
                }
                else{
                    int l = 0, r = func_reg_map[cur_func_name].virtual_float_regs[i].size()-1;
                    float ave1 = (func_reg_map[cur_func_name].virtual_float_regs[i][l].st_id+func_reg_map[cur_func_name].virtual_float_regs[i][l].ed_id)/2.0;
                    float ave2 = (func_reg_map[cur_func_name].virtual_float_regs[i][r].st_id+func_reg_map[cur_func_name].virtual_float_regs[i][r].ed_id)/2.0;
                    if(ave <= ave1 || ave >= ave2){
                        if(itv.ed_id<func_reg_map[cur_func_name].virtual_float_regs[i][l].st_id){
                            index = l;
                        }
                        else if(itv.st_id>func_reg_map[cur_func_name].virtual_float_regs[i][r].ed_id ){
                            index = r+1;
                        }
                        else{
                            conflict = true;
                        }
                    }
                    else{
                        while(r-l>1){
                            float ave1 = (func_reg_map[cur_func_name].virtual_float_regs[i][l].st_id+func_reg_map[cur_func_name].virtual_float_regs[i][l].ed_id)/2.0;
                            float ave2 = (func_reg_map[cur_func_name].virtual_float_regs[i][r].st_id+func_reg_map[cur_func_name].virtual_float_regs[i][r].ed_id)/2.0;
                            float mid = (l+r)/2.0;
                            float mid_l = (func_reg_map[cur_func_name].virtual_float_regs[i][int(mid+0.5)].st_id+func_reg_map[cur_func_name].virtual_float_regs[i][int(mid+0.5)].ed_id)/2.0;
                            // float mid_r = (func_reg_map[cur_func_name].virtual_float_regs[i][int(mid-0.5)].st_id+func_reg_map[cur_func_name].virtual_float_regs[i][int(mid-0.5)].ed_id)/2;
                            if(ave>ave1 && ave<mid_l){
                                r = int(mid+0.5);
                            }else{
                                l = int(mid);
                            }
                        }
                        if (func_reg_map[cur_func_name].virtual_float_regs[i][r].st_id > itv.ed_id &&
                                func_reg_map[cur_func_name].virtual_float_regs[i][l].ed_id < itv.st_id) {  // j 在 itv 后面 并且是第一个
                            index = r;
                        }
                        else{
                            conflict = true;
                        }

                    }
                }
                if (!conflict) {
                    func_reg_map[cur_func_name].virtual_float_regs[i].insert(func_reg_map[cur_func_name].virtual_float_regs[i].begin() + index,
                                            itv);
                    itv.conflict_reg = i;
                    func_reg_map[cur_func_name].v2reg[itv.v].push_back(itv);
                    break;
                }

            }
        }
        else{
               for (int i = 0; i < virtual_reg_max; i++) {
                bool conflict = false;
                int index = 0;
                // LSRA_WARNNING("[sort in here] %d %d",func_reg_map[cur_func_name].virtual_int_regs[i].size()!=0,func_reg_map[cur_func_name].virtual_int_regs[i].empty());
                // LSRA_WARNNING("[sort in here] %d %d",func_reg_map[cur_func_name].virtual_int_regs[i].size(),func_reg_map[cur_func_name].virtual_int_regs[i].empty());
                if(func_reg_map[cur_func_name].virtual_int_regs[i].size()==0){
                    index = 0;
                    // LSRA_WARNNING("直接插入");
                }
                else{
                    // LSRA_WARNNING("判断插入");
                    int l = 0, r = func_reg_map[cur_func_name].virtual_int_regs[i].size()-1;
                    float ave1 = (func_reg_map[cur_func_name].virtual_int_regs[i][l].st_id+func_reg_map[cur_func_name].virtual_int_regs[i][l].ed_id)/2.0;
                    float ave2 = (func_reg_map[cur_func_name].virtual_int_regs[i][r].st_id+func_reg_map[cur_func_name].virtual_int_regs[i][r].ed_id)/2.0;
                    if(ave <= ave1 || ave >= ave2){
                        if(itv.ed_id<func_reg_map[cur_func_name].virtual_int_regs[i][l].st_id){
                            // LSRA_WARNNING("直接插入头");

                            index = l;
                        }
                        else if(itv.st_id>func_reg_map[cur_func_name].virtual_int_regs[i][r].ed_id ){
                            // LSRA_WARNNING("直接插入尾");
                            index = r+1;
                        }
                        else{
                            conflict = true;
                        }
                    }
                    else{
                        // WARNNING("二分查找");
                        while(r-l>1){
                            // LSRA_WARNNING("二分查找 %d %d",l,r);
                            float ave1 = (func_reg_map[cur_func_name].virtual_int_regs[i][l].st_id+func_reg_map[cur_func_name].virtual_int_regs[i][l].ed_id)/2.0;
                            float ave2 = (func_reg_map[cur_func_name].virtual_int_regs[i][r].st_id+func_reg_map[cur_func_name].virtual_int_regs[i][r].ed_id)/2.0;
                            float mid = (l+r)/2.0;
                            float mid_l = (func_reg_map[cur_func_name].virtual_int_regs[i][int(mid+0.5)].st_id+func_reg_map[cur_func_name].virtual_int_regs[i][int(mid+0.5)].ed_id)/2.0;
                            if(ave>ave1 && ave<mid_l){
                                r = int(mid+0.5);
                            }else{
                                l = int(mid);
                            }
                        }
                        if (func_reg_map[cur_func_name].virtual_int_regs[i][r].st_id > itv.ed_id &&
                                func_reg_map[cur_func_name].virtual_int_regs[i][l].ed_id < itv.st_id) {  // j 在 itv 后面 并且是第一个
                            index = r;
                        }
                        else{
                            conflict = true;
                        }

                    }
                    // LSRA_WARNNING("l = %d, r = %d insert pos %d reg %d", l, r, index,i);
                }
                // LSRA_WARNNING("[sort in here]");
                if (!conflict) {
                    // LSRA_WARNNING("insert pos %d reg %d", index,i);
                    func_reg_map[cur_func_name].virtual_int_regs[i].insert(func_reg_map[cur_func_name].virtual_int_regs[i].begin() + index,
                                            itv);
                    itv.conflict_reg = i;
                    func_reg_map[cur_func_name].v2reg[itv.v].push_back(itv);
                    break;
                }
            }
        }

    }

    int use_int_reg_num = 0;
    for (int i = 0; i < virtual_reg_max; i++) {
        if (func_reg_map[cur_func_name].virtual_int_regs[i].size() != 0) {
            use_int_reg_num ++ ;
        };
    }
    int use_float_reg_num = 0;
    for (int i = 0; i < virtual_reg_max; i++) {
        if (func_reg_map[cur_func_name].virtual_float_regs[i].size() != 0) {
            use_float_reg_num ++ ;
        };
    }
    LSRA_WARNNING("vir ireg %d freg %d\n",use_int_reg_num,use_float_reg_num);
    if(use_int_reg_num > virtual_reg_max - 10 || use_float_reg_num > virtual_reg_max - 10){
        ERROR("virtual reg is going to crash!",virtual_reg_full_error);
    }
    func_used_reg_map[func->getPrintName()].first = std::min(use_int_reg_num,int_reg_number);
    func_used_reg_map[func->getPrintName()].second = std::min(use_float_reg_num,float_reg_number);



    LSRA_WARNNING(" LSRA REG ALLOC");
    // debug
    for (int i = 0; i < virtual_reg_max; i++) {
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_int_regs[i].size(); j++) {
            LSRA_WARNNING(
                "[Ireg %d] [%d,%d] v:%s type:%s", i,
                func_reg_map[cur_func_name].virtual_int_regs[i][j].st_id, func_reg_map[cur_func_name].virtual_int_regs[i][j].ed_id,
                func_reg_map[cur_func_name].virtual_int_regs[i][j].v->getPrintName().c_str(),
                getType(func_reg_map[cur_func_name].virtual_int_regs[i][j].type).c_str());
        }
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_float_regs[i].size(); j++) {
            LSRA_WARNNING(
                "[Freg %d] [%d,%d] v:%s  type:%s", i,
                func_reg_map[cur_func_name].virtual_float_regs[i][j].st_id, func_reg_map[cur_func_name].virtual_float_regs[i][j].ed_id,
                func_reg_map[cur_func_name].virtual_float_regs[i][j].v->getPrintName().c_str(),
                getType(func_reg_map[cur_func_name].virtual_float_regs[i][j].type).c_str());
        }
    }
    LSRA_SHOW("-- [reg alloc int graph] --\n");
    for (int i = 0; i < virtual_reg_max; i++) {
        int t = 0;
        if (func_reg_map[cur_func_name].virtual_int_regs[i].size() == 0) continue;
        LSRA_SHOW("[reg %02d] ", i);
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_int_regs[i].size(); j++) {
            while (t < func_reg_map[cur_func_name].virtual_int_regs[i][j].st_id) {
                LSRA_SHOW(" ");
                t++;
            }
            while (t <= func_reg_map[cur_func_name].virtual_int_regs[i][j].ed_id) {
                LSRA_SHOW("#");
                t++;
            }
        }
        LSRA_SHOW("\n");
    }
    LSRA_SHOW("-- [reg alloc float graph] --\n");
    for (int i = 0; i < virtual_reg_max; i++) {
        int t = 0;
        if (func_reg_map[cur_func_name].virtual_float_regs[i].size() == 0) continue;
        LSRA_SHOW("[reg %02d] ", i);
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_float_regs[i].size(); j++) {
            while (t < func_reg_map[cur_func_name].virtual_float_regs[i][j].st_id) {
                LSRA_SHOW(" ");
                t++;
            }
            while (t <= func_reg_map[cur_func_name].virtual_float_regs[i][j].ed_id) {
                LSRA_SHOW("#");
                t++;
            }
        }
        LSRA_SHOW("\n");
    }

    // 虚拟寄存器使用点合并
    for (int i = 0; i < int_reg_number; i++) {
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_int_regs[i].size(); j++) {
            func_reg_map[cur_func_name].virtual_int_reg_use[i].insert(func_reg_map[cur_func_name].virtual_int_regs[i][j].use_id.begin(),
                                          func_reg_map[cur_func_name].virtual_int_regs[i][j].use_id.end());
        }
    }
    for (int i = 0; i < float_reg_number; i++) {
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_float_regs[i].size(); j++) {
            func_reg_map[cur_func_name].virtual_float_reg_use[i].insert(func_reg_map[cur_func_name].virtual_float_regs[i][j].use_id.begin(),
                                          func_reg_map[cur_func_name].virtual_float_regs[i][j].use_id.end());
        }
    }
    // debug
    // LSRA_WARNNING("-- int reg set --");
    // for (int i = 0; i < int_reg_number; i++) {
    //     LSRA_SHOW("[reg %02d]", i);
    //     for (auto reg : func_reg_map[cur_func_name].virtual_int_reg_use[i]) {
    //         LSRA_SHOW(" %02d ", reg);
    //     }
    //     LSRA_SHOW("[reg %02d]\n", i);
    // }
    // debug
    // LSRA_WARNNING("-- float reg set --");
    // for (int i = 0; i < float_reg_number; i++) {
    //     LSRA_SHOW("[reg %02d]", i);
    //     for (auto reg : func_reg_map[cur_func_name].virtual_float_reg_use[i]) {
    //         LSRA_SHOW(" %02d ", reg);
    //     }
    //     LSRA_SHOW("[reg %02d]\n", i);
    // }
    // 进行栈空间计算，首先分配函数传参从第五个开始，栈溢出的变量需要分配四个保护位置，替换寄存器值，分配alloc空间，分配ret，对齐
    for (auto &itv : live_range) {
        if (itv.type == interval_value_type::arg_var && itv.spilled) {
            itv.offset = (itv.specific_reg_idx) * 4;
            func_reg_map[cur_func_name].stack_size += 4;
            func_reg_map[cur_func_name].stack_map.push_back(itv);
        }
    }
    for (int i = 0; i < op_save_stack_num; i++) {
        func_reg_map[cur_func_name].op_save[i] = func_reg_map[cur_func_name].stack_size + i * 4;
    }
    func_reg_map[cur_func_name].stack_size += op_save_stack_num * 4;  //压入四个保护位置
    //计算变量栈溢出空间
    // int use_reg_num = 0;
    for (int i = int_reg_number; i < virtual_reg_max; i++) {
        if (func_reg_map[cur_func_name].virtual_int_regs[i].size() == 0) {
            break;
        } else {
            for (int j = 0; j < func_reg_map[cur_func_name].virtual_int_regs[i].size(); j++) {
                if (func_reg_map[cur_func_name].virtual_int_regs[i][j].type ==
                    interval_value_type::global_var)
                    continue;
                if (func_reg_map[cur_func_name].virtual_int_regs[i][j].type ==
                    interval_value_type::imm_var)
                    continue;
                func_reg_map[cur_func_name].virtual_int_regs[i][j].offset = func_reg_map[cur_func_name].stack_size;
                func_reg_map[cur_func_name].virtual_int_regs[i][j].spilled = true;
                func_reg_map[cur_func_name].stack_map.push_back(func_reg_map[cur_func_name].virtual_int_regs[i][j]);
            }
            func_reg_map[cur_func_name].stack_size += 4;
        }
    }
    for (int i = float_reg_number; i < virtual_reg_max; i++) {
        if (func_reg_map[cur_func_name].virtual_float_regs[i].size() == 0) {
            break;
        } else {
            for (int j = 0; j < func_reg_map[cur_func_name].virtual_float_regs[i].size(); j++) {
                if (func_reg_map[cur_func_name].virtual_float_regs[i][j].type ==
                    interval_value_type::global_var)
                    continue;
                if (func_reg_map[cur_func_name].virtual_float_regs[i][j].type ==
                    interval_value_type::imm_var)
                    continue;
                func_reg_map[cur_func_name].virtual_float_regs[i][j].offset = func_reg_map[cur_func_name].stack_size;
                func_reg_map[cur_func_name].virtual_float_regs[i][j].spilled = true;
                func_reg_map[cur_func_name].stack_map.push_back(func_reg_map[cur_func_name].virtual_float_regs[i][j]);
            }
            func_reg_map[cur_func_name].stack_size += 4;
        }
    }


    // alloca space
    for (auto &itv : live_range) {
        if (itv.is_allocated) {
            bool extended = true;
            auto sizeof_arg = itv.v->getType()->getSize(extended);
            sizeof_arg = ((sizeof_arg + 3) / 4) * 4;
            interval itv_data;
            itv_data.st_id = itv.st_id;
            itv_data.ed_id = itv.ed_id;
            itv_data.type = itv.type;
            itv_data.is_float = itv.v->getType()->isFloatTy();
            itv_data.v = itv.v;
            itv_data.is_data = true;
            itv_data.offset = func_reg_map[cur_func_name].stack_size;
            func_reg_map[cur_func_name].stack_size += sizeof_arg;
            func_reg_map[cur_func_name].stack_map.push_back(itv_data);
        }
    }
    // 分配 ret
    func_reg_map[cur_func_name].return_offset = func_reg_map[cur_func_name].stack_size;
    func_reg_map[cur_func_name].stack_size += 8;
    // 扩大栈空间
    // func_reg_map[cur_func_name].stack_size += 128;
    // 参数栈表示
    for (auto p : live_range) {
        if (p.type == interval_value_type::arg_var && p.spilled) {
            if(p.is_float){
                func_reg_map[cur_func_name].virtual_float_regs[virtual_reg_max-1].push_back(p);
            }else{
                func_reg_map[cur_func_name].virtual_int_regs[virtual_reg_max-1].push_back(p);
            }
        }
    }
    // debug
    LSRA_SHOW("-- stack size %d --\n", func_reg_map[cur_func_name].stack_size);
    #ifdef __LSRA_WARN

    for (auto itv : func_reg_map[cur_func_name].stack_map) {
        LSRA_WARNNING("type: %d V: %s -> sp + %d ", itv.is_data,
                      itv.v->getPrintName().c_str(), itv.offset);
    }
    #endif // DEBUG
    auto tmpBB = BasicBlock::create("");
    //栈溢出处理 检查栈溢出的变量的使用，在其前后插指令
    for (auto bb : func->getBasicBlocks()) {
        std::vector<std::pair<int, std::list<Instruction *>>> insert_instrs;
        int idx = 0;
        for (auto inst_iter = bb->getInstructions().begin();
                inst_iter != bb->getInstructions().end(); inst_iter++) {
            auto inst = *inst_iter;
            if (!inst->isCall() && !inst->isGep() && !inst->isBr()) {
                // call 主要是需要pop push 寄存器mov参数个数不定 ，
                // gep 参数个数不定 转成多个新的指令
                // br 插入前后需要CFG分析
                int op_idx = 0;
                std::pair<int, std::list<Instruction *>> before_inst;
                std::pair<int, std::list<Instruction *>> after_inst;
                before_inst.first = idx;
                if (op_in_inst_is_spilled(inst, inst))  // inst冲突考虑 左值
                {
                    int reg_get = give_reg_at(inst);
                    if (reg_get == -1) {
                        ERROR(
                            "can't give any reg because all the reg is "
                            "using",EXIT_CODE_ERROR_308);
                    }
                    Value *reg_v = value_in_reg_at(inst, reg_get,inst->getType()->isFloatTy());
                    if (reg_v != nullptr) {  // 说明占用了寄存器
                        //!! 插入一条冲突寄存器分配到func_reg_map[cur_func_name]->virtual_int_regs
                        //!! 做好冲突维护
                        // store reg_get to reg_save
                        before_inst.second.push_back(
                            StoreInst::createStore(
                                reg_v,
                                ConstantInt::get(func_reg_map[cur_func_name].op_save[op_idx]),
                                tmpBB,
                                true
                            ));
                        // load reg_get from func_reg_map[cur_func_name].stack_map 不需要！
                        int offset = get_value_sp_offset(inst, inst);
                        // before_inst.second.push_back(
                            // LoadInst::createLoad(
                            //     reg_v,
                            //     ConstantInt::get(offset),
                            //     tmpBB,
                                    // true
                            // ));
                        //---------------------------
                        // store reg_get to func_reg_map[cur_func_name].stack_map
                        after_inst.second.push_back(
                            StoreInst::createStore(
                                reg_v,
                                ConstantInt::get(offset),
                                tmpBB,
                                true
                            ));
                        // load reg_get from reg_save
                        after_inst.second.push_back(
                            LoadInst::createLoad(
                                reg_v,
                                ConstantInt::get(func_reg_map[cur_func_name].op_save[op_idx]),
                                tmpBB,
                                true
                            ));

                    } else {
                        // load reg_get from func_reg_map[cur_func_name].stack_map 不需要~!
                        int offset = get_value_sp_offset(inst, inst);
                        // before_inst.second.push_back(
                            // LoadInst::createLoad(
                            //     inst,
                            //     ConstantInt::get(offset),
                            //     tmpBB,
                                    // true
                            // ));
                        //---------------------------
                        // store reg_get to func_reg_map[cur_func_name].stack_map
                        after_inst.second.push_back(
                            StoreInst::createStore(
                                inst,
                                ConstantInt::get(offset),
                                tmpBB,
                                true
                            ));
                    }
                    interval itv;
                    if(inst->isMOV()){
                        itv.st_id = func_reg_map[cur_func_name].linear_map[inst];
                        itv.ed_id = func_reg_map[cur_func_name].linear_map[inst];
                        itv.v = dynamic_cast<MovInstr*>(inst)->getLVal();
                        itv.is_float = itv.v->getType()->isFloatTy();
                    }
                    else{
                        itv.st_id = func_reg_map[cur_func_name].linear_map[inst];
                        itv.ed_id = func_reg_map[cur_func_name].linear_map[inst];
                        itv.v = inst;
                    }
                    if(itv.v->getType()->isFloatTy()){
                        itv.is_float = true;
                        func_reg_map[cur_func_name].virtual_float_regs[reg_get].push_back(itv);
                        itv.conflict_reg = reg_get;
                        func_reg_map[cur_func_name].v2reg[itv.v].push_back(itv);
                    }
                    else{
                        func_reg_map[cur_func_name].virtual_int_regs[reg_get].push_back(itv);
                        itv.conflict_reg = reg_get;
                        func_reg_map[cur_func_name].v2reg[itv.v].push_back(itv);
                    }
                }
                op_idx += 1;
                for (auto op : inst->getOperandList()) {
                    if (op_idx >= op_save_stack_num) {
                        ERROR("overwrite op_idx",EXIT_CODE_ERROR_309);
                    }
                    if (op_in_inst_is_spilled(inst, op)) {  // 冲突 右值
                        int reg_get = give_reg_at(inst);
                        if (reg_get == -1) {
                            ERROR(
                                "can't give any reg because all the reg is "
                                "using",EXIT_CODE_ERROR_310);
                        }

                        auto global = dynamic_cast<GlobalVariable *>(op);
                        if (global ||
                            op->isConstant()) {  // 全局变量，给一个寄存器，并把值存栈
                            Value *reg_v =
                                value_in_reg_at(inst, reg_get,op->getType()->isFloatTy());
                            if (reg_v != nullptr) {  // 说明占用了寄存器
                                //!!
                                //!插入一条冲突寄存器分配到func_reg_map[cur_func_name]->virtual_int_regs
                                //!! 做好冲突维护
                                // store reg_get to reg_save
                                before_inst.second.push_back(
                                    StoreInst::createStore(
                                        reg_v,
                                        ConstantInt::get(func_reg_map[cur_func_name].op_save[op_idx]),
                                        tmpBB,
                                        true
                                    ));
                                //---------------------------
                                // load reg_get from reg_save
                                after_inst.second.push_back(
                                    LoadInst::createLoad(
                                        reg_v,
                                        ConstantInt::get(func_reg_map[cur_func_name].op_save[op_idx]),
                                        tmpBB,
                                        true
                                    ));
                            } else {  //没占用寄存器，直接拿着用，立即数不用存
                                //!!
                                //!插入一条冲突寄存器分配到func_reg_map[cur_func_name]->virtual_int_regs
                                //---------------------------
                                //!! 做好冲突维护
                            }
                        } else {
                            Value *reg_v =
                                value_in_reg_at(inst, reg_get,op->getType()->isFloatTy());
                            if (reg_v != nullptr) {  // 说明占用了寄存器
                                //!!
                                //!插入一条冲突寄存器分配到func_reg_map[cur_func_name]->virtual_int_regs
                                //!! 做好冲突维护
                                // store reg_get to reg_save
                                before_inst.second.push_back(
                                    StoreInst::createStore(
                                        reg_v,
                                        ConstantInt::get(func_reg_map[cur_func_name].op_save[op_idx]),
                                        tmpBB,
                                        true
                                    ));
                                // load reg_get from func_reg_map[cur_func_name].stack_map
                                int offset =
                                    get_value_sp_offset(inst, op);
                                before_inst.second.push_back(
                                    LoadInst::createLoad(
                                        reg_v,
                                        ConstantInt::get(offset),
                                        tmpBB,
                                        true
                                    ));
                                //---------------------------
                                // store reg_get to func_reg_map[cur_func_name].stack_map 不需要！
                                // after_inst.second.push_back(
                                    // StoreInst::createStore(
                                    //     reg_v,
                                    //     ConstantInt::get(offset),
                                    //     tmpBB,
                                            // true
                                    // ));
                                // load reg_get from reg_save
                                after_inst.second.push_back(
                                    LoadInst::createLoad(
                                        reg_v,
                                        ConstantInt::get(func_reg_map[cur_func_name].op_save[op_idx]),
                                        tmpBB,
                                        true
                                    ));
                            } else {
                                // load reg_get from func_reg_map[cur_func_name].stack_map
                                int offset =
                                    get_value_sp_offset(inst, op);
                                before_inst.second.push_back(
                                    LoadInst::createLoad(
                                        op,
                                        ConstantInt::get(offset),
                                        tmpBB,
                                        true
                                    ));
                                //---------------------------
                                // store reg_get to func_reg_map[cur_func_name].stack_map 不需要！
                                // after_inst.second.push_back(
                                    // StoreInst::createStore(
                                    //     op,
                                    //     ConstantInt::get(offset),
                                    //     tmpBB,
                                    //        true
                                    // ));
                            }
                        }
                        interval itv;
                        if(dynamic_cast<MovInstr*>(op)){
                            itv.st_id = func_reg_map[cur_func_name].linear_map[inst];
                            itv.ed_id = func_reg_map[cur_func_name].linear_map[inst];
                            itv.v = dynamic_cast<MovInstr*>(op)->getLVal();
                            itv.is_float = itv.v->getType()->isFloatTy();
                        }
                        else{
                            itv.st_id = func_reg_map[cur_func_name].linear_map[inst];
                            itv.ed_id = func_reg_map[cur_func_name].linear_map[inst];
                            itv.v = op;
                        }
                        itv.type = interval_value_type::local_var;
                        if(itv.v->getType()->isFloatTy()){
                            itv.is_float = true;
                            func_reg_map[cur_func_name].virtual_float_regs[reg_get].push_back(itv);
                            itv.conflict_reg = reg_get;
                            func_reg_map[cur_func_name].v2reg[itv.v].push_back(itv);
                        }
                        else{
                            func_reg_map[cur_func_name].virtual_int_regs[reg_get].push_back(itv);
                            itv.conflict_reg = reg_get;
                            func_reg_map[cur_func_name].v2reg[itv.v].push_back(itv);
                        }
                    }
                    op_idx += 1;
                }

                for (auto insert_inst : before_inst.second) {
                    func_reg_map[cur_func_name].linear_map[insert_inst] = func_reg_map[cur_func_name].linear_map[inst];
                }
                for (auto insert_inst : after_inst.second) {
                    func_reg_map[cur_func_name].linear_map[insert_inst] = func_reg_map[cur_func_name].linear_map[inst];
                }
                insert_instrs.push_back(before_inst);
                idx += before_inst.second.size();
                idx++;
                after_inst.first = idx;
                insert_instrs.push_back(after_inst);
                idx += after_inst.second.size();
                // debug begin
                // LSRA_WARNNING("-- insert inst --");
                // if (!before_inst.second.empty()) {
                //     LSRA_WARNNING(" before instr %d ", before_inst.first);
                //     for (auto inst : before_inst.second) {
                //         auto inststore = dynamic_cast<StoreInst *>(inst);
                //         if (inststore)
                //             LSRA_WARNNING("store, v : %s , offset : %d",
                //                           inststore->getOperandList()[0]
                //                               ->getPrintName()
                //                               .c_str(),
                //                           inststore->offset);
                //         auto instload = dynamic_cast<LoadInst *>(inst);
                //         if (instload)
                //             LSRA_WARNNING("load, v : %s , offset : %d",
                //                           instload->getOperandList()[0]
                //                               ->getPrintName()
                //                               .c_str(),
                //                           instload->offset);
                //     }
                // }
                // if (!after_inst.second.empty()) {
                //     // LSRA_WARNNING("-- insert inst --");
                //     LSRA_WARNNING(" after instr %d ", after_inst.first);
                //     for (auto inst : after_inst.second) {
                //         auto inststore = dynamic_cast<StoreInst *>(inst);
                //         if (inststore)
                //             LSRA_WARNNING("store, v : %s , offset : %d",
                //                           inststore->getOperandList()[0]
                //                               ->getPrintName()
                //                               .c_str(),
                //                           inststore->offset);
                //         auto instload = dynamic_cast<LoadInst *>(inst);
                //         if (instload)
                //             LSRA_WARNNING("load, v : %s , offset : %d",
                //                           instload->getOperandList()[0]
                //                               ->getPrintName()
                //                               .c_str(),
                //                           instload->offset);
                //     }
                // }

                // debug end
            } else {
                idx++;
            }
        }
        // 处理插入
        for (auto insert_inst : insert_instrs) {
            auto inst_iter = bb->getInstructions().begin();
            for (int id = 0; inst_iter != bb->getInstructions().end();
                    inst_iter++, id++) {
                if (id == insert_inst.first) {
                    break;
                }
            }
            bb->_instructions.insert(inst_iter,
                                        insert_inst.second.begin(),
                                        insert_inst.second.end());
            if (insert_inst.second.size() > 0) {
                // LSRA_WARNNING("[INSERT] %d %d", insert_inst.first,
                                // insert_inst.second.size());
            }
        }
    }

    LSRA_WARNNING("Lsra finish.");
}

Value *AsmBuilder::value_in_reg_at(
    Value *inst, int reg_idx, bool is_fp) {  // 查看当前寄存器分配的变量
    int tag = func_reg_map[cur_func_name].linear_map[inst];

    if(is_fp){
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_float_regs[reg_idx].size(); j++) {
            if (tag >= func_reg_map[cur_func_name].virtual_float_regs[reg_idx][j].st_id &&
                tag <= func_reg_map[cur_func_name].virtual_float_regs[reg_idx][j].ed_id) {
                // LSRA_WARNNING("replace
                // %s",func_reg_map[cur_func_name].virtual_int_regs[reg_idx][j].v->getPrintName().c_str());;
                return func_reg_map[cur_func_name].virtual_float_regs[reg_idx][j].v;
            }
        }
    }
    else{
        for (int j = 0; j < func_reg_map[cur_func_name].virtual_int_regs[reg_idx].size(); j++) {
            if (tag >= func_reg_map[cur_func_name].virtual_int_regs[reg_idx][j].st_id &&
                tag <= func_reg_map[cur_func_name].virtual_int_regs[reg_idx][j].ed_id) {
                // LSRA_WARNNING("replace
                // %s",func_reg_map[cur_func_name].virtual_int_regs[reg_idx][j].v->getPrintName().c_str());;
                return func_reg_map[cur_func_name].virtual_int_regs[reg_idx][j].v;
            }
        }
    }

    return nullptr;
}

int AsmBuilder::give_reg_at(Value *inst) {  // 请求分配寄存器
    int tag = func_reg_map[cur_func_name].linear_map[inst];
    if(inst->getType()->isFloatTy()){
        for (int i = 0; i < float_reg_number; i++) {
            if (func_reg_map[cur_func_name].virtual_float_reg_use[i].find(tag) ==
                func_reg_map[cur_func_name].virtual_float_reg_use[i]
                    .end()) {  // 没找到使用点说明，不冲突，暂时如下
                func_reg_map[cur_func_name].virtual_float_reg_use[i].insert(
                    tag);  //表示此处已经有使用需求了，防止再次请求
                return i;
            }
        }
    }
    else{
        for (int i = 0; i < int_reg_number; i++) {
            if (func_reg_map[cur_func_name].virtual_int_reg_use[i].find(tag) ==
                func_reg_map[cur_func_name].virtual_int_reg_use[i]
                    .end()) {  // 没找到使用点说明，不冲突，暂时如下
                func_reg_map[cur_func_name].virtual_int_reg_use[i].insert(
                    tag);  //表示此处已经有使用需求了，防止再次请求
                return i;
            }
        }
    }

    return -1;
}

int AsmBuilder::give_used_reg_at(Value *inst) {  // 分配使用过的寄存器
    int tag = func_reg_map[cur_func_name].linear_map[inst];
    if(inst->getType()->isFloatTy()){
        for (int i = 0; i < float_reg_number; i++) {
            if (func_reg_map[cur_func_name].virtual_float_reg_use[i].size()>0 && func_reg_map[cur_func_name].virtual_float_reg_use[i].find(tag) ==
                func_reg_map[cur_func_name].virtual_float_reg_use[i].end()) {  // 没找到使用点说明，不冲突，暂时如下
                func_reg_map[cur_func_name].virtual_float_reg_use[i].insert(
                    tag);  //表示此处已经有使用需求了，防止再次请求
                return i;
            }
        }
    }
    else{
        for (int i = 0; i < int_reg_number; i++) {
            if (func_reg_map[cur_func_name].virtual_int_reg_use[i].size()>0 && func_reg_map[cur_func_name].virtual_int_reg_use[i].find(tag) ==
                func_reg_map[cur_func_name].virtual_int_reg_use[i].end()) {  // 没找到使用点说明，不冲突，暂时如下
                func_reg_map[cur_func_name].virtual_int_reg_use[i].insert(
                    tag);  //表示此处已经有使用需求了，防止再次请求
                return i;
            }
        }
    }

    return -1;
}

bool AsmBuilder::op_in_inst_is_spilled(Value *inst, Value *op) {//#
    // LSRA_WARNNING("op_in_inst_is_spilled");
    if(dynamic_cast<MovInstr *>(op))op = dynamic_cast<MovInstr *>(op)->getLVal();//MOV转化
    int tag = func_reg_map[cur_func_name].linear_map[inst];

    // for(auto itv: func_reg_map[cur_func_name].v2reg[op]){
    //     LSRA_WARNNING("%s value %d %d ", op->getPrintName().c_str(), itv.st_id,itv.ed_id);
    //     if (tag >= itv.st_id &&
    //         tag <= itv.ed_id) {

    //         if(op->getType()->isFloatTy()){
    //             if(itv.conflict_reg>=float_reg_number)
    //                 return true;
    //         }
    //         else{
    //             if(itv.conflict_reg>=int_reg_number)
    //                 return true;
    //         }
    //     }
    // }

    if(op->getType()->isFloatTy()){
        for (int i = float_reg_number; i < virtual_reg_max; i++) {
            for (int j = 0; j < func_reg_map[cur_func_name].virtual_float_regs[i].size(); j++) {
                if (func_reg_map[cur_func_name].virtual_float_regs[i][j].v == op &&
                    tag >= func_reg_map[cur_func_name].virtual_float_regs[i][j].st_id &&
                    tag <= func_reg_map[cur_func_name].virtual_float_regs[i][j].ed_id) {
                    return true;
                }
            }
        }
    }
    else{
        for (int i = int_reg_number; i < virtual_reg_max; i++) {
            for (int j = 0; j < func_reg_map[cur_func_name].virtual_int_regs[i].size(); j++) {
            //     if(func_reg_map[cur_func_name].virtual_int_regs[i][j].v == op){
                    // LSRA_WARNNING("inst %s op %s, itv[%d,%d] tag:%d",inst->getPrintName().c_str(),func_reg_map[cur_func_name].virtual_int_regs[i][j].v->getPrintName().c_str(),
                    // func_reg_map[cur_func_name].virtual_int_regs[i][j].st_id,func_reg_map[cur_func_name].virtual_int_regs[i][j].ed_id,tag);
                // }
                if (func_reg_map[cur_func_name].virtual_int_regs[i][j].v == op &&
                    tag >= func_reg_map[cur_func_name].virtual_int_regs[i][j].st_id &&
                    tag <= func_reg_map[cur_func_name].virtual_int_regs[i][j].ed_id) {
                    return true;
                }
            }
        }
    }
    return false;
}

int AsmBuilder::get_value_sp_offset(Value *inst,
                                        Value *op) {  // 查看变量在栈上的偏移
    if(dynamic_cast<MovInstr *>(op))op = dynamic_cast<MovInstr *>(op)->getLVal();//MOV转化

    // LSRA_WARNNING("ask stack offset of %s", op->getPrintName().c_str());
    int tag = func_reg_map[cur_func_name].linear_map[inst];
    for (int i = 0; i < func_reg_map[cur_func_name].stack_map.size(); i++) {
        if (func_reg_map[cur_func_name].stack_map[i].v == op && tag >= func_reg_map[cur_func_name].stack_map[i].st_id &&
            tag <= func_reg_map[cur_func_name].stack_map[i].ed_id) {
            return func_reg_map[cur_func_name].stack_map[i].offset;
        }
    }
    ERROR("can't find op %s in stack maybe its not in stack",
          EXIT_CODE_ERROR_311,op->getPrintName().c_str());
    return -1;
}

void AsmBuilder::live_interval_analysis(Function *func, bool insert) {
    LSRA_WARNNING("[live_interval_analysis func : %s]",func->getPrintName().c_str());
    cur_func_name = func->getPrintName();
    if(!func_reg_map.count(cur_func_name)){ //第一次
        func_reg_map[cur_func_name] = reg_map();
        func_reg_map[cur_func_name].return_offset = 0;
        func_reg_map[cur_func_name].stack_size = 0;
        for (int i = 0; i < virtual_reg_max; i++) {
            func_reg_map[cur_func_name].virtual_int_regs[i].clear();
            func_reg_map[cur_func_name].virtual_int_reg_use[i].clear();
            func_reg_map[cur_func_name].virtual_float_regs[i].clear();
            func_reg_map[cur_func_name].virtual_float_reg_use[i].clear();
        }
        func_reg_map[cur_func_name].stack_map.clear();
    }
    else{
        return;
    }
    // int instr_index = 0;
    int linear_index = 0;
    int bb_index = 0;
    std::map<Value *, interval> live_map;
    std::map<BasicBlock *, std::set<Value *>> live_in, live_out;
    std::map<BasicBlock *, int> bb_map;
    std::vector<interval> loop_set;
    std::vector<interval> live_res;  // 对于结果进行后处理
    if (func->getBasicBlocks().empty()) {
        func_reg_map[cur_func_name] = reg_map();
        func_used_reg_map[cur_func_name] = std::pair<int,int>(0,0);
        func_reg_map[cur_func_name].return_offset = 0;
        func_reg_map[cur_func_name].stack_size = 0;
        LSRA_WARNNING("[live_interval_analysis func : %s exit]",func->getPrintName().c_str());
        return;
    }
    LSRA_WARNNING(
        "-------------------------------------------------------[LSRA]scan "
        "func: %s",
        func->getPrintName().c_str());
    // find all vars
    int int_arg_idx = 0;
    int float_arg_idx = 0;
    int stack_arg_idx = 0;
    for (auto args : func->getArgs()) {
        if (!live_map.count(args))  // arg interval init
        {
            live_map[args].st_id = -1;
            live_map[args].ed_id = -1;
            live_map[args].def = true;
            live_map[args].is_float = args->getType()->isFloatTy();
            if (int_arg_idx < 4 && !live_map[args].is_float) {
                live_map[args].type = interval_value_type::arg_var;
                live_map[args].specific_reg_idx = int_arg_idx++;
            }
            else if(float_arg_idx<16 && live_map[args].is_float){
                live_map[args].type = interval_value_type::arg_var;
                live_map[args].specific_reg_idx = float_arg_idx++;
            }
            else {  // 放在栈上
                live_map[args].type = interval_value_type::arg_var;

                live_map[args].specific_reg_idx = stack_arg_idx++;

                live_map[args].spilled = true;
            }
            // 还要考虑
        }
    }
    // linear list 对于MOV指令需要特殊处理，需要使用phi进行映射
    // LSRA_WARNNING(" LINEAR LIST");
    std::queue<BasicBlock *>bbq;
    bbq.push(func->getEntryBlock());
    while(!bbq.empty()){
        auto bb = bbq.front();
        bbq.pop();
        if(bb_map.count(bb)){
            continue;
        }
        LSRA_WARNNING(" [BFS] basic block: %s",bb->getPrintName().c_str());
        for(auto b: bb->getSuccBasicBlocks()) bbq.push(b);
        bb_map[bb] = bb_index++;
        for (auto inst : bb->getInstructions()) {
            if (inst->getType()->getSize() > 0) {
                if(inst->isMOV()){
                    auto mov_inst = dynamic_cast<MovInstr *>(inst);
                    if(!live_map.count(mov_inst->getLVal())){ //对于phi进行变量区间分析
                        live_map[mov_inst->getLVal()].def = true;
                        live_map[mov_inst->getLVal()].st_id = linear_index;
                        live_map[mov_inst->getLVal()].ed_id = linear_index;
                        live_map[mov_inst->getLVal()].is_float = mov_inst->getLVal()->getType()->isFloatTy();
                    }
                    else{
                        live_map[mov_inst->getLVal()].def = true;
                        live_map[mov_inst->getLVal()].ed_id = linear_index;
                    }
                    LSRA_WARNNING("is mov %d %d",live_map[mov_inst->getLVal()].st_id,live_map[mov_inst->getLVal()].ed_id);
                }
                else if (!live_map.count(inst))  //其余 first time
                {
                    if (inst->isAlloca()) {
                        live_map[inst].is_allocated = true;
                    }
                    live_map[inst].def = true;
                    live_map[inst].st_id = linear_index;
                    live_map[inst].ed_id = linear_index;
                    live_map[inst].is_float = inst->getType()->isFloatTy();
                }
            }
            if(!func_reg_map[cur_func_name].linear_map.count(inst)){ // 此处需要对不同mov加以区分
                func_reg_map[cur_func_name].linear_map[inst] = linear_index++;
                if(dynamic_cast<MovInstr*>(inst))
                    LSRA_WARNNING("scan Line :[%d:%s]",func_reg_map[cur_func_name].linear_map[inst],dynamic_cast<MovInstr*>(inst)->getLVal()->getPrintName().c_str());
                else
                    LSRA_WARNNING("scan Line :[%d:%s]",func_reg_map[cur_func_name].linear_map[inst],inst->getPrintName().c_str());
            }
        }
    }
    // 成环定义：如果一个块的前驱节点是大于等于自身的则形成环路，且该环路的上下界可知
    std::map<int,int> rec;
    for (auto bb : func->getBasicBlocks()) {
        for (auto pb : bb->getPreBasicBlocks()) {
            if (bb_map[pb] >= bb_map[bb]) {
                LSRA_WARNNING(" LOOP BLOCK CHECK: bb %d %s -- bb %d %s",
                              bb_map[bb], bb->getPrintName().c_str(),
                              bb_map[pb], pb->getPrintName().c_str());
                LSRA_WARNNING(" LOOP CHECK: from %d to %d",
                              func_reg_map[cur_func_name].linear_map[bb->getInstructions().front()],
                              func_reg_map[cur_func_name].linear_map[pb->getInstructions().back()]);
                interval itv;
                itv.st_id = func_reg_map[cur_func_name].linear_map[bb->getInstructions().front()];
                itv.ed_id = func_reg_map[cur_func_name].linear_map[pb->getInstructions().back()];
                loop_set.push_back(itv);
                rec[itv.st_id*100000+itv.ed_id]=1;
            }
        }
    }
    // 环合并 处理环路之间的交叉情况
    std::vector<interval> tmp_loop=loop_set;
    std::vector<interval> res_loop;
    LSRA_WARNNING("size %d",tmp_loop.size());
    while(tmp_loop.size()){
        for(int i=0;i<tmp_loop.size()-1;i++){
            for(int j=i+1;j<tmp_loop.size();j++){
                if((tmp_loop[i].ed_id >= tmp_loop[j].st_id && tmp_loop[i].ed_id <= tmp_loop[j].ed_id
                && tmp_loop[i].st_id >= tmp_loop[j].st_id && tmp_loop[i].st_id <= tmp_loop[j].ed_id)
                || (tmp_loop[j].ed_id >= tmp_loop[i].st_id && tmp_loop[j].ed_id <= tmp_loop[i].ed_id
                && tmp_loop[j].st_id >= tmp_loop[i].st_id && tmp_loop[j].st_id <= tmp_loop[i].ed_id)){
                    continue;
                }
                if((tmp_loop[i].ed_id >= tmp_loop[j].st_id && tmp_loop[i].ed_id <= tmp_loop[j].ed_id
                && tmp_loop[j].st_id >= tmp_loop[i].st_id && tmp_loop[j].st_id <= tmp_loop[i].ed_id)
                || (tmp_loop[j].ed_id >= tmp_loop[i].st_id && tmp_loop[j].ed_id <= tmp_loop[i].ed_id
                && tmp_loop[i].st_id >= tmp_loop[j].st_id && tmp_loop[i].st_id <= tmp_loop[j].ed_id)){
                    interval itv;
                    itv.st_id = std::min(tmp_loop[i].st_id,tmp_loop[j].st_id);
                    itv.ed_id = std::max(tmp_loop[i].ed_id,tmp_loop[j].ed_id);
                    if(rec[itv.st_id*100000+itv.ed_id])continue;
                    //LSRA_WARNNING(" LOOP CHECK: from %d to %d",itv.st_id,itv.ed_id);
                    //getchar();
                    rec[itv.st_id*100000+itv.ed_id]=1;
                    res_loop.push_back(itv);
                }
            }
        }
        if(res_loop.empty()){
            break;
        }
        LSRA_WARNNING("next turn");
        loop_set.insert(loop_set.end(),res_loop.begin(),res_loop.end());
        tmp_loop.clear();
        tmp_loop = res_loop;
        res_loop.clear();
    }
    for(auto lp: loop_set){
        LSRA_WARNNING(" LOOP CHECK: from %d to %d",lp.st_id,lp.ed_id);
    }
    LSRA_WARNNING("LOOP CHECK DONE");
    // cal ls
    for (auto bb : func->getBasicBlocks()) {
        // LSRA_WARNNING("linear scan bb %s",bb->getPrintName().c_str());
        for (auto inst : bb->getInstructions()) {
            // LSRA_WARNNING("linear scan instr
            // %s",inst->getPrintName().c_str());
            if (inst->isBr()) {
                if (inst->getOperandList().size() <= 1) {
                    continue;
                }
                auto op = inst->getOperandList()[0];

                auto global = dynamic_cast<GlobalVariable *>(op);
                auto const_int = dynamic_cast<ConstantInt *>(op);
                if (global) {
                    live_map[op].type = interval_value_type::global_var;
                } else if (const_int) {
                    live_map[op].type = interval_value_type::imm_var;
                }
                //
                if(dynamic_cast<MovInstr *>(op) && func_reg_map[cur_func_name].linear_map.count(op)){ // 变量必有左值定义
                    auto op_inst = dynamic_cast<MovInstr *>(op);
                    if(op_inst){
                        op = op_inst->getLVal(); //!!进行变量映射 映射为对应PHI
                    }
                }
                //
                if (!live_map.count(op))  // first time
                {
                    live_map[op].st_id = func_reg_map[cur_func_name].linear_map[inst];
                    live_map[op].ed_id = func_reg_map[cur_func_name].linear_map[inst];
                    live_map[op].is_float = op->getType()->isFloatTy();
                } else {
                    if (func_reg_map[cur_func_name].linear_map[inst] > live_map[op].ed_id) {
                        live_map[op].ed_id = func_reg_map[cur_func_name].linear_map[inst];
                    }
                    if (func_reg_map[cur_func_name].linear_map[inst] < live_map[op].st_id) {
                        live_map[op].st_id = func_reg_map[cur_func_name].linear_map[inst];
                    }
                }
                for (auto itv : loop_set) {
                    if (func_reg_map[cur_func_name].linear_map[inst] <= itv.ed_id &&
                        func_reg_map[cur_func_name].linear_map[inst] >=
                            itv.st_id) {  // 该指令在该循环中使用
                        if (live_map[op].def &&
                            live_map[op].st_id < itv.st_id &&
                            live_map[op].ed_id < itv.ed_id &&
                            live_map[op].ed_id >= itv.st_id ) {  // 该变量是定义且使用过的变量
                            live_map[op].ed_id = itv.ed_id;
                            live_map[op].weight += 0.2;
                        }
                    }
                }
                live_map[op].use_id.insert(func_reg_map[cur_func_name].linear_map[inst]);
            } else {
                int op_id = 0;
                int int_param_id=0;
                int float_param_id=0;
                for (auto op : inst->getOperandList()) {
                    // ?
                    if(inst->isCall() && op_id == 0) {
                        {
                            interval itv;
                            itv.st_id = func_reg_map[cur_func_name].linear_map[inst];
                            itv.ed_id = func_reg_map[cur_func_name].linear_map[inst];
                            itv.def = true;
                            itv.type = interval_value_type::call_val;
                            itv.is_float = op->getType()->isFloatTy();
                            itv.specific_reg_idx = 12;
                            itv.v = op;
                            itv.weight = 10000;
                            live_res.push_back(itv);
                        }

                        {
                            interval itv;
                            itv.st_id = func_reg_map[cur_func_name].linear_map[inst];
                            itv.ed_id = func_reg_map[cur_func_name].linear_map[inst];
                            itv.def = true;
                            itv.type = interval_value_type::call_val;
                            itv.is_float = op->getType()->isFloatTy();
                            itv.specific_reg_idx = 14;
                            itv.v = op;
                            itv.weight = 10000;
                            live_res.push_back(itv);
                        }
                        op_id++;
                        continue;
                    }
                    if(inst->isCall() && op_id != 0) {
                        if(op->getType()->isFloatTy()){
                            if(float_param_id<16){
                                live_map[op].weight = (16-float_param_id) * 100;
                                interval itv;
                                itv.st_id = func_reg_map[cur_func_name].linear_map[inst];
                                itv.ed_id = func_reg_map[cur_func_name].linear_map[inst];
                                itv.def = true;
                                itv.type = interval_value_type::call_val;
                                itv.is_float = op->getType()->isFloatTy();
                                itv.specific_reg_idx = float_param_id;
                                itv.v = op;
                                itv.weight = 10000;
                                live_res.push_back(itv);
                            }
                            float_param_id+=1;
                        }
                        else{
                            if(int_param_id<4 || int_param_id==12 || int_param_id == 14){
                                live_map[op].weight = (4-int_param_id) * 100;
                                interval itv;
                                itv.st_id = func_reg_map[cur_func_name].linear_map[inst];
                                itv.ed_id = func_reg_map[cur_func_name].linear_map[inst];
                                itv.def = true;
                                itv.type = interval_value_type::call_val;
                                itv.is_float = op->getType()->isFloatTy();
                                itv.specific_reg_idx = int_param_id;
                                itv.v = op;
                                itv.weight = 10000;
                                live_res.push_back(itv);
                            }
                            int_param_id+=1;
                        }
                    }
                    if (inst->isMOV() && op_id == 0 && op->isConstant()) {
                        op_id++;
                        continue;
                    }
                    if((inst->isAdd() || inst->isSub() || inst->isDiv() || inst->isCmp() ||
                        inst->isAshr() || inst->isLshr() || inst->isShl() || inst->isAnd() || inst->isOr())
                     && op_id == 1 && op->isConstant() && !op->getType()->isFloatTy()) {
                        op_id++;
                        continue;
                    }
                    if((inst->isRem()) && (op_id == 0 || op_id == 1 )
                    && op->isConstant() && !op->getType()->isFloatTy() ) {
                        op_id++;
                        continue;
                    }
                    if (inst->isLoad() && dynamic_cast<LoadInst *>(inst)->isLoadOff() && op_id == 1) {
                        op_id++;
                        continue;
                    }
                    if (inst->isStore() && dynamic_cast<StoreInst *>(inst)->isStoreOff() && op_id == 1) {
                        op_id++;
                        continue;
                    }
                    if (inst->isMla() && op_id == 0 && inst->getOperand(0)->isConstant() &&
                            atoi(inst->getOperand(0)->getPrintName().c_str()) == 1) {
                        op_id++;
                        continue;
                    }
                    if (inst->isMOV() && op_id == 0 && inst->getOperand(0)->isConstant()) {
                        op_id++;
                        continue;
                    }
                    if (op == nullptr){
                        op_id++;
                        continue;
                    }
                    assert(op != nullptr);

                    auto global = dynamic_cast<GlobalVariable *>(op);
                    auto const_int = dynamic_cast<ConstantInt *>(op);
                    if (global) {
                        live_map[op].type = interval_value_type::global_var;
                        // auto array = dynamic_cast<ConstantArray *>(op);
                        // if(         array&&
                        // static_cast<ArrayType*>(array->getType())->getElementType()->isIntegerTy()) {
                        //     live_map[op].def = true; // INT 全局变量活跃性分析 FLOAT 全局变量视为立即数
                        // }
                    } else if (const_int) {
                        live_map[op].type = interval_value_type::imm_var;
                    }
                    //
                    if(func_reg_map[cur_func_name].linear_map.count(op)){ // 变量必有左值定义
                        auto op_inst = dynamic_cast<MovInstr *>(op);
                        if(op_inst){
                            op = op_inst->getLVal(); //!!进行变量映射 映射为对应PHI
                        }
                    }
                    // ?
                    if (!live_map.count(op))  // first time
                    {
                        if(dynamic_cast<PhiInstr*>(op)) ERROR("mov lsra wrong",1);
                        live_map[op].st_id = func_reg_map[cur_func_name].linear_map[inst];
                        live_map[op].ed_id = func_reg_map[cur_func_name].linear_map[inst];
                        live_map[op].is_float = op->getType()->isFloatTy();
                    } else {
                        if (func_reg_map[cur_func_name].linear_map[inst] > live_map[op].ed_id) {
                            live_map[op].ed_id = func_reg_map[cur_func_name].linear_map[inst];
                        }
                        if (func_reg_map[cur_func_name].linear_map[inst] < live_map[op].st_id) {
                            live_map[op].st_id = func_reg_map[cur_func_name].linear_map[inst];
                        }
                    }
                    for (auto itv : loop_set) {
                        if (func_reg_map[cur_func_name].linear_map[inst] <= itv.ed_id &&
                            func_reg_map[cur_func_name].linear_map[inst] >=
                                itv.st_id) {  // 该指令在该循环中使用
                            if (live_map[op].def &&
                            live_map[op].st_id < itv.st_id &&
                            live_map[op].ed_id < itv.ed_id &&
                            live_map[op].ed_id >= itv.st_id ) {  // 该变量是定义且使用过的变量
                            live_map[op].ed_id = itv.ed_id;
                            live_map[op].weight += 0.2;
                        }
                        }
                    }
                    live_map[op].use_id.insert(func_reg_map[cur_func_name].linear_map[inst]);
                    op_id++;
                }
            }
            if (inst->getType()->getSize() > 0) {
                if(dynamic_cast<MovInstr*>(inst)){
                    live_map[dynamic_cast<MovInstr*>(inst)->getLVal()].use_id.insert(func_reg_map[cur_func_name].linear_map[inst]);
                }
                else{
                    live_map[inst].use_id.insert(func_reg_map[cur_func_name].linear_map[inst]);
                }
            }
        }
    }
    // 如果有一个变量，在循环外侧被定义，但在循环内被使用，则将它的生命周期选为循环下界和它自身右界的最大值
    LSRA_WARNNING("LINEAR SCAN DONE");

    //对于结果后处理全局变量立即数处理
    for (auto p : live_map) {
        if (p.second.def || dynamic_cast<PhiInstr*>(p.first)) {
            if(dynamic_cast<PhiInstr*>(p.first)){
                LSRA_WARNNING("###HIT### %s",p.first->getPrintName().c_str());
            }
            interval itv;
            itv.st_id = p.second.st_id;
            itv.ed_id = p.second.ed_id;
            itv.def = p.second.def;
            itv.type = p.second.type;
            itv.is_float =p.first->getType()->isFloatTy();
            itv.spilled = p.second.spilled;
            itv.is_allocated = p.second.is_allocated;
            itv.specific_reg_idx = p.second.specific_reg_idx;
            itv.v = p.first;
            itv.weight = p.second.weight;
            itv.use_id = p.second.use_id;
            live_res.push_back(itv);

        } else {
            for (auto id : p.second.use_id) {
                interval itv;
                itv.st_id = id;
                itv.ed_id = id;
                itv.def = p.second.def;
                itv.type = p.second.type;
                itv.is_float =p.first->getType()->isFloatTy();
                itv.is_float = p.second.is_float;
                itv.spilled = p.second.spilled;
                itv.is_allocated = p.second.is_allocated;
                itv.specific_reg_idx = p.second.specific_reg_idx;
                itv.use_id = p.second.use_id;
                itv.weight = p.second.weight;
                itv.v = p.first;
                live_res.push_back(itv);
            }
        }
    }
    // debug
    #ifdef __LSRA_WARN

    LSRA_WARNNING(" LIVE INTERVAL");
    for (auto p : live_res) {
        if(dynamic_cast<MovInstr*>(p.v)){
            LSRA_WARNNING("#!!!!!! %s [%d,%d] float ? %d type: %s",dynamic_cast<MovInstr*>(p.v)->getLVal()->getPrintName().c_str(),p.st_id,p.ed_id,p.is_float,getType(p.type).c_str());
        }
        else{
            LSRA_WARNNING(" %s [%d,%d] float ? %d type: %s",p.v->getPrintName().c_str(),p.st_id,p.ed_id,p.is_float,getType(p.type).c_str());
        }
    }
    #endif // DEBUG
    linear_scan_reg_alloc(live_res, func, insert);
    return;
}

std::string AsmBuilder::getType(interval_value_type t){
    if(t == interval_value_type::local_var){
        return "local_var";
    }
    if(t == interval_value_type::global_var){
        return "global_var";
    }
    if(t == interval_value_type::imm_var){
        return "imm_var";
    }
    if(t == interval_value_type::arg_var){
        return "arg_var";
    }
    if(t == interval_value_type::spill_var){
        return "spill_var";
    }
    return "unknown type";
}