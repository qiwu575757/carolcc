
#ifndef COMPILER_FUNCTION_H
#define COMPILER_FUNCTION_H

#include "BaseBlock.h"
#include "helpers/type_helper.h"


#include <string>
#include <vector>

#include "value.h"
class Value;
class Function;
class Variable;
class Argument;
class Type;
class Argument : public  Value{
public:
    Argument(Type *type, const std::string &name,Function* parent,unsigned arg_no);
private:
    Function *_parent;
    unsigned _arg_no; // 第几个参数
    //TODO: bound_arrays

};



class Function :public Value {
public:
    Function(FuncType* type,const std::string &name);

private:
    std::vector<Argument*> _args;
    std::vector<BaseBlock *> _base_block_list;


};
#endif//COMPILER_FUNCTION_H
