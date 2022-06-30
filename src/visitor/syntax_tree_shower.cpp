//
// Created by Administrator on 2022/5/1.
//
#include "syntax_tree_shower.h"
#include "utils.h"
#include <helpers/graph_helper.h>
#include <stdlib.h>
int index = 0;
std::string last_node = "[*]";
graph g = graph();
void syntax_tree_shower::visit(syntax_tree_node &node) {
}
void syntax_tree_shower::visit(tree_comp_unit &node) {
    index++;
    std::printf("here!\n");
    std::string this_node = "comp_unit_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.set("syntax_graph", graphType::ZhuangTaiTu, "ZhuangTaiTu", "describe attributes here");
    g.begin();
    g.ZhuangTaiTu_add(last_node, "comp_unit_" + std::to_string(index));
    for (auto nd: node.definitions) {
        last_node = this_node;
        nd->accept(*this);
    }
    for (auto nd: node.functions) {
        last_node = this_node;
        nd->accept(*this);
    }
    g.finish("result", "give your description");
    g.close();
}
void syntax_tree_shower::visit(tree_func_def &node) {
    index++;
    std::string this_node = "func_def_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "func_def_" + std::to_string(index));
    for (auto nd: node.block) {
        last_node = this_node;
        nd->accept(*this);
    }
    last_node = this_node;
    if (node.funcfparams) node.funcfparams->accept(*this);
    last_node = this_node;
    if (node.type) node.type->accept(*this);
}
void syntax_tree_shower::visit(tree_block &node) {
    index++;
    std::string this_node = "block_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "block_" + std::to_string(index));
    last_node = this_node;
    if (node.block_item_list) node.block_item_list->accept(*this);
}
void syntax_tree_shower::visit(tree_const_decl &node) {
    index++;
    std::string this_node = "const_decl_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "const_decl_" + std::to_string(index));
    last_node = this_node;
    if (node.const_def_list) node.const_def_list->accept(*this);
    last_node = this_node;
    if (node.b_type) node.b_type->accept(*this);
}
void syntax_tree_shower::visit(tree_basic_type &node) {
    index++;
    std::string this_node = "basic_type_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "basic_type_" + std::to_string(index));
}
void syntax_tree_shower::visit(tree_const_def_list &node) {
    index++;
    std::string this_node = "const_def_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "const_def_" + std::to_string(index));
    for (auto nd: node.const_defs) {
        last_node = this_node;
        nd->accept(*this);
    }
}
void syntax_tree_shower::visit(tree_const_init_val &node) {
    index++;
    std::string this_node = "const_init_val_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "const_init_val_" + std::to_string(index));
    last_node = this_node;
    std::printf("%s\n", this_node.c_str());
    if (node.const_exp) node.const_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_const_exp &node) {
    index++;
    std::string this_node = "const_exp_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "const_exp_" + std::to_string(index));
    last_node = this_node;
    if (node.add_exp) node.add_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_var_decl &node) {
    index++;
    std::string this_node = "var_decl_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "var_decl_" + std::to_string(index));
    last_node = this_node;
    if (node.b_type) node.b_type->accept(*this);
    last_node = this_node;
    if (node.var_def_list) node.var_def_list->accept(*this);
}
void syntax_tree_shower::visit(tree_exp &node) {
    index++;
    std::string this_node = "exp_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "exp_" + std::to_string(index));
    last_node = this_node;
    if (node.add_exp) node.add_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_init_val &node) {
    index++;
    std::string this_node = "init_val_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "init_val_" + std::to_string(index));
    last_node = this_node;
    if (node.exp) node.exp->accept(*this);
    if (node.init_val_list) node.init_val_list->accept(*this);
}
void syntax_tree_shower::visit(tree_decl &node) {
    index++;
    std::string this_node = "decl_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "decl_" + std::to_string(index));
    last_node = this_node;
    if (node.const_decl) node.const_decl->accept(*this);
    last_node = this_node;
    if (node.var_decl) node.var_decl->accept(*this);
}
void syntax_tree_shower::visit(tree_const_def &node) {
    index++;
    std::string this_node = "const_def_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "const_def_" + std::to_string(index));
    last_node = this_node;
    if (node.const_exp_list) node.const_exp_list->accept(*this);
    last_node = this_node;
    if (node.const_init_val) node.const_init_val->accept(*this);
    last_node = this_node;
}
void syntax_tree_shower::visit(tree_var_def_list &node) {
    index++;
    std::string this_node = "var_def_list_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "var_def_list_" + std::to_string(index));
    for (auto nd: node.var_defs) {
        last_node = this_node;
        nd->accept(*this);
    }
}
void syntax_tree_shower::visit(tree_var_def &node) {
    index++;
    std::string this_node = "var_def_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "var_def_" + std::to_string(index) + node.id);
    last_node = this_node;
    if (node.array_def) node.array_def->accept(*this);
    last_node = this_node;
    if (node.init_val) node.init_val->accept(*this);
    last_node = this_node;
}
void syntax_tree_shower::visit(tree_block_item_list &node) {
    index++;
    std::string this_node = "block_item_list_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "block_item_list_" + std::to_string(index));
    for (auto nd: node.block_items) {
        last_node = this_node;
        nd->accept(*this);
    }
}
void syntax_tree_shower::visit(tree_block_item &node) {
    index++;
    std::string this_node = "block_item_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "block_item_" + std::to_string(index));
    last_node = this_node;
    if (node.decl) node.decl->accept(*this);
    last_node = this_node;
    if (node.stmt) node.stmt->accept(*this);
}
void syntax_tree_shower::visit(tree_stmt &node) {
    index++;
    std::string this_node = "stmt_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "stmt_" + std::to_string(index));
    last_node = this_node;
    if (node.assigm_stmt) node.assigm_stmt->accept(*this);
    last_node = this_node;
    if (node.block) node.block->accept(*this);
    last_node = this_node;
    if (node.break_stmt) node.break_stmt->accept(*this);
    last_node = this_node;
    if (node.continue_stmt) node.continue_stmt->accept(*this);
    last_node = this_node;
    if (node.exp) node.exp->accept(*this);
    last_node = this_node;
    if (node.if_else_stmt) node.if_else_stmt->accept(*this);
    last_node = this_node;
    if (node.if_stmt) node.if_stmt->accept(*this);
    last_node = this_node;
    if (node.return_null_stmt) node.return_null_stmt->accept(*this);
    last_node = this_node;
    if (node.return_stmt) node.return_stmt->accept(*this);
    last_node = this_node;
    if (node.while_stmt) node.while_stmt->accept(*this);
}
void syntax_tree_shower::visit(tree_assign_stmt &node) {
    index++;
    std::string this_node = "assign_stmt_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "assign_stmt_" + std::to_string(index));
    last_node = this_node;
    if (node.l_val) node.l_val->accept(*this);
    last_node = this_node;
    if (node.exp) node.exp->accept(*this);
}
void syntax_tree_shower::visit(tree_return_stmt &node) {
    index++;
    std::string this_node = "return_stmt_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "return_stmt_" + std::to_string(index));
    last_node = this_node;
    if (node.exp) node.exp->accept(*this);
}
void syntax_tree_shower::visit(tree_return_null_stmt &node) {
    index++;
    std::string this_node = "return_null_stmt_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "return_null_stmt_" + std::to_string(index));
}
void syntax_tree_shower::visit(tree_l_val &node) {
    index++;
    std::string this_node = "l_val_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "l_val_" + std::to_string(index));
    last_node = this_node;
    if (node.array_ident) node.array_ident->accept(*this);
}
void syntax_tree_shower::visit(tree_number &node) {
    index++;
    std::string this_node = "number_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "number_" + std::to_string(index));
}
void syntax_tree_shower::visit(tree_primary_exp &node) {
    index++;
    std::string this_node = "primary_exp" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "primary_exp" + std::to_string(index));
    last_node = this_node;
    if (node.exp) node.exp->accept(*this);
    last_node = this_node;
    if (node.number) node.number->accept(*this);
    last_node = this_node;
    if (node.l_val) node.l_val->accept(*this);
}
void syntax_tree_shower::visit(tree_unary_exp &node) {
    index++;
    std::string this_node = "unary_exp_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "unary_exp_" + std::to_string(index));
    last_node = this_node;
    if (node.func_call) node.func_call->accept(*this);
    last_node = this_node;
    if (node.primary_exp) node.primary_exp->accept(*this);
    last_node = this_node;
    if (node.unary_exp) node.unary_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_mul_exp &node) {
    index++;
    std::string this_node = "mul_exp_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "mul_exp_" + std::to_string(index));
    last_node = this_node;
    if (node.mul_exp) node.mul_exp->accept(*this);
    last_node = this_node;
    if (node.unary_exp) node.unary_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_add_exp &node) {
    index++;
    std::string this_node = "add_exp_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "add_exp_" + std::to_string(index));
    last_node = this_node;
    if (node.add_exp) node.add_exp->accept(*this);
    last_node = this_node;
    if (node.mul_exp) node.mul_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_rel_exp &node) {
    index++;
    std::string this_node = "rel_exp_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "rel_exp_" + std::to_string(index));
    last_node = this_node;
    if (node.add_exp) node.add_exp->accept(*this);
    last_node = this_node;
    if (node.rel_exp) node.rel_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_eq_exp &node) {
    index++;
    std::string this_node = "eq_exp_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "eq_exp_" + std::to_string(index));
    last_node = this_node;
    if (node.eq_exp) node.eq_exp->accept(*this);
    last_node = this_node;
    if (node.rel_exp) node.rel_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_l_and_exp &node) {
    index++;
    std::string this_node = "l_and_exp_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "l_and_exp_" + std::to_string(index));
    last_node = this_node;
    if (node.eq_exp) node.eq_exp->accept(*this);
    last_node = this_node;
    if (node.l_and_exp) node.l_and_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_l_or_exp &node) {
    index++;
    std::string this_node = "l_or_exp_" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "l_or_exp_" + std::to_string(index));
    last_node = this_node;
    if (node.l_and_exp) node.l_and_exp->accept(*this);
    last_node = this_node;
    if (node.l_or_exp) node.l_or_exp->accept(*this);
}
void syntax_tree_shower::visit(tree_init_val_list &node){
    index++;
    std::string this_node = "init_val_list" + std::to_string(index);
    std::printf("%s\n", this_node.c_str());
    g.ZhuangTaiTu_add(last_node, "init_val_list" + std::to_string(index));
    last_node = this_node;
    if(!node.init_vals.empty())
        for(auto nd : node.init_vals){
            last_node = this_node;
            nd->accept(*this);
        }
}

