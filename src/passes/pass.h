#ifndef COMPILER_PASS_H
#define COMPILER_PASS_H

#include "module.h"
#include <memory>
#include <string>
class pass {
protected:
    std::string _name;
    Module *_m;

public:
    pass(std::string &name) : _name(name){};
    void set_name(std::string &name) {
        _name = name;
    }
    ~pass() = default;
    virtual void run() = 0;
};

class Analysis : public pass {
public:
    Analysis(std::string &name) : pass(name) {}
};

class Transform : public pass {
public:
    Transform(std::string &name) : pass(name) {}
};

#endif// COMPILER_PASS_H