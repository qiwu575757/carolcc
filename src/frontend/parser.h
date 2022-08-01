/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_HOME_QIWU_COMPLIERS_CAROLCC_SRC_FRONTEND_PARSER_H_INCLUDED
# define YY_YY_HOME_QIWU_COMPLIERS_CAROLCC_SRC_FRONTEND_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TIDENTIFIER = 258,             /* TIDENTIFIER  */
    TINTEGER = 259,                /* TINTEGER  */
    TFLOATNUM = 260,               /* TFLOATNUM  */
    TSEMICOLOM = 261,              /* ";"  */
    TCOMMA = 262,                  /* ","  */
    TINT = 263,                    /* "int"  */
    TFLOAT = 264,                  /* "float"  */
    TVOID = 265,                   /* "void"  */
    TRETURN = 266,                 /* "return"  */
    TCONST = 267,                  /* "const"  */
    TBREAK = 268,                  /* "break"  */
    TCONTINUE = 269,               /* "continue"  */
    TIF = 270,                     /* "if"  */
    TELSE = 271,                   /* "else"  */
    TWHILE = 272,                  /* "while"  */
    TLBPAREN = 273,                /* "{"  */
    TRBPAREN = 274,                /* "}"  */
    TMINUS = 275,                  /* "-"  */
    TNOT = 276,                    /* "!"  */
    TPLUS = 277,                   /* "+"  */
    TDIV = 278,                    /* "/"  */
    TMOD = 279,                    /* "%"  */
    TMULTI = 280,                  /* "*"  */
    TLPAREN = 281,                 /* "("  */
    TRPAREN = 282,                 /* ")"  */
    TLMPAREN = 283,                /* "["  */
    TRMPAREN = 284,                /* "]"  */
    TL = 285,                      /* "<"  */
    TLE = 286,                     /* "<="  */
    TG = 287,                      /* ">"  */
    TGE = 288,                     /* ">="  */
    TNE = 289,                     /* "!="  */
    TE = 290,                      /* "=="  */
    TLOGAND = 291,                 /* "&&"  */
    TLOGOR = 292,                  /* "||"  */
    TASSIGN = 293                  /* "="  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 34 "/home/yonchicy/compiler/carolcc/src/frontend/parser.yy"

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
tree_init_val_list     *init_val_list;
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
tree_func_paramlist   *func_param_list;
tree_mul_exp           *mul_exp;
tree_add_exp           *add_exp;
tree_rel_exp           *rel_exp;
tree_eq_exp            *eq_exp;
tree_l_and_exp         *l_and_exp;
tree_l_or_exp          *l_or_exp;


#line 156 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_QIWU_COMPLIERS_CAROLCC_SRC_FRONTEND_PARSER_H_INCLUDED  */
