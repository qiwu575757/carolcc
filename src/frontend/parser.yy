%{
    #include "visitor/syntax_tree_builder.h"
    #include "helpers/type_helper.h"
    #include "syntax_tree.h"
    #include "string.h"
    #include "error.h"
    #include "runtime.h"
    #include <vector>
    #include <typeinfo>
    extern tree_comp_unit *root;
    bool DEBUG = false;

    extern int yyline;
    extern int yylex();
    void yyerror(char*s)
    {
        extern char *yytext;	// defined and maintained in lex
        int len=strlen(yytext);
        int i;
        char buf[512]={0};
        for (i=0;i<len;++i)
        {
            //TODO: may have bugs
            sprintf(buf,"%s%d ",buf,yytext[i]);
        }
        fprintf(stderr, "ERROR: text %s\n",yytext);
        fprintf(stderr, "ERROR: %s at symbol '%s' on line %d\n", s, buf, yyline);
        exit(1);
    }
    void insertVarible(std::string& type,std::string& id);
    void insertFunction(std::string& type,std::string& id);
%}

%union{
std::string            *string;
int                    token;
tree_comp_unit         *comp_unit;
tree_func_def          *func_def;
tree_block             *block;
tree_const_decl        *const_decl;
tree_basic_type        *basic_type;
tree_const_def_list    *const_def_list;
tree_const_init_val    *const_init_val;
tree_const_val_list    *const_init_val_list;
tree_const_exp_list    *const_exp_list;
tree_const_exp         *const_exp;
tree_var_decl          *var_decl;
tree_arrray_def        *array_def;
tree_exp               *exp;
tree_init_val          *init_val;
tree_init_val_array    *init_val_array;
tree_init_val_arraylist *init_val_arraylist;
tree_func_fparams      *func_fparams;
tree_func_fparam       *func_fparam;
tree_func_fparamone    *func_fparamone;
tree_func_fparamarray  *func_fparamarray;
tree_decl              *decl;
tree_const_def         *const_def;
tree_var_def_list      *var_def_list;
tree_var_def           *var_def;
tree_block_item_list   *block_item_list;
tree_block_item        *block_item;
tree_stmt              *stmt;
tree_assign_stmt       *assign_stmt;
tree_if_stmt           *if_stmt;
tree_if_else_stmt      *if_else_stmt;
tree_while_stmt        *while_stmt;
tree_break_stmt        *break_stmt;
tree_continue_stmt     *continue_stmt;
tree_cond              *cond;
tree_return_stmt       *return_stmt;
tree_return_null_stmt  *return_null_stmt;
tree_l_val             *l_val;
tree_array_ident       *array_ident;
tree_number            *number;
tree_primary_exp       *primary_exp;
tree_unary_exp         *unary_exp;
tree_func_call         *func_call;
tree_funcr_paramlist   *funcr_paramlist;
tree_mul_exp           *mul_exp;
tree_add_exp           *add_exp;
tree_rel_exp           *rel_exp;
tree_eq_exp            *eq_exp;
tree_l_and_exp         *l_and_exp;
tree_l_or_exp          *l_or_exp;

}

%token <string> TIDENTIFIER TINTEGER TFLOATNUM
%token TSEMICOLOM ";"
%token TCOMMA   ","
%token TMAIN    "main"
%token TINT     "int"
%token TFLOAT   "float"
%token TVOID    "void"
%token TRETURN  "return"
%token TCONST   "const"
%token TBREAK   "break"
%token TCONTINUE"continue"
%token TIF      "if"
%token TELSE    "else"
%token TWHILE   "while"
%token TLBPAREN "{"
%token TRBPAREN "}"
%token TMINUS   "-"
%token TNOT     "!"
%token TWAVE    "~"
%token TPLUS    "+"
%token TDIV     "/"
%token TMOD     "%"
%token TMULTI   "*"
%token TLPAREN  "("
%token TRPAREN  ")"
%token TLMPAREN "["
%token TRMPAREN "]"
%token TL       "<"
%token TLE      "<="
%token TG       ">"
%token TGE      ">="
%token TNE      "!="
%token TE       "=="
%token TLOGAND  "&&"
%token TLOGOR   "||"
%token TASSIGN  "="


