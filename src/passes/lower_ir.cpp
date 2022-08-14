#include "lower_ir.h"
#include <list>
#include <cmath>

#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "module.h"
#include "../utils.h"

void LowerIR::run() {
    for (auto func : _m->getFunctions()) {
        for (auto bb : func->getBasicBlocks()) {
            splitGEP(bb);
            convertRem2And(bb);
            splitRem(bb);
        }
    }
    for (auto func : _m->getFunctions()) {
        for (auto bb : func->getBasicBlocks()) {
            deleteUnusedInstr(bb);
        }
    }
    for (auto func : _m->getFunctions()) {
        for (auto bb : func->getBasicBlocks()) {
            comvertMulDiv2Shift(bb);
            mergeConstShift(bb);
        }
    }

    for (auto func : _m->getFunctions()) {
        for (auto bb : func->getBasicBlocks()) {
          convertMlaLoad2LoadOffset(bb);
          deleteMla(bb); // 临时用于mla的死代码删除
        }
    }

    for (auto func : _m->getFunctions()) {
        for (auto bb : func->getBasicBlocks()) {
          convert2Mla(bb);
        }
    }

    // for (auto func : _m->getFunctions()) {
    //     for (auto bb : func->getBasicBlocks()) {
    //       mergeShiftArithmetic(bb);
    //     }
    // }

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
    if (inst->isRem() && inst->getType()->isIntegerTy()) {
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
    if (inst->isRem() && inst->getType()->isIntegerTy()) {
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

// 将满足条件的乘除转化为移位
void LowerIR::comvertMulDiv2Shift(BasicBlock *bb) {
  auto &insts = bb->getInstructions();
  for (auto iter = insts.begin(); iter != insts.end();) {
    auto inst = *iter;
    if (inst->isMul() && inst->getType()->isIntegerTy()) {
      auto op0 = inst->getOperand(0);
      if (op0->isConstant()) {
        int val = static_cast<ConstantInt *>(op0)->getValue();
        if (isPowerOfTwo(val)) {
          int s = (int)std::ceil(std::log2(val));
          auto shl = BinaryInst::createShl(inst->getOperand(1),
                  ConstantInt::get(s),nullptr);
          inst->replaceAllUse(shl);
          inst->removeUseOps();
          shl->setParent(bb);
          insts.insert(iter,shl);
          iter = insts.erase(iter);
          continue;
        }
      }

      auto op1 = inst->getOperand(1);
      if (op1->isConstant()) {
        int val = static_cast<ConstantInt *>(op1)->getValue();
        if (isPowerOfTwo(val)) {
          int s = (int)std::ceil(std::log2(val));
          auto shl = BinaryInst::createShl(inst->getOperand(0),
                  ConstantInt::get(s),nullptr);
          inst->replaceAllUse(shl);
          inst->removeUseOps();
          shl->setParent(bb);
          insts.insert(iter,shl);
          iter = insts.erase(iter);
          continue;
        }
      }
    } else if (inst->isDiv() && inst->getType()->isIntegerTy()) {
      auto op1 = inst->getOperand(1);
      if (op1->isConstant()) {
        int val = static_cast<ConstantInt *>(op1)->getValue();
        if (isPowerOfTwo(val)) {
          int s = (int)std::ceil(std::log2(val));
          auto ashr = BinaryInst::createAshr(inst->getOperand(0),
                  ConstantInt::get(s),nullptr);
          inst->replaceAllUse(ashr);
          inst->removeUseOps();
          ashr->setParent(bb);
          insts.insert(iter,ashr);
          iter = insts.erase(iter);
          continue;
        }
      }

    }
    ++iter;
  }
}


void LowerIR::deleteUnusedInstr(BasicBlock *bb) {
  auto &insts = bb->getInstructions();
  for (auto iter = insts.begin(); iter != insts.end();) {
    auto inst = *iter;
    bool has_delete = false;

    // delete a + 0
    if (inst->isAdd()) {
      int i = 0;
      for (auto op : inst->getOperandList()) {
        auto const_val = dynamic_cast<ConstantInt *>(op);
        if (const_val && const_val->getValue() == 0) {
          if (i == 0) {
            if (dynamic_cast<GlobalVariable *>(inst->getOperand(1)))
              break;
            inst->replaceAllUse(inst->getOperand(1));
          } else {
            if (dynamic_cast<GlobalVariable *>(inst->getOperand(0)))
              break;
            inst->replaceAllUse(inst->getOperand(0));
          }
          inst->removeUseOps();
          iter = insts.erase(iter);
          has_delete = true;
          break;
        }
        i++;
      }
    }
    // delete a - 0 or convert 0 - a to neg
    else if (inst->isSub()) {
      auto const_val = dynamic_cast<ConstantInt *>(inst->getOperand(1));
      if (const_val && const_val->getValue() == 0) {
          inst->replaceAllUse(inst->getOperand(0));
          inst->removeUseOps();
          iter = insts.erase(iter);
          has_delete = true;
          break;
      }
    }
    // delte a * 0/1
    else if (inst->isMul()) {
      int i = 0;
      for (auto op : inst->getOperandList()) {
        auto const_val = dynamic_cast<ConstantInt *>(op);
        if (const_val &&
        (const_val->getValue() == 0 || const_val->getValue() == 1)) {
          if (const_val->getValue() == 0) {
            inst->replaceAllUse(ConstantInt::get(0));
          } else { // const val = 1
            if (i == 0) {
              inst->replaceAllUse(inst->getOperand(1));
            } else {
              inst->replaceAllUse(inst->getOperand(0));
            }
          }
          inst->removeUseOps();
          iter = insts.erase(iter);
          has_delete = true;
          break;
        }
        i++;
      }
    }
    // delete a / 1
    else if (inst->isDiv()) {
      auto const_val = dynamic_cast<ConstantInt *>(inst->getOperand(1));
      if (const_val && const_val->getValue() == 1) {
          inst->replaceAllUse(inst->getOperand(0));
          inst->removeUseOps();
          iter = insts.erase(iter);
          has_delete = true;
          break;
        }
    }
    if (!has_delete)
      ++iter;
  }
}

void LowerIR::mergeConstShift(BasicBlock *bb) {
  for (auto inst : bb->getInstructions()) {
    if (inst->isShl() && inst->getType()->isIntegerTy()) {
      auto op0 = dynamic_cast<Instruction *>(inst->getOperand(0));
      if (op0 && op0->isShl()) {
        auto const_val1 = dynamic_cast<ConstantInt *>(inst->getOperand(1));
        auto const_val2 = dynamic_cast<ConstantInt *>(op0->getOperand(1));
        if (const_val1 && const_val2) {
          inst->removeUseOps();
          inst->setOperand(0, op0->getOperand(0));
          inst->setOperand( 1,
            ConstantInt::get(const_val1->getValue() + const_val2->getValue()));
        }
      }
    } else if (inst->isAshr() && inst->getType()->isIntegerTy()) {
      auto op0 = dynamic_cast<Instruction *>(inst->getOperand(0));
      if (op0 && op0->isAshr()) {
        auto const_val1 = dynamic_cast<ConstantInt *>(inst->getOperand(1));
        auto const_val2 = dynamic_cast<ConstantInt *>(op0->getOperand(1));
        if (const_val1 && const_val2) {
          inst->removeUseOps();
          inst->setOperand(0, op0->getOperand(0));
          inst->setOperand( 1,
            ConstantInt::get(const_val1->getValue() + const_val2->getValue()));
        }
      }
    }
  }
}

void LowerIR::convert2Mla(BasicBlock *bb) {
  auto &insts = bb->getInstructions();
  for (auto iter = insts.begin(); iter != insts.end(); ++iter) {
    auto inst = *iter;
    if (inst->isAdd() && inst->getType()->isIntegerTy()) {
      auto op1 = dynamic_cast<Instruction *>(inst->getOperand(0));
      if (op1 && op1->isMul() && (op1->getParent() == bb) &&
          (op1->getUseList().size() == 1)) {
        auto muladd = MlaInst::createMlaInst(
            op1->getOperand(0), op1->getOperand(1), inst->getOperand(1));
        iter = insts.erase(iter);
        iter = insts.insert(iter, muladd);
        muladd->setParent(bb);
        bb->deleteInstr(op1);
        inst->replaceAllUse(muladd);
        inst->removeUseOps();
        continue;
      }
      auto op2 = dynamic_cast<Instruction *>(inst->getOperand(1));
      if (op2 && op2->isMul() && (op2->getParent() == bb) &&
          (op2->getUseList().size() == 1)) {
        auto muladd = MlaInst::createMlaInst(
            op2->getOperand(0), op2->getOperand(1), inst->getOperand(0));
        iter = insts.erase(iter);
        iter = insts.insert(iter, muladd);
        muladd->setParent(bb);
        bb->deleteInstr(op2);
        inst->replaceAllUse(muladd);
        inst->removeUseOps();
      }
    }
  }
}

void LowerIR::mergeShiftArithmetic(BasicBlock *bb) {
  for (auto inst : bb->getInstructions()) {
    if (inst->isAdd() && inst->getOperandNumber() == 2 && inst->getType()->isIntegerTy()) {
      auto op1 = dynamic_cast<Instruction *>(inst->getOperand(0));
      if (op1 && op1->isShl()) {
        auto const_val = dynamic_cast<ConstantInt *>(op1->getOperand(1));
        if (const_val) {
          inst->removeUseOps();
          inst->addOperand(const_val);
          inst->setOperand(0, inst->getOperand(1));
          inst->setOperand(1, op1->getOperand(0));
          continue;
        }
      }
      auto op2 = dynamic_cast<Instruction *>(inst->getOperand(1));
      if (op2 && op2->isShl()) {
        auto const_val = dynamic_cast<ConstantInt *>(op2->getOperand(1));
        if (const_val) {
          inst->removeUseOps();
          inst->addOperand(const_val);
          inst->setOperand(0, inst->getOperand(0));
          inst->setOperand(1, op2->getOperand(0));
        }
      }
    } else if ((inst->isSub()|| inst->isAnd() || inst->isOr())
        && inst->getOperandNumber() == 2 && inst->getType()->isIntegerTy()) {
      auto op2 = dynamic_cast<Instruction *>(inst->getOperand(1));
      if (op2 && op2->isShl()) {
        auto const_val = dynamic_cast<ConstantInt *>(op2->getOperand(1));
        if (const_val) {
          inst->removeUseOps();
          inst->addOperand(const_val);
          inst->setOperand(0, inst->getOperand(0));
          inst->setOperand(1, op2->getOperand(0));
        }
      }
    }
  }
}

void LowerIR::convertMlaLoad2LoadOffset(BasicBlock *bb) {
  for (auto inst : bb->getInstructions()) {
    if (inst->isLoad() && inst->getType()->isIntegerTy()) {
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
