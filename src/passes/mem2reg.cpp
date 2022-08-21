#include "mem2reg.h"
#include "module.h"
#include "analysis/dominators.h"
#include "ir/instruction.h"
#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/value.h"
#include "utils.h"
#include <chrono>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
void Mem2Reg::run() {
    Dominators dom(_m,"dominators");
    dom.run();
    for(auto f : _m->getFunctions()){
        if(!f->getBasicBlocks().empty()){
            _cur_func = f;
            genPhi();
        }

    }
}

void Mem2Reg::genPhi() {
    std::vector<std::vector<BasicBlock*>> defBlocks ;
    std::vector<AllocaInst*> allocas ;
   std::unordered_map<AllocaInst*, int> allocaLookup ;

   // find defs block  -- has store
    for (auto bb : _cur_func->getBasicBlocks()) {
        for (auto instr : bb->getInstructions()) {
            if (instr->isAlloca()) {
                auto alloca= static_cast< AllocaInst*>(instr);
                // only local int
                if (alloca->getAllocaType()->isFloatTy()|| alloca->getAllocaType()->isIntegerTy()) {
                    //            defs.put(allocaInst, new ArrayList<>());
                    allocas.push_back(alloca);
                    allocaLookup.insert({alloca, allocas.size() - 1});
                    defBlocks.emplace_back();
                }
            }
    // 记录定义 alloca指令的 block,即有对alloca 进行store操作的块
            else if (instr->isStore()) {
                auto  storeInst = static_cast<StoreInst*> (instr);
                auto ptr =  dynamic_cast<AllocaInst*>( storeInst->getLVal());

                if (!dynamic_cast<AllocaInst*>(ptr)) {
                    continue;
                }
                auto index = allocaLookup.find(ptr) ;
                if (index != allocaLookup.end()) {
                    defBlocks.at(index->second).push_back(bb);
                }
            }
        }
    }


    // 对store指令所在块的支配边界插入phi指令
    std::queue<BasicBlock*> W ;
    std::unordered_map<PhiInstr*, int> phiToAllocaMap ;
    std::unordered_set<BasicBlock*> visited;
    for (auto allocaInst : allocas) {
        auto index = allocaLookup.find(allocaInst)->second;
        auto alloca_type = allocaInst->getAllocaType();

        visited.clear();

        for(auto  bb : defBlocks.at(index)){
            W.push(bb);
        }

        while (!W.empty()) {
            auto bb = W.front();W.pop();
            // 对每一个支配者边界
            for (auto y : bb->getDomFrontier()) {
                // 如果之前没有在这个支配着边界里里插入与此alloca相关的phi指令
                if ( visited.find(y)==visited.end()) {
                    visited.insert(y);
                    auto phiInst = PhiInstr::createPhi(alloca_type, y);
                    phiToAllocaMap[phiInst]= index;
                    // 插入DF(DF(bb)); 
                    if (std::find(defBlocks.at(index).begin(), defBlocks.at(index).end(), y)== defBlocks.at(index).end()){ W.push(y);
                    }
                }
            }
        }
    }
    // TODO: 把这里的默认是0修改成undefValue来方便后续优化

    std::vector<Value*> values ;
    for (int i = 0; i < allocas.size(); i++) {
        //      values.add(new UndefValue());
        if(allocas.at(i)->getAllocaType()->isIntegerTy()){
            values.push_back(Value::getUncertainValue());
            // values.push_back(ConstantInt::create(0));
        }else if (allocas.at(i)->getAllocaType()->isFloatTy()) {
            values.push_back(Value::getUncertainValue());
        }
    }
    visited.clear();

    std::stack<RenameData*> renameDataStack ;
    renameDataStack.push(new RenameData(_cur_func->getEntryBlock(), nullptr, values));

    while (!renameDataStack.empty()) {
        auto* data = renameDataStack.top() ;renameDataStack.pop();

          std::vector<Value*> currValues ;
          currValues.assign(data->_values.begin(),data->_values.end());

        // 对于插入的 phi 指令，更新 incomingVals 为 values 中的对应值
        for (auto inst : data->_bb->getInstructions()) {

            if (!inst->isPhi()) {
                break;
            }
            auto phiInst = static_cast<PhiInstr*>( inst);
            if (phiToAllocaMap.find(phiInst) == phiToAllocaMap.end()) {
                continue;
            }
//            auto  predIndex = data._bb->getPreBasicBlockList().indexOf(data.pred);
            phiInst->setParams(
                data->_values.at(phiToAllocaMap.find(phiInst)->second),
                data->_pred);
        }

        // 已经删除过 alloca/load/store，但是可能有来自其他前驱基本块的 incomingVals，所以在这里才 `continue;`
        if (visited.find(data->_bb)!=visited.end()) {
            continue;
        }

        visited.insert(data->_bb);

        std::vector<Instruction*> wait_delete;
        std::vector<Instruction*> wait_delete_alloca;
        for (auto inst : data->_bb->getInstructions()) {
            // AllocaInst
                MEM2REG_LOG("visiting instr %s",inst->getPrintName().c_str());
            if(inst->isRet()){
                MEM2REG_LOG("get ret");
            }
            if (inst->isAlloca() && allocaLookup.count(static_cast<AllocaInst*>(inst))) {
                wait_delete_alloca.push_back(inst);
            }
                // LoadInst
            else if (inst->isLoad()) {
                auto loadInst = static_cast<LoadInst*>(inst);
                auto ptr_is_alloca = dynamic_cast< AllocaInst*>(loadInst->getOperand(0));
                if (!ptr_is_alloca) {
                    continue;
                }

                if (!ptr_is_alloca->getAllocaType()->isFloatTy()&& !ptr_is_alloca->getAllocaType()->isIntegerTy()) {
                    continue;
                }
                int allocaIndex = allocaLookup.find(ptr_is_alloca)->second;

                if(auto phi_instr = dynamic_cast<PhiInstr*>(currValues.at(allocaIndex))){
                    MEM2REG_LOG("using phi ");
                }
                loadInst->replaceAllUse(currValues.at(allocaIndex));
                inst->removeUseOps();
                wait_delete.push_back(loadInst);
            }
                // StoreInst
            else if (inst->isStore()) {
                auto storeInst = static_cast<StoreInst*>( inst);
                auto ptr_store = dynamic_cast< AllocaInst*>(storeInst->getOperand(1));
                if (!ptr_store) {
                    continue;
                }
                auto allocaInst = static_cast<AllocaInst*>(ptr_store);
                if (!allocaInst->getAllocaType()->isFloatTy() && ! allocaInst->getAllocaType()->isIntegerTy()) {
                    continue;
                }
                auto allocaIndex = allocaLookup.find(allocaInst)->second;
                MEM2REG_LOG("cur alloca index is %d",allocaIndex);
                MEM2REG_LOG("cur value size is %d",currValues.size());
                currValues[allocaIndex] = storeInst->getOperand(0);
                // 不应该有指令用到了store
                inst->replaceAllUse(nullptr);
                inst->removeUseOps();
                wait_delete.push_back(inst);
           }
        //    // Phi
           else if (inst->isPhi()) {
               auto phiInst = static_cast<PhiInstr*>( inst);
               auto allocaIndex = phiToAllocaMap.find(phiInst)->second;
               currValues[allocaIndex] = phiInst;
           }
       }
       for (auto instr : wait_delete_alloca) {
           data->_bb->deleteInstr(instr);
        }
       for (auto instr : wait_delete) {
           data->_bb->deleteInstr(instr);
        }

        for (auto b  : data->_bb->getSuccBasicBlockList()) {
           renameDataStack.push(new RenameData(b, data->_bb, currValues));
       }
   }
}
