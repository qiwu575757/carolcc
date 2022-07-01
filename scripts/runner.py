# -- coding:UTF-8 --

import os
from re import T
import subprocess
from pretty_print import BOLD, Print_C
from file_diff import is_two_file_same
import time

has_error = False
build_file = "build"
test_results = "build/test_results/"
output = "build/output/"
log = "build/log/"
bin_file_path = "build/test_results/"  # build/test_results/00_name/bin/test_method.out
out_file_path = "build/output/"  # build/output/00_name/test_method.out
runner_log_path = "build/log/run_log/"  # build/log/run_log/00_name/test_method.out
'''
 fixed dyb much powerful than before
'''

lib = "stdlib/libsysy_x86.a"
header = "stdlib/sylib.h"
compiler_obj_path = "build/compiler"
error_log = "build/log/"
# clang -x c -c -Ofast -mcpu=cortex-a72 -mfpu=neon -mfloat-abi=hard -S -emit-llvm -include src/stdlib/include/sylib.h test/02_var_defn3.sy -o test/02_var_defn3.ir
clang_llvm_on_chip_scheme = {"scheme": "clang_llvm",
                             "frontend_instr": "clang -x c -c -O0 -mcpu=cortex-a72 -mfpu=neon -mfloat-abi=hard -S -emit-llvm -include stdlib/sylib.h {sy} -o {ir}",
                             "emit_llvm_ir": True}

clang_llvm_scheme = {"scheme": "clang_llvm",
                     "frontend_instr": "clang -Xclang -disable-O0-optnone -x c -c -O0 -S -emit-llvm -include stdlib/lib.h {sy} -o {ir} ",
                     "emit_llvm_ir": True}
npu_llvm_scheme = {"scheme": "npu_llvm",
                   "frontend_instr": "build/compiler" + " -emit-mir -o {ir} {sy}",
                   "emit_llvm_ir": True}

npu_npu_scheme = {"scheme": "npu_npu",
                  "frontend_instr": "build/compiler" + "-o {asm} {sy}",
                  "emit_llvm_ir": False}


