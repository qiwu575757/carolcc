/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"

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

#line 103 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TIDENTIFIER = 3,                /* TIDENTIFIER  */
  YYSYMBOL_TINTEGER = 4,                   /* TINTEGER  */
  YYSYMBOL_TFLOATNUM = 5,                  /* TFLOATNUM  */
  YYSYMBOL_TSEMICOLOM = 6,                 /* ";"  */
  YYSYMBOL_TCOMMA = 7,                     /* ","  */
  YYSYMBOL_TINT = 8,                       /* "int"  */
  YYSYMBOL_TFLOAT = 9,                     /* "float"  */
  YYSYMBOL_TVOID = 10,                     /* "void"  */
  YYSYMBOL_TRETURN = 11,                   /* "return"  */
  YYSYMBOL_TCONST = 12,                    /* "const"  */
  YYSYMBOL_TBREAK = 13,                    /* "break"  */
  YYSYMBOL_TCONTINUE = 14,                 /* "continue"  */
  YYSYMBOL_TIF = 15,                       /* "if"  */
  YYSYMBOL_TELSE = 16,                     /* "else"  */
  YYSYMBOL_TWHILE = 17,                    /* "while"  */
  YYSYMBOL_TLBPAREN = 18,                  /* "{"  */
  YYSYMBOL_TRBPAREN = 19,                  /* "}"  */
  YYSYMBOL_TMINUS = 20,                    /* "-"  */
  YYSYMBOL_TNOT = 21,                      /* "!"  */
  YYSYMBOL_TPLUS = 22,                     /* "+"  */
  YYSYMBOL_TDIV = 23,                      /* "/"  */
  YYSYMBOL_TMOD = 24,                      /* "%"  */
  YYSYMBOL_TMULTI = 25,                    /* "*"  */
  YYSYMBOL_TLPAREN = 26,                   /* "("  */
  YYSYMBOL_TRPAREN = 27,                   /* ")"  */
  YYSYMBOL_TLMPAREN = 28,                  /* "["  */
  YYSYMBOL_TRMPAREN = 29,                  /* "]"  */
  YYSYMBOL_TL = 30,                        /* "<"  */
  YYSYMBOL_TLE = 31,                       /* "<="  */
  YYSYMBOL_TG = 32,                        /* ">"  */
  YYSYMBOL_TGE = 33,                       /* ">="  */
  YYSYMBOL_TNE = 34,                       /* "!="  */
  YYSYMBOL_TE = 35,                        /* "=="  */
  YYSYMBOL_TLOGAND = 36,                   /* "&&"  */
  YYSYMBOL_TLOGOR = 37,                    /* "||"  */
  YYSYMBOL_TASSIGN = 38,                   /* "="  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_CompUnit = 40,                  /* CompUnit  */
  YYSYMBOL_Decl = 41,                      /* Decl  */
  YYSYMBOL_ConstDecl = 42,                 /* ConstDecl  */
  YYSYMBOL_ConstDefList = 43,              /* ConstDefList  */
  YYSYMBOL_BType = 44,                     /* BType  */
  YYSYMBOL_ConstDef = 45,                  /* ConstDef  */
  YYSYMBOL_ConstExpArrayList = 46,         /* ConstExpArrayList  */
  YYSYMBOL_ConstInitVal = 47,              /* ConstInitVal  */
  YYSYMBOL_ConstInitVallist = 48,          /* ConstInitVallist  */
  YYSYMBOL_ConstExp = 49,                  /* ConstExp  */
  YYSYMBOL_VarDecl = 50,                   /* VarDecl  */
  YYSYMBOL_VarDefList = 51,                /* VarDefList  */
  YYSYMBOL_VarDef = 52,                    /* VarDef  */
  YYSYMBOL_ArrayDef = 53,                  /* ArrayDef  */
  YYSYMBOL_InitVal = 54,                   /* InitVal  */
  YYSYMBOL_InitValList = 55,               /* InitValList  */
  YYSYMBOL_FuncDef = 56,                   /* FuncDef  */
  YYSYMBOL_FuncFParams = 57,               /* FuncFParams  */
  YYSYMBOL_FuncFParam = 58,                /* FuncFParam  */
  YYSYMBOL_FuncFParamOne = 59,             /* FuncFParamOne  */
  YYSYMBOL_FuncFParamArray = 60,           /* FuncFParamArray  */
  YYSYMBOL_Block = 61,                     /* Block  */
  YYSYMBOL_BlockItemList = 62,             /* BlockItemList  */
  YYSYMBOL_BlockItem = 63,                 /* BlockItem  */
  YYSYMBOL_Stmt = 64,                      /* Stmt  */
  YYSYMBOL_Exp = 65,                       /* Exp  */
  YYSYMBOL_Cond = 66,                      /* Cond  */
  YYSYMBOL_LVal = 67,                      /* LVal  */
  YYSYMBOL_ArrayIdent = 68,                /* ArrayIdent  */
  YYSYMBOL_Number = 69,                    /* Number  */
  YYSYMBOL_PrimaryExp = 70,                /* PrimaryExp  */
  YYSYMBOL_UnaryExp = 71,                  /* UnaryExp  */
  YYSYMBOL_FuncCall = 72,                  /* FuncCall  */
  YYSYMBOL_FuncParamList = 73,             /* FuncParamList  */
  YYSYMBOL_MulExp = 74,                    /* MulExp  */
  YYSYMBOL_AddExp = 75,                    /* AddExp  */
  YYSYMBOL_RelExp = 76,                    /* RelExp  */
  YYSYMBOL_EqExp = 77,                     /* EqExp  */
  YYSYMBOL_LAndExp = 78,                   /* LAndExp  */
  YYSYMBOL_LOrExp = 79                     /* LOrExp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
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

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   293


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
static const yytype_int16 yyrline[] =
{
       0,   170,   170,   177,   182,   188,   195,   201,   210,   220,
     226,   234,   240,   246,   255,   263,   273,   279,   287,   293,
     298,   307,   313,   321,   330,   340,   346,   354,   360,   367,
     374,   385,   391,   400,   406,   410,   418,   423,   431,   439,
     452,   458,   466,   472,   481,   491,   498,   506,   511,   520,
     526,   534,   540,   550,   560,   565,   571,   578,   587,   599,
     608,   615,   623,   630,   642,   651,   660,   666,   675,   682,
     690,   697,   707,   713,   719,   728,   734,   741,   748,   756,
     765,   771,   781,   787,   797,   803,   811,   819,   829,   835,
     843,   853,   859,   867,   875,   883,   893,   899,   907,   917,
     923,   932,   938
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TIDENTIFIER",
  "TINTEGER", "TFLOATNUM", "\";\"", "\",\"", "\"int\"", "\"float\"",
  "\"void\"", "\"return\"", "\"const\"", "\"break\"", "\"continue\"",
  "\"if\"", "\"else\"", "\"while\"", "\"{\"", "\"}\"", "\"-\"", "\"!\"",
  "\"+\"", "\"/\"", "\"%\"", "\"*\"", "\"(\"", "\")\"", "\"[\"", "\"]\"",
  "\"<\"", "\"<=\"", "\">\"", "\">=\"", "\"!=\"", "\"==\"", "\"&&\"",
  "\"||\"", "\"=\"", "$accept", "CompUnit", "Decl", "ConstDecl",
  "ConstDefList", "BType", "ConstDef", "ConstExpArrayList", "ConstInitVal",
  "ConstInitVallist", "ConstExp", "VarDecl", "VarDefList", "VarDef",
  "ArrayDef", "InitVal", "InitValList", "FuncDef", "FuncFParams",
  "FuncFParam", "FuncFParamOne", "FuncFParamArray", "Block",
  "BlockItemList", "BlockItem", "Stmt", "Exp", "Cond", "LVal",
  "ArrayIdent", "Number", "PrimaryExp", "UnaryExp", "FuncCall",
  "FuncParamList", "MulExp", "AddExp", "RelExp", "EqExp", "LAndExp",
  "LOrExp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293
};
#endif

#define YYPACT_NINF (-160)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
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
static const yytype_int8 yydefact[] =
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
static const yytype_uint8 yydefgoto[] =
{
       0,     5,   106,     7,    19,     8,    20,    29,    64,    96,
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
static const yytype_int8 yystos[] =
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
static const yytype_int8 yyr1[] =
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
static const yytype_int8 yyr2[] =
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


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 2: /* CompUnit: FuncDef  */
#line 171 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            root = new tree_comp_unit();
            root->_line_no = yyline+1;
            printf("func def is %s\n",(yyvsp[0].func_def)->id.c_str());
		    root->functions.push_back(std::shared_ptr<tree_func_def>((yyvsp[0].func_def)));
        }
