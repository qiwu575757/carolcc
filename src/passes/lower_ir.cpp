#include "lower_ir.h"
#include <list>

#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "module.h"
#include "../utils.h"

void LowerIR::run() {
    for (auto func : _m->getFunctions()) {
        for (auto bb : func->getBasicBlocks()) {
            splitGEP(bb);
            convertRem2And(bb);
        }
    }

    for (auto func : _m->getFunctions()) {
        for (auto bb : func->getBasicBlocks()) {
          convertMlaLoad2LoadOffset(bb);
          deleteMla(bb); // 临时用于mla的死代码删除
        }
    }

    _m->setIRLevel(Module::LIR);
}

void LowerIR::splitGEP(BasicBlock *bb) {
    auto &insts = bb->getInstructions();
    for (auto iter = insts.begin(); iter != insts.end(); ) {
        auto inst = *iter;
        if (inst->isGep()) {
            int off = 0; // 用于gep参数为立即数的情况，节约指令
            std::vector<Instruction *> inserts; // 存储插入的指令
            auto &operands = inst->getOperandList();

            // 先由立即数获得其对应偏移，然后将其加入mla
            auto array_ty = operands[0]->getType()->getPointerElementType();
            for (int i = 1; i < operands.size(); i++) {
                auto element_size = array_ty->getSize();

                // 这里语法！！！
                 if (operands[i]->isConstant() &&
                 dynamic_cast<ConstantInt*>(operands[i])->getValue() != 0) { // 如果该参数是常量
                    off +=  dynamic_cast<ConstantInt*>(operands[i])->getValue() * element_size;
                }
                array_ty = static_cast<ArrayType*>(array_ty)->getElementType();
            }

            // 插入 mla指令
            array_ty = operands[0]->getType()->getPointerElementType();
            bool off_no_use = true;
            for (int i = 1; i < operands.size(); i++) {
                auto element_size = array_ty->getSize();

                if (!operands[i]->isConstant()&&off_no_use) {
                    auto muladd = MlaInst::createMlaInst(
                        operands[i],
                        ConstantInt::create(element_size),
                        ConstantInt::create(off)
                    );
                    muladd->setParent(bb);
                    inserts.push_back(muladd);
                    off_no_use = false;
                } else if (!operands[i]->isConstant()&&!off_no_use) {
                    auto muladd = MlaInst::createMlaInst(
                        operands[i],
                        ConstantInt::create(element_size),
                        inserts[inserts.size()-1]
                    );
                    muladd->setParent(bb);
                    inserts.push_back(muladd);
                }
                array_ty = static_cast<ArrayType*>(array_ty)->getElementType();
            }

            // base + offset
            auto addbase = inserts.size() == 0 ?
                        MlaInst::createMlaInst(
                            inst->getType(),ConstantInt::create(1),
                            operands[0],ConstantInt::create(off)
                            ) :
                        MlaInst::createMlaInst(
                            inst->getType(),ConstantInt::create(1),
                            operands[0],inserts[inserts.size()-1]
                        );
            addbase->setParent(bb);
            inserts.push_back(addbase);

            // insert into bb
            for (auto insert_instr : inserts) {
                insts.insert(iter,insert_instr);
            }

            // replace and erase gep
            inst->replaceAllUse(inserts[inserts.size()-1]);
            inst->removeUseOps();
            iter = insts.erase(iter);
        } else {
            iter++;
        }
    }
}

bool LowerIR::isPowerOfTwo(int x) {
  return x && (!(x & (x - 1)));
}

void LowerIR::convertRem2And(BasicBlock *bb) {
  auto &insts = bb->getInstructions();
  for (auto iter = insts.begin(); iter != insts.end();) {
    auto inst = *iter;
    if (inst->isRem()) {
      auto op2 = dynamic_cast<Constant *>(inst->getOperand(1));
      if (op2) {
        int v = static_cast<ConstantInt *>(op2)->getValue();
        if (isPowerOfTwo(v)) {
          int p = v - 1;
          auto tmp = BinaryInst::createAnd(inst->getOperand(0),
                            ConstantInt::get(p),nullptr);
          inst->replaceAllUse(tmp);
          inst->removeUseOps();
          tmp->setParent(bb);
          insts.insert(iter, tmp);
          iter = insts.erase(iter);
          continue;
        }
      }
    }
    ++iter;
  }
}

// 将取摸转化为 除法、乘法、减法
void LowerIR::splitRem(BasicBlock *bb) {
  auto &insts = bb->getInstructions();
  for (auto iter = insts.begin(); iter != insts.end();) {
    auto inst = *iter;
    if (inst->isRem()) {
      auto op1 = inst->getOperand(0);
      auto op2 = inst->getOperand(1);
      auto div = BinaryInst::createDiv(op1, op2, nullptr);
      div->setParent(bb);
      auto mul = BinaryInst::createMul(div, op2, nullptr);
      mul->setParent(bb);
      auto sub = BinaryInst::createSub(op1, mul, nullptr);
      sub->setParent(bb);
      insts.insert(iter, div);
      insts.insert(iter, mul);
      insts.insert(iter, sub);
      inst->replaceAllUse(sub);
      inst->removeUseOps();
      iter = insts.erase(iter);
    } else
      ++iter;
  }
}


void LowerIR::convertMlaLoad2LoadOffset(BasicBlock *bb) {
  for (auto inst : bb->getInstructions()) {
    if (inst->isLoad()) {
      auto load = static_cast<LoadInst *>(inst);
      if (!load->hasOffset()) {
        auto ptr = dynamic_cast<Instruction *>(load->getOperand(0));
        if (ptr && ptr->isMla() && !inst->getType()->isFloatTy()) {
          // load 使用到的 mla 均可转化为 add, 浮点指令无 ldr base, offset, shift
          MyAssert("load use mla can't convert to add",atoi(ptr->getOperand(0)->getPrintName().c_str()) == 1, ERROR_DEFUALT);
          load->removeUseOps();
          load->setNumOps(2);
          load->setLVal(ptr->getOperand(1));
          load->setOffset(ptr->getOperand(2));
          WARNNING("come in");
        }
      }
    } else if (inst->isStore()) {
      auto store = static_cast<StoreInst *>(inst);
      if (!store->hasOffset()) {
        auto ptr = dynamic_cast<Instruction *>(store->getLVal());
        if (ptr && ptr->isMla()&& !inst->getOperand(0)->getType()->isFloatTy()) {
          MyAssert("load use mla can't convert to add",atoi(ptr->getOperand(0)->getPrintName().c_str()) == 1, ERROR_DEFUALT);
          ptr->removeUse(inst, 1);
          store->setNumOps(3);
          store->setLVal(ptr->getOperand(1));
          store->setOffset(ptr->getOperand(2));
        }
      }
    }
  }
}

void LowerIR::deleteMla(BasicBlock *bb) {
    auto &insts = bb->getInstructions();
    for (auto iter = insts.begin(); iter != insts.end(); ) {
        auto inst = *iter;
        if (inst->isMla() && inst->getUseList().empty()) {
            inst->removeUseOps();
            iter = insts.erase(iter);
            WARNNING("delete mla");
        } else {
          iter++;
        }
    }
}
