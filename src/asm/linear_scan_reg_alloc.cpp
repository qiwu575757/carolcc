#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include "asm_builder.h"
#include "asm_instr.h"
#include "utils.h"
#include <memory.h>

bool cmp(interval lhs,interval rhs)//升序
{
	return lhs.weight>rhs.weight;
}

bool AsmBuilder::value_in_reg(Value *v){ //计算栈空间
    for(int i=0;i<int_reg_number;i++){
        for(int j=0;j<virtual_int_regs[i].size();j++){
            if(virtual_int_regs[i][j].v == v){
                return true;
            }
        }
    }
    return false;
}

int AsmBuilder::getRegIndexOfValue(Value *inst,Value *v){
    int tag = linear_map[inst];
    LSRA_WARNNING("inst %d",tag);
    for(int i=0;i<int_reg_number;i++){
        for(int j=0;j<virtual_int_regs[i].size();j++){
            if(virtual_int_regs[i][j].v == v && tag >= virtual_int_regs[i][j].st_id &&  tag <= virtual_int_regs[i][j].ed_id){
                return i;
            }
        }
    }
    ERROR("cant find int value in reg alloc!");
    return -1;
}

void AsmBuilder::linear_scan_reg_alloc(std::vector<interval> live_range){
    //cal weight
    for (auto &p : live_range) {
        int total_use_num = p.use_id.size();
        int sum_range = p.ed_id - p.st_id;
        p.use_freq = (total_use_num+1e-4)/(sum_range + 1e-4);
        p.weight = sum_range; // 暂时考虑按这个
    }
    for(int i=0;i<int_reg_number;i++){
        virtual_int_regs[i].clear();
    }
    // sort by weight
    std:LSRA:sort(live_range.begin(),live_range.end(),cmp);

    for(auto &itv : live_range){
        for(int i=0;i<int_reg_number;i++){
            bool conflict = false;
            int index = 0;
            for(int j=0;j<virtual_int_regs[i].size();j++){
                conflict = true;
                if(virtual_int_regs[i][j].ed_id<itv.st_id && j == virtual_int_regs[i].size()-1){ // j 在 itv 前面 并且是最后一个
                    conflict = false;
                    index = j+1;
                    break;
                }
                else if(virtual_int_regs[i][j].st_id > itv.ed_id && j == 0){ // j 在 itv 后面 并且是第一个
                    conflict = false;
                    index = j;
                    break;
                }
                else if(j != virtual_int_regs[i].size()-1 && virtual_int_regs[i][j+1].st_id > itv.ed_id && virtual_int_regs[i][j].ed_id<itv.st_id){ // j 在 itv 后面 并且是第一个
                    conflict = false;
                    index = j+1;
                    break;
                }
            }
            if(!conflict){
                virtual_int_regs[i].insert(virtual_int_regs[i].begin()+index,itv);
                break;
            }
        }
    }
    LSRA_WARNNING(" LSRA REG ALLOC");
    // debug
    for(int i=0;i<int_reg_number;i++){
        for(int j=0;j<virtual_int_regs[i].size();j++){
            LSRA_WARNNING("[reg %d] [%d,%d] v:%s w:%lf",i,virtual_int_regs[i][j].st_id,virtual_int_regs[i][j].ed_id,virtual_int_regs[i][j].v->getPrintName().c_str(),virtual_int_regs[i][j].weight);
        }
    }
    LSRA_SHOW("-- [reg alloc graph] --\n");
    for(int i=0;i<int_reg_number;i++){
        int t=0;
        if(virtual_int_regs[i].size()==0)continue;
        LSRA_SHOW("[reg %02d] ",i);
        for(int j=0;j<virtual_int_regs[i].size();j++){
            while(t<virtual_int_regs[i][j].st_id){
                LSRA_SHOW(" ");
                t++;
            }
            while(t<=virtual_int_regs[i][j].ed_id){
                LSRA_SHOW("#");
                t++;
            }
        }
        LSRA_SHOW("\n");
    }
}