#line 1333 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 3: /* CompUnit: CompUnit FuncDef  */
#line 178 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            printf("func def is %s\n",(yyvsp[0].func_def)->id.c_str());
		    root->functions.push_back(std::shared_ptr<tree_func_def>((yyvsp[0].func_def)));
        }
#line 1342 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 4: /* CompUnit: Decl  */
#line 183 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            root = new tree_comp_unit();
            root->_line_no = yyline+1;
            root->definitions.push_back(std::shared_ptr<tree_decl>((yyvsp[0].decl)));
        }
#line 1352 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 5: /* CompUnit: CompUnit Decl  */
#line 189 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            root->definitions.push_back(std::shared_ptr<tree_decl>((yyvsp[0].decl)));
        }
#line 1360 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 6: /* Decl: ConstDecl  */
#line 196 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.decl) = new tree_decl();
            (yyval.decl)->_line_no = yyline+1;
            (yyval.decl)->const_decl=std::shared_ptr<tree_const_decl>((yyvsp[0].const_decl));
        }
#line 1370 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 7: /* Decl: VarDecl  */
#line 202 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.decl) = new tree_decl();
            (yyval.decl)->_line_no = yyline+1;
            (yyval.decl)->var_decl=std::shared_ptr<tree_var_decl>((yyvsp[0].var_decl));
        }
#line 1380 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 8: /* ConstDecl: "const" BType ConstDefList ";"  */
#line 211 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_decl) = new tree_const_decl();
            (yyval.const_decl)->_line_no = yyline+1;
            (yyval.const_decl)->b_type=std::shared_ptr<tree_basic_type>((yyvsp[-2].basic_type));
            (yyval.const_decl)->const_def_list=std::shared_ptr<tree_const_def_list>((yyvsp[-1].const_def_list));
        }
