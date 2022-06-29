import re
from runner import Runner
from pretty_print import Print_C
'''
check by dyb not work now
'''
class TrivalPresenter:
    kases = Runner.run_kases
    splitter = re.compile('[-@ :\t\r\n]+')
    scheme_time_print_template = "{0:<10} :  {1:<10.0f}us"

    def __init__(self, schemes, testcases):
        self.schemes = schemes
        self.testcases = testcases
        self.wrong_cases = []

    def __diff(self, file1, file2):
        file1_lines = list(filter(lambda line: not(line == ""), map(lambda line: line.rstrip('\n'), open(file1).readlines())))
        file2_lines = list(filter(lambda line: not(line == ""), map(lambda line: line.rstrip('\n'), open(file2).readlines())))
        return file1_lines == file2_lines

    def present_single_testcase(self, testcase):
        Print_C.print_subheader("[Checking {testcase}]")
        for scheme in self.schemes:
            stdout = "testcases/{testcase}.out"
            myout = "build/output/{testcase}/{scheme}.out"
            if not self.__diff(stdout, myout):
                self.wrong_cases.append(testcase)
                Print_C.print_error("[Checking {testcase}] WA")
                Print_C.print_procedure("====== std ======")
                print(list(filter(lambda line: not(line == ""), map(lambda line: line.rstrip('\n'), open(stdout).readlines()))))
                Print_C.print_procedure("====== yours ======")
                print(list(filter(lambda line: not(line == ""), map(lambda line: line.rstrip('\n'), open(myout).readlines()))))
                print()
            else:
                Print_C.print_pass("[Checking {testcase}] AC")

    def present_all_testcases(self):
        Print_C.print_header("[Checking & Racing]")

        for testcase in self.testcases:
            self.present_single_testcase(testcase=testcase)
            print()

        Print_C.print_header("[[Wrong Cases ({len(self.wrong_cases)}):]]")
        print(self.wrong_cases)