%type <comp_unit>        CompUnit
%type <decl>             Decl
%type <const_decl>       ConstDecl
%type <basic_type>       BType
%type <const_exp>        ConstExp
%type <const_def>        ConstDef
%type <const_def_list>   ConstDefList
%type <const_exp_list>   ConstExpArrayList
%type <const_init_val>   ConstInitVal
%type <const_init_val_list>   ConstInitVallist
%type <var_decl>         VarDecl
%type <var_def>          VarDef
%type <var_def_list>     VarDefList
%type <array_def>        ArrayDef
%type <init_val>         InitVal
%type <init_val_array>   InitValArray
%type <init_val_arraylist>InitValArrayList
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
%type <number>           Number
%type <exp>              Exp
%type <l_val>            LVal
%type <array_ident>      ArrayIdent
%type <primary_exp>      PrimaryExp
%type <unary_exp>        UnaryExp
%type <func_call>        FuncCall
%type <funcr_paramlist>  FuncRParamList
%type <mul_exp>          MulExp
%type <add_exp>          AddExp
%type <rel_exp>          RelExp
%type <eq_exp>           EqExp
%type <l_and_exp>        LAndExp
%type <l_or_exp>         LOrExp


%start CompUnit
%expect 1
%%
//jyx
CompUnit
    : FuncDef
        {
            root = new tree_comp_unit();
		    if (DEBUG)
                printf("CompUnit\n");
		    root->functions.push_back(std::shared_ptr<tree_func_def>($1));
        }
    | CompUnit FuncDef
        {
		    if (DEBUG)
                printf("CompUnit\n");
		    root->functions.push_back(std::shared_ptr<tree_func_def>($2));
        }
    | Decl
        {
            root = new tree_comp_unit();
		    if (DEBUG)
                printf("CompUnit\n");
            root->definitions.push_back(std::shared_ptr<tree_decl>($1));
        }
    | CompUnit Decl
        {
		    if (DEBUG)
                printf("CompUnit\n");
            root->definitions.push_back(std::shared_ptr<tree_decl>($2));
        }
    ;

Decl
    : ConstDecl
        {
		    if (DEBUG)
                printf("Decl\n");
            $$ = new tree_decl();
            $$->const_decl=std::shared_ptr<tree_const_decl>($1);
        }
    | VarDecl
        {
		    if (DEBUG)
                printf("Decl\n");
            $$ = new tree_decl();
            $$->var_decl=std::shared_ptr<tree_var_decl>($1);
        }
    ;

ConstDecl
    : "const" BType ConstDefList ";"
        {
		    if (DEBUG)
                printf("ConstDecl\n");
            $$ = new tree_const_decl();
            $$->b_type=std::shared_ptr<tree_basic_type>($2);
            $$->const_def_list=std::shared_ptr<tree_const_def_list>($3);
        }
    ;

ConstDefList
    : ConstDef
        {
		    if (DEBUG)
                printf("ConstDefList\n");
            $$ = new tree_const_def_list();
            $$->const_defs.push_back(std::shared_ptr<tree_const_def>($1));
        }
    | ConstDefList "," ConstDef
        {
		    if (DEBUG)
                printf("ConstDefList\n");
            $1->const_defs.push_back(std::shared_ptr<tree_const_def>($3));
            $$ = $1;
        }
    ;

BType
    : "int"
        {
		    if (DEBUG)
                printf("BType\n");
            $$ = new tree_basic_type();
            $$->type=type_helper::INT;
        };
    | "float"
        {
		    if (DEBUG)
                printf("BType\n");
            $$ = new tree_basic_type();
            $$->type=type_helper::FLOAT;
        }
    | "void"
        {
		    if (DEBUG)
                printf("BType\n");
            $$ = new tree_basic_type();
            $$->type=type_helper::VOID;
        }
    ;

