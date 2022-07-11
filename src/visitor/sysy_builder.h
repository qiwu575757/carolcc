#ifndef VISTOR_SYSYBUILDER_HPP
#define VISTOR_SYSYBUILDER_HPP

#include <map>

#include "passes/module.h"
#include "tree_visitor_base.h"
#include <queue>
class Scope;
class IRBuilder;
class Module;


class SYSYBuilder : public tree_visitor_base {
   public:
    SYSYBuilder(const std::string &name);
    void build(tree_comp_unit *node) ;
    std::shared_ptr<Module> getModule() { return this->module; }

   private:
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

    std::unique_ptr<IRBuilder> builder;
    Scope scope;
    std::shared_ptr<Module> module;



     // for cast
    // 用于暂存操作数
    std::queue<Value*> _oprd_queue;
    // 用于计算立即数
    void calBinary(const std::string oprt);
    void buildBinary(const std::string oprt);
    Value* checkAndCast(Value* value,Type* target_type);
};

#endif