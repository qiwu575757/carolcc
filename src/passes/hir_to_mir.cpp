#include "hir_to_mir.h"
#include "ir/basic_block.h"

void HIRToMIR::run() {
    for (auto func : _m->getFunctions()) {
        auto base_bbs = func->getBaseBlocks();
        BasicBlock *next_bb = nullptr;
        for (auto b = base_bbs.rbegin(); b != base_bbs.rend(); b++) {
            auto base_bb = *b;
            next_bb = genBasicBlock(base_bb, next_bb, nullptr, nullptr, func);
        }
        // mir 不在需要 baseblocks
        func->getBaseBlocks().clear();
    }

    _m->setIRLevel(Module::MIR_MEM);
}

BasicBlock *HIRToMIR::genBasicBlock(BaseBlock *base_bb, BasicBlock *next_bb,
                           BasicBlock *while_entry, BasicBlock *while_exit,
                           Function *func) {
    auto &basic_bbs = func->getBasicBlocks();

    if (base_bb->_block_type == BaseBlock::BASIC) {
        auto this_bb = dynamic_cast<BasicBlock *>(base_bb);
//        if (this_bb == nullptr && this_bb->getT)
    }
}