// void syntax_tree_shower::visit(tree_init_val_array &node) {
//     index++;
//     std::string this_node = "init_val_array"+std::to_string(index);
//     std::printf("%s\n",this_node.c_str());
//     g.ZhuangTaiTu_add(last_node,"init_val_array"+std::to_string(index));
//     last_node = this_node;
//     if(node.init_val_arraylist)node.init_val_arraylist->accept(*this);
// }
// void syntax_tree_shower::visit(tree_init_val_arraylist &node) {
//     index++;
//     std::string this_node = "init_val_arraylist"+std::to_string(index);
//     std::printf("%s\n",this_node.c_str());
//     g.ZhuangTaiTu_add(last_node,"init_val_arraylist"+std::to_string(index));
//     if(!node.init_var_array.empty())
//     for(auto nd : node.init_var_array){
//         last_node = this_node;
//         nd->accept(*this);
//     }
//     if(!node.init_vars.empty())
//     for(auto nd : node.init_vars){
//         last_node = this_node;
//         nd->accept(*this);
//     }
// }
void syntax_tree_shower::visit(tree_func_fparams &node) {
    index++;
    std::string this_node = "func_fparams"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,"func_fparams"+std::to_string(index));
    if(!node.funcfparamlist.empty())
    for(auto nd : node.funcfparamlist){
        last_node = this_node;
        nd->accept(*this);
    }
}
void syntax_tree_shower::visit(tree_func_fparam &node) {
    index++;
    std::string this_node = "func_fparam"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,"func_fparam"+std::to_string(index));
    last_node = this_node;
    if(node.funcfparamarray)node.funcfparamarray->accept(*this);
    last_node = this_node;
    if(node.funcfparamone)node.funcfparamone->accept(*this);
}
void syntax_tree_shower::visit(tree_func_fparamone &node) {
    index++;
    std::string this_node = "func_fparamone"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,"func_fparamone"+std::to_string(index));
    last_node = this_node;
    if(node.b_type)node.b_type->accept(*this);
    last_node = this_node;
}
void syntax_tree_shower::visit(tree_func_fparamarray &node) {
    index++;
    std::string this_node = "func_fparamarray"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,"func_fparamarray"+std::to_string(index));
    last_node = this_node;
    if(node.b_type)node.b_type->accept(*this);
    last_node = this_node;
    if(!node.exps.empty())
    for(auto nd : node.exps){
        last_node = this_node;
        nd->accept(*this);
    }
}

