#include "lower_ir.h"

#include "ir/basic_block.h"
#include "ir/instruction.h"
#include "module.h"

void LowerIR::run() {
    for (auto func : _m->getFunctions()) {
        for (auto bb : func->getBasicBlocks()) {
            splitGEP(bb);
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
                if (!operands[i]->isConstant()){}
                else if (dynamic_cast<ConstantInt*>(operands[i])->getValue() != 0) { // 如果该参数是常量
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
            WARNNING("inserts instr off is %d.",off);
            WARNNING("inserts instr is %d.",inserts.size());
            // replace and erase gep
            inst->replaceAllUse(inserts[inserts.size()-1]);
            inst->removeUseOps();
            iter = insts.erase(iter);
        } else {
            iter++;
        }
    }
}