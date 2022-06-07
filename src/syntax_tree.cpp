
#include "syntax_tree.h"
#include "visitor/tree_visitor_base.h"
void syntax_tree_node::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_comp_unit::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_func_def::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_block::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_const_decl::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_basic_type::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_const_def_list::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_const_init_val::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_const_val_list::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_const_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_var_decl::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_arrray_def::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_init_val::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_init_val_list::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_func_fparams::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_func_fparam::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_func_fparamone::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_func_fparamarray::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_decl::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_const_def::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_const_exp_list::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_var_def_list::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_var_def::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_block_item_list::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_block_item::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_stmt::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_assign_stmt::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_if_stmt::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_if_else_stmt::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_while_stmt::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_break_stmt::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_continue_stmt::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_cond::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_return_stmt::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_return_null_stmt::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_l_val::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_array_ident::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_number::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_primary_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_unary_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_func_call::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_func_paramlist::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_mul_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_add_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_rel_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_eq_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_l_and_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}
void tree_l_or_exp::accept(tree_visitor_base &v) {
    v.visit(*this);
}