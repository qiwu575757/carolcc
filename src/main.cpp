#include <getopt.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include "asm/asm_builder.h"
#include "passes/hir_to_mir.h"
#include "passes/lowerir.h"
#include "passes/pass_manager.h"
#include "utils.h"
#include "visitor/syntax_detail_shower.h"
#include "visitor/syntax_tree_shower.h"
#include "visitor/sysy_builder.h"
#include "visitor/tree_visitor_base.h"
extern int yyparse();
extern int yyrestart(FILE *);
extern FILE *yyin;
debug_log parser_logger("parser_logger");
int yyline;
tree_comp_unit *root;
bool debug = true;
// std::shared_ptr<tree_comp_unit> root(new tree_comp_unit());

FILE *output;
void print_help(char *exec_name) {
    printf(
        "Usage : %s [-o <target-file-name>] [-emit-hir] [-emit-mir] [-h | "
        "--help]\n",
        exec_name);
    printf("-h  --help print this message\n");
    printf("-emit-hir  emit hir\n");
    printf("-emit-mir  emit llvm ir\n");
};
int main(int argc, char **argv) {
    //     处理input_file
    //    if (argc < 2) {
    //        yyparse();
    //        return 0;
    //    }
    bool is_emit_hir = false;
    bool is_emit_mir = false;
    bool is_show_hir_pad_graph = false;
    bool is_debug = false;
    std::string input_file, output_file;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = std::string(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "-emit-hir") == 0) {
            is_emit_hir = true;
        } else if (strcmp(argv[i], "-emit-mir") == 0) {
            is_emit_mir = true;
        } else if (strcmp(argv[i], "-debug") == 0) {
            is_debug = true;
        } else if (strcmp(argv[i], "-pad") == 0) {
            is_show_hir_pad_graph = true;
        } else if (strcmp(argv[i], "-h") == 0 ||
                   strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else {
            input_file = std::string(argv[i]);
        }
    }

    yyin = fopen(input_file.c_str(), "r");
    if (output_file.empty()) {
        output_file = input_file;
        output_file.replace(output_file.end() - 2, output_file.end(), "s");
    }
    output = fopen(output_file.c_str(), "w");
    if (!yyin) perror(input_file.c_str());
    parser_logger.set_screen(false);
    yyparse();

    auto *builder = new SYSYBuilder(input_file);
    builder->build(root);
    // auto *shower = new syntax_detail_shower();
    // shower->visit(*root);
    if (is_emit_hir) {
        WARNNING("emitting  hir");
        auto hir_output_file = input_file;
        hir_output_file.replace(hir_output_file.end() - 2,
                                hir_output_file.end(), "hir");
        if (is_debug) {
            builder->getModule()->HighIRprint(std::string(hir_output_file));
            if (is_show_hir_pad_graph)
                builder->getModule()->HIRSHOW(std::string(hir_output_file));
        } else {
            builder->getModule()->HighIRprint(std::string(output_file));
            // builder->getModule()->HIRSHOW(std::string(output_file));
        }
    }
    
    pass_manager PM(builder->getModule().get());
    PM.add_pass<HIRToMIR>("HIRToMIR");
    //  PM.add_pass<LowerIR>("LowerIR");
    PM.run();


    if (is_emit_mir) {
        WARNNING("emitting  mir");
        auto mir_output_file = input_file;
        mir_output_file.replace(mir_output_file.end() - 2,
                                mir_output_file.end(), "ir");
        if (is_debug)
            builder->getModule()->MIRMEMprint(mir_output_file);
        else
            builder->getModule()->MIRMEMprint(output_file);
    }
    
    AsmBuilder asm_builder(builder->getModule(), debug);
    std::string asm_code = asm_builder.generate_asm(input_file.c_str());
    std::cout<<"################-asm_code-#################"<<std::endl;
    std::fflush(0);
    std::cout<<asm_code;
    std::cout<<"################-asm_code-#################"<<std::endl;

    

    return 0;
}
