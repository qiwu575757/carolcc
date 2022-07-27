# -- coding:UTF-8 --

import os
from re import T
import subprocess
from pretty_print import BOLD, Print_C
from file_diff import is_two_file_same
import time
import argparse
import shutil

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
arm_lib = "stdlib/libsysy_float.a"
lib = "stdlib/libsysy_x86.a"
header = "stdlib/sylib.h"
compiler_obj_path = "build/compiler"
error_log = "build/log/"
# clang -x c -c -Ofast -mcpu=cortex-a72 -mfpu=neon -mfloat-abi=hard -S -emit-llvm -include src/stdlib/include/sylib.h test/02_var_defn3.sy -o test/02_var_defn3.ir
clang_llvm_on_chip_scheme = {"scheme": "clang_llvm",
                             "frontend_instr": "clang{v} -x c -c -O0 -mcpu=cortex-a72 -mfpu=neon -mfloat-abi=hard -S -emit-llvm -include stdlib/sylib.h {sy} -o {ir}",
                             "emit_llvm_ir": True}

clang_llvm_scheme = {"scheme": "clang_llvm",
                     "frontend_instr": "clang{v} -Xclang -disable-O0-optnone -x c -c -O0 -S -emit-llvm -include stdlib/lib.h {sy} -o {ir} ",
                     "emit_llvm_ir": True}
npu_llvm_scheme = {"scheme": "npu_llvm",
                  "frontend_instr": "build/compiler" + " -emit-mir -o {ir} {sy}",
                   "emit_llvm_ir": True}

npu_npu_scheme = {"scheme": "npu_npu",
                  "frontend_instr": "build/compiler" + " -S  -o {asm} {sy}",
                  "emit_llvm_ir": True}