ConstDef
    : TIDENTIFIER ConstExpArrayList "=" ConstInitVal
        {
		    if (DEBUG)
                printf("ConstDef\n");
            $$ = new tree_const_def();
            $$->id=*$1;
            $$->const_exp_list=std::shared_ptr<tree_const_exp_list>($2);
            $$->const_init_val=std::shared_ptr<tree_const_init_val>($4);
        }
    | TIDENTIFIER "=" ConstInitVal
        {
		    if (DEBUG)
                printf("ConstDef\n");
            $$ = new tree_const_def();
            $$->id=*$1;
            $$->const_init_val=std::shared_ptr<tree_const_init_val>($3);
        }
    ;

ConstExpArrayList
    : "[" ConstExp "]"
        {
		    if (DEBUG)
                printf("ConstExpArrayList\n");
            $$ = new tree_const_exp_list();
            $$->const_exp.push_back(std::shared_ptr<tree_const_exp>($2));
        }
    | ConstExpArrayList "[" ConstExp "]"
        {
		    if (DEBUG)
                printf("ConstExpArrayList\n");
            $1->const_exp.push_back(std::shared_ptr<tree_const_exp>($3));
            $$ = $1;
        }
    ;

ConstInitVal
    : ConstExp
        {
		    if (DEBUG)
                printf("ConstInitVal\n");
            $$ = new tree_const_init_val();
            $$->const_exp= std::shared_ptr<tree_const_exp>($1) ;
        }
    | "{"  "}"
        {
		    if (DEBUG)
                printf("ConstInitVal\n");
            $$ = new tree_const_init_val();
        }
    | "{" ConstInitVallist "}"
        {
		    if (DEBUG)
                printf("ConstInitVal\n");
            $$ = new tree_const_init_val();
            $$->const_val_list = std::shared_ptr<tree_const_val_list>($2) ;
        }
    ;

ConstInitVallist
    : ConstInitVal
        {
		    if (DEBUG)
                printf("ConstInitVallist\n");
            $$ = new tree_const_val_list();
            $$->const_init_vals.push_back(std::shared_ptr<tree_const_init_val>($1));
        }
    | ConstInitVallist "," ConstInitVal
        {
		    if (DEBUG)
                printf("ConstInitVallist\n");
            $1->const_init_vals.push_back(std::shared_ptr<tree_const_init_val>($3));
            $$ = $1;
        }
    ;

ConstExp
    : AddExp
        {
		    if (DEBUG)
                printf("ConstExp\n");
            $$ = new tree_const_exp();
            $$->add_exp = std::shared_ptr<tree_add_exp>($1);
        }
    ;

VarDecl
    : BType VarDefList ";"
        {
		    if (DEBUG)
                printf("VarDecl\n");
            $$ = new tree_var_decl();
            $$->b_type=std::shared_ptr<tree_basic_type>($1);
            $$->var_def_list=std::shared_ptr<tree_var_def_list>($2);
        }
    ;

VarDefList
    : VarDef
        {
		    if (DEBUG)
                printf("VarDefList\n");
            $$ = new tree_var_def_list();
            $$->var_defs.push_back(std::shared_ptr<tree_var_def>($1));
        }
    |  VarDefList "," VarDef
        {
		    if (DEBUG)
                printf("VarDefList\n");
            $1->var_defs.push_back(std::shared_ptr<tree_var_def>($3));
            $$ = $1;
        }
    ;

VarDef
    : TIDENTIFIER
        {
		    if (DEBUG)
                printf("VarDef\n");
            $$ = new tree_var_def();
            $$->id = *$1;
        }
    | TIDENTIFIER "=" InitVal
        {
		    if (DEBUG)
                printf("VarDef\n");
            $$ = new tree_var_def();
            $$->id = *$1;
            $$->init_val = std::shared_ptr<tree_init_val>($3);
        };
    | TIDENTIFIER ArrayDef
        {
		    if (DEBUG)
                printf("VarDef\n");
            $$ = new tree_var_def();
            $$->id = *$1;
            $$->array_def = std::shared_ptr<tree_arrray_def>($2);
        }
    | TIDENTIFIER ArrayDef "=" InitValArray
        {
		    if (DEBUG)
                printf("VarDef\n");
            $$ = new tree_var_def();
            $$->id = *$1;
            $$->array_def = std::shared_ptr<tree_arrray_def>($2);
            $$->init_val_array = std::shared_ptr<tree_init_val_array>($4);
        }
    ;

