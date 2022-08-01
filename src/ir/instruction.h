#ifndef COMPILER_INSTRUCTION_H
#define COMPILER_INSTRUCTION_H
#include <algorithm>
#include <cctype>
#include <vector>

#include "basic_block.h"
#include "user.h"

class BasicBlock;
class Instruction : public User {
   public:
    enum OpKind {
        // terminator instructions
        RET,
        BR,

        // unary instructions
        //        NEG,
        NOT,
        // binary instructions
        ADD,
        //        FADD,//float add
        SUB,
        //        FSUB,
        MUL,
        DIV,
        REM,
        //        FMUL,

        // bitwise binary instructions
        SHL,
        LSHR,
        ASHR,
        AND,
        OR,
        XOR,

        // memory access nad addressing operations
        ALLOCA,
        LOAD,
        STORE,
        //
        STORE_OFFSET,//自定义指令
        LOAD_OFFSET,
        // other instructions
        CMP,
        PHI,
        GEP,  // get element ptr， will be replaced by mla
        MLA,
        CALL,
        ZEXT,
        CAST,

        // HIR
        BREAK,
        CONTINUE
    };

    OpKind getInstructionKind() const { return _op_id; }
    bool isRet() const { return _op_id == Instruction::RET; }
    bool isBr() const { return _op_id == Instruction::BR; }
    //    bool isNeg()const { return _op_id == Instruction::NEG; }
    bool isNot() const { return _op_id == Instruction::NOT; }
    bool isCast() const { return _op_id == Instruction::CAST; }

    bool isAdd() const { return _op_id == Instruction::ADD; }
    bool isSub() const { return _op_id == Instruction::SUB; }
    bool isMul() const { return _op_id == Instruction::MUL; }
    bool isDiv() const { return _op_id == Instruction::DIV; }
    bool isRem() const { return _op_id == Instruction::REM; }
    bool isShl() const { return _op_id == Instruction::SHL; }
    bool isLshr() const { return _op_id == Instruction::LSHR; }
    bool isAshr() const { return _op_id == Instruction::ASHR; }
    bool isAnd() const { return _op_id == Instruction::AND; }
    bool isOr() const { return _op_id == Instruction::OR; }
    bool isXor() const { return _op_id == Instruction::XOR; }
    bool isBinary() const {return isAdd() || isSub() || isMul() || isDiv()||isRem()||isShl()||isLshr()||isAnd()||isOr()||isXor()||isCmp();}
    bool isCommutative() const;

    bool isAlloca() const { return _op_id == Instruction::ALLOCA; }
    bool isLoad() const { return _op_id == Instruction::LOAD; }
    bool isStore() const { return _op_id == Instruction::STORE; }
    bool isCmp() const { return _op_id == Instruction::CMP; }
    bool isPhi() const { return _op_id == Instruction::PHI; }
    bool isGep() const { return _op_id == Instruction::GEP; }
    bool isMla() const { return _op_id == Instruction::MLA; }
    bool isCall() const { return _op_id == Instruction::CALL; }
    bool isZext() const { return _op_id == Instruction::ZEXT; }
    bool isBreak() const { return _op_id == Instruction::BREAK; }
    bool isContinue() const { return _op_id == Instruction::CONTINUE; }
    bool isSTORE_OFFSET() const { return _op_id == Instruction::STORE_OFFSET; }
    bool isLOAD_OFFSET() const { return _op_id == Instruction::LOAD_OFFSET; }
    BasicBlock *getParent() const { return _parent; }
    void setParent(BasicBlock *parent) { _parent = parent; }
    Function *getFunction() const { return getParent()->getParentFunc(); }

   protected:
    Instruction(Type *type, OpKind op_id, unsigned op_nums);
    Instruction(Type *type, OpKind op_id, unsigned op_nums, BasicBlock *parent);

   private:
    BasicBlock *_parent;
    OpKind _op_id;
};

class UnaryInst : public Instruction {
   private:
    // UnaryInst(Type *type, OpKind op_id, Value *v1);
    UnaryInst(Type *type, OpKind op_id, Value *v1, BasicBlock *parent);

   public:
    //    static UnaryInst *createNeg(Value *v1, BasicBlock *parent);
    static UnaryInst *createNot(Value *v1, BasicBlock *parent);
    static UnaryInst *createCast(Value *v1, Type *type_cast_to,
                                 BasicBlock *parent);
    void accept(IrVisitorBase *v) override;
};

class BinaryInst : public Instruction {
   private:
    // BinaryInst(Type *type, OpKind op_id, Value *v1, Value *v2);
    BinaryInst(Type *type, OpKind op_id, Value *v1, Value *v2,
               BasicBlock *parent);

