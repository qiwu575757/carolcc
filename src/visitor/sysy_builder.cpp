#include "sysy_builder.h"
#include <iostream>

/*** 全局变量 ***/



/*** 全局变量 ***/

// store temporary value
Value *G_tmp_val = nullptr;
// 函数传参是否需要地址
bool G_require_address = false;
// function that is being built
Function *G_cur_fun = nullptr;
// detect scope pre-enter (for elegance only)
bool G_pre_enter_scope = false;
// 数组初始化填入值
std::vector<Value *> G_array_init;
// 维护上下block关系
enum BaseListType { WHILE_COND, WHILE_BODY, IF_COND, IF_THEN, IF_ELSE }; //
// 维护上下block关系
std::vector<BaseListType> G_base_layer_types;
// 
std::vector<BaseBlock *> G_base_layer;
// 用于计算数值与val转换
int G_tmp_int = 0;
// 标记当前为计算数值状态 并且使用到 G_tmp_int
bool G_tmp_int_using = false;
// 标记当前为全局初始化状态
bool G_in_global_init = false;


void SYSYBuilder::visit(tree_comp_unit &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_def &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_block &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_decl &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_basic_type &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_def_list &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_init_val &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_var_decl &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_init_val &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_init_val_array &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_init_val_arraylist &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_fparams &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_fparam &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_fparamone &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_fparamarray &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_decl &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_def &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_var_def_list &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_var_def &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_block_item_list &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_block_item &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_stmt &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_assign_stmt &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_return_stmt &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_return_null_stmt &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_l_val &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_number &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_primary_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_unary_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_mul_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_add_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_rel_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_eq_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_l_and_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_l_or_exp &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_val_list &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_exp_list &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_arrray_def &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_if_stmt &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_if_else_stmt &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_while_stmt &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_break_stmt &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_continue_stmt &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_cond &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_array_ident &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_call &node){
    /*TODO*/
}

void SYSYBuilder::visit(tree_funcr_paramlist &node){
    /*TODO*/
};