ArrayDef
    : "[" ConstExp "]"
        {
		    if (DEBUG)
                printf("ArrayDef\n");
            $$ = new tree_arrray_def();
            $$->const_exps.push_back(std::shared_ptr<tree_const_exp>($2));
        }
    | ArrayDef "[" ConstExp "]"
        {
		    if (DEBUG)
                printf("ArrayDef\n");
            $$->const_exps.push_back(std::shared_ptr<tree_const_exp>($3));
            $$ = $1;
        }
    ;

/* wq */
InitVal
    : Exp
        {
		    if (DEBUG)
                printf("InitVal\n");
            $$ = new tree_init_val();
            $$->exp=std::shared_ptr<tree_exp>($1);
        }
    ;

InitValArray
    : "{" "}"
        {
		    if (DEBUG)
                printf("InitValArray\n");
            $$ = new tree_init_val_array();
        }
    | "{" InitValArrayList "}"
        {
		    if (DEBUG)
                printf("InitValArray\n");
            $$ = new tree_init_val_array();
            $$->init_val_arraylist = std::shared_ptr<tree_init_val_arraylist>($2);
        }
    ;

// InitValArrayList是InitVal(AddExp)和InitValArray的任意组合
InitValArrayList
    : InitValArray
        {
		    if (DEBUG)
                printf("InitValArrayList\n");
            $$ = new tree_init_val_arraylist();
            $$->initvalarrays.push_back(std::shared_ptr<tree_init_val_array>($1));
        }
    | InitValArrayList "," InitValArray
        {
		    if (DEBUG)
                printf("InitValArrayList\n");
            $1->initvalarrays.push_back(std::shared_ptr<tree_init_val_array>($3));
            $$ = $1;
        }
    | InitVal
        {
		    if (DEBUG)
                printf("InitValArrayList\n");
            $$ = new tree_init_val_arraylist();
            $$->initvals.push_back(std::shared_ptr<tree_init_val>($1));
        }
    | InitValArrayList "," InitVal
        {
		    if (DEBUG)
                printf("InitValArrayList\n");
            $1->initvals.push_back(std::shared_ptr<tree_init_val>($3));
            $$ = $1;
        }
    ;

/* 函数相关 */
FuncDef
    : BType TIDENTIFIER "("")" Block
        {
		    if (DEBUG)
                printf("FuncDef\n");
            $$ = new tree_func_def();
            $$->type = std::shared_ptr<tree_basic_type>($1);
            $$->id = *$2;
            $$->block.push_back(std::shared_ptr<tree_block>($5));
        }
    | BType TIDENTIFIER "(" FuncFParams ")" Block
        {
		    if (DEBUG)
                printf("FuncDef\n");
            $$ = new tree_func_def();
            $$->type = std::shared_ptr<tree_basic_type>($1);
            $$->id = *$2;
            $$->funcfparams = std::shared_ptr<tree_func_fparams>($4);

            $$->block.push_back(std::shared_ptr<tree_block>($6));
        }
    ;

FuncFParams
    : FuncFParam
        {
		    if (DEBUG)
                printf("FuncFParams\n");
            $$ = new tree_func_fparams();
            $$->funcfparamlist.push_back(std::shared_ptr<tree_func_fparam>($1));
        }
    | FuncFParams "," FuncFParam
        {
		    if (DEBUG)
                printf("FuncFParams\n");
            $1->funcfparamlist.push_back(std::shared_ptr<tree_func_fparam>($3));
            $$ = $1;
        }
    ;

