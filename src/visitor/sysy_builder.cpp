#include "sysy_builder.h"
#include "ir/function.h"
#include "ir/type.h"
#include "ir/value.h"
#include "utils.h"

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
float G_tmp_float = 0.0;

// 标记当前为计算数值状态 并且使用到 G_tmp_int/G_tmp_float
bool G_tmp_computing = false;
// 标记当前为全局初始化状态 说明全局变量必须计算初值
bool G_in_global_init = false;


void SYSYBuilder::visit(tree_comp_unit &node) {
    INFO("line:%d", node._line_no);
    MyAssert("no function defined", node.functions.size() != 0);
    INFO(" comp_unit functions %d definitions %d 42", node.functions.size(), node.definitions.size());
    // 这种写法会允许 后置全局变量定义出现，这是因为语法设计的问题
    for (auto defs: node.definitions) {
        G_in_global_init = true;
        defs->accept(*this);
        G_in_global_init = false;
    }
    for (auto func: node.functions) {
        INFO("visiting %s", func->id.c_str());
        func->accept(*this);
    }
}

void SYSYBuilder::visit(tree_func_def &node) {
    INFO("line:%d", node._line_no);
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
    if (node.funcfparams != nullptr) {
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
                Type *array_type;
                if (param->funcfparamarray->b_type->type == type_helper::INT) {
                    array_type = Type::getInt32Ty();
                } else if (param->funcfparamarray->b_type->type == type_helper::FLOAT) {
                    array_type = Type::getFloatTy();
                }
                for (auto array_param: param->funcfparamarray->exps) {
                    auto bnd = static_cast<ConstantInt *>(G_tmp_val)->getValue();
                    array_type = ArrayType::get(array_type, bnd);
                }
                param_types.push_back(PointerType::get(array_type));
            }
        }
    }

    fun_type = FunctionType::get(ret_type, param_types);
    auto fun = Function::create(fun_type, node.id, module.get());
    scope.push(node.id, fun);

    G_cur_fun = fun;

    auto funBB = BasicBlock::create("", fun);// entry
    builder->SetEntryBlock(funBB);
    builder->SetInstrInsertPoint(funBB);
    scope.enter();

    G_pre_enter_scope = true;

    std::vector<Argument *> args;
    for (auto arg = fun->arg_begin(); arg != fun->arg_end(); arg++) {
        args.push_back(*arg);
    }

    int i = 0;
    if (node.funcfparams != nullptr) {
        for (auto param: node.funcfparams->funcfparamlist) {
            if (param->funcfparamarray != nullptr) {
                auto array_alloc = builder->createAllocaAtEntry(args[i]->getType());
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
            } else {// 单个
                auto alloc = builder->createAllocaAtEntry(TyInt32);
                builder->createStore(args[i], alloc);
                scope.push(param->funcfparamone->id, alloc);
            }
            i++;
        }
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

    if (node.block_item_list != nullptr) {
        for (auto &blockitem: (node.block_item_list)->block_items) {
            if (blockitem->decl != nullptr) {
                if (builder->GetInsertBaseBlockList() == nullptr) {
                    auto allocaBB = BasicBlock::create("", G_cur_fun);
                    builder->SetInstrInsertPoint(allocaBB);
                    blockitem->accept(*this);
                } else {
                    auto allocaBB = BasicBlock::create("");
                    builder->SetInstrInsertPoint(allocaBB);
                    builder->pushBaseBlock(allocaBB);
                    blockitem->accept(*this);
                }
            } else if (blockitem->stmt != nullptr) {
                if (blockitem->stmt->assigm_stmt || blockitem->stmt->break_stmt ||
                    blockitem->stmt->continue_stmt || blockitem->stmt->exp ||
                    blockitem->stmt->return_stmt || blockitem->stmt->return_null_stmt) {
                    if (builder->GetInsertBaseBlockList() == nullptr) {
                        INFO("block 2");
                        if (builder->GetInsertBasicBlock()->getBaseFather() != nullptr) {
                            INFO("block 3");
                            auto baseBB = BasicBlock::create("", G_cur_fun);
                            builder->SetInstrInsertPoint(baseBB);
                        }
                        blockitem->accept(*this);
                    } else {
                        INFO("block 4");
                        auto baseBB = BasicBlock::create("");
                        builder->SetInstrInsertPoint(baseBB);
                        builder->pushBaseBlock(baseBB);
                        blockitem->accept(*this);
                    }
                } else {
                    blockitem->accept(*this);
                }
            }
        }
    }
    if (need_exit_scope) {
        scope.exit();
    }
}

