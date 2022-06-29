#-- coding:UTF-8 --
import os
'''
生成一个显示和记录 fixed dyb much powerful than before
'''
GREEN = '\033[32m'
RED = '\033[31m'
YELLOW = '\033[33m'
BLUE = '\033[34m'
BOLD = '\033[1m'
ENDC = '\033[0m'

log_path = "../build/log"
build_file = "../build"
class Print_C():

    def __init__(self):
        if not os.path.exists(build_file):
            os.mkdir(build_file)
        if not os.path.exists(log_path):
            os.mkdir(log_path)
        if os.path.exists(log_path):
            self.log_file = open(log_path+"test_result.log", "a+")
        else:
            self.log_file = open(log_path+"test_result.log", "w+")

    def open(self):
        self.log_file = open(log_path+"test_result.log", "a+")
    
    def print_error(self,msg):
        self.open()
        content = "{}{}{}".format(RED,msg,ENDC)
        print(content)
        self.log_file.write(content + "\n")
        self.log_file.close()

    def print_header(self,msg):
        self.open()
        content = "{}{}{}".format(GREEN,msg,ENDC)
        print(content)
        self.log_file.write(content + "\n")
        self.log_file.close()

    def ppass(self,msg):
        self.open()
        content = "{}{}{}".format(BOLD,"\t"+"pass\t\t"+msg,ENDC)
        print(content)
        self.log_file.write(content + "\n")
        self.log_file.close()

    def print_subheader(self,msg):
        self.open()
        content = "{}{}{}".format(YELLOW,msg,ENDC)
        print(content)
        self.log_file.write(content + "\n")
        self.log_file.close()

    def print_procedure(self,msg):
        self.open()
        content = "{}{}{}".format(BLUE,msg,ENDC)
        print(content)
        self.log_file.write(content + "\n")
        self.log_file.close()

    def print_line(self,pattern="-",color=BOLD):
        self.open()
        content = "{}{}{}".format(color,pattern*50,ENDC)
        print(content)
        self.log_file.write(content + "\n")
        self.log_file.close()

    def print_pass(self,pass_file="unknown"):
        self.open()
        self.print_line("▣",color=YELLOW)
        self.print_line("◎",color=GREEN)
        self.ppass(msg=pass_file)
        self.print_line("◎",color=GREEN)
        self.print_line("▣",color=YELLOW)

if __name__ == '__main__':
    Print_C().print_pass("test01__")