FuncFParam
    : FuncFParamOne
        {
		    if (DEBUG)
                printf("FuncFParam\n");
            $$ = new tree_func_fparam();
            $$->funcfparamone = std::shared_ptr<tree_func_fparamone>($1);
        }
    | FuncFParamArray
        {
		    if (DEBUG)
                printf("FuncFParam\n");
            $$ = new tree_func_fparam();
            $$->funcfparamarray = std::shared_ptr<tree_func_fparamarray>($1);
        }
    ;

FuncFParamOne
    : BType TIDENTIFIER
        {
		    if (DEBUG)
                printf("FuncFParamOne\n");
            $$ = new tree_func_fparamone();
            $$->b_type = std::shared_ptr<tree_basic_type>($1);
            $$->id = *$2;
        }
    ;

FuncFParamArray
    : BType TIDENTIFIER "[" "]"
        {
		    if (DEBUG)
                printf("FuncFParamArray\n");
            $$ = new tree_func_fparamarray();
            $$->b_type = std::shared_ptr<tree_basic_type>($1);
            $$->id = *$2;
        }
    | FuncFParamArray "[" Exp "]"
        {
		    if (DEBUG)
                printf("FuncFParamArray\n");
            $1->exps.push_back(std::shared_ptr<tree_exp>($3));
            $$ = $1;
        }
    ;

Block
    : "{" "}"
        {
		    if (DEBUG)
                printf("Block\n");
            $$ = new tree_block();
        }
    | "{" BlockItemList "}"
        {
		    if (DEBUG)
                printf("Block\n");
            $$ = new tree_block();
            $$->block_item_list=std::shared_ptr<tree_block_item_list>($2);
        }
    ;

BlockItemList
    : BlockItem
        {
		    if (DEBUG)
                printf("BlockItemList\n");
            $$ = new tree_block_item_list();
            $$->block_items.push_back(std::shared_ptr<tree_block_item>($1));
        }
    |  BlockItemList BlockItem
        {
		    if (DEBUG)
                printf("BlockItemList\n");
            $1->block_items.push_back(std::shared_ptr<tree_block_item>($2));
            $$=$1;
        }
    ;

BlockItem
    : Decl
        {
		    if (DEBUG)
                printf("BlockItem\n");
            $$ = new tree_block_item();
            $$->decl=std::shared_ptr<tree_decl>($1);
        }
    | Stmt
        {
		    if (DEBUG)
                printf("BlockItem\n");
            $$ = new tree_block_item();
            $$->stmt=std::shared_ptr<tree_stmt>($1);
        }
    ;

/* 语句相关 */
Stmt
    : LVal "=" Exp ";"
        /* assign statement */
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            auto a_stmt = new tree_assign_stmt();
            a_stmt->l_val=std::shared_ptr<tree_l_val>($1);
            a_stmt->exp=std::shared_ptr<tree_exp>($3);
            $$->assigm_stmt=std::shared_ptr<tree_assign_stmt>(a_stmt) ;
        }
    | ";"
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
        }
    | Exp ";"
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            $$->exp=std::shared_ptr<tree_exp>($1) ;
        }
    | Block
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            $$->block=std::shared_ptr<tree_block>($1) ;
        }
    /* if statement */
    | "if" "(" Cond ")" Stmt
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            auto if_stmt = new tree_if_stmt();
            if_stmt->cond = std::shared_ptr<tree_cond>($3);
            if_stmt->stmt = std::shared_ptr<tree_stmt>($5);
            $$->if_stmt = std::shared_ptr<tree_if_stmt>(if_stmt) ;
        }
    | "if" "(" Cond ")" Stmt "else" Stmt
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            auto if_else_stmt = new tree_if_else_stmt();
            if_else_stmt->cond = std::shared_ptr<tree_cond>($3);
            if_else_stmt->stmt1 = std::shared_ptr<tree_stmt>($5);
            if_else_stmt->stmt2 = std::shared_ptr<tree_stmt>($7);
            $$->if_else_stmt = std::shared_ptr<tree_if_else_stmt>(if_else_stmt) ;

        }
    /* while statement */
    | "while" "(" Cond ")" Stmt
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            auto while_stmt = new tree_while_stmt();
            while_stmt->cond = std::shared_ptr<tree_cond>($3);
            while_stmt->stmt = std::shared_ptr<tree_stmt>($5);
            $$->while_stmt = std::shared_ptr<tree_while_stmt>(while_stmt) ;
        }
    | "continue" ";"
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            auto continue_stmt = new tree_continue_stmt();
            $$->continue_stmt = std::shared_ptr<tree_continue_stmt>(continue_stmt) ;
        }
    | "break" ";"
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            auto break_stmt = new tree_break_stmt();
            $$->break_stmt = std::shared_ptr<tree_break_stmt>(break_stmt) ;
        }
    /* return statement */
    | "return" ";"
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            auto a_stmt = new tree_return_null_stmt();
            $$->return_null_stmt=std::shared_ptr<tree_return_null_stmt>(a_stmt) ;
        }
    | "return" Exp ";"
        {
		    if (DEBUG)
                printf("Stmt\n");
            $$ = new tree_stmt();
            auto a_stmt = new tree_return_stmt();
            a_stmt->exp=std::shared_ptr<tree_exp>($2);
            $$->return_stmt=std::shared_ptr<tree_return_stmt>(a_stmt) ;
        }
    ;

