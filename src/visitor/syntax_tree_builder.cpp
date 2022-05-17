//
// Created by Administrator on 2022/5/1.
//
#include "ir/value.h"
#include "syntax_tree_builder.h"
#include "utils.h"
#include <stdlib.h>

value* temp_val;


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
    auto func_type = new type(node.type->type);
    
    // todo:args 
    // auto args
    for(auto param :node.funcfparams->funcfparamlist){
        if(param->funcfparamarray!=nullptr){
                param->funcfparamarray->accept(*this);
        }
        else if(param->funcfparamone!=nullptr){

        }
        else {
            ERROR("syntax_tree");
        }

    }
    
    // auto temp_func = new 
    // function(function,node.id,node);
    // _module->_func_list.push_back()

}
void syntax_tree_builder::visit(tree_block &node) {
}
void syntax_tree_builder::visit(tree_const_decl &node) {
}
void syntax_tree_builder::visit(tree_basic_type &node) {
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
