/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:339  */

    #include "helpers/type_helper.h"
    #include "syntax_tree.h"
    #include "utils.h"
    #include "string.h"
    #include "error.h"
    #include "runtime.h"
    #include <vector>
    #include <typeinfo>
    extern tree_comp_unit *root;

    extern int yyline;
    extern int yylex();
    void yyerror(char*s)
    {
        extern char *yytext;	// defined and maintained in lex
        // int len=strlen(yytext);
        // int i;
        char buf[1024]={0};
        strcpy(buf,yytext);
        // for (i=0;i<len;++i)
        // {
        //     //TODO: may have bugs
        //     sprintf(buf,"%s%d ",buf,yytext[i]);

        // }
        // std::string txt = "ERROR: text :"+to
        fprintf(stderr, "ERROR: text %s\n",yytext);
        fprintf(stderr, "ERROR: %s at symbol '%s' on line %d\n", s, buf, yyline);
        exit(YYERROR);
    }
    void insertVarible(std::string& type,std::string& id);
    void insertFunction(std::string& type,std::string& id);

#line 101 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.h".  */
#ifndef YY_YY_ROOT_CAROLCC_SRC_FRONTEND_PARSER_H_INCLUDED
# define YY_YY_ROOT_CAROLCC_SRC_FRONTEND_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TIDENTIFIER = 258,
    TINTEGER = 259,
    TFLOATNUM = 260,
    TSEMICOLOM = 261,
    TCOMMA = 262,
    TINT = 263,
    TFLOAT = 264,
    TVOID = 265,
    TRETURN = 266,
    TCONST = 267,
    TBREAK = 268,
    TCONTINUE = 269,
    TIF = 270,
    TELSE = 271,
    TWHILE = 272,
    TLBPAREN = 273,
    TRBPAREN = 274,
    TMINUS = 275,
    TNOT = 276,
    TPLUS = 277,
    TDIV = 278,
    TMOD = 279,
    TMULTI = 280,
    TLPAREN = 281,
    TRPAREN = 282,
    TLMPAREN = 283,
    TRMPAREN = 284,
    TL = 285,
    TLE = 286,
    TG = 287,
    TGE = 288,
    TNE = 289,
    TE = 290,
    TLOGAND = 291,
    TLOGOR = 292,
    TASSIGN = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 36 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:355  */

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


#line 234 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ROOT_CAROLCC_SRC_FRONTEND_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 251 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   275

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  186

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   173,   173,   180,   185,   191,   198,   204,   213,   223,
     229,   237,   243,   249,   258,   266,   276,   282,   290,   296,
     301,   310,   316,   324,   333,   343,   349,   357,   363,   370,
     377,   388,   394,   403,   409,   413,   421,   426,   434,   442,
     455,   461,   469,   475,   484,   494,   501,   509,   514,   523,
     529,   537,   543,   553,   563,   568,   574,   581,   590,   602,
     611,   618,   626,   633,   645,   654,   663,   669,   678,   685,
     693,   700,   710,   716,   722,   731,   737,   744,   751,   759,
     768,   774,   784,   790,   800,   806,   814,   822,   832,   838,
     846,   856,   862,   870,   878,   886,   896,   902,   910,   920,
     926,   935,   941
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENTIFIER", "TINTEGER", "TFLOATNUM",
  "\";\"", "\",\"", "\"int\"", "\"float\"", "\"void\"", "\"return\"",
  "\"const\"", "\"break\"", "\"continue\"", "\"if\"", "\"else\"",
  "\"while\"", "\"{\"", "\"}\"", "\"-\"", "\"!\"", "\"+\"", "\"/\"",
  "\"%\"", "\"*\"", "\"(\"", "\")\"", "\"[\"", "\"]\"", "\"<\"", "\"<=\"",
  "\">\"", "\">=\"", "\"!=\"", "\"==\"", "\"&&\"", "\"||\"", "\"=\"",
  "$accept", "CompUnit", "Decl", "ConstDecl", "ConstDefList", "BType",
  "ConstDef", "ConstExpArrayList", "ConstInitVal", "ConstInitVallist",
  "ConstExp", "VarDecl", "VarDefList", "VarDef", "ArrayDef", "InitVal",
  "InitValList", "FuncDef", "FuncFParams", "FuncFParam", "FuncFParamOne",
  "FuncFParamArray", "Block", "BlockItemList", "BlockItem", "Stmt", "Exp",
  "Cond", "LVal", "ArrayIdent", "Number", "PrimaryExp", "UnaryExp",
  "FuncCall", "FuncParamList", "MulExp", "AddExp", "RelExp", "EqExp",
  "LAndExp", "LOrExp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293
};
# endif