#line 1391 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 9: /* ConstDefList: ConstDef  */
#line 221 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_def_list) = new tree_const_def_list();
            (yyval.const_def_list)->_line_no = yyline+1;
            (yyval.const_def_list)->const_defs.push_back(std::shared_ptr<tree_const_def>((yyvsp[0].const_def)));
        }
#line 1401 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 10: /* ConstDefList: ConstDefList "," ConstDef  */
#line 227 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyvsp[-2].const_def_list)->const_defs.push_back(std::shared_ptr<tree_const_def>((yyvsp[0].const_def)));
            (yyval.const_def_list) = (yyvsp[-2].const_def_list);
        }
#line 1410 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 11: /* BType: "int"  */
#line 235 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.basic_type) = new tree_basic_type();
            (yyval.basic_type)->_line_no = yyline+1;
            (yyval.basic_type)->type=type_helper::INT;
        }
#line 1420 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 12: /* BType: "float"  */
#line 241 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.basic_type) = new tree_basic_type();
            (yyval.basic_type)->_line_no = yyline+1;
            (yyval.basic_type)->type=type_helper::FLOAT;
        }
#line 1430 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 13: /* BType: "void"  */
#line 247 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.basic_type) = new tree_basic_type();
            (yyval.basic_type)->_line_no = yyline+1;
            (yyval.basic_type)->type=type_helper::VOID;
        }
#line 1440 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 14: /* ConstDef: TIDENTIFIER ConstExpArrayList "=" ConstInitVal  */
#line 256 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_def) = new tree_const_def();
            (yyval.const_def)->_line_no = yyline+1;
            (yyval.const_def)->id=*(yyvsp[-3].string);
            (yyval.const_def)->const_exp_list=std::shared_ptr<tree_const_exp_list>((yyvsp[-2].const_exp_list));
            (yyval.const_def)->const_init_val=std::shared_ptr<tree_const_init_val>((yyvsp[0].const_init_val));
        }
#line 1452 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 15: /* ConstDef: TIDENTIFIER "=" ConstInitVal  */
#line 264 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_def) = new tree_const_def();
            (yyval.const_def)->_line_no = yyline+1;
            (yyval.const_def)->id=*(yyvsp[-2].string);
            (yyval.const_def)->const_init_val=std::shared_ptr<tree_const_init_val>((yyvsp[0].const_init_val));
        }
#line 1463 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 16: /* ConstExpArrayList: "[" ConstExp "]"  */
#line 274 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_exp_list) = new tree_const_exp_list();
            (yyval.const_exp_list)->_line_no = yyline+1;
            (yyval.const_exp_list)->const_exp.push_back(std::shared_ptr<tree_const_exp>((yyvsp[-1].const_exp)));
        }
#line 1473 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 17: /* ConstExpArrayList: ConstExpArrayList "[" ConstExp "]"  */
#line 280 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyvsp[-3].const_exp_list)->const_exp.push_back(std::shared_ptr<tree_const_exp>((yyvsp[-1].const_exp)));
            (yyval.const_exp_list) = (yyvsp[-3].const_exp_list);
        }
#line 1482 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 18: /* ConstInitVal: ConstExp  */
#line 288 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_init_val) = new tree_const_init_val();
            (yyval.const_init_val)->_line_no = yyline+1;
            (yyval.const_init_val)->const_exp= std::shared_ptr<tree_const_exp>((yyvsp[0].const_exp)) ;
        }
#line 1492 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 19: /* ConstInitVal: "{" "}"  */
#line 294 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_init_val) = new tree_const_init_val();
            (yyval.const_init_val)->_line_no = yyline+1;
        }
#line 1501 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 20: /* ConstInitVal: "{" ConstInitVallist "}"  */
#line 299 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_init_val) = new tree_const_init_val();
            (yyval.const_init_val)->_line_no = yyline+1;
            (yyval.const_init_val)->const_val_list = std::shared_ptr<tree_const_val_list>((yyvsp[-1].const_init_val_list)) ;
        }
#line 1511 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 21: /* ConstInitVallist: ConstInitVal  */
#line 308 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_init_val_list) = new tree_const_val_list();
            (yyval.const_init_val_list)->_line_no = yyline+1;
            (yyval.const_init_val_list)->const_init_vals.push_back(std::shared_ptr<tree_const_init_val>((yyvsp[0].const_init_val)));
        }
#line 1521 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 22: /* ConstInitVallist: ConstInitVallist "," ConstInitVal  */
#line 314 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyvsp[-2].const_init_val_list)->const_init_vals.push_back(std::shared_ptr<tree_const_init_val>((yyvsp[0].const_init_val)));
            (yyval.const_init_val_list) = (yyvsp[-2].const_init_val_list);
        }
#line 1530 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 23: /* ConstExp: AddExp  */
#line 322 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.const_exp) = new tree_const_exp();
            (yyval.const_exp)->_line_no = yyline+1;
            (yyval.const_exp)->add_exp = std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 1540 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 24: /* VarDecl: BType VarDefList ";"  */
