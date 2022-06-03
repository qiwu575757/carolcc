#include "sysy_builder.h"
#include "ir/function.h"
#include "ir/value.h"
#include "utils.h"
#include <iostream>

#define CONST_INT(num) ConstantInt::create(num)

#define CONST_FLOAT(num) ConstantFloat::create(num)

auto TyVoid = Type::getVoidTy();        // 改
auto TyFloat = Type::getFloatTy();      // 改
auto TyInt32 = Type::getInt32Ty();      // 改
auto TyInt32Ptr = Type::getInt32PtrTy();// 改
auto TyFloatPtr = Type::getFloatPtrTy();// 改
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
// 用于记录维度数目
// 用于计算数值与val转换
int G_tmp_int = 0;
float G_tmp_float = 0;
// 标记当前为计算数值状态 并且使用到 G_tmp_int
bool G_tmp_int_using = false;
bool G_tmp_float_using = false;
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
%type <func_def>         FuncDef // done
%type<func_fparams>      FuncFParams // done
%type<func_fparam>       FuncFParam // done
%type<func_fparamone>    FuncFParamOne // done
%type<func_fparamarray>  FuncFParamArray // done
%type <block>            Block // done
%type <block_item>       BlockItem // done
%type <block_item_list>  BlockItemList // done
%type <stmt>             Stmt // done
%type <cond>             Cond // done
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
    /*dyb TODO*/
    FunctionType *fun_type;
    Type *ret_type;
    std::vector<Type *> param_types;

    if (node.type->type == type_helper::INT) {
        ret_type = TyInt32;
    } else if (node.type->type == type_helper::FLOAT) {
        ret_type = TyFloat;
    } else {
        ret_type = TyVoid;
    }

    for (auto &param: node.funcfparams->funcfparamlist) {
        if (param->funcfparamone != nullptr) {
            if (param->funcfparamone->b_type->type == type_helper::INT) {
                param_types.push_back(TyInt32);
            } else if (param->funcfparamone->b_type->type == type_helper::FLOAT) {
                param_types.push_back(TyFloat);
            } else {
                param_types.push_back(TyVoid);
            }
        } else if (param->funcfparamarray != nullptr) {
            if (param->funcfparamarray->b_type->type == type_helper::INT) {
                param_types.push_back(TyInt32Ptr);
            } else if (param->funcfparamarray->b_type->type == type_helper::FLOAT) {
                param_types.push_back(TyFloatPtr);
            }
        }
    }

    fun_type = FunctionType::get(ret_type, param_types);
    auto fun = Function::create(fun_type, node.id, module.get());
    scope.push(node.id, fun);

    G_cur_fun = fun;

    auto funBB = BasicBlock::create(&*module, "entry", fun);
    builder->SetInstrInsertPoint(funBB);
    scope.enter();

    G_pre_enter_scope = true;

    std::vector<Argument *> args;
    for (auto arg = fun->arg_begin(); arg != fun->arg_end(); arg++) {
        args.push_back(*arg);
    }

    int i = 0;
    for (auto param: node.funcfparams->funcfparamlist) {
        if (param->funcfparamarray != nullptr) {
            if (param->funcfparamarray->b_type->type == type_helper::INT) {
                auto array_alloc = builder->createAlloca(TyInt32Ptr);
                builder->createStore(static_cast<Value *>(args[i]), array_alloc);
                std::vector<Value *> array_params;
                array_params.push_back(CONST_INT(0));
                for (auto array_param: param->funcfparamarray->exps) {
                    array_param->accept(*this);
                    array_params.push_back(G_tmp_val);
                }
                scope.push(param->funcfparamarray->id, array_alloc);
                scope.push(param->funcfparamarray->id, array_alloc, array_params);
                args[i]->setArrayBound(array_params);
            } else {
                auto array_alloc = builder->createAlloca(TyFloatPtr);
                builder->createStore(static_cast<Value *>(args[i]), array_alloc);
                std::vector<Value *> array_params;
                array_params.push_back(new ConstantInt(TyFloat, 0));
                for (auto array_param: param->funcfparamarray->exps) {
                    array_param->accept(*this);
                    array_params.push_back(G_tmp_val);
                }
                scope.push(param->funcfparamarray->id, array_alloc);
                scope.push(param->funcfparamarray->id, array_alloc, array_params);
                args[i]->setArrayBound(array_params);
            }
        } else {// 单个
            if (param->funcfparamone->b_type->type == type_helper::INT) {
                auto alloc = builder->createAlloca(TyInt32);
                builder->createStore(args[i], alloc);
                scope.push(param->funcfparamone->id, alloc);
            } else {
                auto alloc = builder->createAlloca(TyFloat);
                builder->createStore(args[i], alloc);
                scope.push(param->funcfparamone->id, alloc);
            }
        }
        i++;
    }
    for (auto &b: node.block) {
        b->accept(*this);
    }
    scope.exit();
}

