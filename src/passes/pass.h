#ifndef COMPILER_PASS_H
#define COMPILER_PASS_H

#include "module.h"
#include <memory>
#include <string>
class pass {
private:
    std::string _name;
    Module *_m;

public:
    pass(std::string &name, Module *m) : _name(name), _m(m){};
    void set_name(std::string &name) {
        _name = name;
    }

    ~pass() = default;
};


#endif// COMPILER_PASS_H