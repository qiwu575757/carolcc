#-- coding:UTF-8 --
from fileinput import filename
import os
from runner import Runner
from analyzer import Analyzer
from pretty_print import Print_C
'''
fixed dyb much powerful than before
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

def get_sy_test_cases(test_path="../test"):
    file_list = os.listdir(test_path)
    test_file_list = []
    #遍历当前文件名列表
    for path_name, dir, files_name in os.walk(test_path):
        for file in files_name:
            #如果是目录
            print(file)
            if os.path.splitext(file)[-1] == ".sy" \
                or os.path.splitext(file)[-1] == ".c"\
                or os.path.splitext(file)[-1] == ".cpp":
                test_file_list.append(file)
    return sorted(test_file_list)



class Tester():
    def __init__(self, a_scheme,test_path="../test"):
        self.scheme = a_scheme["scheme"]
        self.frontend_instr = a_scheme["frontend_instr"]
        self.emit_llvm_ir = a_scheme["emit_llvm_ir"]
        self.test_cases = get_sy_test_cases(test_path)
        self.runner = Runner(scheme=self.scheme, testcases=self.test_cases)
        self.analyzer = Analyzer(scheme=self.scheme, emit_llvm_ir=self.emit_llvm_ir, testcases=self.test_cases)
        self.max_case = None
        self.max_time = None
        self.sum_time = None
        self.time_list = []
    def generate_ir(self):
        for testcase in self.test_cases: self.compiler.sy_to_ir(frontend_instr=self.frontend_instr, testcase=testcase)

    def run(self):
        self.time_list,self.sum_time,self.max_time,self.max_case = self.runner.compile_all_tests(frontend_instr=self.frontend_instr, emit_llvm_ir=self.emit_llvm_ir)

    def analyze(self):
        Print_C().print_line('=')
        self.analyzer.analyze()
        for item in self.time_list:
            print("case:\t"+item[0]+"\t time:"+"{:.4f}".format((item[1]))+"s")
        Print_C().print_line('+')
        print(" total time:\t"+"{:.4f}".format((self.sum_time))+"s\n worst case:"+str(self.max_case)+"\t time:"+"{:.4f}".format((self.max_time))+"s")
        Print_C().print_line('=')

    def test(self):
        Print_C().print_header("[TESTING {self.scheme}]")
        self.run()
        self.analyze()
        Print_C().print_line()
        

    def test_ir(self):
        Print_C().print_header("[TESTING {self.scheme}] (IR only)")
        for testcase in self.test_cases:
            self.runner.sy_to_ir(frontend_instr=self.frontend_instr, testcase=testcase)
            Print_C().print_line()

if __name__ == '__main__':
    test = Tester(clang_llvm_scheme).test()