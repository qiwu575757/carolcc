import os
'''
生成一个显示和记录
'''
class Print_C:

    GREEN = '\033[32m'
    RED = '\033[31m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    BOLD = '\033[1m'
    ENDC = '\033[0m'

    log = "test_result.log"
    if os.path.exists(log):
        log_file = open(log, "a+")
    else:
        log_file = open(log, "w+")
    def print_error(msg):
        content = f"{Print_C.RED}{msg}{Print_C.ENDC}"
        print(content)
        Print_C.log_file.write("test")
        Print_C.log_file.close()

    def print_header(msg):
        content = f"{Print_C.GREEN}{Print_C.BOLD}{msg}{Print_C.ENDC}"
        print(content)
        Print_C.log_file.write(content + "\n")
        Print_C.log_file.close()

    def print_pass(msg):
        content = f"{Print_C.GREEN}{msg}{Print_C.ENDC}"
        print(content)
        Print_C.log_file.write(content + "\n")
        Print_C.log_file.close()

    def print_subheader(msg):
        content = f"{Print_C.YELLOW}{msg}{Print_C.ENDC}"
        print(content)
        Print_C.log_file.write(content + "\n")
        Print_C.log_file.close()



    def print_procedure(msg):
        content = f"{Print_C.BLUE}{msg}{Print_C.ENDC}"
        print(content)
        Print_C.log_file.write(content + "\n")
        Print_C.log_file.close()


if __name__ == '__main__':
    Print_C.print_error("test")
