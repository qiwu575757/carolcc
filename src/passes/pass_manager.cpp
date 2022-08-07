#include "pass_manager.h"
#include "pass.h"
#include "hir_to_mir.h"
#include "utils.h"
#include "module.h"
//template <typename pass_type> void pass_manager::add_pass(const std::string &name){
//    passes.push_back(new pass_type(name));
//}




void pass_manager::run() {
    auto i = 0;
    for (pass* ps : passes) {
        i++;
        MyAssert("passes is nullptr",ps != nullptr,EXIT_CODE_ERROR_350);
        try {
            printf("running pass [%s]\n",ps->getName().c_str());
            ps->run();
        }
        catch (...) {
            std::cerr << "IRCheck ERROR after pass " << ps->getName()
                    << std::endl;
            exit(i * 2);
        }
    }
    std::cout<<"all passes finish"<<std::endl;
}
