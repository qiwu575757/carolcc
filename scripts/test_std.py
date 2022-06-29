import subprocess
from tester import Tester
from presenter import Presenter
from tester import get_sy_testcases
from pretty_print import Print_C

thu_compiler = "build/bin/thu_compiler "
ustc_compiler = "build/bin/ustc_compiler "
ustc_compiler_no_vec = "build/bin/ustc_compiler_no_vec "

clang_llvm_scheme = {"scheme": "clang_llvm",
                     "frontend_instr": "clang -x c -c -Ofast -mcpu=cortex-a72 -mfpu=neon -mfloat-abi=hard -S -emit-llvm -include {header} {sy} -o {ir}",
                     "emit_llvm_ir": True}

thu_llvm_scheme = {"scheme": "thu_llvm",
                   "frontend_instr": thu_compiler + "-l {ir} {sy}",
                   "emit_llvm_ir": True}

thu_thu_scheme = {"scheme": "thu_thu",
                  "frontend_instr": thu_compiler + "-o {asm} {sy}",
                  "emit_llvm_ir": False}


all_schemes = [clang_llvm_scheme, thu_llvm_scheme, thu_thu_scheme] # gcc_gcc_scheme, ustc_ustc_scheme, ustc_ustc_no_vec_scheme]
testers = []

Print_C.print_header("[Removing old data...]\n\n")
subprocess.run("rm -rf build/test_results/".split())
subprocess.run("rm -rf build/output/".split())
subprocess.run("rm -rf build/log/compile_log".split())
subprocess.run("rm -rf build/log/run_log".split())
subprocess.run("rm -rf build/log/test_result.log".split())

for scheme in all_schemes:
    tester = Tester(scheme, is_trivial=True)
    testers.append(tester)
    tester.test()

# Tester(ustc_llvm_scheme).test_ir()

presenter = Presenter(schemes=[scheme["scheme"] for scheme in all_schemes], testcases=get_sy_testcases())
presenter.present_all_testcases()
