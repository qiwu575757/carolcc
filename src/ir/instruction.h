#ifndef COMPILER_INSTRUCTION_H
#define COMPILER_INSTRUCTION_H
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

        // other instructions
        CMP,
        PHI,
        GEP,  // get element ptr
        CALL,
        ZEXT,
        // HIR
        BREAK,
        CONTINUE,

    };

    OpKind getInstructionKind() const { return _op_id; }
    bool isRet() { return _op_id == Instruction::RET; }
    bool isBr() { return _op_id == Instruction::BR; }
    //    bool isNeg() { return _op_id == Instruction::NEG; }
    bool isNot() { return _op_id == Instruction::NOT; }
    bool isAdd() { return _op_id == Instruction::ADD; }
    bool isSub() { return _op_id == Instruction::SUB; }
    bool isMul() { return _op_id == Instruction::MUL; }
    bool isDiv() { return _op_id == Instruction::DIV; }
    bool isRem() { return _op_id == Instruction::REM; }
    bool isShl() { return _op_id == Instruction::SHL; }
    bool isLshr() { return _op_id == Instruction::LSHR; }
    bool isAshr() { return _op_id == Instruction::ASHR; }
    bool isAnd() { return _op_id == Instruction::AND; }
    bool isOr() { return _op_id == Instruction::OR; }
    bool isXor() { return _op_id == Instruction::XOR; }
    bool isAlloca() { return _op_id == Instruction::ALLOCA; }
    bool isLoad() { return _op_id == Instruction::LOAD; }
    bool isStore() { return _op_id == Instruction::STORE; }
    bool isCmp() { return _op_id == Instruction::CMP; }
    bool isPhi() { return _op_id == Instruction::PHI; }
    bool isGep() { return _op_id == Instruction::GEP; }
    bool isCall() { return _op_id == Instruction::CALL; }
    bool isZext() { return _op_id == Instruction::ZEXT; }
    bool isBreak() { return _op_id == Instruction::BREAK; }
    bool isContinue() { return _op_id == Instruction::CONTINUE; }
    inline BasicBlock *getParent() { return _parent; }

   protected:
    Instruction(Type *type, OpKind op_id, unsigned op_nums);
    Instruction(Type *type, OpKind op_id, unsigned op_nums, BasicBlock *parent);

   private:
    BasicBlock *_parent;
    OpKind _op_id;
};

class UnaryInst : public Instruction {
   private:
    UnaryInst(Type *type, OpKind op_id, Value *v1);
    UnaryInst(Type *type, OpKind op_id, Value *v1, BasicBlock *parent);

   public:
    //    static UnaryInst *createNeg(Value *v1, BasicBlock *parent);
    static UnaryInst *createNot(Value *v1, BasicBlock *parent);
};

class BinaryInst : public Instruction {
   private:
    BinaryInst(Type *type, OpKind op_id, Value *v1, Value *v2);
    BinaryInst(Type *type, OpKind op_id, Value *v1, Value *v2,
               BasicBlock *parent);

   public:
    static BinaryInst *createAdd(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createSub(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createMul(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createDiv(Value *v1, Value *v2, BasicBlock *parent);
    static BinaryInst *createRem(Value *v1, Value *v2, BasicBlock *parent);
    //    static BinaryInst *createShl(Value *v1, Value *v2, BasicBlock
    //    *parent); static BinaryInst *createLshr(Value *v1, Value *v2,
    //    BasicBlock *parent); static BinaryInst *createAshr(Value *v1, Value
    //    *v2, BasicBlock *parent);
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

    bool isEq() { return _cmp_op == CmpInst::EQ; }
    bool isNeq() { return _cmp_op == CmpInst::NEQ; }
    bool isGt() { return _cmp_op == CmpInst::GT; }
    bool isGe() { return _cmp_op == CmpInst::GE; }
    bool isLt() { return _cmp_op == CmpInst::LT; }
    bool isLe() { return _cmp_op == CmpInst::LE; }

   private:
    CmpInst(Type *type, CmpOp op_id, Value *v1, Value *v2);
    CmpInst(Type *type, CmpOp op_id, Value *v1, Value *v2, BasicBlock *parent);
};
class ReturnInst : public Instruction {
   private:
    ReturnInst(Type *type, BasicBlock *parent);
    ReturnInst(Type *type, Value *v);
    ReturnInst(Type *type, Value *v, BasicBlock *parent);
    ReturnInst(Type *type, OpKind op_id, Value *v);

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
    };
    void accept(IrVisitorBase *v) override;

    bool isWhile() { return _br_kind == BranchInst::WHILE; }
    bool isIf() { return _br_kind == BranchInst::IF; }
    bool isBranch() { return _br_kind == BranchInst::BRANCH; }

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
    Value *getLVal() { return getOperand(1); }
    Value *getRVal() { return getOperand(0); }
};
class LoadInst : public Instruction {
   private:
    LoadInst(Value *ptr, BasicBlock *parent);

   public:
    static LoadInst *createLoad(Value *ptr, BasicBlock *parent);
    void accept(IrVisitorBase *v) override;
};
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
        parent->getInstructions().push_front(this);
    };

   public:
    void accept(IrVisitorBase *v) override;
    static PhiInstr *createPhi(Type *ty, BasicBlock *bb);
    static PhiInstr *createPhi(Type *ty, unsigned num_ops, BasicBlock *parent) {
        return new PhiInstr(ty, num_ops, parent);
    };
    Value *getLVal() { return getOperand(0); }
    void *setLVal(Value *v) { setOperand(0, v); }
    void setParams(Value *val, Value *pre_bb) {
        this->addOperand(val);
        this->addOperand(pre_bb);
    }
};

#endif  // COMPILER_INSTRUCTION_H
