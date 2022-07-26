#include "instruction.h"

#include "function.h"
#include "passes/module.h"
#include "type.h"
#include "user.h"
#include "utils.h"
#include "value.h"
#include "visitor/ir_visitor_base.h"

Instruction::Instruction(Type *type, Instruction::OpKind op_id,
                         unsigned int op_nums, BasicBlock *parent)
    : User(type, "", op_nums), _parent(parent), _op_id(op_id) {
    if (_parent == nullptr) {
        WARNNING("null parent");
    }
    if (_parent != nullptr) _parent->addInstr(this);
}
Instruction::Instruction(Type *type, Instruction::OpKind op_id,
                         unsigned int op_nums)
    : Instruction(type, op_id, op_nums, nullptr) {}


bool Instruction::isCommutative()const{
     bool res =  isAdd()||isMul()||isOr()||isXor();
     if(!res){
        if(isCmp()){
            auto cmp = static_cast<  CmpInst const*>(this);
            res = cmp->isEq()||cmp->isNeq();
        }
     }
     return res;
}
UnaryInst::UnaryInst(Type *type, OpKind op_id, Value *v1)
    : Instruction(type, op_id, 1) {
    setOperand(0, v1);
}
UnaryInst::UnaryInst(Type *type, Instruction::OpKind op_id, Value *v1,
                     BasicBlock *parent)
    : Instruction(type, op_id, 1, parent) {
    setOperand(0, v1);
}
// UnaryInst *UnaryInst::createNeg(Value *v1, BasicBlock *parent) {
//     return new UnaryInst(v1->getType(), Instruction::NEG, v1, parent);
// }
UnaryInst *UnaryInst::createNot(Value *v1, BasicBlock *parent) {
    return new UnaryInst(v1->getType(), Instruction::NOT, v1, parent);
}
UnaryInst *UnaryInst::createCast(Value *v1, Type *type_cast_to,
                                 BasicBlock *parent) {
    if (type_cast_to->isFloatTy()) {
        MyAssert("error type", v1->getType()->isInt32());
    } else if (type_cast_to->isInt32()) {
        MyAssert("error type", v1->getType()->isFloatTy());
    } else {
        ERROR("error type");
    }
    return new UnaryInst(type_cast_to, Instruction::CAST, v1, parent);
}