#define YYPACT_NINF -160

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-160)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      89,  -160,  -160,  -160,    18,   125,  -160,  -160,    17,  -160,
    -160,    56,  -160,  -160,  -160,     9,    33,  -160,    14,    44,
    -160,    75,   249,   215,    39,  -160,    90,   249,   222,    66,
    -160,    56,    -1,    97,     3,  -160,  -160,    83,    81,  -160,
    -160,   249,   249,   249,   249,    87,  -160,    92,  -160,  -160,
    -160,  -160,    47,    99,    70,  -160,  -160,    99,   249,   215,
      67,  -160,    93,   110,  -160,  -160,   249,   222,  -160,   147,
    -160,   126,    18,    -1,   249,    60,   249,  -160,  -160,  -160,
      19,  -160,   249,   249,   249,   249,   249,   249,  -160,  -160,
      12,    98,  -160,  -160,  -160,  -160,    15,   134,  -160,  -160,
     225,   166,   172,   161,   168,  -160,  -160,    90,  -160,   171,
    -160,  -160,   198,   167,   178,  -160,  -160,   182,  -160,  -160,
       5,   185,  -160,   193,  -160,  -160,  -160,    47,    47,   215,
    -160,  -160,   222,  -160,  -160,  -160,   217,  -160,  -160,   249,
     249,  -160,  -160,  -160,   249,  -160,  -160,   249,  -160,  -160,
    -160,  -160,  -160,  -160,   197,    99,   108,   136,   196,   201,
     207,   233,  -160,   195,   249,   249,   249,   249,   249,   249,
     249,   249,   195,  -160,   234,    99,    99,    99,    99,   108,
     108,   136,   196,  -160,   195,  -160
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    11,    12,    13,     0,     0,     4,     6,     0,     7,
       2,     0,     1,     5,     3,    27,     0,    25,     0,     0,
       9,     0,     0,     0,    29,    24,     0,     0,     0,     0,
       8,     0,     0,     0,     0,    40,    42,    43,    66,    70,
      71,     0,     0,     0,     0,     0,    73,    67,    74,    75,
      84,    79,    88,    23,     0,    28,    33,    64,     0,     0,
      27,    26,     0,     0,    15,    18,     0,     0,    10,     0,
      38,    44,     0,     0,     0,     0,     0,    77,    78,    76,
       0,    31,     0,     0,     0,     0,     0,     0,    34,    36,
       0,     0,    30,    16,    19,    21,     0,     0,    14,    54,
       0,     0,     0,     0,     0,    47,    51,     0,    56,     0,
      49,    52,     0,    73,     0,    41,    39,     0,    80,    82,
       0,     0,    72,     0,    86,    87,    85,    90,    89,     0,
      35,    32,     0,    20,    17,    62,     0,    61,    60,     0,
       0,    48,    50,    55,     0,    45,    46,     0,    81,    68,
      69,    37,    22,    63,     0,    91,    96,    99,   101,    65,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,    57,    92,    94,    93,    95,    98,
      97,   100,   102,    59,     0,    58
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -160,  -160,    43,  -160,  -160,    -3,   218,  -160,   -56,  -160,
       2,  -160,  -160,   229,  -160,   -51,  -160,   251,  -160,   186,
    -160,  -160,   -11,  -160,   148,  -159,   -21,   119,   -60,  -160,
    -160,  -160,   -27,  -160,  -160,   109,   -22,    34,    91,    94,
    -160
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,   106,     7,    19,     8,    20,    29,    64,    96,
      65,     9,    16,    17,    24,    55,    90,    10,    34,    35,
      36,    37,   108,   109,   110,   111,   112,   154,    46,    47,
      48,    49,    50,    51,   120,    52,    57,   156,   157,   158,
     159
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      53,    11,    56,    89,   174,    53,    53,    95,    92,   113,
      72,    98,   147,   183,    77,    78,    79,    69,    33,   129,
      15,    70,   132,    80,    45,   185,     1,     2,     3,    62,
      73,   130,   148,    56,   133,    21,    53,    22,    56,    25,
      26,    53,    27,     6,    53,    53,   122,    23,    13,   113,
      30,    31,    28,   117,   119,   121,   124,   125,   126,    18,
      91,   123,   116,    38,    39,    40,   107,    58,    97,    33,
      83,    84,    85,    38,    39,    40,   152,    59,   151,   136,
      41,    42,    43,     1,     2,     3,    44,   118,    54,    88,
      41,    42,    43,    60,    66,    22,    44,     1,     2,     3,
      71,     4,    32,   113,    67,    23,   107,    75,    56,    76,
      53,    74,   113,    38,    39,    40,    81,   155,   155,    86,
      82,    87,    93,   161,   113,    12,   162,   131,    63,    94,
      41,    42,    43,     1,     2,     3,    44,     4,   164,   165,
     166,   167,   175,   176,   177,   178,   155,   155,   155,   155,
      38,    39,    40,    99,   114,     1,     2,     3,   100,     4,
     101,   102,   103,   134,   104,    69,   105,    41,    42,    43,
     168,   169,   137,    44,    38,    39,    40,    99,   138,     1,
       2,     3,   100,     4,   101,   102,   103,   139,   104,    69,
     141,    41,    42,    43,   140,   127,   128,    44,    38,    39,
      40,    99,   179,   180,   143,   144,   100,   145,   101,   102,
     103,   146,   104,    69,   149,    41,    42,    43,    38,    39,
      40,    44,   150,   153,   163,    38,    39,    40,    38,    39,
      40,   135,   170,    54,   172,    41,    42,    43,   171,   173,
      63,    44,    41,    42,    43,    41,    42,    43,    44,    68,
     184,    44,    38,    39,    40,    61,    14,   142,   115,   160,
       0,   181,     0,     0,     0,   182,     0,     0,     0,    41,
      42,    43,     0,     0,     0,    44
};

