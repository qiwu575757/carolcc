#include "sysy_builder.h"
#include "ir/function.h"
#include "ir/value.h"
#include "ir/type.h"
#include "utils.h"
#include <iostream>

#define CONST_INT(num) ConstantInt::get(num, &*module)
#define CONST_FLOAT(num) ConstantFloat::get(num, &*module)

auto TyVoid = Type::getVoidTy(); // 改
auto TyFloat = Type::getFloatTy(); // 改
auto TyInt32 = Type::getInt32Ty(); // 改
auto TyInt32Ptr = Type::getInt32PtrTy(); // 改
auto TyFloatPtr = Type::getFloatPtrTy(); // 改
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
int G_tmp_float = 0.0;
// 标记当前为计算数值状态 并且使用到 G_tmp_int/G_tmp_float
bool G_tmp_value_using = false;
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
%type <block>            Block
%type <block_item>       BlockItem
%type <block_item_list>  BlockItemList
%type <stmt>             Stmt
%type <cond>             Cond
*/

/* wuqi TODO
%type <number>           Number //
%type <exp>              Exp
%type <l_val>            LVal
%type <array_ident>      ArrayIdent
%type <primary_exp>      PrimaryExp
%type <unary_exp>        UnaryExp
%type <mul_exp>          MulExp
%type <add_exp>          AddExp
%type <rel_exp>          RelExp
%type <eq_exp>           EqExp
%type <l_and_exp>        LAndExp
%type <l_or_exp>         LOrExp
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
    }
    else if (node.type->type == type_helper::FLOAT) {
        ret_type = TyFloat;
    }
    else {
        ret_type = TyVoid;
    }

    for (auto &param : node.funcfparams->funcfparamlist) {
        if(param->funcfparamone!=nullptr){
            if (param->funcfparamone->b_type->type == type_helper::INT) {
                param_types.push_back(TyInt32);
            }
            else if (param->funcfparamone->b_type->type == type_helper::FLOAT) {
                param_types.push_back(TyFloat);
            }
            else{
                param_types.push_back(TyVoid);
            }
        }
        else if(param->funcfparamarray!=nullptr){
            if (param->funcfparamarray->b_type->type == type_helper::INT) {
                param_types.push_back(TyInt32Ptr);
            }
            else if (param->funcfparamarray->b_type->type == type_helper::FLOAT) {
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
    for (auto param : node.funcfparams->funcfparamlist) {
        if (param->funcfparamarray!=nullptr) {
            if(param->funcfparamarray->b_type->type == type_helper::INT){
                auto array_alloc = builder->createAlloca(TyInt32Ptr);
                builder->createStore(static_cast<Value *>(args[i]), array_alloc);
                std::vector<Value *> array_params;
                array_params.push_back(new ConstantInt(TyInt32,0));
                for (auto array_param : param->funcfparamarray->exps) {
                    array_param->accept(*this);
                    array_params.push_back(G_tmp_val);
                }
                scope.push(param->funcfparamarray->id, array_alloc);
                scope.push(param->funcfparamarray->id, array_alloc, array_params);
                args[i]->setArrayBound(array_params);
            }
            else{
                auto array_alloc = builder->createAlloca(TyFloatPtr);
                builder->createStore(static_cast<Value *>(args[i]), array_alloc);
                std::vector<Value *> array_params;
                array_params.push_back(new ConstantInt(TyFloat,0));
                for (auto array_param : param->funcfparamarray->exps) {
                    array_param->accept(*this);
                    array_params.push_back(G_tmp_val);
                }
                scope.push(param->funcfparamarray->id, array_alloc);
                scope.push(param->funcfparamarray->id, array_alloc, array_params);
                args[i]->setArrayBound(array_params);
            }
        } else { // 单个
            if(param->funcfparamone->b_type->type == type_helper::INT){
                auto alloc = builder->createAlloca(TyInt32);
                builder->createStore(args[i], alloc);
                scope.push(param->funcfparamone->id, alloc);
            }
            else{
                auto alloc = builder->createAlloca(TyFloat);
                builder->createStore(args[i], alloc);
                scope.push(param->funcfparamone->id, alloc);
            }
        }
        i++;
    }
    for(auto b : node.block){
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

    for (auto &blockitem : node.block_item_list->block_items) {
        if (blockitem->decl!=nullptr) {
            if (builder->GetInsertBaseBlockList().size()==0) {
                auto allocaBB = BasicBlock::create(&*module, "", G_cur_fun);
                builder->SetInstrInsertPoint(allocaBB);
                blockitem->accept(*this);
            } else {
                auto allocaBB = BasicBlock::create(&*module, "");
                builder->SetInstrInsertPoint(allocaBB);

                builder->pushBaseBlock(allocaBB);

                blockitem->accept(*this);
            }



        } else if (blockitem->Stmt) {
        if (blockitem->Stmt->AssignStmt || blockitem->Stmt->BreakStmt ||
            blockitem->Stmt->ContinueStmt || blockitem->Stmt->Exp ||
            blockitem->Stmt->ReturnStmt) {
            if (base_layer.size() == 0) {
            if (builder->GetInsertBlock()->getBaseFather() != nullptr) {
                auto baseBB = BasicBlock::create(&*module, "", cur_fun);
                builder->SetInsertPoint(baseBB);
            }
            blockitem->accept(*this);
            } else {
            auto baseBB = BasicBlock::create(&*module, "");
            builder->SetInsertPoint(baseBB);
            auto &cur_base_list = getCurBaseList(BL_types[BL_types.size() - 1]);
            cur_base_list.push_back(baseBB);
            auto cur_base = base_layer[base_layer.size() - 1];
            baseBB->setBaseFather(cur_base);
            blockitem->accept(*this);
            }
        } else {
            blockitem->accept(*this);
        }
        }
        // if (builder->GetInsertBlock()->getTerminator() != nullptr)
        // break;
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
    /*dyb TODO*/
    if (node.exp != nullptr) {
        node.exp->accept(*this);
    }
}

