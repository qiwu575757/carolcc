
#ifndef COMPILER_FUNCTION_H
#define COMPILER_FUNCTION_H

#include "BaseBlock.h"
#include "helpers/type_helper.h"
#include "type.h"
#include "utils.h"
#include "value.h"
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

extern error_log function_logger;
class Argument : public Value {
public:
    Argument(Type *type, const std::string &name, Function *parent, unsigned arg_no);
    inline Function *getParent() const { return _parent; }
    unsigned getArgNo() const {
        if (_parent == nullptr) {
            ERROR("Argument have no parent");
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


class Function : public Value {
public:
    Function(FunctionType *type, const std::string &name, Module *parent);
    static Function *create(FunctionType *type, const std::string &name, Module *parent);

    FunctionType *getFunctionType() const;
    Type *getResultType() const;
    Module *getParent() const;

    // Method about basicblock
    void addBasicBlock(BasicBlock *basicblock);
    void addBasicBlockAfter(std::vector<BasicBlock *>::iterator after_pos,
                            BasicBlock *bb);
    unsigned getNumBasicBlocks() const {
        return _basic_block_list.size();
    }
    std::vector<BasicBlock *> &getBasicBlocks() {
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
        std::vector<BaseBlock *>::iterator pos = find(_base_block_list.begin(), _base_block_list.end(), baseblock);
        _base_block_list.erase(pos);
    }
    void addBaseBlock(BaseBlock *baseblock);
    void insertBaseBlock(std::vector<BaseBlock *>::iterator iter,
                         BaseBlock *baseblock) {
        _base_block_list.insert(iter, baseblock);
        baseblock->clearFather();
    }
    std::vector<BaseBlock *> &getBaseBlocks() {
        return _base_block_list;
    }

private:
    std::vector<Argument *> _args;
    std::vector<BaseBlock *> _base_block_list;
    Module *_parent;
    std::vector<BasicBlock *> _basic_block_list;

private:
    void buildArgs();
};
#endif//COMPILER_FUNCTION_H