void SYSYBuilder::visit(tree_block &node) {
    /*dyb TODO*/
    bool need_exit_scope = !G_pre_enter_scope;
    if (G_pre_enter_scope) {
        G_pre_enter_scope = false;
    } else {
        scope.enter();
    }

    for (auto &blockitem: node.block_item_list->block_items) {
        if (blockitem->decl != nullptr) {
            if (builder->GetInsertBaseBlockList().empty()) {
                auto allocaBB = BasicBlock::create(&*module, "", G_cur_fun);
                builder->SetInstrInsertPoint(allocaBB);
                blockitem->accept(*this);
            } else {
                auto allocaBB = BasicBlock::create(&*module, "");
                builder->SetInstrInsertPoint(allocaBB);
                builder->pushBaseBlock(allocaBB);
                blockitem->accept(*this);
            }
        } else if (blockitem->stmt != nullptr) {
            if (blockitem->stmt->assigm_stmt || blockitem->stmt->break_stmt ||
                blockitem->stmt->continue_stmt || blockitem->stmt->exp ||
                blockitem->stmt->return_stmt || blockitem->stmt->return_null_stmt) {
                if (builder->GetInsertBaseBlockList().size() == 0) {
                    if (builder->GetInsertBasicBlock()->getBaseFather() != nullptr) {
                        auto baseBB = BasicBlock::create(&*module, "", G_cur_fun);
                        builder->SetInstrInsertPoint(baseBB);
                    }
                    blockitem->accept(*this);
                } else {
                    auto baseBB = BasicBlock::create(&*module, "");
                    builder->SetInstrInsertPoint(baseBB);
                    builder->pushBaseBlock(baseBB);
                    blockitem->accept(*this);
                }
            } else {
                blockitem->accept(*this);
            }
        }
    }

    if (need_exit_scope) {
        scope.exit();
    }
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
    //    std::shared_ptr<tree_const_val_list> const_val_list;
    //    std::shared_ptr<tree_const_exp> const_exp;
    if (node.const_exp != nullptr) {
        node.const_exp->accept(*this);
    } else {
        auto cur_bnd = node.bounds[0];
        auto dim_length = 1;
        for (int i = 1; i < node.bounds.size(); i++) {
            dim_length *= node.bounds[i];
        }
        std::vector<Value *> init_list;
        for (auto &init_val: node.const_val_list->const_init_vals) {
            if (init_val->const_exp != nullptr) {
                init_val->accept(*this);
                init_list.push_back(G_tmp_val);
            } else {
                auto pos = init_list.size();
                for (int i = 0; i < (dim_length - (pos % dim_length)) % dim_length; i++) {
                    if (G_tmp_type == Type::getInt32Ty()) {
                        init_list.push_back(CONST_INT(0));
                    } else if (G_tmp_type == Type::getFloatTy()) {
                        init_list.push_back(CONST_FLOAT(0));
                    }
                }
                init_val->bounds.assign(node.bounds.begin() + 1, node.bounds.end());
                init_val->accept(*this);
                init_list.insert(init_list.end(), G_array_init.begin(), G_array_init.end());
            }
        }
        for (int i = init_list.size(); i < dim_length * cur_bnd; i++) {
            if (G_tmp_type == Type::getInt32Ty()) {
                init_list.push_back(CONST_INT(0));
            } else if (G_tmp_type == Type::getFloatTy()) {
                init_list.push_back(CONST_FLOAT(0));
            }
        }
        G_array_init.assign(init_list.begin(), init_list.end());
    }
}