std::vector<interval> AsmBuilder::live_interval_analysis(Function *func){
    int instr_index = 0;
    int linear_index = 0;
    int bb_index = 0;
    std::map<Value *,interval>  live_map;
    std::map<BasicBlock *, std::set<Value *>> live_in, live_out;
    std::set<Value *> values;
    linear_map.clear();
    std::map<BasicBlock *,int > bb_map;
    std::vector<interval> loop_set;
    std::vector<interval>  live_res;// 对于结果进行后处理
    if (func->getBasicBlocks().empty()) {
      return live_res;
    }
    // find all vars
    for (auto &args : func->getArgs()) {
      values.insert(args);
    }
    // linear list
    LSRA_WARNNING(" LINEAR LIST");
    for (auto &bb : func->getBasicBlocks()) {
        LSRA_WARNNING(" [%d:B] ",bb_index);
        bb_map[bb]=bb_index++;
        for (auto &inst : bb->getInstructions()) {
            if (inst->getType()->getSize() > 0) {
                if(!live_map.count(inst)) // first time
                {
                    live_map[inst].def = true;
                    live_map[inst].st_id =linear_index;
                    live_map[inst].ed_id =linear_index;
                }
            values.insert(inst);
            }
            LSRA_WARNNING(" [%d:%s] ",linear_index,inst->getName().c_str());
            linear_map[inst]=linear_index++;
        }
    }
    // 成环定义：如果一个块的前驱节点是大于等于自身的则形成环路，且该环路的上下界可知
    for (auto &bb : func->getBasicBlocks()) {
        for(auto &pb : bb->getPreBasicBlocks()){
            if(bb_map[pb]>=bb_map[bb]){
                LSRA_WARNNING(" LOOP BLOCK CHECK: bb %d -- bb %d",bb_map[bb],bb_map[pb]);
                LSRA_WARNNING(" LOOP CHECK: from %d to %d",linear_map[bb->getInstructions().front()],linear_map[pb->getInstructions().back()]);
                interval itv;
                itv.st_id = linear_map[bb->getInstructions().front()];
                itv.ed_id = linear_map[pb->getInstructions().back()];
                loop_set.push_back(itv);
            }
        }
    }
    // cal ls(&&
    for (auto &bb : func->getBasicBlocks()) {
        for (auto &inst : bb->getInstructions()) {
            if (inst->isBr()){
                if(inst->getOperandList().size() <= 1){
                    continue;
                }
                auto &op = inst->getOperandList()[0];
                if(!live_map.count(op)) // first time
                {
                    live_map[op].st_id = linear_map[inst];
                    live_map[op].ed_id = linear_map[inst];
                }
                else{
                    if(linear_map[inst] > live_map[op].ed_id){
                        live_map[op].ed_id = linear_map[inst];
                    }
                }
                for(auto &itv : loop_set){
                    if(linear_map[inst]<=itv.ed_id && linear_map[inst]>=itv.st_id){ // 该指令在该循环中使用
                        if(live_map[op].def && live_map[op].st_id < itv.st_id && live_map[op].st_id < live_map[op].ed_id){ // 该变量是定义且使用过的变量
                            live_map[op].ed_id = live_map[op].ed_id > itv.ed_id ? live_map[op].ed_id : itv.ed_id;
                        }
                    }
                }
                live_map[op].use_id.insert(linear_map[inst]);
            }
            else{
                for (auto &op : inst->getOperandList()) {
                    if(!live_map.count(op)) // first time
                    {
                        live_map[op].st_id = linear_map[inst];
                        live_map[op].ed_id = linear_map[inst];
                    }
                    else{
                        if(linear_map[inst] > live_map[op].ed_id){
                            live_map[op].ed_id = linear_map[inst];
                        }
                    }
                    for(auto &itv : loop_set){
                        if(linear_map[inst]<=itv.ed_id && linear_map[inst]>=itv.st_id){ // 该指令在该循环中使用
                            if(live_map[op].def && live_map[op].st_id < itv.st_id && live_map[op].st_id < live_map[op].ed_id){ // 该变量是定义且使用过的变量
                                live_map[op].ed_id = live_map[op].ed_id > itv.ed_id ? live_map[op].ed_id : itv.ed_id;
                            }
                        }
                    }
                    live_map[op].use_id.insert(linear_map[inst]);
                }
            }
        }
    }
    // 如果有一个变量，在循环外侧被定义，但在循环内被使用，则将它的生命周期选为循环下界和它自身右界的最大值

    //对于结果后处理全局变量立即数处理
    for (auto &p : live_map) {
        if(p.second.def){
            interval itv;
            itv.st_id = p.second.st_id;
            itv.ed_id = p.second.ed_id;
            itv.def = p.second.def;
            itv.v = p.first;
            live_res.push_back(itv);
        }
        else{
            for(auto id : p.second.use_id){
                interval itv;
                itv.st_id = id;
                itv.ed_id = id;
                itv.def = p.second.def;
                itv.v = p.first;
                live_res.push_back(itv);
            }
        }
    }
    // debug
    // LSRA_WARNNING(" LIVE INTERVAL");
    // for (auto &p : live_res) {
    //     LSRA_WARNNING(" %%%s [%d,%d] def?%d",p.v->getName().c_str(),p.st_id,p.ed_id,p.def);
    // }
    linear_scan_reg_alloc(live_res);
    return live_res;
}
