import subprocess
from tester import Tester
from scripts.trival_presenter import TrivialPresenter
from tester import get_sy_testcases
from pretty_print import Print_C
'''
check by dyb not work now
'''
clang_llvm_on_chip_scheme = {"scheme": "clang_llvm",
                     "frontend_instr": "clang -x c -c -Ofast -mcpu=cortex-a72 -mfpu=neon -mfloat-abi=hard -S -emit-llvm -include ../stdlib/sylib.h {sy} -o {ir}",
                     "emit_llvm_ir": True}

clang_llvm_scheme = {"scheme": "clang_llvm",
                     "frontend_instr": "clang -x c -c -Ofast -S -emit-llvm -include ../stdlib/sylib.h {sy} -o {ir}",
                     "emit_llvm_ir": True}
npu_llvm_scheme = {"scheme": "npu_llvm",
                   "frontend_instr": "../build/compiler" + " -l {ir} {sy}",
                   "emit_llvm_ir": True}

npu_npu_scheme = {"scheme": "npu_npu",
                  "frontend_instr": "../build/compiler" + "-o {asm} {sy}",
                  "emit_llvm_ir": False}

Print_C.print_header("[Removing old data...]\n\n")
subprocess.run("rm -rf ../build/test_results/".split())
subprocess.run("rm -rf ../build/output/".split())
subprocess.run("rm -rf ../build/log/".split())

all_schemes = [clang_llvm_scheme]#, ayame_llvm_scheme]
testers = []

for scheme in all_schemes:
    tester = Tester(scheme, is_trivial=True)
    testers.append(tester)
    tester.test()

presenter = TrivialPresenter(schemes=[scheme["scheme"] for scheme in all_schemes], testcases=get_sy_testcases())
presenter.present_all_testcases()