   public:
    static BinaryInst *createAdd(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createSub(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createMul(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createDiv(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createRem(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createShl(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createLshr(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createAshr(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createAnd(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createOr(Value *v1, Value *v2, BasicBlock *parent);
    //    static BinaryInst *createXor(Value *v1, Value *v2, BasicBlock
    //    *parent);
    std::string getOperatorString() const;
    void accept(IrVisitorBase *v) override;
};

class CmpInst : public Instruction {
   public:
    enum CmpOp { EQ, NEQ, GT, GE, LT, LE };
    CmpOp _cmp_op;
    //    static CmpInst *createCmp(Type *type,CmpOp op_id,Value *v1,Value *v2);
    static CmpInst *createEQ(Type *type, Value *v1, Value *v2,
                             BasicBlock *parent);
    static CmpInst *createNEQ(Type *type, Value *v1, Value *v2,
                              BasicBlock *parent);
    static CmpInst *createGT(Type *type, Value *v1, Value *v2,
                             BasicBlock *parent);
    static CmpInst *createGE(Type *type, Value *v1, Value *v2,
                             BasicBlock *parent);
    static CmpInst *createLT(Type *type, Value *v1, Value *v2,
                             BasicBlock *parent);
    static CmpInst *createLE(Type *type, Value *v1, Value *v2,
                             BasicBlock *parent);
    void accept(IrVisitorBase *v) override;

    bool isEq()const { return _cmp_op == CmpInst::EQ; }
    bool isNeq()const { return _cmp_op == CmpInst::NEQ; }
    bool isGt() const{ return _cmp_op == CmpInst::GT; }
    bool isGe() const{ return _cmp_op == CmpInst::GE; }
    bool isLt() const{ return _cmp_op == CmpInst::LT; }
    bool isLe() const{ return _cmp_op == CmpInst::LE; }
    CmpOp getCmpOp() { return _cmp_op; }

   private:
    // CmpInst(Type *type, CmpOp op_id, Value *v1, Value *v2);
    CmpInst(Type *type, CmpOp op_id, Value *v1, Value *v2, BasicBlock *parent);
};
class ReturnInst : public Instruction {
   private:
    ReturnInst(Type *type, BasicBlock *parent);
    // ReturnInst(Type *type, Value *v);
    ReturnInst(Type *type, Value *v, BasicBlock *parent);
    // ReturnInst(Type *type, OpKind op_id, Value *v);

   public:
    static ReturnInst *createRet(Value *v, BasicBlock *parent);
    static ReturnInst *createVoidRet(BasicBlock *parent);
    void accept(IrVisitorBase *v) override;
};

class BranchInst : public Instruction {
   public:
    enum BrOp {
        WHILE,
        IF,
        BRANCH,
        COND_BRANCH,
    };
    void accept(IrVisitorBase *v) override;

    bool isWhile() { return _br_kind == BranchInst::WHILE; }
    bool isIf() { return _br_kind == BranchInst::IF; }
    bool isBranch() { return _br_kind == BranchInst::BRANCH; }
    bool isConBranch() { return _br_kind == BranchInst::COND_BRANCH; }

   private:
    BrOp _br_kind;
    BranchInst(BrOp br_op, Value *cond, Value *true_block, Value *false_block,
               BasicBlock *parent);
    BranchInst(BrOp br_op, Value *cond, Value *block, BasicBlock *parent);
    BranchInst(BrOp br_op, Value *block, BasicBlock *parent);

   public:
    static BranchInst *createIf(Value *cond, BasicBlock *true_block,
                                BasicBlock *false_block, BasicBlock *parent);
    static BranchInst *createWhile(Value *cond, BasicBlock *block,
                                   BasicBlock *parent);
    static BranchInst *createBranch(BasicBlock *block, BasicBlock *parent);
    static BranchInst *createCondBr(Value *cond, BasicBlock *if_true,
                                    BasicBlock *if_false, BasicBlock *bb);
};

class StoreInst : public Instruction {
   private:
    StoreInst(Value *value, Value *ptr, BasicBlock *parent);

   public:
    static StoreInst *createStore(Value *value, Value *ptr, BasicBlock *parent);
    void accept(IrVisitorBase *v) override;
    std::string getPrintName() override;
    Value *getLVal() { return getOperand(1); }
    Value *getRVal() { return getOperand(0); }
};
// DYB DEFINE
class StoreOffset : public Instruction {
   private:
    StoreOffset(Value *value,int offset, BasicBlock *parent);
   public:
    int offset;
    static StoreOffset *createStoreOffset(Value *value,int offset, BasicBlock *parent);
    void accept(IrVisitorBase *v) override;
};
class LoadOffset : public Instruction {
   private:
    LoadOffset(Value *value,int offset, BasicBlock *parent);
   public:
    int offset;
    static LoadOffset *createLoadOffset(Value *value,int offset, BasicBlock *parent);
    void accept(IrVisitorBase *v) override;
};
//
class LoadInst : public Instruction {
   private:
    LoadInst(Value *ptr, BasicBlock *parent);

   public:
    static LoadInst *createLoad(Value *ptr, BasicBlock *parent);
    void accept(IrVisitorBase *v) override;
};

// wuqi define
class MlaInst : public Instruction {
   private:
    MlaInst(Value *v1, Value *v2, Value *v3);
    MlaInst(Type *ty, Value *v1, Value *v2, Value *v3);
    MlaInst(Value *v1, Value *v2, Value *v3, BasicBlock *parent);
    MlaInst(Type *ty, OpKind id, BasicBlock *bb) : Instruction(ty, id, 3, bb) {}

   public:
    static MlaInst *createMlaInst(Value *v1, Value *v2, Value *v3);
    static MlaInst *createMlaInst(Type *ty, Value *v1, Value *v2, Value *v3);
    static MlaInst *createMlaInst(Value *v1, Value *v2, Value *v3,
                                      BasicBlock *bb);
    void accept(IrVisitorBase *v) override;
};
//

class GetElementPtrInst : public Instruction {
   private:
    GetElementPtrInst(Value *ptr, std::vector<Value *> &idxs,
                      BasicBlock *parent);
    GetElementPtrInst(Type *ty, unsigned num_ops, BasicBlock *parent,
                      Type *elem_ty);
    Type *_elem_ty;

   public:
    static Type *getElementType(Value *ptr, std::vector<Value *> &idxs);
    static GetElementPtrInst *createGEP(Value *ptr, std::vector<Value *> &idxs,
                                        BasicBlock *parent);
    Type *getElementType() const;
    void accept(IrVisitorBase *v) override;
};
class CallInst : public Instruction {
   private:
    CallInst(Function *func, std::vector<Value *> &args, BasicBlock *parent);
    CallInst(Function *func, BasicBlock *parent);

   public:
    void accept(IrVisitorBase *v) override;

   public:
    static CallInst *createCall(Function *func, std::vector<Value *> &args,
                                BasicBlock *parent);
    FunctionType *getFunctionType() const;
    Function *getFunction() const;
};

class ZExtInst : public Instruction {
   private:
    ZExtInst(Type *ty, Value *val, BasicBlock *parent);
    Type *_dest_ty;

   public:
    static ZExtInst *creatZExtInst(Type *ty, Value *val, BasicBlock *parent);
    Type *getDestType() const;
    void accept(IrVisitorBase *v) override;
};
class AllocaInst : public Instruction {
   private:
    AllocaInst(Type *ty, BasicBlock *parent);

    Type *_alloca_ty;
    bool _init;

   public:
    void accept(IrVisitorBase *v) override;
    static AllocaInst *createAlloca(Type *ty, BasicBlock *parent);
    void setInit() { _init = false; }
    bool getInit() { return _init; }
    Type *getAllocaType() const;
};
class HIR : public Instruction {
   private:
    HIR(Type *type, OpKind op_id, BasicBlock *parent);

   public:
    static HIR *createBreak(BasicBlock *parent);
    static HIR *createContinue(BasicBlock *parent);
    void accept(IrVisitorBase *v) override;
};

class PhiInstr : public Instruction {
   private:
    PhiInstr(Type *ty, std::vector<Value *> vals,
             std::vector<BasicBlock *> val_bbs, BasicBlock *parent);

    PhiInstr(Type *ty, unsigned num_ops, BasicBlock *parent)
        : Instruction(ty, Instruction::PHI, num_ops) {
        this->setParent(parent);
        parent->getInstructions().push_front(this);
    };

   public:
    void accept(IrVisitorBase *v) override;
    static PhiInstr *createPhi(Type *ty, BasicBlock *bb);
    static PhiInstr *createPhi(Type *ty, unsigned num_ops, BasicBlock *parent) {
        return new PhiInstr(ty, num_ops, parent);
    };
    Value *getLVal() { return getOperand(0); }
    void setLVal(Value *v) { setOperand(0, v); }
    void setParams(Value *val, Value *pre_bb) {
        this->addOperand(val);
        this->addOperand(pre_bb);
    }
};

#endif  // COMPILER_INSTRUCTION_H