static const yytype_int16 yycheck[] =
{
      22,     4,    23,    54,   163,    27,    28,    63,    59,    69,
       7,    67,     7,   172,    41,    42,    43,    18,    21,     7,
       3,    32,     7,    44,    22,   184,     8,     9,    10,    27,
      27,    19,    27,    54,    19,    26,    58,    28,    59,     6,
       7,    63,    28,     0,    66,    67,    27,    38,     5,   109,
       6,     7,    38,    74,    75,    76,    83,    84,    85,     3,
      58,    82,    73,     3,     4,     5,    69,    28,    66,    72,
      23,    24,    25,     3,     4,     5,   132,    38,   129,   100,
      20,    21,    22,     8,     9,    10,    26,    27,    18,    19,
      20,    21,    22,     3,    28,    28,    26,     8,     9,    10,
       3,    12,    27,   163,    38,    38,   109,    26,   129,    28,
     132,    28,   172,     3,     4,     5,    29,   139,   140,    20,
      28,    22,    29,   144,   184,     0,   147,    29,    18,    19,
      20,    21,    22,     8,     9,    10,    26,    12,    30,    31,
      32,    33,   164,   165,   166,   167,   168,   169,   170,   171,
       3,     4,     5,     6,    28,     8,     9,    10,    11,    12,
      13,    14,    15,    29,    17,    18,    19,    20,    21,    22,
      34,    35,     6,    26,     3,     4,     5,     6,     6,     8,
       9,    10,    11,    12,    13,    14,    15,    26,    17,    18,
      19,    20,    21,    22,    26,    86,    87,    26,     3,     4,
       5,     6,   168,   169,     6,    38,    11,    29,    13,    14,
      15,    29,    17,    18,    29,    20,    21,    22,     3,     4,
       5,    26,    29,     6,    27,     3,     4,     5,     3,     4,
       5,     6,    36,    18,    27,    20,    21,    22,    37,     6,
      18,    26,    20,    21,    22,    20,    21,    22,    26,    31,
      16,    26,     3,     4,     5,    26,     5,   109,    72,   140,
      -1,   170,    -1,    -1,    -1,   171,    -1,    -1,    -1,    20,
      21,    22,    -1,    -1,    -1,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,     9,    10,    12,    40,    41,    42,    44,    50,
      56,    44,     0,    41,    56,     3,    51,    52,     3,    43,
      45,    26,    28,    38,    53,     6,     7,    28,    38,    46,
       6,     7,    27,    44,    57,    58,    59,    60,     3,     4,
       5,    20,    21,    22,    26,    49,    67,    68,    69,    70,
      71,    72,    74,    75,    18,    54,    65,    75,    28,    38,
       3,    52,    49,    18,    47,    49,    28,    38,    45,    18,
      61,     3,     7,    27,    28,    26,    28,    71,    71,    71,
      65,    29,    28,    23,    24,    25,    20,    22,    19,    54,
      55,    49,    54,    29,    19,    47,    48,    49,    47,     6,
      11,    13,    14,    15,    17,    19,    41,    44,    61,    62,
      63,    64,    65,    67,    28,    58,    61,    65,    27,    65,
      73,    65,    27,    65,    71,    71,    71,    74,    74,     7,
      19,    29,     7,    19,    29,     6,    65,     6,     6,    26,
      26,    19,    63,     6,    38,    29,    29,     7,    27,    29,
      29,    54,    47,     6,    66,    75,    76,    77,    78,    79,
      66,    65,    65,    27,    30,    31,    32,    33,    34,    35,
      36,    37,    27,     6,    64,    75,    75,    75,    75,    76,
      76,    77,    78,    64,    16,    64
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    40,    40,    40,    41,    41,    42,    43,
      43,    44,    44,    44,    45,    45,    46,    46,    47,    47,
      47,    48,    48,    49,    50,    51,    51,    52,    52,    52,
      52,    53,    53,    54,    54,    54,    55,    55,    56,    56,
      57,    57,    58,    58,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    65,    66,    67,    67,    68,    68,
      69,    69,    70,    70,    70,    71,    71,    71,    71,    71,
      72,    72,    73,    73,    74,    74,    74,    74,    75,    75,
      75,    76,    76,    76,    76,    76,    77,    77,    77,    78,
      78,    79,    79
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     1,     4,     1,
       3,     1,     1,     1,     4,     3,     3,     4,     1,     2,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     2,
       4,     3,     4,     1,     2,     3,     1,     3,     5,     6,
       1,     3,     1,     1,     2,     4,     4,     2,     3,     1,
       2,     1,     1,     4,     1,     2,     1,     5,     7,     5,
       2,     2,     2,     3,     1,     1,     1,     1,     4,     4,
       1,     1,     3,     1,     1,     1,     2,     2,     2,     1,
       3,     4,     1,     3,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 174 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            root = new tree_comp_unit();
            root->_line_no = yyline+1;
            printf("func def is %s\n",(yyvsp[0].func_def)->id.c_str());
		    root->functions.push_back(std::shared_ptr<tree_func_def>((yyvsp[0].func_def)));
        }