class Runner():

    def __init__(self, scheme="method_simple", testcases=[], run_cases=3, on_chip=False):
        '''

        '''
        parser = argparse.ArgumentParser(description='compiler runner parser')
        parser.add_argument('-ir', action='store_true', default=False, help='-ir 测试ir')
        parser.add_argument('-asm', action='store_true', default=False, help='-asm 测试汇编')
        parser.add_argument('-v', type=int, default=-1, help='制定工具链版本是clang-v')
        parser.add_argument('-single', type=str, default="all", help='-single test.sy 测试单个文件 ')
        parser.add_argument('-debug',  action='store_true', default=False, help=' show all in windows')
        parser.add_argument('-all_test', type=str, default="all", help='全部测试(default)')
        parser.add_argument('-remake', action='store_true', default=False, help='重新编译')
        parser.add_argument('-clean', action='store_true', default=False, help='-clean 清除生成物 : build/log build/test_result build/output test 保留 test/**.sy test/**.out')
        self.args = parser.parse_args()

        if self.args.v > 0:
            self.v = "-{}".format(self.args.v)
        else:
            self.v = ""
        self.on_chip = on_chip
        self.run_cases = run_cases
        if self.args.asm:
            self.scheme = npu_npu_scheme["scheme"]
        else:
            self.scheme = scheme
        self.testcases = testcases
        if len(self.testcases) == 0:
            self.testcases = self.get_sy_test_cases()
        self.single_test_case = []
        for  test in self.testcases:
            if self.args.single in test:
                self.single_test_case.append(test)
        if self.args.single != 'all':
            self.testcases = self.single_test_case

        if self.args.clean: self.clean()
        if self.args.remake:
            self.clean()
            self.remake()

        self.generate_path()

    def remake(self):
        os.chdir('build')
        os.system('cmake ..')
        os.system('make clean')
        os.system('make -j20')
        os.chdir('..')

    def open_ir(self,test):
        os.system('code '+test_results+"/"+test+"/ir/" + self.scheme + ".ir")
    def clean(self):
        if os.path.exists(build_file+"/compiler"):
            os.remove(build_file+"/compiler")
        if os.path.exists(test_results):
            if os.path.isfile(test_results):
                os.remove(test_results)
            else:
                shutil.rmtree(test_results)

        if os.path.exists(output):
            if os.path.isfile(output):
                os.remove(output)
            else:
                shutil.rmtree(output)
        if os.path.exists(log):
            if os.path.isfile(log):
                os.remove(log)
            else:
                shutil.rmtree(log)
        for path_name, dir, files_name in os.walk("test/"):
            for file in files_name:
                # 如果是目录
                if file[-3:] != ".sy" and file[-4:] != '.out' and file[-3:] !=".in":
                    os.remove(path_name+file)

    def check(self,file="",case=""):
        self.detector = open(error_log+"error.log", "r")
        line_ = 0
        for l in self.detector.readlines():
            if l.startswith("TOTAL") or l.startswith("Timer") or l.startswith("qemu: Unsupported syscall: 397"):
                continue
            line_+=1
        self.detector.close()
        self.detector = open(error_log+"error.log", "r")
        if line_ > 1:
            if self.args.asm:
                Print_C().print_line("X",color=BOLD)
                Print_C().print_error("#[CRASH] asm test error "+ case)
                Print_C().print_line("X",color=BOLD)

                for l in self.detector.readlines():
                    Print_C().print_error(l[:-1])
                npu_ir_path = "build/test_results/" + str(case) + "/ir/" + clang_llvm_scheme['scheme'] + ".ir"
                Print_C().print_error("npu ir path is {}".format(npu_ir_path))
            else:
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

            if not os.path.exists(bin_file_index_path + "/arm_asm/"):
                os.mkdir(bin_file_index_path + "/arm_asm/")


        if os.path.exists(error_log):
            self.error_log_file = open(error_log+"error.log", "w+")
            self.error_log_file.write("-"*10+str(time.strftime('%Y-%m-%d %H:%M:%S'))+"-"*10)
        else:
            self.error_log_file = open(error_log+"error.log", "w+")
            self.error_log_file.write("-"*10+str(time.strftime('%Y-%m-%d %H:%M:%S'))+"-"*10)

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
        if self.args.debug:
            subprocess.run(frontend_instr.format(v=self.v,sy=sy_path, ir=ir).split(), bufsize=1)
        else:
            subprocess.run(frontend_instr.format(v=self.v,sy=sy_path, ir=ir).split(), stdout=log_file, stderr=self.error_log_file, bufsize=1)
        log_file.close()

        if self.args.debug:
            self.open_ir(testcase)


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
        if self.args.debug:
            if self.on_chip:
                subprocess.run(
                    "llc{v} -O0 -march=arm -mcpu=cortex-a72 -float-abi=hard -filetype=asm {ir} -o {asm}".format(v=self.v,asm=asm,
                                                                                                            ir=ir).split(), bufsize=1)
            else:
                subprocess.run("llc{v} -O0  -filetype=asm {ir} -o {asm} ".format(v=self.v,asm=asm, ir=ir).split(), stdout=log_file, bufsize=1)
        else:
            if self.on_chip:
                subprocess.run(
                    "llc{v} -O0 -march=arm -mcpu=cortex-a72 -float-abi=hard -filetype=asm {ir} -o {asm}".format(v=self.v,asm=asm,
                                                                                                            ir=ir).split(),
                    stdout=log_file, stderr=self.error_log_file, bufsize=1)
            else:
                subprocess.run("llc{v} -O0  -filetype=asm {ir} -o {asm} ".format(v=self.v,asm=asm, ir=ir).split(), stdout=log_file,
                            stderr=self.error_log_file, bufsize=1)

        log_file.close()
        self.check("",testcase)
    def asm_to_obj(self, testcase):
        self.check("",testcase)
        if self.args.asm:
            asm = "build/test_results/" + str(testcase) + "/arm_asm/" + self.scheme + ".s"
            obj = "build/test_results/" + str(testcase) + "/obj/" + self.scheme + ".o"
        else:
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
        if self.args.asm:
            subprocess.run("arm-linux-gnueabihf-gcc -c {asm} -o {obj} -static".format(asm=asm, obj=obj).split(), bufsize=1)
        else:
            if self.args.debug:
                if self.on_chip:
                    subprocess.run("as -march=armv7-a -mfloat-abi=hard {asm} -o {obj}".format(asm=asm, obj=obj).split(), bufsize=1)
                else:
                    subprocess.run("as {asm} -o {obj} ".format(asm=asm, obj=obj).split(), bufsize=1)
            else:
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
        if self.args.asm:
            obj = "build/test_results/" + str(testcase) + "/obj/" + self.scheme + ".o"
            bin = "build/test_results/" + str(testcase) + "/bin/" + self.scheme + ".out"
        else:
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
        if self.args.asm:
            subprocess.run(
                        "arm-linux-gnueabihf-gcc {obj} stdlib/libsysy_float.a -o {bin} -static".format(
                            bin=bin, obj=obj).split(), bufsize=1)
        else:
            if self.args.debug:
                if self.on_chip:
                    subprocess.run(
                        "clang{v} -O0 -marm -march=armv7-a -mfpu=neon -mfloat-abi=hard {obj} stdlib/libsysy_x86.a -o {bin}".format(
                            v=self.v,  bin=bin, obj=obj).split(), bufsize=1)
                else:
                    subprocess.run("clang{v} -O0 {obj} stdlib/libsysy_x86.a -o {bin} -no-pie".format(v=self.v,bin=bin, obj=obj).split(),
                            bufsize=1)
            else:
                if self.on_chip:
                    subprocess.run(
                        "clang{v} -O0 -marm -march=armv7-a -mfpu=neon -mfloat-abi=hard {obj} stdlib/libsysy_x86.a -o {bin}".format(
                            v=self.v,  bin=bin, obj=obj).split(), stdout=log_file, stderr=self.error_log_file, bufsize=1)
                else:
                    subprocess.run("clang{v} -O0 {obj} stdlib/libsysy_x86.a -o {bin} -no-pie".format(v=self.v,bin=bin, obj=obj).split(),
                                    stdout=log_file, stderr=self.error_log_file, bufsize=1)

        log_file.close()
        self.check("",testcase)

    def sy_to_asm(self, frontend_instr, testcase):
        self.check("",testcase)
        if self.args.asm:
            asm = "build/test_results/" + str(testcase) + "/arm_asm/" + self.scheme + ".s"
        else:
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
        if self.args.asm:
            subprocess.run(frontend_instr.format(asm=asm, sy=sy_path).split(), bufsize=1)
        else:
            if self.args.debug:
                if self.on_chip:
                    subprocess.run(frontend_instr.format(v=self.v,header=header, ir=asm, sy=sy_path).split(), bufsize=1)
                else:
                    subprocess.run(frontend_instr.format(v=self.v,header=header, ir=asm, sy=sy_path).split(), bufsize=1)
            else:
                if self.on_chip:
                    subprocess.run(frontend_instr.format(v=self.v,header=header, ir=asm, sy=sy_path).split(), stdout=log_file,
                                    stderr=self.error_log_file, bufsize=1)
                else:
                    subprocess.run(frontend_instr.format(v=self.v,header=header, ir=asm, sy=sy_path).split(), stdout=log_file,
                                    stderr=self.error_log_file, bufsize=1)
        log_file.close()
        self.check("",testcase)

    def compile_one_test(self, frontend_instr, emit_llvm_ir, testcase):
        if emit_llvm_ir:
            if self.args.asm:
                Print_C().print_subheader("[Compiling {} | {}]".format(self.scheme, testcase))
                self.sy_to_asm(frontend_instr=npu_npu_scheme["frontend_instr"], testcase=testcase)
                self.asm_to_obj(testcase=testcase)
                self.obj_to_bin(testcase=testcase)
                self.sy_to_ir(frontend_instr=npu_llvm_scheme["frontend_instr"], testcase=testcase)
                return self.run_single_test(testcase=testcase)
            else:
                Print_C().print_subheader("[Compiling {} | {}]".format(self.scheme, testcase))
                self.sy_to_ir(frontend_instr=frontend_instr, testcase=testcase)
                if self.args.ir: return 0
                self.ir_to_asm(testcase=testcase)
                self.asm_to_obj(testcase=testcase)
                self.obj_to_bin(testcase=testcase)
                return self.run_single_test(testcase=testcase)
        else:
            if self.args.asm:
                Print_C().print_subheader("[Compiling {} | {}]".format(self.scheme, testcase))
                self.sy_to_asm(frontend_instr=npu_npu_scheme["frontend_instr"], testcase=testcase)
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
                if self.args.asm:
                    Print_C().print_error("passed testcase number is {}".format(len(time_list)))
                    self.sy_to_ir(npu_llvm_scheme["frontend_instr"], testcase)
                    self.sy_to_asm(npu_npu_scheme["frontend_instr"], testcase)
                    error_asm_path = "build/test_results/" + str(testcase) + "/arm_asm/" + self.scheme + ".s"
                    Print_C().print_error("error asm path is {}".format(error_asm_path))
                    std_ir_path = "build/test_results/" + str(testcase) + "/ir/" + npu_llvm_scheme['scheme'] + ".ir"
                    Print_C().print_error("npu ir path is {}".format(std_ir_path))
                else:
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
        if self.args.asm:
            bin = "build/test_results/" + str(testcase) + "/bin/" + self.scheme + ".out"
        else:
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
        if self.args.debug:
            if os.path.exists(stdin):
                stdin_file = open(stdin, "r+")
                p = subprocess.run(bin.split(), stdin=stdin_file, bufsize=1)
            else:
                p = subprocess.run(bin.split(), bufsize=1)

            subprocess.run("echo".split(),bufsize=1)
            subprocess.run(("echo " + str(p.returncode)).split(), bufsize=1)

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
    # r.compile_all_tests(npu_npu_scheme["frontend_instr"],npu_npu_scheme["emit_llvm_ir"])
