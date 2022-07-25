import os
from pretty_print import Print_C


# compare two files
def is_two_file_same(std_out_file_path, our_out_path):
    std_out_file = open(std_out_file_path, 'r')
    our_out_file = open(our_out_path, 'r')
    std_out_lines = std_out_file.readlines()
    our_out_lines = our_out_file.readlines()
    diff_list = []
    if len(std_out_lines) - len(our_out_lines) < -1:
        Print_C().print_error("file length not equal")
        std_out_file.close()
        our_out_file.close()
        return False
    if len(our_out_lines) == len(std_out_lines) and len(std_out_lines)== 0:
        return True
    j = 0
    if our_out_lines[0] == "\n":
        j +=1
    for i in range(len(std_out_lines)):
        if std_out_lines[i] != our_out_lines[j]:
            if i == len(std_out_lines)-1 and j < len(our_out_lines)-1:
                j +=1
                if std_out_lines[i] ==our_out_lines[j]:
                    return True
            Print_C().print_error("expected line {}: {}".format(i, std_out_lines[i]))
            Print_C().print_error("but get line {}: {}".format(i, our_out_lines[j]))
            std_out_file.close()
            our_out_file.close()
            return False
        j+=1

    std_out_file.close()
    our_out_file.close()
    return True