#line 1486 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 181 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            printf("func def is %s\n",(yyvsp[0].func_def)->id.c_str());
		    root->functions.push_back(std::shared_ptr<tree_func_def>((yyvsp[0].func_def)));
        }
#line 1495 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 186 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            root = new tree_comp_unit();
            root->_line_no = yyline+1;
            root->definitions.push_back(std::shared_ptr<tree_decl>((yyvsp[0].decl)));
        }
#line 1505 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 192 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            root->definitions.push_back(std::shared_ptr<tree_decl>((yyvsp[0].decl)));
        }
#line 1513 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 199 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.decl) = new tree_decl();
            (yyval.decl)->_line_no = yyline+1;
            (yyval.decl)->const_decl=std::shared_ptr<tree_const_decl>((yyvsp[0].const_decl));
        }
#line 1523 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 205 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.decl) = new tree_decl();
            (yyval.decl)->_line_no = yyline+1;
            (yyval.decl)->var_decl=std::shared_ptr<tree_var_decl>((yyvsp[0].var_decl));
        }
#line 1533 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 214 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_decl) = new tree_const_decl();
            (yyval.const_decl)->_line_no = yyline+1;
            (yyval.const_decl)->b_type=std::shared_ptr<tree_basic_type>((yyvsp[-2].basic_type));
            (yyval.const_decl)->const_def_list=std::shared_ptr<tree_const_def_list>((yyvsp[-1].const_def_list));
        }
#line 1544 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 224 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_def_list) = new tree_const_def_list();
            (yyval.const_def_list)->_line_no = yyline+1;
            (yyval.const_def_list)->const_defs.push_back(std::shared_ptr<tree_const_def>((yyvsp[0].const_def)));
        }
#line 1554 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 230 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyvsp[-2].const_def_list)->const_defs.push_back(std::shared_ptr<tree_const_def>((yyvsp[0].const_def)));
            (yyval.const_def_list) = (yyvsp[-2].const_def_list);
        }
#line 1563 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 238 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.basic_type) = new tree_basic_type();
            (yyval.basic_type)->_line_no = yyline+1;
            (yyval.basic_type)->type=type_helper::INT;
        }
#line 1573 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 244 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.basic_type) = new tree_basic_type();
            (yyval.basic_type)->_line_no = yyline+1;
            (yyval.basic_type)->type=type_helper::FLOAT;
        }
#line 1583 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 250 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.basic_type) = new tree_basic_type();
            (yyval.basic_type)->_line_no = yyline+1;
            (yyval.basic_type)->type=type_helper::VOID;
        }
#line 1593 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 259 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_def) = new tree_const_def();
            (yyval.const_def)->_line_no = yyline+1;
            (yyval.const_def)->id=*(yyvsp[-3].string);
            (yyval.const_def)->const_exp_list=std::shared_ptr<tree_const_exp_list>((yyvsp[-2].const_exp_list));
            (yyval.const_def)->const_init_val=std::shared_ptr<tree_const_init_val>((yyvsp[0].const_init_val));
        }
#line 1605 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 267 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_def) = new tree_const_def();
            (yyval.const_def)->_line_no = yyline+1;
            (yyval.const_def)->id=*(yyvsp[-2].string);
            (yyval.const_def)->const_init_val=std::shared_ptr<tree_const_init_val>((yyvsp[0].const_init_val));
        }
#line 1616 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 277 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_exp_list) = new tree_const_exp_list();
            (yyval.const_exp_list)->_line_no = yyline+1;
            (yyval.const_exp_list)->const_exp.push_back(std::shared_ptr<tree_const_exp>((yyvsp[-1].const_exp)));
        }
#line 1626 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 283 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyvsp[-3].const_exp_list)->const_exp.push_back(std::shared_ptr<tree_const_exp>((yyvsp[-1].const_exp)));
            (yyval.const_exp_list) = (yyvsp[-3].const_exp_list);
        }
#line 1635 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 291 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_init_val) = new tree_const_init_val();
            (yyval.const_init_val)->_line_no = yyline+1;
            (yyval.const_init_val)->const_exp= std::shared_ptr<tree_const_exp>((yyvsp[0].const_exp)) ;
        }
#line 1645 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 297 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_init_val) = new tree_const_init_val();
            (yyval.const_init_val)->_line_no = yyline+1;
        }
#line 1654 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 302 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_init_val) = new tree_const_init_val();
            (yyval.const_init_val)->_line_no = yyline+1;
            (yyval.const_init_val)->const_val_list = std::shared_ptr<tree_const_val_list>((yyvsp[-1].const_init_val_list)) ;
        }
#line 1664 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 311 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_init_val_list) = new tree_const_val_list();
            (yyval.const_init_val_list)->_line_no = yyline+1;
            (yyval.const_init_val_list)->const_init_vals.push_back(std::shared_ptr<tree_const_init_val>((yyvsp[0].const_init_val)));
        }