#line 331 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.var_decl) = new tree_var_decl();
            (yyval.var_decl)->_line_no = yyline+1;
            (yyval.var_decl)->b_type=std::shared_ptr<tree_basic_type>((yyvsp[-2].basic_type));
            (yyval.var_decl)->var_def_list=std::shared_ptr<tree_var_def_list>((yyvsp[-1].var_def_list));
        }
#line 1551 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 25: /* VarDefList: VarDef  */
#line 341 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.var_def_list) = new tree_var_def_list();
            (yyval.var_def_list)->_line_no = yyline+1;
            (yyval.var_def_list)->var_defs.push_back(std::shared_ptr<tree_var_def>((yyvsp[0].var_def)));
        }
#line 1561 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 26: /* VarDefList: VarDefList "," VarDef  */
#line 347 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyvsp[-2].var_def_list)->var_defs.push_back(std::shared_ptr<tree_var_def>((yyvsp[0].var_def)));
            (yyval.var_def_list) = (yyvsp[-2].var_def_list);
        }
#line 1570 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 27: /* VarDef: TIDENTIFIER  */
#line 355 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.var_def) = new tree_var_def();
            (yyval.var_def)->_line_no = yyline+1;
            (yyval.var_def)->id = *(yyvsp[0].string);
        }
#line 1580 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 28: /* VarDef: TIDENTIFIER "=" InitVal  */
#line 361 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.var_def) = new tree_var_def();
            (yyval.var_def)->_line_no = yyline+1;
            (yyval.var_def)->id = *(yyvsp[-2].string);
            (yyval.var_def)->init_val = std::shared_ptr<tree_init_val>((yyvsp[0].init_val));
        }
#line 1591 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 29: /* VarDef: TIDENTIFIER ArrayDef  */
#line 368 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.var_def) = new tree_var_def();
            (yyval.var_def)->_line_no = yyline+1;
            (yyval.var_def)->id = *(yyvsp[-1].string);
            (yyval.var_def)->array_def = std::shared_ptr<tree_arrray_def>((yyvsp[0].array_def));
        }
#line 1602 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 30: /* VarDef: TIDENTIFIER ArrayDef "=" InitVal  */
#line 375 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.var_def) = new tree_var_def();
            (yyval.var_def)->_line_no = yyline+1;
            (yyval.var_def)->id = *(yyvsp[-3].string);
            (yyval.var_def)->array_def = std::shared_ptr<tree_arrray_def>((yyvsp[-2].array_def));
            (yyval.var_def)->init_val = std::shared_ptr<tree_init_val>((yyvsp[0].init_val));
        }
#line 1614 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 31: /* ArrayDef: "[" ConstExp "]"  */
#line 386 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.array_def) = new tree_arrray_def();
            (yyval.array_def)->_line_no = yyline+1;
            (yyval.array_def)->const_exps.push_back(std::shared_ptr<tree_const_exp>((yyvsp[-1].const_exp)));
        }
#line 1624 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 32: /* ArrayDef: ArrayDef "[" ConstExp "]"  */
#line 392 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.array_def)->const_exps.push_back(std::shared_ptr<tree_const_exp>((yyvsp[-1].const_exp)));
            (yyval.array_def) = (yyvsp[-3].array_def);
        }
#line 1633 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 33: /* InitVal: Exp  */
#line 401 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.init_val) = new tree_init_val();
            (yyval.init_val)->_line_no = yyline+1;
            (yyval.init_val)->exp=std::shared_ptr<tree_exp>((yyvsp[0].exp));
        }
#line 1643 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 34: /* InitVal: "{" "}"  */
#line 406 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
              {
            (yyval.init_val) = new tree_init_val();
            (yyval.init_val)->_line_no = yyline+1;
    }
#line 1652 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 35: /* InitVal: "{" InitValList "}"  */
#line 410 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
                          {
            (yyval.init_val) = new tree_init_val();
            (yyval.init_val)->_line_no = yyline+1;
            (yyval.init_val)->init_val_list = std::shared_ptr<tree_init_val_list>((yyvsp[-1].init_val_list));
    }
#line 1662 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 36: /* InitValList: InitVal  */
#line 418 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
              {
        (yyval.init_val_list) = new tree_init_val_list();
        (yyval.init_val_list)->_line_no = yyline+1;
        (yyval.init_val_list)->init_vals.push_back(std::shared_ptr<tree_init_val>((yyvsp[0].init_val)));
    }
#line 1672 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 37: /* InitValList: InitValList "," InitVal  */
#line 423 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
                              {
        (yyvsp[-2].init_val_list)->init_vals.push_back(std::shared_ptr<tree_init_val>((yyvsp[0].init_val)));
        (yyval.init_val_list) = (yyvsp[-2].init_val_list);
    }
#line 1681 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 38: /* FuncDef: BType TIDENTIFIER "(" ")" Block  */
#line 432 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_def) = new tree_func_def();
            (yyval.func_def)->_line_no = yyline+1;
            (yyval.func_def)->type = std::shared_ptr<tree_basic_type>((yyvsp[-4].basic_type));
            (yyval.func_def)->id = *(yyvsp[-3].string);
            (yyval.func_def)->block.push_back(std::shared_ptr<tree_block>((yyvsp[0].block)));
        }
