//
// Created by Administrator on 2022/5/1.
//
#ifndef COMPILER_SYNTAX_DETAIL_SHOWER_H
#define COMPILER_SYNTAX_DETAIL_SHOWER_H
#include "helpers/type_helper.h"
#include "tree_visitor_base.h"

class syntax_detail_shower : public tree_visitor_base {
public:
     void visit(syntax_tree_node &node) final;
     void visit(tree_comp_unit &node) final;
     void visit(tree_func_def &node) final;
     void visit(tree_block &node) final;
     void visit(tree_const_decl &node) final;
     void visit(tree_basic_type &node) final;
     void visit(tree_const_def_list &node) final;
     void visit(tree_const_init_val &node) final;
     void visit(tree_const_exp &node) final;
     void visit(tree_var_decl &node) final;
     void visit(tree_exp &node) final;
     void visit(tree_init_val &node) final;
     void visit(tree_init_val_list &node) final;
     void visit(tree_func_fparams &node) final;
     void visit(tree_func_fparam &node) final;
     void visit(tree_func_fparamone &node) final;
     void visit(tree_func_fparamarray &node) final;
     void visit(tree_decl &node) final;
     void visit(tree_const_def &node) final;
     void visit(tree_var_def_list &node) final;
     void visit(tree_var_def &node) final;
     void visit(tree_block_item_list &node) final;
     void visit(tree_block_item &node) final;
     void visit(tree_stmt &node) final;
     void visit(tree_assign_stmt &node) final;
     void visit(tree_return_stmt &node) final;
     void visit(tree_return_null_stmt &node) final;
     void visit(tree_l_val &node) final;
     void visit(tree_number &node) final;
     void visit(tree_primary_exp &node) final;
     void visit(tree_unary_exp &node) final;
     void visit(tree_mul_exp &node) final;
     void visit(tree_add_exp &node) final;
     void visit(tree_rel_exp &node) final;
     void visit(tree_eq_exp &node) final;
     void visit(tree_l_and_exp &node) final;
     void visit(tree_l_or_exp &node) final;
     void visit(tree_const_val_list &node) final;
     void visit(tree_const_exp_list &node) final;
     void visit(tree_arrray_def &node) final;
     void visit(tree_if_stmt &node) final;
     void visit(tree_if_else_stmt &node) final;
     void visit(tree_while_stmt &node) final;
     void visit(tree_break_stmt &node) final;
     void visit(tree_continue_stmt &node) final;
     void visit(tree_cond &node) final;
     void visit(tree_array_ident &node) final;
     void visit(tree_func_call &node) final;
     void visit(tree_func_paramlist &node) final;
};


#endif//COMPILER_SYNTAX_TREE_BUILDER_H