/* 表达式相关 */
Exp
    : AddExp
        {
		    if (DEBUG)
                printf("Exp\n");
            $$ = new tree_exp();
            $$->add_exp = std::shared_ptr<tree_add_exp>($1);
        }
    ;

Cond
    : LOrExp
        {
		    if (DEBUG)
                printf("Cond\n");
            $$ = new tree_cond();
            $$->l_or_exp = std::shared_ptr<tree_l_or_exp>($1);
        }
    ;

LVal
    : TIDENTIFIER
        {
		    if (DEBUG)
                printf("LVal\n");
            $$ = new tree_l_val();
            $$->id = *$1;
        }
    | ArrayIdent
        {
		    if (DEBUG)
                printf("LVal\n");
            $$ = new tree_l_val();
            $$->array_ident = std::shared_ptr<tree_array_ident>($1);
        }
    ;

ArrayIdent
    : TIDENTIFIER "[" Exp "]"
        {
		    if (DEBUG)
                printf("ArrayIdent\n");
            $$ = new tree_array_ident();
            $$->id = *$1;
            $$->exps.push_back(std::shared_ptr<tree_exp>($3));
        }
    | ArrayIdent "[" Exp "]"
        {
		    if (DEBUG)
                printf("ArrayIdent\n");
            $1->exps.push_back(std::shared_ptr<tree_exp>($3));
            $$ = $1;
        }
    ;

Number
    : TINTEGER
        {
		    if (DEBUG)
                printf("Number INTEGER\n");
            $$ = new tree_number();
            $$->int_value = atoi($1->c_str());
        }
    | TFLOATNUM
        {
		    if (DEBUG)
                printf("Number TFLOATNUM\n");
            $$ = new tree_number();
            $$->float_value = (float)atof($1->c_str());
        }
    ;

PrimaryExp
    : "(" Exp ")"
        {
		    if (DEBUG)
                printf("PrimaryExp\n");
            $$ = new tree_primary_exp();
            $$->exp = std::shared_ptr<tree_exp>($2);
        }
    | LVal
        {
		    if (DEBUG)
                printf("PrimaryExp\n");
            $$ = new tree_primary_exp();
            $$->l_val = std::shared_ptr<tree_l_val>($1);
        }
    | Number
        {
		    if (DEBUG)
                printf("PrimaryExp\n");
            $$ = new tree_primary_exp();
            $$->number = std::shared_ptr<tree_number>($1);
        }
    ;