#line 1693 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 39: /* FuncDef: BType TIDENTIFIER "(" FuncFParams ")" Block  */
#line 440 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_def) = new tree_func_def();
            (yyval.func_def)->_line_no = yyline+1;
            (yyval.func_def)->type = std::shared_ptr<tree_basic_type>((yyvsp[-5].basic_type));
            (yyval.func_def)->id = *(yyvsp[-4].string);
            (yyval.func_def)->funcfparams = std::shared_ptr<tree_func_fparams>((yyvsp[-2].func_fparams));

            (yyval.func_def)->block.push_back(std::shared_ptr<tree_block>((yyvsp[0].block)));
        }
#line 1707 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 40: /* FuncFParams: FuncFParam  */
#line 453 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_fparams) = new tree_func_fparams();
            (yyval.func_fparams)->_line_no = yyline+1;
            (yyval.func_fparams)->funcfparamlist.push_back(std::shared_ptr<tree_func_fparam>((yyvsp[0].func_fparam)));
        }
#line 1717 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 41: /* FuncFParams: FuncFParams "," FuncFParam  */
#line 459 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyvsp[-2].func_fparams)->funcfparamlist.push_back(std::shared_ptr<tree_func_fparam>((yyvsp[0].func_fparam)));
            (yyval.func_fparams) = (yyvsp[-2].func_fparams);
        }
#line 1726 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 42: /* FuncFParam: FuncFParamOne  */
#line 467 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_fparam) = new tree_func_fparam();
            (yyval.func_fparam)->_line_no = yyline+1;
            (yyval.func_fparam)->funcfparamone = std::shared_ptr<tree_func_fparamone>((yyvsp[0].func_fparamone));
        }
#line 1736 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 43: /* FuncFParam: FuncFParamArray  */
#line 473 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_fparam) = new tree_func_fparam();
            (yyval.func_fparam)->_line_no = yyline+1;
            (yyval.func_fparam)->funcfparamarray = std::shared_ptr<tree_func_fparamarray>((yyvsp[0].func_fparamarray));
        }
#line 1746 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 44: /* FuncFParamOne: BType TIDENTIFIER  */
#line 482 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_fparamone) = new tree_func_fparamone();
            (yyval.func_fparamone)->_line_no = yyline+1;
            (yyval.func_fparamone)->b_type = std::shared_ptr<tree_basic_type>((yyvsp[-1].basic_type));
            (yyval.func_fparamone)->id = *(yyvsp[0].string);
        }
#line 1757 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 45: /* FuncFParamArray: BType TIDENTIFIER "[" "]"  */
#line 492 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_fparamarray) = new tree_func_fparamarray();
            (yyval.func_fparamarray)->_line_no = yyline+1;
            (yyval.func_fparamarray)->b_type = std::shared_ptr<tree_basic_type>((yyvsp[-3].basic_type));
            (yyval.func_fparamarray)->id = *(yyvsp[-2].string);
        }
#line 1768 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 46: /* FuncFParamArray: FuncFParamArray "[" Exp "]"  */
#line 499 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyvsp[-3].func_fparamarray)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[-1].exp)));
            (yyval.func_fparamarray) = (yyvsp[-3].func_fparamarray);
        }
#line 1777 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 47: /* Block: "{" "}"  */
#line 507 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.block) = new tree_block();
            (yyval.block)->_line_no = yyline+1;
        }
#line 1786 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 48: /* Block: "{" BlockItemList "}"  */
#line 512 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.block) = new tree_block();
            (yyval.block)->_line_no = yyline+1;
            (yyval.block)->block_item_list=std::shared_ptr<tree_block_item_list>((yyvsp[-1].block_item_list));
        }
#line 1796 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 49: /* BlockItemList: BlockItem  */
#line 521 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.block_item_list) = new tree_block_item_list();
            (yyval.block_item_list)->_line_no = yyline+1;
            (yyval.block_item_list)->block_items.push_back(std::shared_ptr<tree_block_item>((yyvsp[0].block_item)));
        }
#line 1806 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 50: /* BlockItemList: BlockItemList BlockItem  */
#line 527 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyvsp[-1].block_item_list)->block_items.push_back(std::shared_ptr<tree_block_item>((yyvsp[0].block_item)));
            (yyval.block_item_list)=(yyvsp[-1].block_item_list);
        }
#line 1815 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 51: /* BlockItem: Decl  */
#line 535 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.block_item) = new tree_block_item();
            (yyval.block_item)->_line_no = yyline+1;
            (yyval.block_item)->decl=std::shared_ptr<tree_decl>((yyvsp[0].decl));
        }
#line 1825 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 52: /* BlockItem: Stmt  */
#line 541 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.block_item) = new tree_block_item();
            (yyval.block_item)->_line_no = yyline+1;
            (yyval.block_item)->stmt=std::shared_ptr<tree_stmt>((yyvsp[0].stmt));
        }
#line 1835 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 53: /* Stmt: LVal "=" Exp ";"  */
#line 552 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto a_stmt = new tree_assign_stmt();
            a_stmt->l_val=std::shared_ptr<tree_l_val>((yyvsp[-3].l_val));
            a_stmt->exp=std::shared_ptr<tree_exp>((yyvsp[-1].exp));
            (yyval.stmt)->assigm_stmt=std::shared_ptr<tree_assign_stmt>(a_stmt) ;
        }