#line 1674 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 317 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyvsp[-2].const_init_val_list)->const_init_vals.push_back(std::shared_ptr<tree_const_init_val>((yyvsp[0].const_init_val)));
            (yyval.const_init_val_list) = (yyvsp[-2].const_init_val_list);
        }
#line 1683 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 325 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.const_exp) = new tree_const_exp();
            (yyval.const_exp)->_line_no = yyline+1;
            (yyval.const_exp)->add_exp = std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 1693 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 334 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.var_decl) = new tree_var_decl();
            (yyval.var_decl)->_line_no = yyline+1;
            (yyval.var_decl)->b_type=std::shared_ptr<tree_basic_type>((yyvsp[-2].basic_type));
            (yyval.var_decl)->var_def_list=std::shared_ptr<tree_var_def_list>((yyvsp[-1].var_def_list));
        }
#line 1704 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 344 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.var_def_list) = new tree_var_def_list();
            (yyval.var_def_list)->_line_no = yyline+1;
            (yyval.var_def_list)->var_defs.push_back(std::shared_ptr<tree_var_def>((yyvsp[0].var_def)));
        }
#line 1714 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 350 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyvsp[-2].var_def_list)->var_defs.push_back(std::shared_ptr<tree_var_def>((yyvsp[0].var_def)));
            (yyval.var_def_list) = (yyvsp[-2].var_def_list);
        }
#line 1723 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 358 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.var_def) = new tree_var_def();
            (yyval.var_def)->_line_no = yyline+1;
            (yyval.var_def)->id = *(yyvsp[0].string);
        }
#line 1733 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 364 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.var_def) = new tree_var_def();
            (yyval.var_def)->_line_no = yyline+1;
            (yyval.var_def)->id = *(yyvsp[-2].string);
            (yyval.var_def)->init_val = std::shared_ptr<tree_init_val>((yyvsp[0].init_val));
        }
#line 1744 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 371 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.var_def) = new tree_var_def();
            (yyval.var_def)->_line_no = yyline+1;
            (yyval.var_def)->id = *(yyvsp[-1].string);
            (yyval.var_def)->array_def = std::shared_ptr<tree_arrray_def>((yyvsp[0].array_def));
        }
#line 1755 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 378 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.var_def) = new tree_var_def();
            (yyval.var_def)->_line_no = yyline+1;
            (yyval.var_def)->id = *(yyvsp[-3].string);
            (yyval.var_def)->array_def = std::shared_ptr<tree_arrray_def>((yyvsp[-2].array_def));
            (yyval.var_def)->init_val = std::shared_ptr<tree_init_val>((yyvsp[0].init_val));
        }
#line 1767 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 389 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.array_def) = new tree_arrray_def();
            (yyval.array_def)->_line_no = yyline+1;
            (yyval.array_def)->const_exps.push_back(std::shared_ptr<tree_const_exp>((yyvsp[-1].const_exp)));
        }
#line 1777 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 395 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.array_def)->const_exps.push_back(std::shared_ptr<tree_const_exp>((yyvsp[-1].const_exp)));
            (yyval.array_def) = (yyvsp[-3].array_def);
        }
#line 1786 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 404 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.init_val) = new tree_init_val();
            (yyval.init_val)->_line_no = yyline+1;
            (yyval.init_val)->exp=std::shared_ptr<tree_exp>((yyvsp[0].exp));
        }
#line 1796 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 409 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.init_val) = new tree_init_val();
            (yyval.init_val)->_line_no = yyline+1;
    }
#line 1805 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 413 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.init_val) = new tree_init_val();
            (yyval.init_val)->_line_no = yyline+1;
            (yyval.init_val)->init_val_list = std::shared_ptr<tree_init_val_list>((yyvsp[-1].init_val_list));
    }
#line 1815 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 421 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
        (yyval.init_val_list) = new tree_init_val_list();
        (yyval.init_val_list)->_line_no = yyline+1;
        (yyval.init_val_list)->init_vals.push_back(std::shared_ptr<tree_init_val>((yyvsp[0].init_val)));
    }
#line 1825 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 426 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
        (yyvsp[-2].init_val_list)->init_vals.push_back(std::shared_ptr<tree_init_val>((yyvsp[0].init_val)));
        (yyval.init_val_list) = (yyvsp[-2].init_val_list);
    }
#line 1834 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 435 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_def) = new tree_func_def();
            (yyval.func_def)->_line_no = yyline+1;
            (yyval.func_def)->type = std::shared_ptr<tree_basic_type>((yyvsp[-4].basic_type));
            (yyval.func_def)->id = *(yyvsp[-3].string);
            (yyval.func_def)->block.push_back(std::shared_ptr<tree_block>((yyvsp[0].block)));
        }
#line 1846 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 443 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_def) = new tree_func_def();
            (yyval.func_def)->_line_no = yyline+1;
            (yyval.func_def)->type = std::shared_ptr<tree_basic_type>((yyvsp[-5].basic_type));
            (yyval.func_def)->id = *(yyvsp[-4].string);
            (yyval.func_def)->funcfparams = std::shared_ptr<tree_func_fparams>((yyvsp[-2].func_fparams));

            (yyval.func_def)->block.push_back(std::shared_ptr<tree_block>((yyvsp[0].block)));
        }
