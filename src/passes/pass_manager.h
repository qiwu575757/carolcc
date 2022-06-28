
#ifndef COMPILER_PASS_MANAGER_H
#define COMPILER_PASS_MANAGER_H
#include "pass.h"
#include "module.h"
#include <string>
#include <vector>
class pass_manager
{
private:
    std::vector<pass*> passes;
    Module *_m;
public:
    pass_manager(Module* m):_m(m){};
    ~pass_manager() = default;
    template <typename pass_type> void add_pass(const std::string &name);

    void run();
};


#endif//COMPILER_PASS_MANAGER_H