BinaryInst::BinaryInst(Type *type, Instruction::OpKind op_id, Value *v1,
                       Value *v2)
    : Instruction(type, op_id, 2) {
    setOperand(0, v1);
    setOperand(1, v2);
}
// BinaryInst *BinaryInst::createAdd(Value *v1, Value *v2) {
//     //fixme: 类型没设置好
//     return new BinaryInst(v1->getType(), Instruction::ADD, v1, v2);
// }
// BinaryInst *BinaryInst::createSub(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::SUB, v1, v2);
// }
// BinaryInst *BinaryInst::createMul(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::MUL, v1, v2);
// }
// BinaryInst *BinaryInst::createDiv(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::DIV, v1, v2);
// }
// BinaryInst *BinaryInst::createRem(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::REM, v1, v2);
// }
// BinaryInst *BinaryInst::createShl(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::SHL, v1, v2);
// }
// BinaryInst *BinaryInst::createLshr(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::LSHR, v1, v2);
// }
// BinaryInst *BinaryInst::createAshr(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::ASHR, v1, v2);
// }
// BinaryInst *BinaryInst::createAnd(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::AND, v1, v2);
// }
// BinaryInst *BinaryInst::createOr(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::OR, v1, v2);
// }
// BinaryInst *BinaryInst::createXor(Value *v1, Value *v2) {
//     return new BinaryInst(v1->getType(), Instruction::XOR, v1, v2);
// }
BinaryInst *BinaryInst::createAdd(Value *v1, Value *v2, BasicBlock *parent) {
    // fixme: 类型没设置好
    return new BinaryInst(v1->getType(), Instruction::ADD, v1, v2, parent);
}
BinaryInst *BinaryInst::createSub(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::SUB, v1, v2, parent);
}
BinaryInst *BinaryInst::createMul(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::MUL, v1, v2, parent);
}
BinaryInst *BinaryInst::createDiv(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::DIV, v1, v2, parent);
}
BinaryInst *BinaryInst::createRem(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::REM, v1, v2, parent);
}
BinaryInst *BinaryInst::createShl(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::SHL, v1, v2, parent);
}
BinaryInst *BinaryInst::createLshr(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::LSHR, v1, v2, parent);
}
BinaryInst *BinaryInst::createAshr(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::ASHR, v1, v2, parent);
}
BinaryInst *BinaryInst::createAnd(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::AND, v1, v2, parent);
}
BinaryInst *BinaryInst::createOr(Value *v1, Value *v2, BasicBlock *parent) {
    return new BinaryInst(v1->getType(), Instruction::OR, v1, v2, parent);
}
// BinaryInst *BinaryInst::createXor(Value *v1, Value *v2, BasicBlock *parent) {
//     return new BinaryInst(v1->getType(), Instruction::XOR, v1, v2, parent);
// }
BinaryInst::BinaryInst(Type *type, Instruction::OpKind op_id, Value *v1,
                       Value *v2, BasicBlock *parent)
    : Instruction(type, op_id, 2, parent) {
    setOperand(0, v1);
    setOperand(1, v2);
}
void BinaryInst::accept(IrVisitorBase *v) { v->visit(this); }
void UnaryInst::accept(IrVisitorBase *v) { v->visit(this); }
std::string BinaryInst::getOperatorString() const {
    auto op_kind = getInstructionKind();
    switch (op_kind) {
        case Instruction::ADD:
            if (getType()->isIntegerTy())
                return "add";
            else if (getType()->isFloatTy())
                return "fadd";
            else
                return "not supported";
        case Instruction::SUB: {
            if (getType()->isIntegerTy())
                return "sub";
            else if (getType()->isFloatTy())
                return "fsub";
            else
                return "not supported";
        }
        case Instruction::MUL: {
            if (getType()->isIntegerTy())
                return "mul";
            else if (getType()->isFloatTy())
                return "fmul";
            else
                return "not supported";
        }
        case Instruction::DIV: {
            if (getType()->isIntegerTy())
                return "sdiv";
            else if (getType()->isFloatTy())
                return "fdiv";
            else
                return "not supported";
        }
        case Instruction::REM: {
            if (getType()->isIntegerTy())
                return "srem";
            else if (getType()->isFloatTy())
                return "frem";
            else
                return "not supported";
        }

            //        case Instruction::SHL:return "shl";
            //        case Instruction::LSHR:return "lshr";
            //        case Instruction::ASHR:return "ashr";
        case Instruction::AND:
            return "and";
        case Instruction::OR:
            return "or";
            //        case Instruction::XOR:return "xor";
        default:
            ERROR("error binary op kind");
    }
}
ReturnInst::ReturnInst(Type *type, Instruction::OpKind op_id, Value *v)
    : Instruction(type, op_id, 1) {
    setOperand(0, v);
}
ReturnInst::ReturnInst(Type *type, Value *v)
    : Instruction(type, Instruction::RET, 1) {
    setOperand(0, v);
}
ReturnInst *ReturnInst::createRet(Value *v, BasicBlock *parent) {
    return new ReturnInst(Type::getVoidTy(), v, parent);
}
ReturnInst::ReturnInst(Type *type, Value *v, BasicBlock *parent)
    : Instruction(Type::getVoidTy(), OpKind::RET, 1, parent) {
    MyAssert("error return type",
             v->getType()->isIntegerTy() || v->getType()->isFloatTy());
    setOperand(0, v);
}
ReturnInst *ReturnInst::createVoidRet(BasicBlock *parent) {
    return new ReturnInst(Type::getVoidTy(), parent);
}
ReturnInst::ReturnInst(Type *type, BasicBlock *parent)
    : Instruction(type, Instruction::RET, 0, parent) {}
