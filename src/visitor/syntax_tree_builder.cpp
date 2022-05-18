//
// Created by Administrator on 2022/5/1.
//
#include "ir/value.h"
#include "syntax_tree_builder.h"
#include "utils.h"

value* temp_val;
type* temp_type;


void syntax_tree_builder::visit(syntax_tree_node &node) {
    ERROR("visiting syntax tree node!! ");
    
}
void syntax_tree_builder::visit(tree_comp_unit &node) {
    for(const auto& decl:node.definitions){
        decl->accept(*this);

    }
    for (const auto& func : node.functions) {
        func->accept(*this);
    }
}
void syntax_tree_builder::visit(tree_func_def &node) {
    node.type->accept(*this);
    auto func_type = temp_type;

    arguments* func_args ;
    if(node.funcfparams!= nullptr){
        node.funcfparams->accept(*this);
        func_args = static_cast<arguments*>(temp_val);
    }
    else {
        func_args= nullptr;
    }



}
void syntax_tree_builder::visit(tree_block &node) {
}
void syntax_tree_builder::visit(tree_const_decl &node) {
}
void syntax_tree_builder::visit(tree_basic_type &node) {
    temp_type = new type(node.type);
}
void syntax_tree_builder::visit(tree_const_def_list &node) {
}
void syntax_tree_builder::visit(tree_const_init_val &node) {}
void syntax_tree_builder::visit(tree_const_exp &node) {}
void syntax_tree_builder::visit(tree_var_decl &node) {}
void syntax_tree_builder::visit(tree_exp &node) {}
void syntax_tree_builder::visit(tree_init_val &node) {}
void syntax_tree_builder::visit(tree_decl &node) {}
void syntax_tree_builder::visit(tree_const_def &node) {}
void syntax_tree_builder::visit(tree_var_def_list &node) {}
void syntax_tree_builder::visit(tree_var_def &node) {}
void syntax_tree_builder::visit(tree_block_item_list &node) {}
void syntax_tree_builder::visit(tree_block_item &node) {}
void syntax_tree_builder::visit(tree_stmt &node) {}
void syntax_tree_builder::visit(tree_assign_stmt &node) {}
void syntax_tree_builder::visit(tree_return_stmt &node) {}
void syntax_tree_builder::visit(tree_return_null_stmt &node) {}
void syntax_tree_builder::visit(tree_l_val &node) {}
void syntax_tree_builder::visit(tree_number &node) {}
void syntax_tree_builder::visit(tree_primary_exp &node) {}
void syntax_tree_builder::visit(tree_unary_exp &node) {}
void syntax_tree_builder::visit(tree_mul_exp &node) {}
void syntax_tree_builder::visit(tree_add_exp &node) {}
void syntax_tree_builder::visit(tree_rel_exp &node) {}
void syntax_tree_builder::visit(tree_eq_exp &node) {}
void syntax_tree_builder::visit(tree_l_and_exp &node) {}
void syntax_tree_builder::visit(tree_l_or_exp &node) {}
void syntax_tree_builder::visit(tree_func_fparams &node) {
    auto* args = new arguments(node.funcfparamlist.size());
    for(const auto& arg:node.funcfparamlist){
        arg->accept(*this);
        args->add_arg(static_cast<variable *>(temp_val));
    }
    temp_val=args;

}
void syntax_tree_builder::visit(tree_func_fparam &node) {
    if(node.funcfparamone!=nullptr){
        node.funcfparamone->accept(*this);
    }
    else if(node.funcfparamarray!=nullptr){
        node.funcfparamarray->accept(*this);
    }
    else {
        ERROR("error node");
    }

}
void syntax_tree_builder::visit(tree_func_fparamone &node) {
    auto var = new variable(node.id,new type(type_helper::INT)) ;
    temp_val = var;

}
void syntax_tree_builder::visit(tree_func_fparamarray &node) {
    auto var = new variable(node.id,new type(type_helper::PTR)) ;
    temp_val = var;
}
void syntax_tree_builder::visit(tree_init_val_array &node) {
    ERROR("TODO");
}
void syntax_tree_builder::visit(tree_init_val_arraylist &node) {
    ERROR("TODO");
}