#line 1848 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 54: /* Stmt: ";"  */
#line 561 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
        }
#line 1857 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 55: /* Stmt: Exp ";"  */
#line 566 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            (yyval.stmt)->exp=std::shared_ptr<tree_exp>((yyvsp[-1].exp)) ;
        }
#line 1867 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 56: /* Stmt: Block  */
#line 572 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            (yyval.stmt)->block=std::shared_ptr<tree_block>((yyvsp[0].block)) ;
        }
#line 1877 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 57: /* Stmt: "if" "(" Cond ")" Stmt  */
#line 579 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto if_stmt = new tree_if_stmt();
            if_stmt->cond = std::shared_ptr<tree_cond>((yyvsp[-2].cond));
            if_stmt->stmt = std::shared_ptr<tree_stmt>((yyvsp[0].stmt));
            (yyval.stmt)->if_stmt = std::shared_ptr<tree_if_stmt>(if_stmt) ;
        }
#line 1890 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 58: /* Stmt: "if" "(" Cond ")" Stmt "else" Stmt  */
#line 588 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto if_else_stmt = new tree_if_else_stmt();
            if_else_stmt->cond = std::shared_ptr<tree_cond>((yyvsp[-4].cond));
            if_else_stmt->then_stmt = std::shared_ptr<tree_stmt>((yyvsp[-2].stmt));
            if_else_stmt->else_stmt = std::shared_ptr<tree_stmt>((yyvsp[0].stmt));
            (yyval.stmt)->if_else_stmt = std::shared_ptr<tree_if_else_stmt>(if_else_stmt) ;

        }
#line 1905 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 59: /* Stmt: "while" "(" Cond ")" Stmt  */
#line 600 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto while_stmt = new tree_while_stmt();
            while_stmt->cond = std::shared_ptr<tree_cond>((yyvsp[-2].cond));
            while_stmt->stmt = std::shared_ptr<tree_stmt>((yyvsp[0].stmt));
            (yyval.stmt)->while_stmt = std::shared_ptr<tree_while_stmt>(while_stmt) ;
        }
#line 1918 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 60: /* Stmt: "continue" ";"  */
#line 609 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto continue_stmt = new tree_continue_stmt();
            (yyval.stmt)->continue_stmt = std::shared_ptr<tree_continue_stmt>(continue_stmt) ;
        }
#line 1929 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 61: /* Stmt: "break" ";"  */
#line 616 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto break_stmt = new tree_break_stmt();
            (yyval.stmt)->break_stmt = std::shared_ptr<tree_break_stmt>(break_stmt) ;
        }
#line 1940 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 62: /* Stmt: "return" ";"  */
#line 624 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto a_stmt = new tree_return_null_stmt();
            (yyval.stmt)->return_null_stmt=std::shared_ptr<tree_return_null_stmt>(a_stmt) ;
        }
#line 1951 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 63: /* Stmt: "return" Exp ";"  */
#line 631 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.stmt) = new tree_stmt();
            (yyval.stmt)->_line_no = yyline+1;
            auto a_stmt = new tree_return_stmt();
            a_stmt->exp=std::shared_ptr<tree_exp>((yyvsp[-1].exp));
            (yyval.stmt)->return_stmt=std::shared_ptr<tree_return_stmt>(a_stmt) ;
        }
#line 1963 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 64: /* Exp: AddExp  */
#line 643 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.exp) = new tree_exp();
            (yyval.exp)->_line_no = yyline+1;
            (yyval.exp)->add_exp = std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 1973 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 65: /* Cond: LOrExp  */
#line 652 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.cond) = new tree_cond();
            (yyval.cond)->_line_no = yyline+1;
            (yyval.cond)->l_or_exp = std::shared_ptr<tree_l_or_exp>((yyvsp[0].l_or_exp));
        }
#line 1983 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 66: /* LVal: TIDENTIFIER  */
#line 661 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.l_val) = new tree_l_val();
            (yyval.l_val)->_line_no = yyline+1;
            (yyval.l_val)->id = *(yyvsp[0].string);
        }
#line 1993 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 67: /* LVal: ArrayIdent  */
#line 667 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.l_val) = new tree_l_val();
            (yyval.l_val)->_line_no = yyline+1;
            (yyval.l_val)->array_ident = std::shared_ptr<tree_array_ident>((yyvsp[0].array_ident));
        }
#line 2003 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 68: /* ArrayIdent: TIDENTIFIER "[" Exp "]"  */
#line 676 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.array_ident) = new tree_array_ident();
            (yyval.array_ident)->_line_no = yyline+1;
            (yyval.array_ident)->id = *(yyvsp[-3].string);
            (yyval.array_ident)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[-1].exp)));
        }
#line 2014 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 69: /* ArrayIdent: ArrayIdent "[" Exp "]"  */
#line 683 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyvsp[-3].array_ident)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[-1].exp)));
            (yyval.array_ident) = (yyvsp[-3].array_ident);
        }
#line 2023 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 70: /* Number: TINTEGER  */
#line 691 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.number) = new tree_number();
            (yyval.number)->_line_no = yyline+1;
            (yyval.number)->int_value = std::stoi((yyvsp[0].string)->c_str(), nullptr, 0);
            (yyval.number)->is_int=true;
        }