UnaryExp
    : PrimaryExp
        {
		    if (DEBUG)
                printf("UnaryExp\n");
            $$ = new tree_unary_exp();
            $$->primary_exp = std::shared_ptr<tree_primary_exp>($1);
        }
    | "+" UnaryExp
        {
		    if (DEBUG)
                printf("UnaryExp\n");
            $$ = new tree_unary_exp();
            $$->unary_exp=std::shared_ptr<tree_unary_exp>($2);
            $$->oprt="+";
        }
    | "-" UnaryExp
        {
		    if (DEBUG)
                printf("UnaryExp\n");
            $$ = new tree_unary_exp();
            $$->unary_exp=std::shared_ptr<tree_unary_exp>($2);
            $$->oprt="-";
        }
    | "!" UnaryExp
        {
		    if (DEBUG)
                printf("UnaryExp\n");
            $$ = new tree_unary_exp();
            $$->unary_exp=std::shared_ptr<tree_unary_exp>($2);
            $$->oprt="!";
        };
    /* FUNCTION CALL */
    | FuncCall
        {
		    if (DEBUG)
                printf("FuncCall\n");
            $$ = new tree_unary_exp();
            $$->func_call = std::shared_ptr<tree_func_call>($1);
        }
    ;

FuncCall
    : TIDENTIFIER "(" ")"
        {
		    if (DEBUG)
                printf("FuncCall\n");
            $$ = new tree_func_call();
            $$->id = *$1;
        }
    | TIDENTIFIER "(" FuncRParamList ")"
        {
		    if (DEBUG)
                printf("FuncCall\n");
            $$ = new tree_func_call();
            $$->id = *$1;
            $$->funcr_paramlist = std::shared_ptr<tree_funcr_paramlist>($3);
        }
    ;

FuncRParamList
    : Exp
        {
		    if (DEBUG)
                printf("FuncRParamList\n");
            $$ = new tree_funcr_paramlist();
            $$->exps.push_back(std::shared_ptr<tree_exp>($1));
        }
    | FuncRParamList "," Exp
        {
		    if (DEBUG)
                printf("FuncRParamList\n");
            $1->exps.push_back(std::shared_ptr<tree_exp>($3));
            $$ = $1;
        }
    ;
/* jyx */


MulExp
    : UnaryExp
        {
		    if (DEBUG)
                printf("MulExp\n");
            $$ = new tree_mul_exp();
            $$->unary_exp=std::shared_ptr<tree_unary_exp>($1);
        }
    | MulExp "*" UnaryExp
        {
		    if (DEBUG)
                printf("MulExp\n");
            $$ = new tree_mul_exp();
            $$->mul_exp=std::shared_ptr<tree_mul_exp>($1);
            $$->unary_exp=std::shared_ptr<tree_unary_exp>($3);
            $$->oprt="*";
        }
    | MulExp "/" UnaryExp
        {
		    if (DEBUG)
                printf("MulExp\n");
            $$ = new tree_mul_exp();
            $$->mul_exp=std::shared_ptr<tree_mul_exp>($1);
            $$->unary_exp=std::shared_ptr<tree_unary_exp>($3);
            $$->oprt="/";
        }
    | MulExp "%" UnaryExp
        {
		    if (DEBUG)
                printf("MulExp\n");
            $$ = new tree_mul_exp();
            $$->mul_exp=std::shared_ptr<tree_mul_exp>($1);
            $$->unary_exp=std::shared_ptr<tree_unary_exp>($3);
            $$->oprt="%";
        }
    ;
AddExp
    : MulExp
        {
		    if (DEBUG)
                printf("AddExp\n");
            $$ = new tree_add_exp();
            $$->mul_exp=std::shared_ptr<tree_mul_exp>($1);
        }
    | AddExp "+" MulExp
        {
		    if (DEBUG)
                printf("AddExp\n");
            $$ = new tree_add_exp();
            $$->add_exp=std::shared_ptr<tree_add_exp>($1);
            $$->oprt="+";
            $$->mul_exp=std::shared_ptr<tree_mul_exp>($3);
        }
    | AddExp "-" MulExp
        {
		    if (DEBUG)
                printf("AddExp\n");
            $$ = new tree_add_exp();
            $$->add_exp=std::shared_ptr<tree_add_exp>($1);
            $$->oprt="-";
            $$->mul_exp=std::shared_ptr<tree_mul_exp>($3);
        }
    ;
