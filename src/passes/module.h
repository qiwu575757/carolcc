#ifndef COMPILER_MODULE_H
#define COMPILER_MODULE_H

#include "ir/function.h"
#include<vector>
#include<unordered_map>

class scope;
class function;
class varible;
class module
{
public:
    module(/* args */)=default;
    ~module()=default;
private:
    std::vector<function*> _func_list;
    std::vector<varible*> _global_variable_list;
};

class scope{
public:
    void enter();
    void exit();

private:
    std::vector<std::unordered_map<std::string,varible*>> _table;
};

#endif // !COMPILER_MODULE_H