#line 1860 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 456 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_fparams) = new tree_func_fparams();
            (yyval.func_fparams)->_line_no = yyline+1;
            (yyval.func_fparams)->funcfparamlist.push_back(std::shared_ptr<tree_func_fparam>((yyvsp[0].func_fparam)));
        }
#line 1870 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 462 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyvsp[-2].func_fparams)->funcfparamlist.push_back(std::shared_ptr<tree_func_fparam>((yyvsp[0].func_fparam)));
            (yyval.func_fparams) = (yyvsp[-2].func_fparams);
        }
#line 1879 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 470 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_fparam) = new tree_func_fparam();
            (yyval.func_fparam)->_line_no = yyline+1;
            (yyval.func_fparam)->funcfparamone = std::shared_ptr<tree_func_fparamone>((yyvsp[0].func_fparamone));
        }
#line 1889 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 476 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_fparam) = new tree_func_fparam();
            (yyval.func_fparam)->_line_no = yyline+1;
            (yyval.func_fparam)->funcfparamarray = std::shared_ptr<tree_func_fparamarray>((yyvsp[0].func_fparamarray));
        }
#line 1899 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 485 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_fparamone) = new tree_func_fparamone();
            (yyval.func_fparamone)->_line_no = yyline+1;
            (yyval.func_fparamone)->b_type = std::shared_ptr<tree_basic_type>((yyvsp[-1].basic_type));
            (yyval.func_fparamone)->id = *(yyvsp[0].string);
        }
#line 1910 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 495 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_fparamarray) = new tree_func_fparamarray();
            (yyval.func_fparamarray)->_line_no = yyline+1;
            (yyval.func_fparamarray)->b_type = std::shared_ptr<tree_basic_type>((yyvsp[-3].basic_type));
            (yyval.func_fparamarray)->id = *(yyvsp[-2].string);
        }
#line 1921 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 502 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyvsp[-3].func_fparamarray)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[-1].exp)));
            (yyval.func_fparamarray) = (yyvsp[-3].func_fparamarray);
        }
#line 1930 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 510 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.block) = new tree_block();
            (yyval.block)->_line_no = yyline+1;
        }
#line 1939 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 515 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.block) = new tree_block();
            (yyval.block)->_line_no = yyline+1;
            (yyval.block)->block_item_list=std::shared_ptr<tree_block_item_list>((yyvsp[-1].block_item_list));
        }
#line 1949 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 524 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.block_item_list) = new tree_block_item_list();
            (yyval.block_item_list)->_line_no = yyline+1;
            (yyval.block_item_list)->block_items.push_back(std::shared_ptr<tree_block_item>((yyvsp[0].block_item)));
        }
#line 1959 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 530 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyvsp[-1].block_item_list)->block_items.push_back(std::shared_ptr<tree_block_item>((yyvsp[0].block_item)));
            (yyval.block_item_list)=(yyvsp[-1].block_item_list);
        }
#line 1968 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 538 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.block_item) = new tree_block_item();
            (yyval.block_item)->_line_no = yyline+1;
            (yyval.block_item)->decl=std::shared_ptr<tree_decl>((yyvsp[0].decl));
        }
#line 1978 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 544 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.block_item) = new tree_block_item();
            (yyval.block_item)->_line_no = yyline+1;
            (yyval.block_item)->stmt=std::shared_ptr<tree_stmt>((yyvsp[0].stmt));
        }
#line 1988 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 555 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto a_stmt = new tree_assign_stmt();
            a_stmt->l_val=std::shared_ptr<tree_l_val>((yyvsp[-3].l_val));
            a_stmt->exp=std::shared_ptr<tree_exp>((yyvsp[-1].exp));
            (yyval.stmt)->assigm_stmt=std::shared_ptr<tree_assign_stmt>(a_stmt) ;
        }
#line 2001 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 564 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
        }
#line 2010 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 569 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            (yyval.stmt)->exp=std::shared_ptr<tree_exp>((yyvsp[-1].exp)) ;
        }
#line 2020 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 575 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            (yyval.stmt)->block=std::shared_ptr<tree_block>((yyvsp[0].block)) ;
        }
#line 2030 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 582 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto if_stmt = new tree_if_stmt();
            if_stmt->cond = std::shared_ptr<tree_cond>((yyvsp[-2].cond));
            if_stmt->stmt = std::shared_ptr<tree_stmt>((yyvsp[0].stmt));
            (yyval.stmt)->if_stmt = std::shared_ptr<tree_if_stmt>(if_stmt) ;
        }
#line 2043 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 591 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto if_else_stmt = new tree_if_else_stmt();
            if_else_stmt->cond = std::shared_ptr<tree_cond>((yyvsp[-4].cond));
            if_else_stmt->then_stmt = std::shared_ptr<tree_stmt>((yyvsp[-2].stmt));
            if_else_stmt->else_stmt = std::shared_ptr<tree_stmt>((yyvsp[0].stmt));
            (yyval.stmt)->if_else_stmt = std::shared_ptr<tree_if_else_stmt>(if_else_stmt) ;

        }