RelExp
    : AddExp
        {
		    if (DEBUG)
                printf("RelExp\n");
            $$ = new tree_rel_exp();
            $$->add_exp=std::shared_ptr<tree_add_exp>($1);
        }
    | RelExp "<" AddExp
        {
		    if (DEBUG)
                printf("RelExp\n");
            $$ = new tree_rel_exp();
            $$->rel_exp=std::shared_ptr<tree_rel_exp>($1);
            $$->oprt="<";
            $$->add_exp=std::shared_ptr<tree_add_exp>($3);
        }
    | RelExp ">" AddExp
        {
		    if (DEBUG)
                printf("RelExp\n");
            $$ = new tree_rel_exp();
            $$->rel_exp=std::shared_ptr<tree_rel_exp>($1);
            $$->oprt=">";
            $$->add_exp=std::shared_ptr<tree_add_exp>($3);
        }
    | RelExp "<=" AddExp
        {
		    if (DEBUG)
                printf("RelExp\n");
            $$ = new tree_rel_exp();
            $$->rel_exp=std::shared_ptr<tree_rel_exp>($1);
            $$->oprt="<=";
            $$->add_exp=std::shared_ptr<tree_add_exp>($3);
        }
    | RelExp ">=" AddExp
        {
		    if (DEBUG)
                printf("RelExp\n");
            $$ = new tree_rel_exp();
            $$->rel_exp=std::shared_ptr<tree_rel_exp>($1);
            $$->oprt=">=";
            $$->add_exp=std::shared_ptr<tree_add_exp>($3);
        }
    ;
EqExp
    : RelExp
        {
		    if (DEBUG)
                printf("EqExp\n");
            $$ = new tree_eq_exp();
            $$->rel_exp=std::shared_ptr<tree_rel_exp>($1);
        }
    | EqExp "==" RelExp
        {
		    if (DEBUG)
                printf("EqExp\n");
            $$ = new tree_eq_exp();
            $$->eq_exp=std::shared_ptr<tree_eq_exp>($1);
            $$->oprt="==";
            $$->rel_exp=std::shared_ptr<tree_rel_exp>($3);
        }
    | EqExp "!=" RelExp
        {
		    if (DEBUG)
                printf("EqExp\n");
            $$ = new tree_eq_exp();
            $$->eq_exp=std::shared_ptr<tree_eq_exp>($1);
            $$->oprt="!=";
            $$->rel_exp=std::shared_ptr<tree_rel_exp>($3);
        }
    ;
LAndExp
    : EqExp
        {
		    if (DEBUG)
                printf("LAndExp\n");
            $$ = new tree_l_and_exp();
            $$->eq_exp=std::shared_ptr<tree_eq_exp>($1);
        }
    | LAndExp "&&" EqExp
        {
		    if (DEBUG)
                printf("LAndExp\n");
            $$ = new tree_l_and_exp();
            $$->l_and_exp=std::shared_ptr<tree_l_and_exp>($1);
            $$->eq_exp=std::shared_ptr<tree_eq_exp>($3);
        }
    ;
LOrExp
    : LAndExp
        {
		    if (DEBUG)
                printf("LOrExp\n");
            $$ = new tree_l_or_exp();
            $$->l_and_exp=std::shared_ptr<tree_l_and_exp>($1);
        }
    | LOrExp "||" LAndExp
        {
		    if (DEBUG)
                printf("LOrExp\n");
            $$ = new tree_l_or_exp();
            $$->l_or_exp=std::shared_ptr<tree_l_or_exp>($1);
            $$->l_and_exp=std::shared_ptr<tree_l_and_exp>($3);
        }
    ;
%%
void insertVarible(std::string& type,std::string& id){
    VaribleTable.insert(std::make_pair<std::string, VaribleInfo>(std::string(id),VaribleInfo(type)));
}
void insertFunction(std::string& type,std::string& id){
    FunctionTable.insert(
        std::make_pair<std::string, FunctionInfo>
            (std::string(id),FunctionInfo(type,VaribleTable)));
    VaribleTable.clear();
}
