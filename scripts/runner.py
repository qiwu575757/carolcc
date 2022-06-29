#-- coding:UTF-8 --

import os
import subprocess
from pretty_print import Print_C

build_file = "../build"
test_results = "../build/test_results/"
output = "../build/output/"
log = "../build/log/"
bin_file_path = "../build/test_results/" # ../build/test_results/00_name/bin/test_method.out
out_file_path = "../build/output/" # ../build/output/00_name/test_method.out
runner_log_path = "../build/log/run_log/" # ../build/log/run_log/00_name/test_method.out
'''
 fixed dyb much powerful than before
'''

lib = "../stdlib/include/libsysy.a"
header = "../stdlib/include/sylib.h"
compiler_obj_path = "../build/compiler"
# clang -x c -c -Ofast -mcpu=cortex-a72 -mfpu=neon -mfloat-abi=hard -S -emit-llvm -include ../src/stdlib/include/sylib.h ../test/02_var_defn3.sy -o ../test/02_var_defn3.ir
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
class Runner():

    def __init__(self, scheme="method_simple", testcases=[],run_cases=3,on_chip=False):
        '''
        
        '''
        self.on_chip = on_chip
        self.run_cases = run_cases
        self.scheme = scheme
        self.testcases = testcases
        if len(self.testcases) == 0:
            self.testcases = self.get_sy_test_cases()
        self.generate_path()

    def generate_path(self):
        if not os.path.exists(build_file):
            os.mkdir(build_file)
        
        if not os.path.exists(test_results):
            os.mkdir(test_results)
        if not os.path.exists(output):
            os.mkdir(output)
        if not os.path.exists(log):
            os.mkdir(log)
        for testcase in self.testcases:
            out_file_index_path = out_file_path+str(testcase)+"/"
            runner_log_index_path = runner_log_path+str(testcase)+"/"
            bin_file_index_path = bin_file_path+str(testcase)+"/"
            if not os.path.exists(out_file_index_path):
                os.makedirs(out_file_index_path)

            if not os.path.exists(runner_log_index_path):
                os.makedirs(runner_log_index_path)

            if not os.path.exists(bin_file_index_path):
                os.makedirs(bin_file_index_path)

            if not os.path.exists(bin_file_index_path+"/bin/"):
                os.mkdir(bin_file_index_path+"/bin/")

            if not os.path.exists(bin_file_index_path+"/ir/"):
                os.mkdir(bin_file_index_path+"/ir/")
            
            if not os.path.exists(bin_file_index_path+"/asm/"):
                os.mkdir(bin_file_index_path+"/asm/")
            
            if not os.path.exists(bin_file_index_path+"/obj/"):
                os.mkdir(bin_file_index_path+"/obj/")

            if not os.path.exists(bin_file_index_path+"/bin/"):
                os.mkdir(bin_file_index_path+"/bin/")
    def sy_to_ir(self, frontend_instr,testcase):

        sy_path = "../test/"+str(testcase)
        ir = "../build/test_results/"+str(testcase)+"/ir/"+self.scheme+".ir"
        runner_log_index_path = runner_log_path+str(testcase)+"/"+self.scheme+".log"
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")
        Print_C().print_procedure("Generating {}/{}.ir".format(testcase,self.scheme))
        subprocess.run(frontend_instr.format(sy=sy_path, ir=ir).split(), stdout=log_file, stderr=log_file, bufsize=1)

        log_file.close()


    def ir_to_asm(self, testcase):
        ir = "../build/test_results/"+str(testcase)+"/ir/"+self.scheme+".ir"
        asm = "../build/test_results/"+str(testcase)+"/asm/"+self.scheme+".asm"
        runner_log_index_path = runner_log_path+str(testcase)+"/"+self.scheme+".log"
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")

        Print_C().print_procedure("Generating {}.s".format(self.scheme))
        if self.on_chip:
            subprocess.run("llc -O3 -march=arm -mcpu=cortex-a72 -float-abi=hard -filetype=asm {ir} -o {asm}".format(asm=asm, ir=ir).split(), stdout=log_file, stderr=log_file, bufsize=1)
        else:
            subprocess.run("llc -O3  -filetype=asm {ir} -o {asm}".format(asm=asm, ir=ir).split(), stdout=log_file, stderr=log_file, bufsize=1)

        log_file.close()


    def asm_to_obj(self, testcase):
        asm = "../build/test_results/"+str(testcase)+"/asm/"+self.scheme+".asm"
        obj = "../build/test_results/"+str(testcase)+"/obj/"+self.scheme+".obj"
        runner_log_index_path = runner_log_path+str(testcase)+"/"+self.scheme+".log"
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")

        Print_C().print_procedure("Generating {}.o".format(self.scheme))
        if self.on_chip:
            subprocess.run("as -march=armv7-a -mfloat-abi=hard {asm} -o {obj}".format(asm=asm, obj=obj).split(), stdout=log_file, stderr=log_file, bufsize=1)
        else:
            subprocess.run("as {asm} -o {obj}".format(asm=asm, obj=obj).split(), stdout=log_file, stderr=log_file, bufsize=1)

        log_file.close()

    def obj_to_bin(self, testcase):
        obj = "../build/test_results/"+str(testcase)+"/obj/"+self.scheme+".obj"
        bin = "../build/test_results/"+str(testcase)+"/bin/"+self.scheme+".bin"
        runner_log_index_path = runner_log_path+str(testcase)+"/"+self.scheme+".log"
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")

        Print_C().print_procedure("Generating {}".format(self.scheme))
        if self.on_chip:
            subprocess.run("clang -Ofast -marm -march=armv7-a -mfpu=neon -mfloat-abi=hard {obj} ../stdlib/libsysy.a -o {bin}".format(bin=bin, obj=obj).split(), stdout=log_file, stderr=log_file, bufsize=1)
        else:
            subprocess.run("clang -Ofast {obj} ../stdlib/libsysy.a -o {bin}".format(bin=bin, obj=obj).split(), stdout=log_file, stderr=log_file, bufsize=1)
        log_file.close()


    def sy_to_asm(self, frontend_instr, testcase):
        asm = "../build/test_results/"+str(testcase)+"/asm/"+self.scheme+".asm"
        sy_path = "../test/"+str(testcase)

        runner_log_index_path = runner_log_path+str(testcase)+"/"+self.scheme+".log"
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")

        Print_C().print_procedure("Generating {}.s".format(self.scheme))
        if self.on_chip:
            subprocess.run(frontend_instr.format(header=header, ir=asm, sy=sy_path).split(), stdout=log_file, stderr=log_file, bufsize=1)
        else:
            subprocess.run(frontend_instr.format(header=header, ir=asm, sy=sy_path).split(), stdout=log_file, stderr=log_file, bufsize=1)

        log_file.close()

    def compile_one_test(self, frontend_instr, emit_llvm_ir,testcase):
        if emit_llvm_ir:
            Print_C().print_subheader("[Compiling {} | {}]".format(self.scheme,testcase))
            self.sy_to_ir(frontend_instr=frontend_instr,testcase=testcase)
            self.ir_to_asm(testcase=testcase)
            self.asm_to_obj(testcase=testcase)
            self.obj_to_bin(testcase=testcase)
            return self.run_single_test(testcase=testcase)
        else:
            Print_C().print_subheader("[Compiling {} | {}]".format(self.scheme,testcase))
            self.sy_to_asm(frontend_instr=frontend_instr, testcase=testcase)
            self.asm_to_obj(testcase=testcase)
            self.obj_to_bin(testcase=testcase)
            return self.run_single_test(testcase=testcase)

    def compile_all_tests(self, frontend_instr, emit_llvm_ir):
        import time
        time_list = []
        sum_time = 0
        max_time = 0
        max_case = None
        for testcase in self.testcases:
            start = time.perf_counter()
            ######## 实际程序开始 ##########
            self.compile_one_test(frontend_instr, emit_llvm_ir,testcase)
            ######## 实际程序结束 ##########
            end = time.perf_counter()
            Print_C().print_header("运行时间为"+"{:03f}".format(end-start)+'seconds')
            sum_time+=end-start
            time_list.append((testcase,end-start))
            if max_time < end-start:
                max_time = end-start
                max_case = testcase
        return time_list,sum_time,max_time,max_case

    def run_single_test(self, testcase):
        bin = "../build/test_results/"+str(testcase)+"/bin/"+self.scheme+".bin"
        stdin = "test/"+testcase[:-3]+".in"
        runner_log_index_path = runner_log_path+str(testcase)+"/"+self.scheme+".log"
        our_out = runner_log_path+str(testcase)+"/"+self.scheme+"_our.log"
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")
        if os.path.exists(our_out):
            our_out_file = open(our_out, "a+")
        else:
            our_out_file = open(our_out, "w+")

        Print_C().print_procedure("Running {}_{}".format(self.scheme,testcase))
        if os.path.exists(stdin):
            stdin_file = open(stdin, "w+")
            p = subprocess.run(bin.split(), stdin=stdin_file, stdout=our_out_file, stderr=log_file, bufsize=1)
        else:
            p = subprocess.run(bin.split(), stdout=our_out_file, stderr=log_file, bufsize=1)
        subprocess.run("echo".split(), stdout=our_out_file, bufsize=1)
        subprocess.run(("echo "+str(p.returncode)).split(), stdout=our_out_file, bufsize=1)
        Print_C().print_procedure("Return {}".format(p.returncode))
        Print_C().print_pass(pass_file=testcase)

        our_out_file.close()
        log_file.close()
        
        return p.returncode


    def get_sy_test_cases(test_path="../test"):
        test_file_list = []
        for path_name, dir, files_name in os.walk("../test"):
            for file in files_name:
                #如果是目录
                if file[-3:] == ".sy":
                    print(file)
                    test_file_list.append(file)
        return sorted(test_file_list)

if __name__ == '__main__':
    r = Runner()
    r.compile_all_tests(clang_llvm_scheme["frontend_instr"],clang_llvm_scheme["emit_llvm_ir"])