void ReturnInst::accept(IrVisitorBase *v) { v->visit(this); }
CmpInst::CmpInst(Type *type, CmpInst::CmpOp op_id, Value *v1, Value *v2,
                 BasicBlock *parent)
    : Instruction(type, Instruction::CMP, 2, parent), _cmp_op(op_id) {
    setOperand(0, v1);
    setOperand(1, v2);
}
CmpInst::CmpInst(Type *type, CmpInst::CmpOp op_id, Value *v1, Value *v2)
    : Instruction(type, Instruction::CMP, 2), _cmp_op(op_id) {
    setOperand(0, v1);
    setOperand(1, v2);
}
// CmpInst *CmpInst::createCmp(Type *type, CmpInst::CmpOp op_id, Value *v1,
// Value *v2) {
//     return new CmpInst(type, op_id, v1, v2);
// }
// CmpInst *CmpInst::createEQ(Type *type, Value *v1, Value *v2) {
//     return new CmpInst(type, CmpOp::EQ, v1, v2);
// }
// CmpInst *CmpInst::createNEQ(Type *type, Value *v1, Value *v2) {
//     return new CmpInst(type, CmpOp::NEQ, v1, v2);
// }
// CmpInst *CmpInst::createGT(Type *type, Value *v1, Value *v2) {
//     return new CmpInst(type, CmpOp::GT, v1, v2);
// }
// CmpInst *CmpInst::createGE(Type *type, Value *v1, Value *v2) {
//     return new CmpInst(type, CmpOp::GE, v1, v2);
// }
// CmpInst *CmpInst::createLT(Type *type, Value *v1, Value *v2) {
//     return new CmpInst(type, CmpOp::LT, v1, v2);
// }
// CmpInst *CmpInst::createLE(Type *type, Value *v1, Value *v2) {
//     return new CmpInst(type, CmpOp::LE, v1, v2);
// }
CmpInst *CmpInst::createEQ(Type *type, Value *v1, Value *v2,
                           BasicBlock *parent) {
    return new CmpInst(type, CmpOp::EQ, v1, v2, parent);
}
CmpInst *CmpInst::createNEQ(Type *type, Value *v1, Value *v2,
                            BasicBlock *parent) {
    return new CmpInst(type, CmpOp::NEQ, v1, v2, parent);
}
CmpInst *CmpInst::createGT(Type *type, Value *v1, Value *v2,
                           BasicBlock *parent) {
    return new CmpInst(type, CmpOp::GT, v1, v2, parent);
}
CmpInst *CmpInst::createGE(Type *type, Value *v1, Value *v2,
                           BasicBlock *parent) {
    return new CmpInst(type, CmpOp::GE, v1, v2, parent);
}
CmpInst *CmpInst::createLT(Type *type, Value *v1, Value *v2,
                           BasicBlock *parent) {
    return new CmpInst(type, CmpOp::LT, v1, v2, parent);
}
CmpInst *CmpInst::createLE(Type *type, Value *v1, Value *v2,
                           BasicBlock *parent) {
    return new CmpInst(type, CmpOp::LE, v1, v2, parent);
}
void CmpInst::accept(IrVisitorBase *v) { v->visit(this); }
BranchInst::BranchInst(BranchInst::BrOp br_op, Value *cond, Value *true_block,
                       Value *false_block, BasicBlock *parent)
    : Instruction(Type::getVoidTy(), Instruction::BR, 3, parent),
      _br_kind(br_op) {
    setOperand(0, cond);
    setOperand(1, true_block);
    setOperand(2, false_block);
}
BranchInst::BranchInst(BranchInst::BrOp br_op, Value *cond, Value *block,
                       BasicBlock *parent)
    : Instruction(Type::getVoidTy(), Instruction::BR, 2, parent),
      _br_kind(br_op) {
    setOperand(0, cond);
    setOperand(1, block);
}
BranchInst::BranchInst(BranchInst::BrOp br_op, Value *block, BasicBlock *parent)
    : Instruction(Type::getVoidTy(), Instruction::BR, 1, parent),
      _br_kind(br_op) {
    setOperand(0, block);
}
BranchInst *BranchInst::createIf(Value *cond, BasicBlock *true_block,
                                 BasicBlock *false_block, BasicBlock *parent) {
    true_block->addPreBasicBlock(parent);
    false_block->addPreBasicBlock(parent);
    parent->addSuccBasicBlock(true_block);
    parent->addSuccBasicBlock(false_block);
    return new BranchInst(BranchInst::IF, cond, true_block, false_block,
                          parent);
}
BranchInst *BranchInst::createWhile(Value *cond, BasicBlock *block,
                                    BasicBlock *parent) {
    return new BranchInst(BranchInst::WHILE, cond, block, parent);
}
BranchInst *BranchInst::createBranch(BasicBlock *block, BasicBlock *parent) {
    parent->addSuccBasicBlock(block);
    block->addPreBasicBlock(parent);
    return new BranchInst(BranchInst::BRANCH, block, parent);
}