#line 2058 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 603 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto while_stmt = new tree_while_stmt();
            while_stmt->cond = std::shared_ptr<tree_cond>((yyvsp[-2].cond));
            while_stmt->stmt = std::shared_ptr<tree_stmt>((yyvsp[0].stmt));
            (yyval.stmt)->while_stmt = std::shared_ptr<tree_while_stmt>(while_stmt) ;
        }
#line 2071 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 612 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto continue_stmt = new tree_continue_stmt();
            (yyval.stmt)->continue_stmt = std::shared_ptr<tree_continue_stmt>(continue_stmt) ;
        }
#line 2082 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 619 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto break_stmt = new tree_break_stmt();
            (yyval.stmt)->break_stmt = std::shared_ptr<tree_break_stmt>(break_stmt) ;
        }
#line 2093 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 627 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto a_stmt = new tree_return_null_stmt();
            (yyval.stmt)->return_null_stmt=std::shared_ptr<tree_return_null_stmt>(a_stmt) ;
        }
#line 2104 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 634 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto a_stmt = new tree_return_stmt();
            a_stmt->exp=std::shared_ptr<tree_exp>((yyvsp[-1].exp));
            (yyval.stmt)->return_stmt=std::shared_ptr<tree_return_stmt>(a_stmt) ;
        }
#line 2116 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 646 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.exp) = new tree_exp();
            (yyval.exp)->_line_no = yyline+1;
            (yyval.exp)->add_exp = std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2126 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 655 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.cond) = new tree_cond();
            (yyval.cond)->_line_no = yyline+1;
            (yyval.cond)->l_or_exp = std::shared_ptr<tree_l_or_exp>((yyvsp[0].l_or_exp));
        }
#line 2136 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 664 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.l_val) = new tree_l_val();
            (yyval.l_val)->_line_no = yyline+1;
            (yyval.l_val)->id = *(yyvsp[0].string);
        }
#line 2146 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 670 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.l_val) = new tree_l_val();
            (yyval.l_val)->_line_no = yyline+1;
            (yyval.l_val)->array_ident = std::shared_ptr<tree_array_ident>((yyvsp[0].array_ident));
        }
#line 2156 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 679 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.array_ident) = new tree_array_ident();
            (yyval.array_ident)->_line_no = yyline+1;
            (yyval.array_ident)->id = *(yyvsp[-3].string);
            (yyval.array_ident)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[-1].exp)));
        }
#line 2167 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 686 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyvsp[-3].array_ident)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[-1].exp)));
            (yyval.array_ident) = (yyvsp[-3].array_ident);
        }
#line 2176 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 694 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.number) = new tree_number();
            (yyval.number)->_line_no = yyline+1;
            (yyval.number)->int_value = std::stoi((yyvsp[0].string)->c_str(), nullptr, 0);
            (yyval.number)->is_int=true;
        }
#line 2187 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 701 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.number) = new tree_number();
            (yyval.number)->_line_no = yyline+1;
            (yyval.number)->float_value = (float)atof((yyvsp[0].string)->c_str());
            (yyval.number)->is_int=false;
        }
#line 2198 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 711 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.primary_exp) = new tree_primary_exp();
            (yyval.primary_exp)->_line_no = yyline+1;
            (yyval.primary_exp)->exp = std::shared_ptr<tree_exp>((yyvsp[-1].exp));
        }
#line 2208 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 717 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.primary_exp) = new tree_primary_exp();
            (yyval.primary_exp)->_line_no = yyline+1;
            (yyval.primary_exp)->l_val = std::shared_ptr<tree_l_val>((yyvsp[0].l_val));
        }
#line 2218 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 723 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.primary_exp) = new tree_primary_exp();
            (yyval.primary_exp)->_line_no = yyline+1;
            (yyval.primary_exp)->number = std::shared_ptr<tree_number>((yyvsp[0].number));
        }
#line 2228 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 732 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->primary_exp = std::shared_ptr<tree_primary_exp>((yyvsp[0].primary_exp));
        }
#line 2238 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 738 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.unary_exp)->oprt="+";
        }
#line 2249 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 745 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.unary_exp)->oprt="-";
        }
#line 2260 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 752 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.unary_exp)->oprt="!";
        }
#line 2271 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 760 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->func_call = std::shared_ptr<tree_func_call>((yyvsp[0].func_call));
        }
#line 2281 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 769 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_call) = new tree_func_call();
            (yyval.func_call)->_line_no = yyline+1;
            (yyval.func_call)->id = *(yyvsp[-2].string);
        }
#line 2291 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 775 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_call) = new tree_func_call();
            (yyval.func_call)->_line_no = yyline+1;
            (yyval.func_call)->id = *(yyvsp[-3].string);
            (yyval.func_call)->func_param_list = std::shared_ptr<tree_func_paramlist>((yyvsp[-1].func_param_list));
        }
#line 2302 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 785 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.func_param_list) = new tree_func_paramlist();
            (yyval.func_param_list)->_line_no = yyline+1;
            (yyval.func_param_list)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[0].exp)));
        }
#line 2312 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 791 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyvsp[-2].func_param_list)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[0].exp)));
            (yyval.func_param_list) = (yyvsp[-2].func_param_list);
        }
