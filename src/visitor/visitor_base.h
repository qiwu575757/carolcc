
#ifndef CAROLCC_VISITOR_BASE_H
#define CAROLCC_VISITOR_BASE_H
#include "syntax_tree.h"
class tree_comp_unit;
class visitor_base {
public:
    virtual void visit(syntax_tree_node &node) = 0;
    virtual void visit(tree_comp_unit &node) = 0;
    virtual void visit(tree_func_def &node) = 0;
    virtual void visit(tree_block &node) = 0;
    virtual void visit(tree_const_decl &node) = 0;
    virtual void visit(tree_basic_type &node) = 0;
    virtual void visit(tree_const_def_list &node) = 0;
    virtual void visit(tree_const_init_val &node) = 0;
    virtual void visit(tree_const_exp &node) = 0;
    virtual void visit(tree_var_decl &node) = 0;
    virtual void visit(tree_exp &node) = 0;
    virtual void visit(tree_init_val &node) = 0;
    virtual void visit(tree_init_val_array &node) = 0;
    virtual void visit(tree_init_val_arraylist &node) = 0;
    virtual void visit(tree_func_fparams &node) = 0;
    virtual void visit(tree_func_fparam &node) = 0;
    virtual void visit(tree_func_fparamone &node) = 0;
    virtual void visit(tree_func_fparamarray &node) = 0;
    virtual void visit(tree_decl &node) = 0;
    virtual void visit(tree_const_def &node) = 0;
    virtual void visit(tree_var_def_list &node) = 0;
    virtual void visit(tree_var_def &node) = 0;
    virtual void visit(tree_block_item_list &node) = 0;
    virtual void visit(tree_block_item &node) = 0;
    virtual void visit(tree_stmt &node) = 0;
    virtual void visit(tree_assign_stmt &node) = 0;
    virtual void visit(tree_return_stmt &node) = 0;
    virtual void visit(tree_return_null_stmt &node) = 0;
    virtual void visit(tree_l_val &node) = 0;
    virtual void visit(tree_number &node) = 0;
    virtual void visit(tree_primary_exp &node) = 0;
    virtual void visit(tree_unary_exp &node) = 0;
    virtual void visit(tree_mul_exp &node) = 0;
    virtual void visit(tree_add_exp &node) = 0;
    virtual void visit(tree_rel_exp &node) = 0;
    virtual void visit(tree_eq_exp &node) = 0;
    virtual void visit(tree_l_and_exp &node) = 0;
    virtual void visit(tree_l_or_exp &node) = 0;
    virtual void visit(tree_const_val_list &node) = 0;
    virtual void visit(tree_const_exp_list &node) = 0;
    virtual void visit(tree_arrray_def &node) = 0;
    virtual void visit(tree_if_stmt &node) = 0;
    virtual void visit(tree_if_else_stmt &node) = 0;
    virtual void visit(tree_while_stmt &node) = 0;
    virtual void visit(tree_break_stmt &node) = 0;
    virtual void visit(tree_continue_stmt &node) = 0;
    virtual void visit(tree_cond &node) = 0;
    virtual void visit(tree_array_ident &node) = 0;
    virtual void visit(tree_func_call &node) = 0;
    virtual void visit(tree_funcr_paramlist &node) = 0;
};
#endif// CAROLCC_VISITOR_BASE_H
