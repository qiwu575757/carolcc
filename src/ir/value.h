#ifndef COMPILER_VALUE_H
#define COMPILER_VALUE_H
#include "helpers/type_helper.h"
#include <string>
#include <vector>
class value;
class type;
class variable;
class value
{
private:
    /* data */
public:
    value(/* args */)=default;
    ~value()=default;
};

class variable:public value
{
public:
    variable(std::string &id,type* type):value(),_id(id),_type(type){}
private:
    std::string _id;
    type* _type;

};

class array : public variable{
public:
    array(std::string &id,type* type): variable(id,type){};
    void add_exp(value* exp);
private:
    std::vector<value*> _exps;
};




#endif // !COMPILER_VALUE_H