void syntax_tree_shower::visit(tree_const_val_list &node){
    index++;
    std::string this_node = "tree_const_val_list"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;

    if(!node.const_init_vals.empty())
    for(auto nd : node.const_init_vals){
        last_node = this_node;
        nd->accept(*this);
    }
}
void syntax_tree_shower::visit(tree_const_exp_list &node){
    index++;
    std::string this_node = "tree_const_exp_list"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;

    if(!node.const_exp.empty())
    for(auto nd : node.const_exp){
        last_node = this_node;
        nd->accept(*this);
    }
}
void syntax_tree_shower::visit(tree_arrray_def &node){
    index++;
    std::string this_node = "tree_arrray_def"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;

    if(!node.const_exps.empty())
    for(auto nd : node.const_exps){
        last_node = this_node;
        nd->accept(*this);
    }
}
void syntax_tree_shower::visit(tree_if_stmt &node){
    index++;
    std::string this_node = "tree_if_stmt"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;
    if(node.cond)node.cond->accept(*this);
    last_node = this_node;
    if(node.stmt)node.stmt->accept(*this);
}
void syntax_tree_shower::visit(tree_if_else_stmt &node){
    index++;
    std::string this_node = "tree_if_else_stmt"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;
    if(node.cond)node.cond->accept(*this);
    last_node = this_node;
    if(node.then_stmt)node.then_stmt->accept(*this);
    last_node = this_node;
    if(node.else_stmt)node.else_stmt->accept(*this);
}
void syntax_tree_shower::visit(tree_while_stmt &node){
    index++;
    std::string this_node = "tree_while_stmt"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;
    if(node.cond)node.cond->accept(*this);
    last_node = this_node;
    if(node.stmt)node.stmt->accept(*this);
    last_node = this_node;
}
void syntax_tree_shower::visit(tree_break_stmt &node){
    index++;
    std::string this_node = "tree_break_stmt"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;
}
void syntax_tree_shower::visit(tree_continue_stmt &node){
    index++;
    std::string this_node = "tree_continue_stmt"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;
}
void syntax_tree_shower::visit(tree_cond &node){
    index++;
    std::string this_node = "tree_cond"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;
    if(node.l_or_exp)node.l_or_exp->accept(*this);
    last_node = this_node;
}
void syntax_tree_shower::visit(tree_array_ident &node){
    index++;
    std::string this_node = "tree_array_ident"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;

    if(!node.exps.empty())
    for(auto nd : node.exps){
        last_node = this_node;
        nd->accept(*this);
    }
}
void syntax_tree_shower::visit(tree_func_call &node){
    index++;
    std::string this_node = "tree_func_call"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;
    if(node.func_param_list)node.func_param_list->accept(*this);
}
void syntax_tree_shower::visit(tree_func_paramlist &node){
    index++;
    std::string this_node = "tree_func_paramlist"+std::to_string(index);
    std::printf("%s\n",this_node.c_str());
    g.ZhuangTaiTu_add(last_node,this_node);
    last_node = this_node;

    if(!node.exps.empty())
    for(auto nd : node.exps){
        last_node = this_node;
        nd->accept(*this);
    }
}