#line 2321 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 801 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.mul_exp) = new tree_mul_exp();
            (yyval.mul_exp)->_line_no = yyline+1;
            (yyval.mul_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
        }
#line 2331 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 807 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.mul_exp) = new tree_mul_exp();
            (yyval.mul_exp)->_line_no = yyline+1;
            (yyval.mul_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[-2].mul_exp));
            (yyval.mul_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.mul_exp)->oprt="*";
        }
#line 2343 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 815 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.mul_exp) = new tree_mul_exp();
            (yyval.mul_exp)->_line_no = yyline+1;
            (yyval.mul_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[-2].mul_exp));
            (yyval.mul_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.mul_exp)->oprt="/";
        }
#line 2355 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 823 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.mul_exp) = new tree_mul_exp();
            (yyval.mul_exp)->_line_no = yyline+1;
            (yyval.mul_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[-2].mul_exp));
            (yyval.mul_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.mul_exp)->oprt="%";
        }
#line 2367 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 833 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.add_exp) = new tree_add_exp();
            (yyval.add_exp)->_line_no = yyline+1;
            (yyval.add_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[0].mul_exp));
        }
#line 2377 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 839 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.add_exp) = new tree_add_exp();
            (yyval.add_exp)->_line_no = yyline+1;
            (yyval.add_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[-2].add_exp));
            (yyval.add_exp)->oprt="+";
            (yyval.add_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[0].mul_exp));
        }
#line 2389 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 847 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.add_exp) = new tree_add_exp();
            (yyval.add_exp)->_line_no = yyline+1;
            (yyval.add_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[-2].add_exp));
            (yyval.add_exp)->oprt="-";
            (yyval.add_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[0].mul_exp));
        }
#line 2401 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 857 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2411 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 863 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[-2].rel_exp));
            (yyval.rel_exp)->oprt="<";
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2423 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 871 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[-2].rel_exp));
            (yyval.rel_exp)->oprt=">";
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2435 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 879 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[-2].rel_exp));
            (yyval.rel_exp)->oprt="<=";
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2447 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 887 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[-2].rel_exp));
            (yyval.rel_exp)->oprt=">=";
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2459 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 897 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.eq_exp) = new tree_eq_exp();
            (yyval.eq_exp)->_line_no = yyline+1;
            (yyval.eq_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[0].rel_exp));
        }
#line 2469 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 903 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.eq_exp) = new tree_eq_exp();
            (yyval.eq_exp)->_line_no = yyline+1;
            (yyval.eq_exp)->eq_exp=std::shared_ptr<tree_eq_exp>((yyvsp[-2].eq_exp));
            (yyval.eq_exp)->oprt="==";
            (yyval.eq_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[0].rel_exp));
        }
#line 2481 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 911 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.eq_exp) = new tree_eq_exp();
            (yyval.eq_exp)->_line_no = yyline+1;
            (yyval.eq_exp)->eq_exp=std::shared_ptr<tree_eq_exp>((yyvsp[-2].eq_exp));
            (yyval.eq_exp)->oprt="!=";
            (yyval.eq_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[0].rel_exp));
        }
#line 2493 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 921 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.l_and_exp) = new tree_l_and_exp();
            (yyval.l_and_exp)->_line_no = yyline+1;
            (yyval.l_and_exp)->eq_exp=std::shared_ptr<tree_eq_exp>((yyvsp[0].eq_exp));
        }
#line 2503 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 927 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.l_and_exp) = new tree_l_and_exp();
            (yyval.l_and_exp)->_line_no = yyline+1;
            (yyval.l_and_exp)->l_and_exp=std::shared_ptr<tree_l_and_exp>((yyvsp[-2].l_and_exp));
            (yyval.l_and_exp)->eq_exp=std::shared_ptr<tree_eq_exp>((yyvsp[0].eq_exp));
        }
#line 2514 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 936 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.l_or_exp) = new tree_l_or_exp();
            (yyval.l_or_exp)->_line_no = yyline+1;
            (yyval.l_or_exp)->l_and_exp=std::shared_ptr<tree_l_and_exp>((yyvsp[0].l_and_exp));
        }
#line 2524 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 942 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1646  */
    {
            (yyval.l_or_exp) = new tree_l_or_exp();
            (yyval.l_or_exp)->_line_no = yyline+1;
            (yyval.l_or_exp)->l_or_exp=std::shared_ptr<tree_l_or_exp>((yyvsp[-2].l_or_exp));
            (yyval.l_or_exp)->l_and_exp=std::shared_ptr<tree_l_and_exp>((yyvsp[0].l_and_exp));
        }
#line 2535 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
    break;


#line 2539 "/root/carolcc/src/frontend/parser.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 949 "/root/carolcc/src/frontend/parser.yy" /* yacc.c:1906  */

void insertVarible(std::string& type,std::string& id){
    VaribleTable.insert(std::make_pair<std::string, VaribleInfo>(std::string(id),VaribleInfo(type)));
}
void insertFunction(std::string& type,std::string& id){
    FunctionTable.insert(
        std::make_pair<std::string, FunctionInfo>
            (std::string(id),FunctionInfo(type,VaribleTable)));
    VaribleTable.clear();
}