void SYSYBuilder::visit(tree_init_val_array &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_init_val_arraylist &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_func_fparams &node) {
    /*TODO*/
    TRACE("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_func_fparam &node) {
    /*TODO*/
    TRACE("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_func_fparamone &node) {
    /*TODO*/
    TRACE("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_func_fparamarray &node) {
    /*TODO*/
    TRACE("visit tree_var_def_list error");
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

void SYSYBuilder::visit(tree_var_def_list &node) {
    //ysx todo
    TRACE("visit tree_var_def_list error");
}

void SYSYBuilder::visit(tree_var_def &node) {
    //ysx todo
    if (node.array_def != nullptr) {// 数组
        std::vector<int32_t> array_bounds;
        // dim v
        for(auto exp : node.array_def->const_exps){
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
                scope.push(node.id, (Value*)(&var));
            } else {
                auto var = GlobalVariable(node.id, &*module, TyInt32, false,new ConstantInt(TyInt32,0));
                scope.push(node.id, (Value*)(&var));
            }
        }
         else {
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
    /*wuqi TODO*/
    if (G_tmp_value_using) {
        if (G_tmp_type->getTypeID() == Type::FloatTyID) {
            G_tmp_float = node.float_value;
        } else {
            G_tmp_int = node.int_value;
        }
    } else {
        if (G_tmp_type->getTypeID() == Type::FloatTyID) {
            G_tmp_val = CONST_GLOAT(node.float_value);
        } else {
            G_tmp_val = CONST_INT(node.int_value);
        }
    }
}

void SYSYBuilder::visit(tree_primary_exp &node) {
    /*wuqi TODO*/

}

void SYSYBuilder::visit(tree_unary_exp &node) {
    /*TODO*/
}

void SYSYBuilder::visit(tree_mul_exp &node) {
    /*wuqi TODO*/
    if (node.mul_exp == nullptr) {
        node.unary_exp->accept(*this);
    } else {
        if (G_tmp_value_using) {
            node.mul_exp->accept(*this);
            auto l_val = (G_tmp_type->getTypeID() == Type::FloatTyID) ? G_tmp_float : G_tmp_int;
            node.unary_exp->accept(*this);
            auto r_val = (G_tmp_type->getTypeID() == Type::FloatTyID) ? G_tmp_float : G_tmp_int;

            if (node.oprt == "*") {
                if (G_tmp_type->getTypeID() == Type::FloatTyID) {
                    G_tmp_float = l_val * r_val;
                } else {
                    G_tmp_int = l_val * r_val;
                }
            } else if (node.oprt == "/") {
                if (G_tmp_type->getTypeID() == Type::FloatTyID) {
                    G_tmp_float = l_val / r_val;
                } else {
                    G_tmp_int = l_val / r_val;
                }
            } else if (node.oprt == "%") {
                if (G_tmp_type->getTypeID() == Type::IntegerTyID) {
                    G_tmp_int = l_val / r_val;
                } else {
                    ERROR("Invalid oprt % for float type");
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
                G_tmp_val = builder->createMul(l_val,r_val);
            } else if (node.oprt == "/") {
                G_tmp_val = builder->createDiv(l_val,r_val);
            } else if (node.oprt == "%") {
                G_tmp_val = builder->createRem(l_val,r_val);
            }
        }
    }
}

void SYSYBuilder::visit(tree_add_exp &node) {
    /*wuqi TODO*/
    if (node.add_exp == nullptr) {
        node.mul_exp->accept(*this);
    } else {
        if (G_tmp_value_using) {
            node.add_exp->accept(*this);
            auto l_val = (G_tmp_type->getTypeID() == Type::FloatTyID) ? G_tmp_float : G_tmp_int;
            node.mul_exp->accept(*this);
            auto r_val = (G_tmp_type->getTypeID() == Type::FloatTyID) ? G_tmp_float : G_tmp_int;

            if (node.oprt == "+") {
                if (G_tmp_type->getTypeID() == Type::FloatTyID) {
                    G_tmp_float = l_val + r_val;
                } else {
                    G_tmp_int = l_val + r_val;
                }
            } else if (node.oprt == "-") {
                if (G_tmp_type->getTypeID() == Type::FloatTyID) {
                    G_tmp_float = l_val - r_val;
                } else {
                    G_tmp_int = l_val - r_val;
                }
            }
        } else {
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
                G_tmp_val = builder->createAdd(l_val,r_val);
            } else if (node.oprt == "-") {
                G_tmp_val = builder->createSub(l_val,r_val);
            }
        }
    }
}

void SYSYBuilder::visit(tree_rel_exp &node) {
    /*TODO*/
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
            G_tmp_val = builder->createLE(new IntegerType(1),l_val,r_val);
        } else if (node.oprt == "<") {
            G_tmp_val = builder->createLT(new IntegerType(1),l_val,r_val);
        } else if (node.oprt == ">") {
            G_tmp_val = builder->createGT(new IntegerType(1),l_val,r_val);
        } else if (node.oprt == ">") {
            G_tmp_val = builder->createGE(new IntegerType(1),l_val,r_val);
        }
    }
}

void SYSYBuilder::visit(tree_eq_exp &node) {
    /*TODO*/
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
            G_tmp_val = builder->createEQ(new IntegerType(1),l_val,r_val);
        } else if (node.oprt == "!=") {
            G_tmp_val = builder->createNEQ(new IntegerType(1),l_val,r_val);
        }
    }
}

void SYSYBuilder::visit(tree_l_and_exp &node) {
    /*TODO*/
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

        G_tmp_val = builder->createAnd(l_val,r_val);
    }
}

void SYSYBuilder::visit(tree_l_or_exp &node) {
    /*TODO*/
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

        G_tmp_val = builder->createOr(l_val,r_val);
    }
    /*保证 G_tmp_val 为 bool 型*/
    if (G_tmp_val->getType()->isInt32()) {
        G_tmp_val = builder->createNEQ(new IntegerType(1),G_tmp_val,CONST_INT(0));
    }else if (G_tmp_val->getType()->isFloatTy()) {
        G_tmp_val = builder->createNEQ(new IntegerType(1),G_tmp_val,CONST_FLOAT(0.0));
    }
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
    TRACE("visit tree_var_def_list error");
};