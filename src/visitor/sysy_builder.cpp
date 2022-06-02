#include "sysy_builder.h"
#include "ir/function.h"
#include "ir/value.h"
#include "utils.h"
#include <iostream>

#define CONST(num) ConstantInt::get(num, &*module)

/*** 全局变量 ***/
// store temporary value
Value *G_tmp_val = nullptr;

// 暂存类型
Type *G_tmp_type = nullptr;
// 函数传参是否需要地址
bool G_require_address = false;
// function that is being built
Function *G_cur_fun = nullptr;
// detect scope pre-enter (for elegance only)
bool G_pre_enter_scope = false;
// 数组初始化填入值
std::vector<Value *> G_array_init;
// 用于计算数值与val转换
int G_tmp_int = 0;
// 标记当前为计算数值状态 并且使用到 G_tmp_int
bool G_tmp_int_using = false;
// 标记当前为全局初始化状态 说明全局变量必须计算初值
bool G_in_global_init = false;

/*** 全局变量 ***/

//%type <comp_unit>        CompUnit // ysx
//%type <basic_type>       BType
//%type <const_exp>        ConstExp
//%type <const_def>        ConstDef
//%type <const_def_list>   ConstDefList
//%type <const_exp_list>   ConstExpArrayList
//%type <const_init_val>   ConstInitVal
//%type <const_init_val_list>   ConstInitVallist
//%type <var_decl>         VarDecl
//%type <func_call>        FuncCall
//%type <funcr_paramlist>  FuncRParamList
//%type <var_def>          VarDef
//%type <var_def_list>     VarDefList
//%type <array_def>        ArrayDef

/*
%type <init_val>         InitVal // dyb var def
%type <init_val_array>   InitValArray // var def
%type <init_val_arraylist>InitValArrayList // var def
%type <func_def>         FuncDef
%type<func_fparams>      FuncFParams
%type<func_fparam>       FuncFParam
%type<func_fparamone>    FuncFParamOne
%type<func_fparamarray>  FuncFParamArray
%type <block>            Block
%type <block_item>       BlockItem
%type <block_item_list>  BlockItemList
%type <stmt>             Stmt
%type <cond>             Cond
*/

void SYSYBuilder::visit(tree_comp_unit &node) {
    for (auto &func: node.functions) {
        func->accept(*this);
    }
    for (auto &defs: node.functions) {
        defs->accept(*this);
    }
}

void SYSYBuilder::visit(tree_func_def &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_block &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_decl &node) {
    switch (node.b_type->type) {
        case type_helper::INT:
            G_tmp_type = Type::getInt32Ty();
            break;
        case type_helper::FLOAT:
            G_tmp_type = Type::getFloatTy();
            break;
        default:
            ERROR("error type");
    }
    node.const_def_list->accept(*this);
}

void SYSYBuilder::visit(tree_basic_type &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_def_list &node) {
    // ysx todo
    for (auto &def: node.const_defs) {
        def->accept(*this);
    }
}

void SYSYBuilder::visit(tree_const_init_val &node) {
    //ysx todo
}

void SYSYBuilder::visit(tree_const_exp &node) {
    // YSX todo
}

void SYSYBuilder::visit(tree_var_decl &node) {
    // ysx todo
    for (auto n: node.var_def_list) {
        n->accept(*this);
    }
}

void SYSYBuilder::visit(tree_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_init_val &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_init_val_array &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_init_val_arraylist &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_fparams &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_fparam &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_fparamone &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_fparamarray &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_decl &node) {
    if (node.const_decl != nullptr) {
        node.const_decl->accept(*this);
    } else if (node.var_decl != nullptr) {
        node.var_decl->accept(*this);
    } else {
        ERROR("tree_decl build error");
    }
}

void SYSYBuilder::visit(tree_const_def &node) {
    // ysx todo
    if (node.const_init_val == nullptr) {
        ERROR("const_def need init value");
    } else {
        if (node.const_exp_list == nullptr) {// 非数组情况
            node.const_init_val->accept(*this);
            MyAssert("error in push scope", scope.push(node.id, G_tmp_val));
        } else {

        }
    }
}
}

void SYSYBuilder::visit(tree_var_def_list &node) {
    //ysx todo
    TRACE("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_var_def &node) {
    //ysx todo
    if (node.array_def != nullptr) {// 数组
        Type *TyArray = TyInt32;
        std::vector<int32_t> array_bounds;
    } else {// 不是数组
        if (scope.in_global_scope()) {
            if (node.init_val != nullptr) {// 变量赋值
                G_in_global_init = true;
                node.init_val->accept(*this);
                G_in_global_init = false;
                auto initializer = static_cast<Constant *>(G_tmp_val);
                auto var = GlobalVariable::create(node.id, &*module, TyInt32, false,
                                                  initializer);
                scope.push(node.id, var);
            } else {
                auto var = GlobalVariable::create(node.id, &*module, TyInt32, false,
                                                  CONST(0));
                scope.push(node.id, var);
            }
        } else {
            auto val_alloc = builder->CreateAlloca(TyInt32);
            scope.push(node.id, val_alloc);
            if (node.init_val != nullptr) {// 变量赋值
                node.init_val->accept(*this);
                builder->CreateStore(G_tmp_val, val_alloc);
            }
        }
    }
}

void SYSYBuilder::visit(tree_block_item_list &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_block_item &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_stmt &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_assign_stmt &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_return_stmt &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_return_null_stmt &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_l_val &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_number &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_primary_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_unary_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_mul_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_add_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_rel_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_eq_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_l_and_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_l_or_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_const_val_list &node) {
    /*TODO*/
    TRACE("visit tree_const_val_list error");
}

void SYSYBuilder::visit(tree_const_exp_list &node) {
    // ysx todo
}

void SYSYBuilder::visit(tree_arrray_def &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_if_stmt &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_if_else_stmt &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_while_stmt &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_break_stmt &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_continue_stmt &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_cond &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_array_ident &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_call &node) {
    // ysx todo
}

void SYSYBuilder::visit(tree_funcr_paramlist &node){
        //ysx todo
};