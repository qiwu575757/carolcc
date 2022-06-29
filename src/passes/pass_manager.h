#ifndef COMPILER_PASS_MANAGER_H
#define COMPILER_PASS_MANAGER_H
#include "pass.h"
#include <string>
#include <vector>
class Module;
class pass;
class pass_manager
{
private:
    std::vector<pass*> passes;
    Module *_m;
public:
    explicit pass_manager(Module* m):_m(m){};
    ~pass_manager() = default;
    template <typename pass_type> void add_pass(const std::string &name){
        passes.push_back(new pass_type(_m, name));
    }

    void run();
};


#endif//COMPILER_PASS_MANAGER_H