#include <getopt.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include "asm/asm_builder.h"
#include "passes/dominators.h"
#include "passes/emit_ir.h"
#include "passes/global_value_numbering.h"
#include "passes/hir_to_mir.h"
#include "passes/lower_ir.h"
#include "passes/function_inline.h"
#include "passes/mem2reg.h"
#include "passes/mir_simplify_cfg.h"
#include "passes/pass_manager.h"
#include "passes/sccp.h"
#include "utils.h"
#include "visitor/syntax_detail_shower.h"
#include "visitor/syntax_tree_shower.h"
#include "visitor/sysy_builder.h"
#include "visitor/tree_visitor_base.h"
#include "passes/dominators.h"
#include "passes/emit_ir.h"
#include "passes/global_value_numbering.h"
#include "passes/hir_to_mir.h"
#include "passes/mir_simplify_cfg.h"
#include "passes/lower_ir.h"
#include "passes/mem2reg.h"
#include "passes/pass_manager.h"
#include "asm/asm_builder.h"

extern int yyparse();
extern int yyrestart(FILE *);
extern FILE *yyin;
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
    bool is_O2 = false;
    bool is_debug = false;
    bool is_emit_asm = false;
    std::string input_file, output_file;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = std::string(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "-S") == 0 && i + 1 < argc) {
            is_emit_asm = true;
            // output_file = std::string(argv[i + 1]);
            // i++;
        } else if (strcmp(argv[i], "-emit-hir") == 0) {
            is_emit_hir = true;
        } else if (strcmp(argv[i], "-emit-mir") == 0) {
            is_emit_mir = true;
        } else if (strcmp(argv[i], "-debug") == 0) {
            is_debug = true;
        } else if (strcmp(argv[i], "-pad") == 0) {
            is_show_hir_pad_graph = true;
        } else if (strcmp(argv[i], "-O2") == 0) {
            is_O2 = true;
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
    yyparse();

    auto *builder = new SYSYBuilder(input_file);
    std::cout<<"building syntax tree...\n";
    builder->build(root);
    // auto *shower = new syntax_detail_shower();
    // shower->visit(*root);

    pass_manager PM(builder->getModule().get());

    if(is_emit_hir )
        PM.add_pass<EmitHir>("EmitHir");
    // if(is_show_hir_pad_graph && is_debug)
    //     PM.add_pass<EmitPadGraph>("EmitPadGraph");


    PM.add_pass<HIRToMIR>("HIRToMIR");
    // if(is_emit_mir && is_debug)
    //     PM.add_pass<EmitIR>("EmitIR");
    if(is_O2){
        PM.add_pass<MirSimplifyCFG>("MirSimplifyCFG");


        // if(is_emit_mir && is_debug)
        // PM.add_pass<EmitIR>("EmitIR");
        if(is_show_hir_pad_graph && is_debug)
            PM.add_pass<EmitPadGraph>("EmitPadGraph");
        PM.add_pass<Mem2Reg>("Mem2Reg");
        if(is_emit_mir && is_debug)
            PM.add_pass<EmitIR>("EmitIR");

        PM.add_pass<MirSimplifyCFG>("MirSimplifyCFG");
        if(is_emit_mir && is_debug)
            PM.add_pass<EmitIR>("EmitIR");
        PM.add_pass<FunctionInline>("FunctionInline");
        if(is_emit_mir && is_debug)
            PM.add_pass<EmitIR>("EmitIR");

        PM.add_pass<SCCP>("SCCP");
        if(is_emit_mir && is_debug)
            PM.add_pass<EmitIR>("EmitIR");

        PM.add_pass<MirSimplifyCFG>("MirSimplifyCFG");
        if(is_emit_mir && is_debug)
            PM.add_pass<EmitIR>("EmitIR");

        PM.add_pass<GlobalVariableNumbering>("GVN");
        if(is_emit_mir && is_debug)
            PM.add_pass<EmitIR>("EmitIR");

    }
    PM.add_pass<LowerIR>("LowerIR");
    PM.run();

    if(is_emit_mir && !is_debug){
    }
    // builder->getModule()->MIRMEMprint("tmp.ir");

    if (is_emit_asm) {
        builder->getModule()->MIRMEMIndex();
        AsmBuilder asm_builder(builder->getModule(), debug);
        std::string asm_code = asm_builder.generate_asm(input_file.c_str());
        fprintf(output,"%s",asm_code.c_str());
        fclose(output);
    }


    return 0;
}