void SYSYBuilder::visit(tree_const_exp &node) {
    if (G_tmp_type == Type::getFloatTy()) {
        G_tmp_float_using = true;
    } else if (G_tmp_type == Type::getInt32Ty()) {
        G_tmp_int_using = true;
    } else {
        ERROR("erorr type");
    }
    node.add_exp->accept(*this);
    if (G_tmp_type->isFloatTy()) {
        G_tmp_val = CONST_INT(G_tmp_int);
    } else if (G_tmp_type->isIntegerTy()) {
        G_tmp_val = CONST_INT(G_tmp_int);
    } else {
        ERROR("error type");
    }
    G_tmp_int_using = false;
    G_tmp_float_using = false;
}

void SYSYBuilder::visit(tree_var_decl &node) {
    // ysx todo
    for (auto& n: node.var_def_list->var_defs) {
        n->accept(*this);
    }
}

void SYSYBuilder::visit(tree_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_init_val &node) {
    /*dyb TODO*/
    if (node.exp != nullptr) {// 表达式
        if (G_tmp_type->isIntegerTy()) {
            G_tmp_int_using = true;
            node.exp->accept(*this);
            G_tmp_int_using = false;
            G_tmp_val = (Value *) (CONST_FLOAT(G_tmp_int));
        } else if (G_tmp_type->isFloatTy()) {
            G_tmp_float_using = true;
            node.exp->accept(*this);
            G_tmp_float_using = false;
            G_tmp_val = (Value *) (CONST_FLOAT(G_tmp_int));
        } else {
            node.exp->accept(*this);
        }
    } else {
        ERROR("tree_init_val has null exp");
    }
}

void SYSYBuilder::visit(tree_init_val_array &node) {
    /*dyb TODO 把处理后的数据放在node.instant_init 里方便父节点处理*/
    ERROR("visit tree_init_val_array error");
}

void SYSYBuilder::visit(tree_init_val_arraylist &node) {
    /*TODO*/
    ERROR("visit tree_init_val_arraylist error");
}