class Runner():

    def __init__(self, scheme="method_simple", testcases=[], run_cases=3, on_chip=False):
        '''
        
        '''
        self.on_chip = on_chip
        self.run_cases = run_cases
        self.scheme = scheme
        self.testcases = testcases
        if len(self.testcases) == 0:
            self.testcases = self.get_sy_test_cases()
        self.generate_path()

    def check(self,file="",case=""):
        self.detector = open(error_log+"error.log", "r")
        line_ = 0
        for l in self.detector.readlines():
            if l.startswith("TOTAL"):
                continue
            line_+=1
        self.detector.close()
        self.detector = open(error_log+"error.log", "r")
        if line_ > 1:
            Print_C().print_line("X",color=BOLD)
            Print_C().print_error("#[CRASH] not generated: in test "+ case)
            Print_C().print_line("X",color=BOLD)


            for l in self.detector.readlines():
                Print_C().print_error(l[:-1])
            std_runner = Runner(clang_llvm_scheme['scheme'])
            std_runner.sy_to_ir(clang_llvm_scheme['frontend_instr'], case)
            std_ir_path = "build/test_results/" + str(case) + "/ir/" + clang_llvm_scheme['scheme'] + ".ir"
            Print_C().print_error("std ir path is {}".format(std_ir_path))
            exit(1)
        self.detector.close()


    def generate_path(self):
        if os.path.exists(error_log):
            self.error_log_file = open(error_log+"error.log", "w+")
            self.error_log_file.write("-"*10+str(time.strftime('%Y-%m-%d %H:%M:%S'))+"-"*10)
        else:
            self.error_log_file = open(error_log+"error.log", "w+")
            self.error_log_file.write("-"*10+str(time.strftime('%Y-%m-%d %H:%M:%S'))+"-"*10)

        if not os.path.exists(build_file):
            os.mkdir(build_file)

        if not os.path.exists(test_results):
            os.mkdir(test_results)
        if not os.path.exists(output):
            os.mkdir(output)
        if not os.path.exists(log):
            os.mkdir(log)
        for testcase in self.testcases:
            out_file_index_path = out_file_path + str(testcase) + "/"
            runner_log_index_path = runner_log_path + str(testcase) + "/"
            bin_file_index_path = bin_file_path + str(testcase) + "/"
            if not os.path.exists(out_file_index_path):
                os.makedirs(out_file_index_path)

            if not os.path.exists(runner_log_index_path):
                os.makedirs(runner_log_index_path)

            if not os.path.exists(bin_file_index_path):
                os.makedirs(bin_file_index_path)

            if not os.path.exists(bin_file_index_path + "/bin/"):
                os.mkdir(bin_file_index_path + "/bin/")

            if not os.path.exists(bin_file_index_path + "/ir/"):
                os.mkdir(bin_file_index_path + "/ir/")

            if not os.path.exists(bin_file_index_path + "/asm/"):
                os.mkdir(bin_file_index_path + "/asm/")

            if not os.path.exists(bin_file_index_path + "/obj/"):
                os.mkdir(bin_file_index_path + "/obj/")

            if not os.path.exists(bin_file_index_path + "/bin/"):
                os.mkdir(bin_file_index_path + "/bin/")

    def sy_to_ir(self, frontend_instr, testcase):
        self.check("",testcase)

        sy_path = "test/" + str(testcase)
        ir = "build/test_results/" + str(testcase) + "/ir/" + self.scheme + ".ir"
        runner_log_index_path = runner_log_path + str(testcase) + "/" + self.scheme + ".log"
        if not os.path.exists(sy_path):
            self.check(sy_path,testcase)
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")
        Print_C().print_procedure("Generating {}/{}.ir".format(testcase, self.scheme))
        subprocess.run(frontend_instr.format(sy=sy_path, ir=ir).split(), stdout=log_file, stderr=self.error_log_file, bufsize=1)
        log_file.close()


    def ir_to_asm(self, testcase):
        self.check("",testcase)
        ir = "build/test_results/" + str(testcase) + "/ir/" + self.scheme + ".ir"
        asm = "build/test_results/" + str(testcase) + "/asm/" + self.scheme + ".asm"
        runner_log_index_path = runner_log_path + str(testcase) + "/" + self.scheme + ".log"
        if not os.path.exists(ir):
            self.check(ir,testcase)
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")

        Print_C().print_procedure("Generating {}.s".format(self.scheme))
        if self.on_chip:
            subprocess.run(
                "llc -O0 -march=arm -mcpu=cortex-a72 -float-abi=hard -filetype=asm {ir} -o {asm}".format(asm=asm,
                                                                                                         ir=ir).split(),
                stdout=log_file, stderr=self.error_log_file, bufsize=1)
        else:
            subprocess.run("llc -O0  -filetype=asm {ir} -o {asm} ".format(asm=asm, ir=ir).split(), stdout=log_file,
                           stderr=self.error_log_file, bufsize=1)

        log_file.close()
        self.check("",testcase)
    def asm_to_obj(self, testcase):
        self.check("",testcase)
        asm = "build/test_results/" + str(testcase) + "/asm/" + self.scheme + ".asm"
        obj = "build/test_results/" + str(testcase) + "/obj/" + self.scheme + ".obj"
        runner_log_index_path = runner_log_path + str(testcase) + "/" + self.scheme + ".log"
        if not os.path.exists(asm):
            self.check(asm,testcase)
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")

        Print_C().print_procedure("Generating {}.o".format(self.scheme))
        if self.on_chip:
            subprocess.run("as -march=armv7-a -mfloat-abi=hard {asm} -o {obj}".format(asm=asm, obj=obj).split(),
                           stdout=log_file, stderr=self.error_log_file, bufsize=1)
        else:
            subprocess.run("as {asm} -o {obj} ".format(asm=asm, obj=obj).split(), stdout=log_file, stderr=self.error_log_file,
                           bufsize=1)

        log_file.close()
        self.check("",testcase)
    def obj_to_bin(self, testcase):
        self.check("",testcase)
        obj = "build/test_results/" + str(testcase) + "/obj/" + self.scheme + ".obj"
        bin = "build/test_results/" + str(testcase) + "/bin/" + self.scheme + ".bin"
        runner_log_index_path = runner_log_path + str(testcase) + "/" + self.scheme + ".log"
        if not os.path.exists(obj):
            self.check(obj,testcase)
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")

        Print_C().print_procedure("Generating {}".format(self.scheme))
        if self.on_chip:
            subprocess.run(
                "clang -O0 -marm -march=armv7-a -mfpu=neon -mfloat-abi=hard {obj} stdlib/libsysy_x86.a -o {bin}".format(
                    bin=bin, obj=obj).split(), stdout=log_file, stderr=self.error_log_file, bufsize=1)
        else:
            subprocess.run("clang -O0 {obj} stdlib/libsysy_x86.a -o {bin} -no-pie".format(bin=bin, obj=obj).split(),
                           stdout=log_file, stderr=self.error_log_file, bufsize=1)
        log_file.close()
        self.check("",testcase)

    def sy_to_asm(self, frontend_instr, testcase):
        self.check("",testcase)
        asm = "build/test_results/" + str(testcase) + "/asm/" + self.scheme + ".asm"
        sy_path = "test/" + str(testcase)
        if not os.path.exists(sy_path):
            self.check(sy_path,testcase)
        runner_log_index_path = runner_log_path + str(testcase) + "/" + self.scheme + ".log"
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")

        Print_C().print_procedure("Generating {}.s".format(self.scheme))
        if self.on_chip:
            subprocess.run(frontend_instr.format(header=header, ir=asm, sy=sy_path).split(), stdout=log_file,
                           stderr=self.error_log_file, bufsize=1)
        else:
            subprocess.run(frontend_instr.format(header=header, ir=asm, sy=sy_path).split(), stdout=log_file,
                           stderr=self.error_log_file, bufsize=1)

        log_file.close()
        self.check("",testcase)

    def compile_one_test(self, frontend_instr, emit_llvm_ir, testcase):
        if emit_llvm_ir:
            Print_C().print_subheader("[Compiling {} | {}]".format(self.scheme, testcase))
            self.sy_to_ir(frontend_instr=frontend_instr, testcase=testcase)
            self.ir_to_asm(testcase=testcase)
            self.asm_to_obj(testcase=testcase)
            self.obj_to_bin(testcase=testcase)
            return self.run_single_test(testcase=testcase)
        else:
            Print_C().print_subheader("[Compiling {} | {}]".format(self.scheme, testcase))
            self.sy_to_asm(frontend_instr=frontend_instr, testcase=testcase)
            self.asm_to_obj(testcase=testcase)
            self.obj_to_bin(testcase=testcase)
            return self.run_single_test(testcase=testcase)

    def compile_all_tests(self, frontend_instr, emit_llvm_ir):
        global has_error
        import time
        time_list = []
        sum_time = 0
        max_time = 0
        max_case = None
        for testcase in self.testcases:
            start = time.perf_counter()
            ######## 实际程序开始 ##########
            self.compile_one_test(frontend_instr, emit_llvm_ir, testcase)
            ######## 实际程序结束 ##########
            if has_error:
                Print_C().print_error("passed testcase number is {}".format(len(time_list)))
                self.sy_to_ir(npu_llvm_scheme["frontend_instr"], testcase)
                error_ir_path = "build/test_results/" + str(testcase) + "/ir/" + self.scheme + ".ir"
                std_runner = Runner(clang_llvm_scheme['scheme'])
                std_runner.sy_to_ir(clang_llvm_scheme['frontend_instr'], testcase)
                std_ir_path = "build/test_results/" + str(testcase) + "/ir/" + clang_llvm_scheme['scheme'] + ".ir"
                Print_C().print_error("std ir path is {}".format(std_ir_path))
                Print_C().print_error("error ir path is {}".format(error_ir_path))

                for l in self.error_log_file.readlines():
                    print(l)
                break
            end = time.perf_counter()
            Print_C().print_header("运行时间为" + "{:03f}".format(end - start) + 'seconds')
            sum_time += end - start
            time_list.append((testcase, end - start))
            if max_time < end - start:
                max_time = end - start
                max_case = testcase
        return time_list, sum_time, max_time, max_case

    def run_single_test(self, testcase):
        global has_error
        bin = "build/test_results/" + str(testcase) + "/bin/" + self.scheme + ".bin"
        stdin = "test/" + testcase[:-3] + ".in"
        std_output = "test/" + testcase[:-3] + ".out"
        runner_log_index_path = runner_log_path + str(testcase) + "/" + self.scheme + ".log"
        our_out = runner_log_path + str(testcase) + "/" + self.scheme + "_out.log"
        if os.path.exists(bin):
            _ = open(bin, "a+")
        else:
            _ = open(bin, "w+")
        _.close()
        if os.path.exists(runner_log_index_path):
            log_file = open(runner_log_index_path, "a+")
        else:
            log_file = open(runner_log_index_path, "w+")
        # if os.path.exists(our_out):
        #     our_out_file = open(our_out, "a+")
        # else:
        our_out_file = open(our_out, "w+")

        Print_C().print_procedure("Running {}_{}".format(self.scheme, testcase))
        if os.path.exists(stdin):
            stdin_file = open(stdin, "r+")
            p = subprocess.run(bin.split(), stdin=stdin_file, stdout=our_out_file, stderr=self.error_log_file, bufsize=1)
        else:
            p = subprocess.run(bin.split(), stdout=our_out_file, stderr=self.error_log_file, bufsize=1)

        subprocess.run("echo".split(), stdout=our_out_file, bufsize=1)
        subprocess.run(("echo " + str(p.returncode)).split(), stdout=our_out_file, bufsize=1)
        Print_C().print_procedure("Return {}".format(p.returncode))
        Print_C().print_pass(pass_file=testcase)

        our_out_file.close()
        log_file.close()
        #  cmp files
        if not is_two_file_same(std_output, our_out):
            Print_C().print_error("ERROR in {}".format(testcase))
            Print_C().print_error("标准输出在  {}".format(std_output))
            Print_C().print_error("错误输出在  {}".format(our_out))
            has_error = True

            for l in self.error_log_file.readlines():
                print(l)

        return p.returncode

    def get_sy_test_cases(test_path="test"):
        test_file_list = []
        for path_name, dir, files_name in os.walk("test"):
            for file in files_name:
                # 如果是目录
                if file[-3:] == ".sy":
                    # print(file)
                    test_file_list.append(file)
        return sorted(test_file_list)


if __name__ == '__main__':
    r = Runner(npu_llvm_scheme["scheme"])
    r.compile_all_tests(npu_llvm_scheme["frontend_instr"], npu_llvm_scheme["emit_llvm_ir"])
    # r.compile_all_tests(clang_llvm_scheme["frontend_instr"],clang_llvm_scheme["emit_llvm_ir"])