BranchInst *BranchInst::createCondBr(Value *cond, BasicBlock *if_true,
                                     BasicBlock *if_false, BasicBlock *bb) {
    if_true->addPreBasicBlock(bb);
    if_false->addPreBasicBlock(bb);
    bb->addSuccBasicBlock(if_false);
    bb->addSuccBasicBlock(if_true);

    return new BranchInst(BranchInst::COND_BRANCH, cond, if_true, if_false, bb);
}

void BranchInst::accept(IrVisitorBase *v) { v->visit(this); }

StoreInst::StoreInst(Value *value, Value *ptr, BasicBlock *parent)
    : Instruction(Type::getVoidTy(), Instruction::STORE, 2, parent) {
    setOperand(0, value);
    setOperand(1, ptr);
}
StoreInst *StoreInst::createStore(Value *value, Value *ptr,
                                  BasicBlock *parent) {
    return new StoreInst(value, ptr, parent);
}
void StoreInst::accept(IrVisitorBase *v) { v->visit(this); }

// DYB
StoreOffset::StoreOffset(Value *value,int offset, BasicBlock *parent)
    : Instruction(Type::getVoidTy(), Instruction::STORE_OFFSET, 1, parent) {
    setOperand(0, value);
    this->offset = offset;
}
StoreOffset *StoreOffset::createStoreOffset(Value *value,int offset,BasicBlock *parent) {
    return new StoreOffset(value, offset, parent);
}
void StoreOffset::accept(IrVisitorBase *v) { v->visit(this); }

LoadOffset::LoadOffset(Value *value,int offset, BasicBlock *parent)
    : Instruction(Type::getVoidTy(), Instruction::LOAD_OFFSET, 1, parent) {
    setOperand(0, value);
    this->offset = offset;
}
LoadOffset *LoadOffset::createLoadOffset(Value *value,int offset,BasicBlock *parent) {
    return new LoadOffset(value, offset, parent);
}
void LoadOffset::accept(IrVisitorBase *v) { v->visit(this); }
// DYB