void SYSYBuilder::visit(tree_func_fparams &node) {
    /*TODO*/
    ERROR("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_func_fparam &node) {
    /*TODO*/
    ERROR("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_func_fparamone &node) {
    /*TODO*/
    ERROR("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_func_fparamarray &node) {
    /*TODO*/
    ERROR("visit tree_var_def_list error");
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
        } else if (node.const_exp_list != nullptr) {// arrray
            Type *array_element_ty = G_tmp_type;
            std::vector<int> bounds;
            for (auto &array_const_exp: node.const_exp_list->const_exp) {
                array_const_exp->accept(*this);
                auto dim_value = static_cast<ConstantInt *>(G_tmp_val)->getValue();
                bounds.push_back(dim_value);
            }
            for (int i = bounds.size() - 1; i >= 0; i--) {
                array_element_ty = ArrayType::get(array_element_ty, bounds[i]);
            }

            node.const_init_val->bounds.assign(bounds.begin(), bounds.end());
            node.const_init_val->accept(*this);
            auto initializer = ConstantArray::turn(bounds, G_array_init);
            if (scope.in_global_scope()) {
                auto var = GlobalVariable::create(node.id, module.get(), initializer->getType(), true, initializer);
                scope.push(node.id, var);
            } else {// local var array
                //  todo 感觉这里可以跟全局做相同的处理。
                auto array_alloca = builder->createAlloca(array_element_ty);
                scope.push(node.id, array_alloca);
                array_alloca->setInit();
                auto ptr = builder->createGEP(array_alloca, {CONST_INT(0)});
                for (int i = 1; i < node.const_init_val->bounds.size(); i++) {
                    ptr = builder->createGEP(ptr, {CONST_INT(0)});
                }
                for (int i = 0; i < G_array_init.size(); i++) {
                    if (i != 0) {
                        auto p = builder->createGEP(ptr, {CONST_INT(i)});
                        builder->createStore(G_array_init[i], p);
                    } else {
                        builder->createStore(G_array_init[i], ptr);
                    }
                }
            }
        }
    }
}

void SYSYBuilder::visit(tree_var_def_list &node) {
    //ysx todo
    ERROR("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_var_def &node) {
    //ysx todo
    if (node.array_def != nullptr) {// 数组
        std::vector<int32_t> array_bounds;
        // dim v
        for (auto exp: node.array_def->const_exps) {
            exp->accept(*this);
            int dim_v = static_cast<ConstantInt *>(G_tmp_val)->getValue();
            array_bounds.push_back(dim_v);
        }
        // array type
        // ysx
    } else {// 不是数组
        if (scope.in_global_scope()) {
            if (node.init_val != nullptr) {// 变量赋值
                G_in_global_init = true;
                node.init_val->accept(*this);
                G_in_global_init = false;
                auto initializer = static_cast<Constant *>(G_tmp_val);
                auto var = GlobalVariable(node.id, &*module, TyInt32, false, initializer);
                scope.push(node.id, (Value *) (&var));
            } else {
                auto var = GlobalVariable(node.id, &*module, TyInt32, false, new ConstantInt(TyInt32, 0));
                scope.push(node.id, (Value *) (&var));
            }
        } else {
            auto val_alloc = builder->createAlloca(TyInt32);
            scope.push(node.id, val_alloc);
            if (node.init_val != nullptr) {// 变量赋值
                node.init_val->accept(*this);
                builder->createStore(G_tmp_val, val_alloc);
            }
        }
    }
}

void SYSYBuilder::visit(tree_block_item_list &node) {
    /*dyb TODO*/
    ERROR("error tree_block_item_list");
}

void SYSYBuilder::visit(tree_block_item &node) {
    /*dyb TODO*/
    if (node.decl->const_decl != nullptr) {
        node.decl->const_decl->accept(*this);
        return;
    } else if (node.decl->var_decl != nullptr) {
        node.decl->var_decl->accept(*this);
        return;
    } else if (node.stmt != nullptr) {
        node.stmt->accept(*this);
        return;
    }
    ERROR("error tree_block_item");
}

void SYSYBuilder::visit(tree_stmt &node) {
    /*TODO*/
    if (node.assigm_stmt != nullptr) {
        node.assigm_stmt->accept(*this);
        return;
    } else if (node.block != nullptr) {
        node.block->accept(*this);
        return;
    } else if (node.break_stmt != nullptr) {
        node.break_stmt->accept(*this);
        return;
    } else if (node.continue_stmt != nullptr) {
        node.continue_stmt->accept(*this);
        return;
    } else if (node.exp != nullptr) {//
        node.exp->accept(*this);
        return;
    } else if (node.if_else_stmt != nullptr) {
        node.if_else_stmt->accept(*this);
        return;
    } else if (node.if_stmt != nullptr) {
        node.if_stmt->accept(*this);
        return;
    } else if (node.return_null_stmt != nullptr) {
        node.return_null_stmt->accept(*this);
        return;
    } else if (node.return_stmt != nullptr) {
        node.return_stmt->accept(*this);
        return;
    } else if (node.while_stmt != nullptr) {
        node.while_stmt->accept(*this);
        return;
    }
    ERROR("error tree_block_item");
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
    // ysx todo
    ERROR("error call");
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
    node.l_or_exp->accept(this *);
}

void SYSYBuilder::visit(tree_array_ident &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_call &node) {
    // ysx todo
}

void SYSYBuilder::visit(tree_func_paramlist &node) {
    //ysx todo
    ERROR("visit tree_var_def_list error");
};