#line 2034 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 71: /* Number: TFLOATNUM  */
#line 698 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.number) = new tree_number();
            (yyval.number)->_line_no = yyline+1;
            (yyval.number)->float_value = (float)atof((yyvsp[0].string)->c_str());
            (yyval.number)->is_int=false;
        }
#line 2045 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 72: /* PrimaryExp: "(" Exp ")"  */
#line 708 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.primary_exp) = new tree_primary_exp();
            (yyval.primary_exp)->_line_no = yyline+1;
            (yyval.primary_exp)->exp = std::shared_ptr<tree_exp>((yyvsp[-1].exp));
        }
#line 2055 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 73: /* PrimaryExp: LVal  */
#line 714 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.primary_exp) = new tree_primary_exp();
            (yyval.primary_exp)->_line_no = yyline+1;
            (yyval.primary_exp)->l_val = std::shared_ptr<tree_l_val>((yyvsp[0].l_val));
        }
#line 2065 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 74: /* PrimaryExp: Number  */
#line 720 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.primary_exp) = new tree_primary_exp();
            (yyval.primary_exp)->_line_no = yyline+1;
            (yyval.primary_exp)->number = std::shared_ptr<tree_number>((yyvsp[0].number));
        }
#line 2075 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 75: /* UnaryExp: PrimaryExp  */
#line 729 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->primary_exp = std::shared_ptr<tree_primary_exp>((yyvsp[0].primary_exp));
        }
#line 2085 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 76: /* UnaryExp: "+" UnaryExp  */
#line 735 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.unary_exp)->oprt="+";
        }
#line 2096 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 77: /* UnaryExp: "-" UnaryExp  */
#line 742 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.unary_exp)->oprt="-";
        }
#line 2107 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 78: /* UnaryExp: "!" UnaryExp  */
#line 749 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.unary_exp)->oprt="!";
        }
#line 2118 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 79: /* UnaryExp: FuncCall  */
#line 757 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.unary_exp) = new tree_unary_exp();
            (yyval.unary_exp)->_line_no = yyline+1;
            (yyval.unary_exp)->func_call = std::shared_ptr<tree_func_call>((yyvsp[0].func_call));
        }
#line 2128 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 80: /* FuncCall: TIDENTIFIER "(" ")"  */
#line 766 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_call) = new tree_func_call();
            (yyval.func_call)->_line_no = yyline+1;
            (yyval.func_call)->id = *(yyvsp[-2].string);
        }
#line 2138 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 81: /* FuncCall: TIDENTIFIER "(" FuncParamList ")"  */
#line 772 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_call) = new tree_func_call();
            (yyval.func_call)->_line_no = yyline+1;
            (yyval.func_call)->id = *(yyvsp[-3].string);
            (yyval.func_call)->func_param_list = std::shared_ptr<tree_func_paramlist>((yyvsp[-1].func_param_list));
        }
#line 2149 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 82: /* FuncParamList: Exp  */
#line 782 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.func_param_list) = new tree_func_paramlist();
            (yyval.func_param_list)->_line_no = yyline+1;
            (yyval.func_param_list)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[0].exp)));
        }
#line 2159 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 83: /* FuncParamList: FuncParamList "," Exp  */
#line 788 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyvsp[-2].func_param_list)->exps.push_back(std::shared_ptr<tree_exp>((yyvsp[0].exp)));
            (yyval.func_param_list) = (yyvsp[-2].func_param_list);
        }
#line 2168 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 84: /* MulExp: UnaryExp  */
#line 798 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.mul_exp) = new tree_mul_exp();
            (yyval.mul_exp)->_line_no = yyline+1;
            (yyval.mul_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
        }
#line 2178 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 85: /* MulExp: MulExp "*" UnaryExp  */
#line 804 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.mul_exp) = new tree_mul_exp();
            (yyval.mul_exp)->_line_no = yyline+1;
            (yyval.mul_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[-2].mul_exp));
            (yyval.mul_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.mul_exp)->oprt="*";
        }
#line 2190 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 86: /* MulExp: MulExp "/" UnaryExp  */
#line 812 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.mul_exp) = new tree_mul_exp();
            (yyval.mul_exp)->_line_no = yyline+1;
            (yyval.mul_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[-2].mul_exp));
            (yyval.mul_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.mul_exp)->oprt="/";
        }
#line 2202 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 87: /* MulExp: MulExp "%" UnaryExp  */
#line 820 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.mul_exp) = new tree_mul_exp();
            (yyval.mul_exp)->_line_no = yyline+1;
            (yyval.mul_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[-2].mul_exp));
            (yyval.mul_exp)->unary_exp=std::shared_ptr<tree_unary_exp>((yyvsp[0].unary_exp));
            (yyval.mul_exp)->oprt="%";
        }
#line 2214 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 88: /* AddExp: MulExp  */
#line 830 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.add_exp) = new tree_add_exp();
            (yyval.add_exp)->_line_no = yyline+1;
            (yyval.add_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[0].mul_exp));
        }
