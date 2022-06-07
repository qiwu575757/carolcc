#ifndef VISTOR_SYSYBUILDER_HPP
#define VISTOR_SYSYBUILDER_HPP

#include "ir/basic_block.h"
#include "ir/constant.h"
#include "ir/function.h"
#include "ir/global_variable.h"
#include "ir_builder.h"
#include "ir/instruction.h"
#include "passes/module.h"
#include "ir/type.h"
#include "ir/user.h"
#include "ir/value.h"
#include "tree_visitor_base.h"
#include "syntax_tree.h"
#include <map>


class SYSYBuilder : public tree_visitor_base {
public:
  SYSYBuilder() {
    module = std::shared_ptr<Module>(new Module("SysY code"));
    builder = std::unique_ptr<IRBuilder>(new IRBuilder(nullptr,nullptr));

    auto TyVoid = Type::getVoidTy(); // 改
    auto TyInt32 = Type::getInt32Ty(); // 改
    auto TyIntPtr = Type::getInt32PtrTy(); // 改
    auto TyFloatPtr = Type::getFloatPtrTy(); // 改

    /**** 库函数引用 ****/
    auto getint_type = FunctionType::get(TyInt32, {});
    auto getfloat_type = FunctionType::get(TyFloat, {});
    // auto getch_type = FunctionType::get(TyInt32, false);
    auto getint_fun = Function::create(getint_type, "getint", module.get());
    auto getch_fun = Function::create(getint_type, "getch", module.get());
    auto getfloat_fun = Function::create(getfloat_type, "getfloat", module.get());

    std::vector<Type *> putint_params;
    putint_params.push_back(TyInt32);
    std::vector<Type *> putfloat_params;
    putfloat_params.push_back(TyFloat);
    auto putfloat_type = FunctionType::get(TyVoid, putfloat_params);
    auto putint_type = FunctionType::get(TyVoid, putint_params);

    auto putint_fun = Function::create(putint_type, "putint", module.get());

    auto putch_fun = Function::create(putint_type, "putch", module.get());
    std::vector<Type *> putfloat_params;
    putfloat_params.push_back(TyFloat);
    auto putfloat_type = FunctionType::get(TyVoid, putfloat_params);
    auto putfloat_fun = Function::create(putfloat_type, "putfloat", module.get());

    std::vector<Type *> getarray_params;
    getarray_params.push_back(TyIntPtr);
    auto getarray_type = FunctionType::get(TyInt32, getarray_params);

    auto getarray_fun =
        Function::create(getarray_type, "getarray", module.get());
    std::vector<Type *> getfarray_params;
    getfarray_params.push_back(TyFloatPtr);
    auto getfarray_type = FunctionType::get(TyInt32, getfarray_params);
    auto getfarray_fun =
        Function::create(getfarray_type, "getfarray", module.get());

    std::vector<Type *> putarray_params;
    putarray_params.push_back(TyInt32);
    putarray_params.push_back(TyIntPtr);
    auto putarray_type = FunctionType::get(TyVoid, putarray_params);

    auto putarray_fun =
        Function::create(putarray_type, "putarray", module.get());

    std::vector<Type *> putfarray_params;
    putfarray_params.push_back(TyInt32);
    putfarray_params.push_back(TyFloatPtr);
    auto putfarray_type = FunctionType::get(TyVoid, putarray_params);

    auto putfarray_fun =
        Function::create(putfarray_type, "putfarray", module.get());

    std::vector<Type *> starttime_params;
    // starttime_params.push_back(TyInt32);

    auto starttime_type = FunctionType::get(TyVoid, starttime_params);

    auto starttime_fun =
        Function::create(starttime_type, "starttime", module.get());

    std::vector<Type *> stoptime_params;
    // stoptime_params.push_back(TyInt32);
    auto stoptime_type = FunctionType::get(TyVoid, stoptime_params);

    auto stoptime_fun =
        Function::create(stoptime_type, "stoptime", module.get());

    auto mtstart_type = FunctionType::get(TyInt32, {});

    auto mtstart_fun =
        Function::create(mtstart_type, "__mtstart", module.get());

    std::vector<Type *> mtend_params;
    mtend_params.push_back(TyInt32);
    auto mtend_type = FunctionType::get(TyVoid, mtend_params);

    auto mtend_fun = Function::create(mtend_type, "__mtend", module.get());

    scope.enter();
    scope.push("getint", getint_fun);
    scope.push("getfloat", getfloat_fun);
    scope.push("getch", getch_fun);
    scope.push("putint", putint_fun);
    scope.push("putch", putch_fun);
    scope.push("putfloat", putfloat_fun);
    scope.push("getarray", getarray_fun);
    scope.push("getfarray", getfarray_fun);
    scope.push("putarray", putarray_fun);
    scope.push("putfarray", putfarray_fun);
    scope.push("starttime", starttime_fun);
    scope.push("stoptime", stoptime_fun);
    scope.push("__mtstart", mtstart_fun);
    scope.push("__mtend", mtend_fun);
    /**** 库函数引用 END ****/

  }
  void build(tree_comp_unit* node){
    node->accept(*this);
  }
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
};

#endif