LoadInst::LoadInst(Value *ptr, BasicBlock *parent)
    : Instruction(ptr->getType()->getPointerElementType(), Instruction::LOAD, 1,
                  parent) {
    setOperand(0, ptr);
}
LoadInst *LoadInst::createLoad(Value *ptr, BasicBlock *parent) {
    return new LoadInst(ptr, parent);
}
void LoadInst::accept(IrVisitorBase *v) { v->visit(this); }
GetElementPtrInst::GetElementPtrInst(Type *ty, unsigned int num_ops,
                                     BasicBlock *parent, Type *elem_ty)
    : Instruction(ty, Instruction::GEP, num_ops, parent), _elem_ty(elem_ty) {
    ERROR("depreciated");
}
GetElementPtrInst::GetElementPtrInst(Value *ptr, std::vector<Value *> &idxs,
                                     BasicBlock *parent)
    : Instruction(PointerType::get(getElementType(ptr, idxs)), Instruction::GEP,
                  1 + idxs.size(), parent) {
    setOperand(0, ptr);
    for (int i = 0; i < idxs.size(); i++) {
        setOperand(i + 1, idxs[i]);
    }
    // ptr[idxs]的类型
    _elem_ty = getElementType(ptr, idxs);
}
Type *GetElementPtrInst::getElementType(Value *ptr,
                                        std::vector<Value *> &idxs) {
    Type *ty = ptr->getType()->getPointerElementType();
    MyAssert("error type",
             ty->isIntegerTy() || ty->isFloatTy() || ty->isArrayTy());
    if (ty->isIntegerTy() || ty->isFloatTy()) {
        return ty;
    } else if (ty->isArrayTy()) {
        auto *arr_type = static_cast<ArrayType *>(ty);
        for (int i = 1; i < idxs.size(); ++i) {
            ty = arr_type->getElementType();
            if (i < idxs.size() - 1) {
                MyAssert("error type", ty->isArrayTy());
            }
            if (ty->isArrayTy()) {
                arr_type = static_cast<ArrayType *>(ty);
            }
        }
    }
    return ty;
}
Type *GetElementPtrInst::getElementType() const { return _elem_ty; }
GetElementPtrInst *GetElementPtrInst::createGEP(Value *ptr,
                                                std::vector<Value *> &idxs,
                                                BasicBlock *parent) {
    return new GetElementPtrInst(ptr, idxs, parent);
}
void GetElementPtrInst::accept(IrVisitorBase *v) { v->visit(this); }
CallInst::CallInst(Function *func, BasicBlock *parent)
    : Instruction(func->getResultType(), Instruction::CALL,
                  func->getNumArgs() + 1, parent) {
    MyAssert("call error args number ", func->getNumArgs() == 0);
    setOperand(0, func);
}
CallInst::CallInst(Function *func, std::vector<Value *> &args,
                   BasicBlock *parent)
    : Instruction(func->getResultType(), Instruction::CALL, args.size() + 1,
                  parent) {
    MyAssert("call error args number ", func->getNumArgs() == args.size());
    setOperand(0, func);
    for (int i = 0; i < args.size(); i++) {
        setOperand(i + 1, args[i]);
    }
}
CallInst *CallInst::createCall(Function *func, std::vector<Value *> &args,
                               BasicBlock *parent) {
    return new CallInst(func, args, parent);
}
FunctionType *CallInst::getFunctionType() const {
    return static_cast<FunctionType *>(getOperand(0)->getType());
}
Function *CallInst::getFunction() const {
    return static_cast<Function *>(getOperand(0));
}
void CallInst::accept(IrVisitorBase *v) { v->visit(this); }

ZExtInst::ZExtInst(Type *ty, Value *val, BasicBlock *parent)
    : Instruction(ty, Instruction::ZEXT, 1, parent), _dest_ty(ty) {
    setOperand(0, val);
}
ZExtInst *ZExtInst::creatZExtInst(Type *ty, Value *val, BasicBlock *parent) {
    return new ZExtInst(ty, val, parent);
}
Type *ZExtInst::getDestType() const { return _dest_ty; }
void ZExtInst::accept(IrVisitorBase *v) { v->visit(this); }
AllocaInst::AllocaInst(Type *ty, BasicBlock *parent)
    : Instruction(PointerType::get(ty), Instruction::ALLOCA, 0, parent),
      _alloca_ty(ty),
      _init(false) {}
AllocaInst *AllocaInst::createAlloca(Type *ty, BasicBlock *parent) {
    return new AllocaInst(ty, parent);
}
Type *AllocaInst::getAllocaType() const { return _alloca_ty; }
void AllocaInst::accept(IrVisitorBase *v) { v->visit(this); }
HIR::HIR(Type *type, OpKind op_id, BasicBlock *parent)
    : Instruction(type, op_id, 0, parent) {}
HIR *HIR::createBreak(BasicBlock *parent) {
    return new HIR(Type::getVoidTy(), Instruction::BREAK, parent);
}
HIR *HIR::createContinue(BasicBlock *parent) {
    return new HIR(Type::getVoidTy(), Instruction::CONTINUE, parent);
}
void HIR::accept(IrVisitorBase *v) { v->visit(this); }
PhiInstr::PhiInstr(Type *ty, std::vector<Value *> vals,
                   std::vector<BasicBlock *> val_bbs, BasicBlock *parent)
    : Instruction(ty, Instruction::PHI, 2 * vals.size()) {
    parent->getInstructions().push_front(this);
    for (int i = 0; i < vals.size(); i++) {
        setOperand(2 * i, vals[i]);
        setOperand(2 * i + 1, val_bbs[i]);
    }
}
PhiInstr *PhiInstr::createPhi(Type *ty, BasicBlock *bb) {
    return new PhiInstr(ty, 0, bb);
}
void PhiInstr::accept(IrVisitorBase *v) { v->visit(this); }