#line 2224 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 89: /* AddExp: AddExp "+" MulExp  */
#line 836 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.add_exp) = new tree_add_exp();
            (yyval.add_exp)->_line_no = yyline+1;
            (yyval.add_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[-2].add_exp));
            (yyval.add_exp)->oprt="+";
            (yyval.add_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[0].mul_exp));
        }
#line 2236 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 90: /* AddExp: AddExp "-" MulExp  */
#line 844 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.add_exp) = new tree_add_exp();
            (yyval.add_exp)->_line_no = yyline+1;
            (yyval.add_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[-2].add_exp));
            (yyval.add_exp)->oprt="-";
            (yyval.add_exp)->mul_exp=std::shared_ptr<tree_mul_exp>((yyvsp[0].mul_exp));
        }
#line 2248 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 91: /* RelExp: AddExp  */
#line 854 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2258 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 92: /* RelExp: RelExp "<" AddExp  */
#line 860 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[-2].rel_exp));
            (yyval.rel_exp)->oprt="<";
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2270 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 93: /* RelExp: RelExp ">" AddExp  */
#line 868 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[-2].rel_exp));
            (yyval.rel_exp)->oprt=">";
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2282 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 94: /* RelExp: RelExp "<=" AddExp  */
#line 876 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[-2].rel_exp));
            (yyval.rel_exp)->oprt="<=";
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2294 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 95: /* RelExp: RelExp ">=" AddExp  */
#line 884 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.rel_exp) = new tree_rel_exp();
            (yyval.rel_exp)->_line_no = yyline+1;
            (yyval.rel_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[-2].rel_exp));
            (yyval.rel_exp)->oprt=">=";
            (yyval.rel_exp)->add_exp=std::shared_ptr<tree_add_exp>((yyvsp[0].add_exp));
        }
#line 2306 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 96: /* EqExp: RelExp  */
#line 894 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.eq_exp) = new tree_eq_exp();
            (yyval.eq_exp)->_line_no = yyline+1;
            (yyval.eq_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[0].rel_exp));
        }
#line 2316 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 97: /* EqExp: EqExp "==" RelExp  */
#line 900 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.eq_exp) = new tree_eq_exp();
            (yyval.eq_exp)->_line_no = yyline+1;
            (yyval.eq_exp)->eq_exp=std::shared_ptr<tree_eq_exp>((yyvsp[-2].eq_exp));
            (yyval.eq_exp)->oprt="==";
            (yyval.eq_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[0].rel_exp));
        }
#line 2328 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 98: /* EqExp: EqExp "!=" RelExp  */
#line 908 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.eq_exp) = new tree_eq_exp();
            (yyval.eq_exp)->_line_no = yyline+1;
            (yyval.eq_exp)->eq_exp=std::shared_ptr<tree_eq_exp>((yyvsp[-2].eq_exp));
            (yyval.eq_exp)->oprt="!=";
            (yyval.eq_exp)->rel_exp=std::shared_ptr<tree_rel_exp>((yyvsp[0].rel_exp));
        }
#line 2340 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 99: /* LAndExp: EqExp  */
#line 918 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.l_and_exp) = new tree_l_and_exp();
            (yyval.l_and_exp)->_line_no = yyline+1;
            (yyval.l_and_exp)->eq_exp=std::shared_ptr<tree_eq_exp>((yyvsp[0].eq_exp));
        }
#line 2350 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 100: /* LAndExp: LAndExp "&&" EqExp  */
#line 924 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.l_and_exp) = new tree_l_and_exp();
            (yyval.l_and_exp)->_line_no = yyline+1;
            (yyval.l_and_exp)->l_and_exp=std::shared_ptr<tree_l_and_exp>((yyvsp[-2].l_and_exp));
            (yyval.l_and_exp)->eq_exp=std::shared_ptr<tree_eq_exp>((yyvsp[0].eq_exp));
        }
#line 2361 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 101: /* LOrExp: LAndExp  */
#line 933 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.l_or_exp) = new tree_l_or_exp();
            (yyval.l_or_exp)->_line_no = yyline+1;
            (yyval.l_or_exp)->l_and_exp=std::shared_ptr<tree_l_and_exp>((yyvsp[0].l_and_exp));
        }
#line 2371 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;

  case 102: /* LOrExp: LOrExp "||" LAndExp  */
#line 939 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"
        {
            (yyval.l_or_exp) = new tree_l_or_exp();
            (yyval.l_or_exp)->_line_no = yyline+1;
            (yyval.l_or_exp)->l_or_exp=std::shared_ptr<tree_l_or_exp>((yyvsp[-2].l_or_exp));
            (yyval.l_or_exp)->l_and_exp=std::shared_ptr<tree_l_and_exp>((yyvsp[0].l_and_exp));
        }
#line 2382 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"
    break;


#line 2386 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 946 "/home/qiwu/文档/Compliers/carolcc/src/frontend/parser.yy"

void insertVarible(std::string& type,std::string& id){
    VaribleTable.insert(std::make_pair<std::string, VaribleInfo>(std::string(id),VaribleInfo(type)));
}
void insertFunction(std::string& type,std::string& id){
    FunctionTable.insert(
        std::make_pair<std::string, FunctionInfo>
            (std::string(id),FunctionInfo(type,VaribleTable)));
    VaribleTable.clear();
}
