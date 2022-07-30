
#ifndef COMPILER_FUNCTION_H
#define COMPILER_FUNCTION_H

#include "base_block.h"
#include "helpers/type_helper.h"
#include "type.h"
#include "utils.h"
#include "value.h"
#include "constant.h"
#include <algorithm>
#include <string>
#include <vector>

class Value;
class Function;
class Variable;
class Argument;
class Type;
class Module;
class BaseBlock;
class BasicBlock;
class AllocaInst;

extern error_log function_logger;
class Argument : public Value {
public:
    void accept(IrVisitorBase *v) override;
    Argument(Type *type, const std::string &name, Function *parent, unsigned arg_no);
    inline Function *getParent() const { return _parent; }
    unsigned getArgNo() const {
        if (_parent == nullptr) {
            ERROR("Argument have no parent",EXIT_CODE_ERROR_428 );
        }
        return _arg_no;
    }

    void setArrayBound(std::vector<Value *> &array_bound) {
        _array_bound.assign(array_bound.begin(), array_bound.end());
    }

    std::vector<Value *> &getArrayBound() { return _array_bound; }

private:
    Function *_parent;
    unsigned _arg_no;// 第几个参数
    std::vector<Value *> _array_bound;
};


class Function : public GlobalValue {
public:
    void addAlloca(AllocaInst*);
    void setAllocaEnd(AllocaInst*);
    Function(FunctionType *type, const std::string &name, Module *parent);
    static Function *create(FunctionType *type, const std::string &name, Module *parent);
    void accept(IrVisitorBase *v) override;
    FunctionType *getFunctionType() const;
    Type *getResultType() const;
    Module *getParent() const;

    // Method about basicblock
    void addBasicBlock(BasicBlock *basicblock);
    void addBasicBlockAfter(std::_List_iterator<BasicBlock *> after_pos,
                            BasicBlock *bb);
    unsigned getNumBasicBlocks() const {
        return _basic_block_list.size();
    }
    std::list<BasicBlock *> &getBasicBlocks() {
        return _basic_block_list;
    }
    BasicBlock *getEntryBlock() {
        return *_basic_block_list.begin();
    }
    void removeBasicBlock(BasicBlock *basicblock);

    // Methods about argument
    unsigned getNumArgs() const;

    std::vector<Argument *> &getArgs() {
        return _args;
    }
    std::vector<Argument *>::iterator arg_begin() {
        return _args.begin();
    }
    std::vector<Argument *>::iterator arg_end() {
        return _args.end();
    }

    // Method about baseblocklist
    void removeBaseBlock(BaseBlock *baseblock) {
        std::list<BaseBlock *>::iterator pos = find(_base_block_list.begin(), _base_block_list.end(), baseblock);
        _base_block_list.erase(pos);
    }
    void addBaseBlock(BaseBlock *baseblock);
    void insertBaseBlock(std::list<BaseBlock *>::iterator iter,
                         BaseBlock *baseblock) {
        _base_block_list.insert(iter, baseblock);
        baseblock->clearFather();
    }
    std::list<BaseBlock *> &getBaseBlocks() {
        return _base_block_list;
    }

    bool isBuiltin() const {return _is_builtin;}
    void setBuiltin(bool flag){_is_builtin = flag;}

private:
    Instruction* _alloca_end = nullptr;
    std::vector<Argument *> _args;
    std::list<BaseBlock *> _base_block_list;
    Module *_parent;
    std::list<BasicBlock *> _basic_block_list;
    bool  _is_builtin;

private:
    void buildArgs();
};
#endif//COMPILER_FUNCTION_H
