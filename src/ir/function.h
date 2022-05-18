
#ifndef COMPILER_FUNCTION_H
#define COMPILER_FUNCTION_H

#include "base_block.h"
#include "helpers/type_helper.h"


#include <string>
#include <vector>

#include "value.h"
class value;
class function;
class variable;
class arguments;
class type;


class arguments:public value{
public:
    explicit arguments(unsigned _arg_number):_arg_number(_arg_number){};
    void add_arg(variable* arg);
private:
    unsigned _arg_number;
    std::vector<variable*> _argument_list;
};


class function :public value{
public:
    function(type* _type,std::string &id,arguments* _args):_type(_type),_id(id),_args(_args){};

private:
    type* _type;
    std::string _id;
    arguments* _args;
    std::vector<base_block*> _base_block_list;

};
#endif//COMPILER_FUNCTION_H
