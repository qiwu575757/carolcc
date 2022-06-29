#ifndef COMPILER_PASS_H
#define COMPILER_PASS_H

//#include "module.h"
#include <memory>
#include <string>
class Module ;
class pass {
protected:
    std::string _name;
    Module *_m;

public:
    pass(Module *m, const std::string &name) : _m(m), _name(name){};
    void set_name(std::string &name) {
        _name = name;
    }
    ~pass() = default;
    virtual void run() = 0;
    std::string getName(){
        return _name;
    }
};

class Analysis : public pass {
public:
    Analysis(Module *m, const std::string &name) : pass(m, name) {}
};

class Transform : public pass {
public:
    Transform(Module *m, const std::string &name) : pass(m, name) {}
};

#endif// COMPILER_PASS_H