void SYSYBuilder::visit(tree_const_decl &node) {
    INFO("line:%d", node._line_no);
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
    ERROR("error call");
}

void SYSYBuilder::visit(tree_const_def_list &node) {
    INFO("line:%d", node._line_no);
    for (auto &def: node.const_defs) {
        def->accept(*this);
    }
}

void SYSYBuilder::visit(tree_const_init_val &node) {
    INFO("line:%d", node._line_no);
    if (node.const_exp != nullptr) {
        node.const_exp->accept(*this);
    } else {
        auto cur_bnd = node.bounds[0];
        auto dim_length = 1;
        for (int i = 1; i < node.bounds.size(); i++) {
            dim_length *= node.bounds[i];
        }
        std::vector<Value *> init_list;
        if (node.const_val_list != nullptr) {

            for (auto &init_val: node.const_val_list->const_init_vals) {
                if (init_val->const_exp != nullptr) {
                    init_val->accept(*this);
                    init_list.push_back(G_tmp_val);
                } else {
                    int pos = init_list.size();
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
    INFO("line:%d", node._line_no);
    INFO(" tree_const_exp 264 %d", G_in_global_init);
    if (G_tmp_type == Type::getFloatTy()) {
        G_tmp_computing = true;
    } else if (G_tmp_type == Type::getInt32Ty()) {
        G_tmp_computing = true;
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
    G_tmp_computing = false;
}

void SYSYBuilder::visit(tree_var_decl &node) {
    INFO("line:%d", node._line_no);
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
    node.var_def_list->accept(*this);
    G_tmp_type = nullptr;
}

void SYSYBuilder::visit(tree_exp &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    if (node.add_exp != nullptr) {
        node.add_exp->accept(*this);
    } else {
        ERROR("tree_exp has null add exp");
    }
}

void SYSYBuilder::visit(tree_init_val &node) {
    INFO("line:%d", node._line_no);
    /*dyb TODO*/
    if (node.exp != nullptr) {// 表达式
        if (G_in_global_init) {
            G_tmp_computing = true;
            if (G_tmp_type->isIntegerTy()) {
                INFO("global exp 313");
                node.exp->accept(*this);
                G_tmp_val = (Value *) (CONST_INT(G_tmp_int));
            } else if (G_tmp_type->isFloatTy()) {
                node.exp->accept(*this);
                G_tmp_val = (Value *) (CONST_FLOAT(G_tmp_float));
            } else {
                ERROR("ERROR TYPE");
            }
            G_tmp_computing = false;
        } else {
            node.exp->accept(*this);
        }

        if (G_in_global_init) {
        }
    } else {//array
        auto cur_bnd = node.bounds[0];
        auto dim_length = 1;
        for (int i = 1; i < node.bounds.size(); ++i) {
            dim_length *= node.bounds[i];
        }
        std::vector<Value *> init_list;
        if (node.init_val_list != nullptr) {
            for (auto &init_val: node.init_val_list->init_vals) {
                if (init_val->exp != nullptr) {
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

void SYSYBuilder::visit(tree_init_val_list &node) {
    INFO("line:%d", node._line_no);
    /*dyb TODO 把处理后的数据放在node.instant_init 里方便父节点处理*/
    ERROR("visit tree_init_val_list error");
}

void SYSYBuilder::visit(tree_func_fparams &node) {
    INFO("line:%d", node._line_no);
    /*TODO*/
    ERROR("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_func_fparam &node) {
    INFO("line:%d", node._line_no);
    /*TODO*/
    ERROR("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_func_fparamone &node) {
    INFO("line:%d", node._line_no);
    /*TODO*/
    ERROR("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_func_fparamarray &node) {
    INFO("line:%d", node._line_no);
    /*TODO*/
    ERROR("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_decl &node) {
    INFO("line:%d", node._line_no);
    if (node.const_decl != nullptr) {
        node.const_decl->accept(*this);
    } else if (node.var_decl != nullptr) {
        node.var_decl->accept(*this);
    } else {
        ERROR("tree_decl build error");
    }
}

void SYSYBuilder::visit(tree_const_def &node) {
    INFO("line:%d", node._line_no);
    INFO(" const_def 370");
    if (node.const_init_val == nullptr) {
        ERROR("const_def need init value");
    } else {
        if (node.const_exp_list == nullptr) {// 非数组情况
                                             //            G_tmp_computing = true;
            INFO(" const_def 374");
            node.const_init_val->accept(*this);
            //            G_tmp_computing = false;
            scope.push(node.id, G_tmp_val);
        } else if (node.const_exp_list != nullptr) {// arrray
            Type *array_element_ty = G_tmp_type;
            std::vector<int32_t> bounds;
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
                auto array_alloca = builder->createAllocaAtEntry(array_element_ty);
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
    INFO("line:%d", node._line_no);
    for (auto &def: node.var_defs) {
        def->accept(*this);
    }
}

void SYSYBuilder::visit(tree_var_def &node) {
    INFO("line:%d", node._line_no);
    INFO("node name %s", node.id.c_str());
    if (node.array_def != nullptr) {// 数组
        std::vector<int32_t> array_bounds;
        // dim v
        for (auto exp: node.array_def->const_exps) {
            exp->accept(*this);
            int dim_v = static_cast<ConstantInt *>(G_tmp_val)->getValue();
            array_bounds.push_back(dim_v);
        }
        Type *ty_array = Type::getInt32Ty();
        for (int i = array_bounds.size() - 1; i >= 0; i--) {
            ty_array = ArrayType::get(ty_array, array_bounds[i]);
        }
        if (scope.in_global_scope()) {
            if (node.init_val != nullptr) {
                node.init_val->bounds.assign(array_bounds.begin(), array_bounds.end());
                G_in_global_init = true;
                node.init_val->accept(*this);
                G_in_global_init = false;
                auto initializer = ConstantArray::turn(array_bounds, G_array_init);
                auto var = GlobalVariable::create(node.id, module.get(), ty_array, false, initializer);
                scope.push(node.id, var);
            } else {
                auto var = GlobalVariable::create(node.id, module.get(), ty_array, false, nullptr);
                scope.push(node.id, var);
            }
        } else {// local array
            auto array_alloca = builder->createAllocaAtEntry(ty_array);
            scope.push(node.id, array_alloca);
            if (node.init_val != nullptr) {
                array_alloca->setInit();
                node.init_val->bounds.assign(array_bounds.begin(), array_bounds.end());
                node.init_val->accept(*this);

                auto ptr = builder->createGEP(array_alloca, {CONST_INT(0)});
                for (int i = 1; i < node.init_val->bounds.size(); i++) {
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

    } else {// 不是数组
        if (scope.in_global_scope()) {
            if (node.init_val != nullptr) {// 变量赋值
                G_in_global_init = true;
                node.init_val->accept(*this);
                G_in_global_init = false;
                auto initializer = static_cast<Constant *>(G_tmp_val);
                auto var = GlobalVariable::create(node.id, &*module, TyInt32, false, initializer);
                scope.push(node.id, var);
            } else {
                auto var = GlobalVariable::create(node.id, &*module, TyInt32, false, CONST_INT(0));
                scope.push(node.id, var);
            }
        } else {
            auto val_alloc = builder->createAllocaAtEntry(TyInt32);
            scope.push(node.id, val_alloc);
            if (node.init_val != nullptr) {// 变量赋值
                node.init_val->accept(*this);
                builder->createStore(G_tmp_val, val_alloc);
            }
        }
    }
}

void SYSYBuilder::visit(tree_block_item_list &node) {
    INFO("line:%d", node._line_no);
    /*dyb TODO*/
    ERROR("error tree_block_item_list");
}

void SYSYBuilder::visit(tree_block_item &node) {
    INFO("line:%d", node._line_no);
    /*dyb TODO*/
    if (node.decl != nullptr) {
        if (node.decl->const_decl != nullptr) {
            node.decl->const_decl->accept(*this);
            return;
        } else if (node.decl->var_decl != nullptr) {
            node.decl->var_decl->accept(*this);
            return;
        }
    } else if (node.stmt != nullptr) {
        node.stmt->accept(*this);
        return;
    }
    ERROR("error tree_block_item");
}

void SYSYBuilder::visit(tree_stmt &node) {
    INFO("line:%d", node._line_no);
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
        INFO("return x");
        node.return_stmt->accept(*this);
        return;
    } else if (node.while_stmt != nullptr) {
        node.while_stmt->accept(*this);
        return;
    } else {// NULL stmt
        WARNNING("null tree_block_item");
        return;
    }
    ERROR("error tree_block_item");
}

void SYSYBuilder::visit(tree_assign_stmt &node) {
    INFO("line:%d", node._line_no);
    node.l_val->accept(*this);
    auto lval = G_tmp_val;
    node.exp->accept(*this);
    auto rval = G_tmp_val;
    builder->createStore(rval, lval);
}

void SYSYBuilder::visit(tree_return_stmt &node) {
    INFO("line:%d", node._line_no);
    if (node.exp == nullptr) {
        INFO("return void");
        builder->createVoidRet();
    } else {
        G_tmp_type = G_cur_fun->getResultType();
        node.exp->accept(*this);
        G_tmp_type = nullptr;

        builder->createRet(G_tmp_val);
    }
}


void SYSYBuilder::visit(tree_return_null_stmt &node) {
    INFO("return null stmt line:%d", node._line_no);
    builder->createVoidRet();
}

void SYSYBuilder::visit(tree_l_val &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    std::string &name = node.id.empty() ? node.array_ident->id : node.id;
    INFO("l_val %s", name.c_str());
    auto var = scope.find(name);
    if (var->getType()->isIntegerTy() || var->getType()->isFloatTy()) {
        G_tmp_val = var;
    } else {
        auto is_int = var->getType()->getPointerElementType()->isIntegerTy();
        auto is_float = var->getType()->getPointerElementType()->isFloatTy();
        auto is_ptr = var->getType()->getPointerElementType()->isPointerTy();

        if (node.array_ident == nullptr) {// like f(a)
            if (is_int || is_float) {
                G_tmp_val = var;
            } else if (is_ptr) {
                G_tmp_val = builder->createLoad(var);
            } else {
                G_tmp_val = builder->createGEP(var, {CONST_INT(0)});
            }
        } else {
            node.array_ident->accept(*this);
        }
    }
}

void SYSYBuilder::visit(tree_number &node) {
    INFO("line:%d", node._line_no);


    /*wuqi TODO*/
    if (G_tmp_computing) {
        if (!node.is_int) {
            INFO("visiting float %f", node.float_value);
            G_tmp_float = node.float_value;
        } else if (node.is_int) {
            INFO("visiting int %d", node.int_value);
            G_tmp_int = node.int_value;
        } else {
            ERROR("ERROR type in tree number");
        }
    } else {
        if (!node.is_int) {
            INFO("visiting float %f", node.float_value);
            G_tmp_val = CONST_FLOAT(node.float_value);
        } else if (node.is_int) {
            INFO("visiting int %d", node.int_value);
            G_tmp_val = CONST_INT(node.int_value);
        } else {
            ERROR("ERROR type in tree number");
        }
    }
}

void SYSYBuilder::visit(tree_primary_exp &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    if (G_tmp_computing) {
        INFO("tree_primary_exp 657");

        if (node.exp != nullptr) {
            node.exp->accept(*this);
        } else if (node.l_val != nullptr) {
            node.l_val->accept(*this);
            if (G_tmp_type->isFloatTy()) {
                G_tmp_float = static_cast<ConstantFloat *>(G_tmp_val)->getValue();
            } else if (G_tmp_type->isInt32()) {
                G_tmp_int = static_cast<ConstantInt *>(G_tmp_val)->getValue();
            } else {
                ERROR("");
            }
        } else if (node.number != nullptr) {
            INFO("exp");
            node.number->accept(*this);
            INFO("exp");
        }
    } else {
        INFO(" tree_primary_exp 676");

        if (node.exp != nullptr) {

            node.exp->accept(*this);
        } else if (node.l_val != nullptr) {
            if (G_require_address) {//若要求传参需要地址
                G_require_address = false;
                node.l_val->accept(*this);
                while (!G_tmp_val->getType()->getPointerElementType()->isFloatTy() && !G_tmp_val->getType()->getPointerElementType()->isIntegerTy()) {
                    G_tmp_val = builder->createGEP(G_tmp_val, {CONST_INT(0)});
                }
            } else {//保证返回值 G_tmp_val 是 float/int num
                node.l_val->accept(*this);
                if (G_tmp_val->getType()->isIntegerTy() || G_tmp_val->getType()->isFloatTy()) {
                    return;
                } else {
                    G_tmp_val = builder->createLoad(G_tmp_val);
                }
            }
        } else if (node.number != nullptr) {
            INFO(" tree_primary_exp 700");
            node.number->accept(*this);
        } else {
            ERROR("exp");
        }
    }
}

void SYSYBuilder::visit(tree_unary_exp &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    TRACE("unary exp The line num is %d", node._line_no);
    if (G_tmp_computing) {
        INFO(" unary exp 698");
        if (node.primary_exp != nullptr) {
            node.primary_exp->accept(*this);
        } else if (node.unary_exp != nullptr) {
            node.unary_exp->accept(*this);
        } else {
            ERROR("Function call in visit tree_unary_exp");
        }

        if (node.oprt == "-") {
            if (G_tmp_type->isFloatTy()) {
                G_tmp_float = -G_tmp_float;
            } else if (G_tmp_type->isInt32()) {
                G_tmp_int = -G_tmp_int;
            } else {
                ERROR("");
            }
        } else if (node.oprt == "!") {// for operation !
            ERROR("not oprt in visit tree_unary_exp");
        }
    } else {
        INFO(" unary exp 719");

        if (node.primary_exp != nullptr) {
            node.primary_exp->accept(*this);
        } else if (node.unary_exp != nullptr) {
            node.unary_exp->accept(*this);
        } else {
            node.func_call->accept(*this);
        }

        if (node.oprt == "-") {
            // auto const0 = (G_tmp_type->getTypeID() == Type::FloatTyID)
            //             ? CONST_FLOAT(0.0) : CONST_INT(0);
            if (G_tmp_val->getType()->isFloatTy()) {
                auto const0 = CONST_FLOAT(0.0);
                G_tmp_val = builder->createSub(const0, G_tmp_val);
            } else if (G_tmp_val->getType()->isIntegerTy()) {
                auto const0 = CONST_INT(0);
                G_tmp_val = builder->createSub(const0, G_tmp_val);
            } else {
                ERROR("");
            }
        } else if (node.oprt == "!") {
            if (G_tmp_val->getType()->isIntegerTy()) {
                auto const0 = CONST_INT(0);
                G_tmp_val = builder->createEQ(new IntegerType(1), G_tmp_val, const0);
            } else {
                ERROR("Not operation for flaot tyoe in visit tree_unary_exp");
            }
        }
    }
}

void SYSYBuilder::visit(tree_mul_exp &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    if (node.mul_exp == nullptr) {
        INFO("mul 763 %d", G_tmp_computing);
        node.unary_exp->accept(*this);
    } else {
        if (G_tmp_computing) {
            node.mul_exp->accept(*this);
            auto l_val = (G_tmp_type->getTypeID() == Type::FloatTyID) ? G_tmp_float : G_tmp_int;
            node.unary_exp->accept(*this);
            auto r_val = (G_tmp_type->getTypeID() == Type::FloatTyID) ? G_tmp_float : G_tmp_int;

            if (node.oprt == "*") {
                if (G_tmp_type->isFloatTy()) {
                    G_tmp_float = l_val * r_val;
                } else if (G_tmp_type->isInt32()) {
                    G_tmp_int = l_val * r_val;
                } else {
                    ERROR("");
                }
            } else if (node.oprt == "/") {
                if (G_tmp_type->isFloatTy()) {
                    G_tmp_float = l_val / r_val;
                } else if (G_tmp_type->isInt32()) {
                    G_tmp_int = l_val / r_val;
                } else {
                    ERROR("");
                }
            } else if (node.oprt == "%") {
                if (G_tmp_type->getTypeID() == Type::IntegerTyID) {
                    G_tmp_int = l_val / r_val;
                } else {
                    ERROR("Invalid oprt Mod for float type");
                }
            }
        } else {
            node.mul_exp->accept(*this);
            auto l_val = G_tmp_val;
            node.unary_exp->accept(*this);
            auto r_val = G_tmp_val;

            if (l_val->getType()->isBool()) {
                l_val = builder->creatZExtInst(TyInt32, l_val);
            }
            if (r_val->getType()->isBool()) {
                r_val = builder->creatZExtInst(TyInt32, r_val);
            }

            if (node.oprt == "*") {
                G_tmp_val = builder->createMul(l_val, r_val);
            } else if (node.oprt == "/") {
                G_tmp_val = builder->createDiv(l_val, r_val);
            } else if (node.oprt == "%") {
                G_tmp_val = builder->createRem(l_val, r_val);
            }
        }
    }
}

void SYSYBuilder::visit(tree_add_exp &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    if (node.add_exp == nullptr) {
        INFO("add 823 %d", G_tmp_computing);
        node.mul_exp->accept(*this);
    } else {
        if (G_tmp_computing) {
            INFO("add exp 813");
            node.add_exp->accept(*this);
            auto l_val = (G_tmp_type->getTypeID() == Type::FloatTyID) ? G_tmp_float : G_tmp_int;
            node.mul_exp->accept(*this);
            auto r_val = (G_tmp_type->getTypeID() == Type::FloatTyID) ? G_tmp_float : G_tmp_int;

            if (node.oprt == "+") {
                if (G_tmp_type->isFloatTy()) {
                    G_tmp_float = l_val + r_val;
                } else if (G_tmp_type->isInt32()) {
                    G_tmp_int = l_val + r_val;
                } else {
                    ERROR("");
                }
            } else if (node.oprt == "-") {
                if (G_tmp_type->isFloatTy()) {
                    G_tmp_float = l_val - r_val;
                } else if (G_tmp_type->isInt32()) {
                    G_tmp_int = l_val - r_val;
                } else {
                    ERROR("");
                }
            }
        } else {
            INFO("add exp 837");
            node.add_exp->accept(*this);
            auto l_val = G_tmp_val;
            node.mul_exp->accept(*this);
            auto r_val = G_tmp_val;

            if (l_val->getType()->isBool()) {
                l_val = builder->creatZExtInst(TyInt32, l_val);
            }
            if (r_val->getType()->isBool()) {
                r_val = builder->creatZExtInst(TyInt32, r_val);
            }

            if (node.oprt == "+") {
                G_tmp_val = builder->createAdd(l_val, r_val);
            } else if (node.oprt == "-") {
                G_tmp_val = builder->createSub(l_val, r_val);
            }
        }
    }
}

void SYSYBuilder::visit(tree_rel_exp &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    if (node.rel_exp == nullptr) {
        node.add_exp->accept(*this);
    } else {
        node.rel_exp->accept(*this);
        auto l_val = G_tmp_val;
        node.add_exp->accept(*this);
        auto r_val = G_tmp_val;

        if (l_val->getType()->isBool()) {
            l_val = builder->creatZExtInst(TyInt32, l_val);
        }
        if (r_val->getType()->isBool()) {
            r_val = builder->creatZExtInst(TyInt32, r_val);
        }

        if (node.oprt == "<=") {
            G_tmp_val = builder->createLE(new IntegerType(1), l_val, r_val);
        } else if (node.oprt == "<") {
            G_tmp_val = builder->createLT(new IntegerType(1), l_val, r_val);
        } else if (node.oprt == ">") {
            G_tmp_val = builder->createGT(new IntegerType(1), l_val, r_val);
        } else if (node.oprt == ">=") {
            G_tmp_val = builder->createGE(new IntegerType(1), l_val, r_val);
        }
    }
}

void SYSYBuilder::visit(tree_eq_exp &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    if (node.eq_exp == nullptr) {
        node.rel_exp->accept(*this);
    } else {
        node.eq_exp->accept(*this);
        auto l_val = G_tmp_val;
        node.rel_exp->accept(*this);
        auto r_val = G_tmp_val;

        if (l_val->getType()->isBool()) {
            l_val = builder->creatZExtInst(TyInt32, l_val);
        }
        if (r_val->getType()->isBool()) {
            r_val = builder->creatZExtInst(TyInt32, r_val);
        }

        if (node.oprt == "==") {
            G_tmp_val = builder->createEQ(new IntegerType(1), l_val, r_val);
        } else if (node.oprt == "!=") {
            G_tmp_val = builder->createNEQ(new IntegerType(1), l_val, r_val);
        }
    }
}

void SYSYBuilder::visit(tree_l_and_exp &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    if (node.l_and_exp == nullptr) {
        node.eq_exp->accept(*this);
    } else {
        node.l_and_exp->accept(*this);
        auto l_val = G_tmp_val;
        node.eq_exp->accept(*this);
        auto r_val = G_tmp_val;

        if (l_val->getType()->isBool()) {
            l_val = builder->creatZExtInst(TyInt32, l_val);
        }
        if (r_val->getType()->isBool()) {
            r_val = builder->creatZExtInst(TyInt32, r_val);
        }

        G_tmp_val = builder->createAnd(l_val, r_val);
    }
}

void SYSYBuilder::visit(tree_l_or_exp &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    if (node.l_or_exp == nullptr) {
        node.l_and_exp->accept(*this);
    } else {
        node.l_or_exp->accept(*this);
        auto l_val = G_tmp_val;
        node.l_and_exp->accept(*this);
        auto r_val = G_tmp_val;

        if (l_val->getType()->isBool()) {
            l_val = builder->creatZExtInst(TyInt32, l_val);
        }
        if (r_val->getType()->isBool()) {
            r_val = builder->creatZExtInst(TyInt32, r_val);
        }

        G_tmp_val = builder->createOr(l_val, r_val);
    }
    /*保证 G_tmp_val 为 bool 型*/
    if (G_tmp_val->getType()->isInt32()) {
        G_tmp_val = builder->createNEQ(new IntegerType(1), G_tmp_val, CONST_INT(0));
    } else if (G_tmp_val->getType()->isFloatTy()) {
        G_tmp_val = builder->createNEQ(new IntegerType(1), G_tmp_val, CONST_FLOAT(0.0));
    }
}

void SYSYBuilder::visit(tree_const_val_list &node) {
    INFO("line:%d", node._line_no);
    ERROR("error call");
}

void SYSYBuilder::visit(tree_const_exp_list &node) {
    ERROR("error call");
}

void SYSYBuilder::visit(tree_arrray_def &node) {
    ERROR("error call");
}

void SYSYBuilder::visit(tree_if_stmt &node) {
    INFO("line:%d", node._line_no);
    IfBlock *if_block;
    auto father_block = builder->GetBaseBlockFatherBlock();
    if (father_block == nullptr) {
        if_block = IfBlock::create("", G_cur_fun);
    } else {
        if_block = IfBlock::create("");
        builder->pushBaseBlock(if_block);
    }
    builder->SetBaseBlockFatherBlock(if_block);

    auto cond_basic_block = BasicBlock::create("");
    builder->SetInstrInsertPoint(cond_basic_block);
    if_block->addCondBaseBlock(cond_basic_block);
    node.cond->accept(*this);
    if (node.stmt->block || node.stmt->if_stmt || node.stmt->if_stmt || node.stmt->if_else_stmt || node.stmt->while_stmt) {
        auto father_list = builder->GetInsertBaseBlockList();
        builder->SetBasicBlockInsertPoint(if_block->getIfBodyBaseBlockList());
        node.stmt->accept(*this);
        builder->SetBasicBlockInsertPoint(father_list);
    } else if (node.stmt->break_stmt || node.stmt->continue_stmt || node.stmt->return_stmt ||
               node.stmt->assigm_stmt || node.stmt->exp) {
        auto then_block = BasicBlock::create("");
        builder->SetInstrInsertPoint(then_block);
        if_block->addIfBodyBaseBlock(then_block);
        node.stmt->accept(*this);
    }

    builder->SetBaseBlockFatherBlock(father_block);
}

void SYSYBuilder::visit(tree_if_else_stmt &node) {
    INFO("line:%d", node._line_no);
    IfBlock *if_block;
    auto father_block = builder->GetBaseBlockFatherBlock();
    if (father_block == nullptr) {
        if_block = IfBlock::create("", G_cur_fun);
    } else {
        if_block = IfBlock::create("");
        builder->pushBaseBlock(if_block);
    }
    builder->SetBaseBlockFatherBlock(if_block);

    auto cond_basic_block = BasicBlock::create("");
    builder->SetInstrInsertPoint(cond_basic_block);
    if_block->addCondBaseBlock(cond_basic_block);
    node.cond->accept(*this);
    if (node.then_stmt->block || node.then_stmt->if_stmt || node.then_stmt->if_stmt || node.then_stmt->if_else_stmt || node.then_stmt->while_stmt) {
        auto father_list = builder->GetInsertBaseBlockList();
        builder->SetBasicBlockInsertPoint(if_block->getIfBodyBaseBlockList());
        node.then_stmt->accept(*this);
        builder->SetBasicBlockInsertPoint(father_list);
    } else if (node.then_stmt->break_stmt || node.then_stmt->continue_stmt || node.then_stmt->return_stmt ||
               node.then_stmt->assigm_stmt || node.then_stmt->exp) {
        auto then_block = BasicBlock::create("");
        builder->SetInstrInsertPoint(then_block);
        if_block->addIfBodyBaseBlock(then_block);
        node.then_stmt->accept(*this);
    }
    if (node.else_stmt->block || node.else_stmt->if_stmt || node.else_stmt->if_stmt || node.else_stmt->if_else_stmt || node.else_stmt->while_stmt) {
        auto father_list = builder->GetInsertBaseBlockList();
        builder->SetBasicBlockInsertPoint(if_block->getIfBodyBaseBlockList());
        node.else_stmt->accept(*this);
        builder->SetBasicBlockInsertPoint(father_list);
    } else if (node.else_stmt->break_stmt || node.else_stmt->continue_stmt || node.else_stmt->return_stmt ||
               node.else_stmt->assigm_stmt || node.else_stmt->exp) {
        auto else_stmt = BasicBlock::create("");
        builder->SetInstrInsertPoint(else_stmt);
        if_block->addIfBodyBaseBlock(else_stmt);
        node.else_stmt->accept(*this);
    }

    builder->SetBaseBlockFatherBlock(father_block);
}

void SYSYBuilder::visit(tree_while_stmt &node) {
    INFO("line:%d", node._line_no);
    WhileBlock *while_block;
    auto father_block = builder->GetBaseBlockFatherBlock();
    if (father_block == nullptr) {
        while_block = WhileBlock::create("", G_cur_fun);
    } else {
        while_block = WhileBlock::create("");
        builder->pushBaseBlock(while_block);
    }
    builder->SetBaseBlockFatherBlock(while_block);

    auto cond_basic_block = BasicBlock::create("");
    builder->SetInstrInsertPoint(cond_basic_block);
    while_block->addCondBaseBlock(cond_basic_block);
    node.cond->accept(*this);
    if (node.stmt->block || node.stmt->if_stmt || node.stmt->if_stmt || node.stmt->if_else_stmt || node.stmt->while_stmt) {
        auto father_list = builder->GetInsertBaseBlockList();
        builder->SetBasicBlockInsertPoint(while_block->getBodyBaseBlockList());
        node.stmt->accept(*this);
        builder->SetBasicBlockInsertPoint(father_list);
    } else if (node.stmt->break_stmt || node.stmt->continue_stmt || node.stmt->return_stmt ||
               node.stmt->assigm_stmt || node.stmt->exp) {
        auto then_block = BasicBlock::create("");
        builder->SetInstrInsertPoint(then_block);
        while_block->addBodyBaseBlock(then_block);
        node.stmt->accept(*this);
    }

    builder->SetBaseBlockFatherBlock(father_block);
}

void SYSYBuilder::visit(tree_break_stmt &node) {
    INFO("line:%d", node._line_no);
    builder->createBreak();
}

void SYSYBuilder::visit(tree_continue_stmt &node) {
    INFO("line:%d", node._line_no);
    builder->createContinue();
}

void SYSYBuilder::visit(tree_cond &node) {
    INFO("line:%d", node._line_no);
    /*TODO*/
    node.l_or_exp->accept(*this);
}

void SYSYBuilder::visit(tree_array_ident &node) {
    INFO("line:%d", node._line_no);
    /*wuqi TODO*/
    auto var = scope.find(node.id);
    auto is_int = var->getType()->getPointerElementType()->isIntegerTy();
    auto is_float = var->getType()->getPointerElementType()->isFloatTy();
    auto is_ptr = var->getType()->getPointerElementType()->isPointerTy();

    Value *tmp_ptr;
    if (is_int || is_float) {
        tmp_ptr = var;
        for (auto exp: node.exps) {
            exp->accept(*this);
            tmp_ptr = builder->createGEP(tmp_ptr, {G_tmp_val});
        }
    } else if (is_ptr) {
        std::vector<Value *> array_params;
        scope.find(node.id, array_params);
        tmp_ptr = builder->createLoad(var);
        int i = 0;
        for (auto exp: node.exps) {
            exp->accept(*this);
            auto val = G_tmp_val;
            for (int j = i + 1; j < array_params.size(); j++) {// 获取偏移地址
                val = builder->createMul(val, array_params[j]);
            }
            tmp_ptr = builder->createGEP(tmp_ptr, {val});
        }
    } else {
        tmp_ptr = var;
        for (auto exp: node.exps) {
            exp->accept(*this);
            tmp_ptr = builder->createGEP(tmp_ptr, {G_tmp_val});
        }
    }
    G_tmp_val = tmp_ptr;
}

void SYSYBuilder::visit(tree_func_call &node) {
    INFO("line:%d", node._line_no);
    auto func = module->getFunction(node.id);
    MyAssert("func not found", func != nullptr);
    std::vector<Value *> args;
    if (node.func_param_list != nullptr) {
        for (int i = 0; i < node.func_param_list->exps.size(); i++) {
            auto arg = node.func_param_list->exps[i];
            auto arg_type = func->getFunctionType()->getArgType(i);
            if (arg_type->isFloatTy() || arg_type->isIntegerTy()) {
                G_require_address = false;
            } else {
                G_require_address = true;
            }
            arg->accept(*this);
            G_require_address = false;
            args.push_back(G_tmp_val);
        }
    }
    builder->createCall(func, args);
}

void SYSYBuilder::visit(tree_func_paramlist &node) {
    INFO("line:%d", node._line_no);
    ERROR("visit tree_var_def_list error");
}
void SYSYBuilder::visit(syntax_tree_node &node) {
    INFO("line:%d", node._line_no);
    ERROR("error call");
};
