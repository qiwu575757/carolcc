#ifndef _LAZY_CODE_MOTION_H
#define _LAZY_CODE_MOTION_H

#include "pass.h"
class Module;
class Function;
class LazyCodeMotion : public Transform {
    public:
    LazyCodeMotion(Module *m, const std::string &name) : Transform(m,name) {}
    void run() override;
    private:
    void init(Function * func);
};
#endif // !_LAZY_CODE_MOTION_H