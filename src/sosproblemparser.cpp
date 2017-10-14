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
#line 1 "src/sosproblemparser.y" /* yacc.c:339  */

#include "psdproblem.h"
#include "sosproblemparser.h"
#include "psdtype.h"
#include "psdutil.h"
#include <cstdio>
#include<iostream>
  using namespace  aiSat::psd;
  int yydebug=1;   
  using std::cerr;
  using std::endl;

  SOSP problem;

  string unkown_name;

  extern  int lineNum;

  extern int yyerror(const char *);
  extern int yyerror(string);
  extern int yylex();
  extern int yyparse();
  //  extern int yywrap();
  extern FILE* yyin;
  //  extern SOSP problem;
 
  bool err;
  

#line 96 "src/sosproblemparser.cpp" /* yacc.c:339  */

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
    NUM = 258,
    IDENT = 259,
    INTEGER = 260,
    MONOMIAL = 261,
    VAR = 262,
    POLY = 263,
    VAR_LIST = 264,
    UNHNOW = 265,
    SOSEQ = 266,
    SOSGEQ = 267,
    SOSLEQ = 268,
    SOSGE = 269,
    SOSLE = 270,
    SOS = 271,
    EQPOLY = 272,
    TOTAL = 273,
    MONO = 274,
    FINDSOS = 275,
    INTERP = 276
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 30 "src/sosproblemparser.y" /* yacc.c:355  */

  
  double dblVal;
  int  dilVal; 
  string * identifier;
  Poly_t *poly;
  vector<int> *iVec;
  vector<SOSMonomial*> *mVec;
  SOSMonomial * mon;
  PolyConstraint* polycons;
  SOSProblem *prob;
 

#line 169 "src/sosproblemparser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 186 "src/sosproblemparser.cpp" /* yacc.c:358  */

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
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   253

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  158

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   276

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
      32,    34,    24,    22,    33,    23,     2,    25,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    29,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    27,     2,    28,    26,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,    31,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    89,    89,    95,    97,   100,   104,   106,   108,   110,
     115,   117,   126,   140,   155,   165,   174,   186,   188,   192,
     197,   206,   212,   221,   224,   228,   238,   246,   260,   270,
     280,   288,   297,   303,   311,   321,   327,   335,   344,   353,
     361,   368,   376,   383,   397,   400,   405,   411,   420,   426,
     435,   441,   448,   455,   462,   480,   509,   542,   584,   592,
     627,   640,   651,   667,   679,   693,   706,   718,   732,   736,
     746,   758,   775,   787,   799,   811,   824,   835,   848,   860,
     871,   875,   886,   898
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "IDENT", "INTEGER", "MONOMIAL",
  "VAR", "POLY", "VAR_LIST", "UNHNOW", "SOSEQ", "SOSGEQ", "SOSLEQ",
  "SOSGE", "SOSLE", "SOS", "EQPOLY", "TOTAL", "MONO", "FINDSOS", "INTERP",
  "'+'", "'-'", "'*'", "'/'", "'^'", "'['", "']'", "'='", "'{'", "'}'",
  "'('", "','", "')'", "$accept", "program", "statement_list",
  "declar_statement", "var_declar", "monomial_declar", "constant_poly",
  "unknow_poly", "int_list", "int_list1", "int_list2", "var_list",
  "var_list1", "var_list2", "poly", "monomial_list", "monomial_list1",
  "monomial_list2", "monomial", "constraint_statement", "constraint_left",
  "constraint_right", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,    43,    45,    42,    47,    94,    91,    93,    61,
     123,   125,    40,    44,    41
};
# endif

#define YYPACT_NINF -52

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-52)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       9,   -20,    24,    29,    14,   -52,   -52,   -52,   -52,   -52,
      57,    20,    30,     5,    42,    38,   -52,   -52,   -52,   -52,
     -52,    61,    85,   102,     0,   -52,    55,   -52,    91,   117,
     127,   130,    24,    -3,    63,   210,    76,    94,   -52,   112,
     -52,   -52,    63,    63,    83,   175,   -11,    63,    63,    89,
     146,   104,    83,   115,   121,   171,   177,   155,   140,   -52,
     148,   160,   163,    -3,    28,   180,    63,   -52,   -52,   -52,
     -52,   -52,   -52,   -10,   196,   181,    83,   215,     4,   -52,
     -52,   -10,   -10,   -52,   182,   -52,    63,   210,   158,   183,
     106,   186,   114,   178,   187,   193,   -52,   208,   -52,    -3,
     191,   209,    -3,   -52,    63,   201,    33,   134,   147,   153,
      83,   210,    63,   162,    63,   168,   -52,   -52,   202,   -52,
     191,   205,   191,   -10,   -52,   -52,   204,   119,   216,   122,
      35,   -10,   -52,   -10,   -52,   237,   238,    63,   179,    63,
     185,   -52,   211,   217,   213,   242,   -52,   -10,   -52,   -10,
     -52,   243,   218,   244,   -52,   -52,   -52,   -52
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     5,     6,     7,     8,     9,
       0,    33,    34,     0,    23,    24,     1,    42,    41,    43,
      40,    71,     0,     0,     0,     3,     0,     4,     0,     0,
       0,     0,     0,     0,     0,    14,     0,     0,    10,     0,
      31,    32,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      53,     0,    44,    45,    49,     0,     0,    27,    29,    28,
      30,    25,    26,    70,     0,    83,     0,     0,     0,    35,
      68,    36,    37,    69,    38,    39,     0,    55,    57,    66,
      64,    67,    65,     0,     0,     0,    11,     0,    12,     0,
      48,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,    15,    16,     0,    52,
      46,    51,    47,    82,    80,    81,    78,    76,    79,    77,
       0,    61,    60,    63,    62,     0,     0,     0,     0,     0,
       0,    59,    22,     0,    17,    18,    50,    73,    72,    75,
      74,     0,     0,     0,    21,    20,    13,    19
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -52,   -52,   -52,   246,   -52,   -52,   -52,   -52,   -52,   -52,
     -52,   219,   -52,   -52,    -4,   -52,   -52,   -52,   -48,   -52,
     229,   -51
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,     7,     8,     9,   143,   144,
     145,    13,    14,    15,    84,    61,    62,    63,    64,    27,
      28,    78
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      26,    88,    59,    17,    60,    18,    35,    19,    20,    10,
      21,    53,    54,     1,    66,   100,    50,    17,     1,    18,
      45,    19,    20,    80,    21,   106,   108,   109,    11,    16,
      65,    12,    24,    38,    22,    23,     2,   110,    73,    74,
      77,     2,    40,    81,    82,    41,    24,    87,    77,    90,
      92,   120,   101,    36,   122,   108,   109,   108,   109,   130,
      17,   102,    18,    37,    19,    20,    17,   124,    18,   141,
      19,    20,   105,    29,    30,    39,    31,    47,    48,    49,
      67,    50,   111,    68,    32,    42,    17,    33,    18,    34,
      19,    20,    17,    75,    18,    34,    19,    20,    69,    83,
     123,    70,    51,    52,   127,   129,    77,    17,   131,    18,
     133,    19,    20,    53,    54,    76,    71,    43,    17,    72,
      18,    34,    19,    20,    17,    89,    18,    86,    19,    20,
     113,    91,    50,   147,    44,   149,    34,    17,   115,    18,
      50,    19,    20,   138,   125,    50,   140,    34,    50,    55,
      17,    85,    18,    34,    19,    20,    17,   126,    18,    56,
      19,    20,    57,   128,    95,    17,    34,    18,    96,    19,
      20,    17,   132,    18,    97,    19,    20,    93,   134,    34,
     108,   109,    17,    94,    18,    34,    19,    20,    17,   148,
      18,    98,    19,    20,    34,   150,    99,    47,    48,    49,
      34,    50,    47,    48,    66,   104,    50,   112,    50,    79,
     114,    34,   116,   119,    79,   101,   121,    34,    47,    48,
      66,   117,    50,    47,    48,   107,   118,    50,   137,   135,
     103,   136,    47,    48,    66,    79,    50,    47,    48,   107,
     139,    50,   142,   146,   151,   152,   153,   154,   155,   157,
      25,    58,   156,    46
};

static const yytype_uint8 yycheck[] =
{
       4,    52,     5,     3,     7,     5,    10,     7,     8,    29,
      10,    22,    23,     4,    24,    63,    26,     3,     4,     5,
      24,     7,     8,    34,    10,    76,    22,    23,     4,     0,
      34,     7,    32,    28,    20,    21,    27,    33,    42,    43,
      44,    27,     4,    47,    48,     7,    32,    51,    52,    53,
      54,    99,    24,    33,   102,    22,    23,    22,    23,   110,
       3,    33,     5,    33,     7,     8,     3,    34,     5,    34,
       7,     8,    76,    16,    17,    33,    19,    22,    23,    24,
       4,    26,    86,     7,    27,    24,     3,    30,     5,    32,
       7,     8,     3,    10,     5,    32,     7,     8,     4,    10,
     104,     7,    11,    12,   108,   109,   110,     3,   112,     5,
     114,     7,     8,    22,    23,    32,     4,    32,     3,     7,
       5,    32,     7,     8,     3,    10,     5,    23,     7,     8,
      24,    10,    26,   137,    32,   139,    32,     3,    24,     5,
      26,     7,     8,    24,    10,    26,    24,    32,    26,    32,
       3,     5,     5,    32,     7,     8,     3,    10,     5,    32,
       7,     8,    32,    10,     9,     3,    32,     5,    28,     7,
       8,     3,    10,     5,    26,     7,     8,     6,    10,    32,
      22,    23,     3,     6,     5,    32,     7,     8,     3,    10,
       5,    31,     7,     8,    32,    10,    33,    22,    23,    24,
      32,    26,    22,    23,    24,    24,    26,    24,    26,    34,
      24,    32,    34,     5,    34,    24,     7,    32,    22,    23,
      24,    34,    26,    22,    23,    24,    33,    26,    24,    27,
      34,    26,    22,    23,    24,    34,    26,    22,    23,    24,
      24,    26,     5,     5,    33,    28,    33,     5,     5,     5,
       4,    32,    34,    24
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    27,    36,    37,    38,    39,    40,    41,    42,
      29,     4,     7,    46,    47,    48,     0,     3,     5,     7,
       8,    10,    20,    21,    32,    38,    49,    54,    55,    16,
      17,    19,    27,    30,    32,    49,    33,    33,    28,    33,
       4,     7,    24,    32,    32,    49,    55,    22,    23,    24,
      26,    11,    12,    22,    23,    32,    32,    32,    46,     5,
       7,    50,    51,    52,    53,    49,    24,     4,     7,     4,
       7,     4,     7,    49,    49,    10,    32,    49,    56,    34,
      34,    49,    49,    10,    49,     5,    23,    49,    56,    10,
      49,    10,    49,     6,     6,     9,    28,    26,    31,    33,
      53,    24,    33,    34,    24,    49,    56,    24,    22,    23,
      33,    49,    24,    24,    24,    24,    34,    34,    33,     5,
      53,     7,    53,    49,    34,    10,    10,    49,    10,    49,
      56,    49,    10,    49,    10,    27,    26,    24,    24,    24,
      24,    34,     5,    43,    44,    45,     5,    49,    10,    49,
      10,    33,    28,    33,     5,     5,    34,     5
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    36,    37,    37,    37,    38,    38,    38,    38,
      39,    39,    40,    40,    41,    42,    42,    43,    43,    44,
      44,    45,    45,    46,    46,    47,    47,    47,    47,    47,
      47,    48,    48,    48,    48,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    50,    50,    51,    51,    52,    52,
      53,    53,    53,    53,    53,    54,    54,    54,    54,    54,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     1,     1,     1,     1,     1,
       3,     5,     5,    10,     3,     6,     6,     1,     1,     3,
       3,     2,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     2,     2,     1,     1,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     1,
       5,     3,     3,     1,     1,     3,     4,     3,     4,     6,
       5,     5,     5,     5,     3,     3,     3,     3,     3,     3,
       3,     1,     5,     5,     5,     5,     3,     3,     3,     3,
       3,     3,     3,     1
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
        case 11:
#line 118 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  problem.varMap[ *((yyvsp[-4].identifier)) ]=(yyvsp[-1].iVec);
  delete (yyvsp[-4].identifier);
}
#line 1390 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 127 "src/sosproblemparser.y" /* yacc.c:1646  */
    {

  problem.monoMap[*((yyvsp[-4].identifier))]=SOSP::addSOSsup( (yyvsp[-1].mVec));
  delete (yyvsp[-4].identifier);
  
  vector<SOSMonomial*>::iterator it=(yyvsp[-1].mVec)->begin();
  for(;it!=(yyvsp[-1].mVec)->end();it++){
    delete (*it);
  }
  delete (yyvsp[-1].mVec);
  
}
#line 1407 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 141 "src/sosproblemparser.y" /* yacc.c:1646  */
    {

  problem.monoMap[*((yyvsp[-9].identifier))]=SOSP::addSOSsup( (yyvsp[-5].iVec), (yyvsp[-2].iVec));
  

  delete (yyvsp[-9].identifier);

  delete (yyvsp[-2].iVec);
}
#line 1421 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 157 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  problem.polyMap[ *((yyvsp[-2].identifier))]= (yyvsp[0].poly) ;
  delete (yyvsp[-2].identifier);
}
#line 1430 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 166 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.polycons)=new PolyConstraint();
  (yyval.polycons)->type=GE;
  (yyval.polycons)->supportId= (yyvsp[-1].dilVal);
  problem.polyConsMap[*((yyvsp[-5].identifier))]=(yyval.polycons);
  delete (yyvsp[-5].identifier);
}
#line 1442 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 175 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.polycons)=new PolyConstraint();
  (yyval.polycons)->type=EQ;
  (yyval.polycons)->supportId=(yyvsp[-1].dilVal);
  problem.polyConsMap[*((yyvsp[-5].identifier))]=(yyval.polycons);
  delete (yyvsp[-5].identifier);
}
#line 1454 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 186 "src/sosproblemparser.y" /* yacc.c:1646  */
    {(yyval.iVec)=(yyvsp[0].iVec); }
#line 1460 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 188 "src/sosproblemparser.y" /* yacc.c:1646  */
    {(yyval.iVec)=(yyvsp[0].iVec); }
#line 1466 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 193 "src/sosproblemparser.y" /* yacc.c:1646  */
    { (yyvsp[-2].iVec)->push_back((yyvsp[0].dilVal));
  (yyval.iVec)=(yyvsp[-2].iVec);
}
#line 1474 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 198 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.iVec)=new vector<int>();
  (yyval.iVec)->push_back((yyvsp[-2].dilVal));
  (yyval.iVec)->push_back((yyvsp[0].dilVal));
}
#line 1484 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 207 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[-1].iVec)->push_back((yyvsp[0].dilVal));
  (yyval.iVec)=(yyvsp[-1].iVec);
}
#line 1493 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 213 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.iVec)=new vector<int>();
  (yyval.iVec)->push_back((yyvsp[0].dilVal));
}
#line 1502 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 222 "src/sosproblemparser.y" /* yacc.c:1646  */
    {(yyval.iVec)=(yyvsp[0].iVec);}
#line 1508 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 225 "src/sosproblemparser.y" /* yacc.c:1646  */
    {(yyval.iVec)=(yyvsp[0].iVec);}
#line 1514 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 229 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  getVarTable<indice_t>().addVar((yyvsp[0].identifier));
  
  int varIndex=getVarTable<indice_t>().findVarIndex((yyvsp[0].identifier));
  (yyvsp[-2].iVec)->push_back(varIndex);
  delete (yyvsp[0].identifier);
  (yyval.iVec)=(yyvsp[-2].iVec);
}
#line 1527 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 239 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].iVec)->push_back((yyvsp[0].dilVal));
  
  (yyval.iVec)=(yyvsp[-2].iVec);
  
}
#line 1538 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 246 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  getVarTable<indice_t>().addVar((yyvsp[-2].identifier));
  getVarTable<indice_t>().addVar((yyvsp[0].identifier));


  (yyval.iVec)=new vector<int>();
  int varIndex=getVarTable<indice_t>().findVarIndex((yyvsp[-2].identifier));
  (yyval.iVec)->push_back(varIndex );
  varIndex=getVarTable<indice_t>().findVarIndex((yyvsp[0].identifier));
  (yyval.iVec)->push_back(varIndex);
  delete (yyvsp[-2].identifier);
  delete (yyvsp[0].identifier);
}
#line 1556 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 260 "src/sosproblemparser.y" /* yacc.c:1646  */
    {

  getVarTable<indice_t>().addVar((yyvsp[0].identifier) );
  (yyval.iVec)=new vector<int>();
  (yyval.iVec)->push_back((yyvsp[-2].dilVal) );
  int  varIndex=getVarTable<indice_t>().findVarIndex((yyvsp[0].identifier));
  (yyval.iVec)->push_back(varIndex);
  delete (yyvsp[0].identifier);
}
#line 1570 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 270 "src/sosproblemparser.y" /* yacc.c:1646  */
    {

  getVarTable<indice_t>().addVar((yyvsp[-2].identifier) );
  (yyval.iVec)=new vector<int>();
  int  varIndex=getVarTable<indice_t>().findVarIndex((yyvsp[-2].identifier));
  (yyval.iVec)->push_back(varIndex);
  (yyval.iVec)->push_back((yyvsp[0].dilVal) );
  delete (yyvsp[-2].identifier);
}
#line 1584 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 280 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.iVec)=new vector<int>();
  (yyval.iVec)->push_back((yyvsp[-2].dilVal));
  (yyval.iVec)->push_back((yyvsp[0].dilVal) );
  
}
#line 1595 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 289 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  getVarTable<indice_t>().addVar((yyvsp[0].identifier) );
  int varIndex=getVarTable<indice_t>().findVarIndex((yyvsp[0].identifier));
  (yyvsp[-1].iVec)->push_back(varIndex);
  delete (yyvsp[0].identifier);
  (yyval.iVec)=(yyvsp[-1].iVec);
}
#line 1607 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 298 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[-1].iVec)->push_back((yyvsp[0].dilVal));
  (yyval.iVec)=(yyvsp[-1].iVec);
}
#line 1616 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 303 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  getVarTable<indice_t>().addVar((yyvsp[0].identifier) );
  int varIndex=getVarTable<indice_t>().findVarIndex((yyvsp[0].identifier));
  (yyval.iVec)=new vector<int>();
  (yyval.iVec)->push_back(varIndex);
  delete (yyvsp[0].identifier);
}
#line 1628 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 311 "src/sosproblemparser.y" /* yacc.c:1646  */
    {

  (yyval.iVec)=new vector<int>();
  (yyval.iVec)->push_back((yyvsp[0].dilVal));
  
}
#line 1639 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 322 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.poly)=(yyvsp[-1].poly);
}
#line 1647 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 328 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].poly)->add_poly((yyvsp[0].poly));
  (yyval.poly)=(yyvsp[-2].poly);
  delete (yyvsp[0].poly);

}
#line 1658 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 336 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[0].poly)->mult(-1);
  (yyvsp[-2].poly)->add_poly((yyvsp[0].poly));
  (yyval.poly)=(yyvsp[-2].poly);
  delete (yyvsp[0].poly);
  
}
#line 1670 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 345 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].poly)->mult_poly((yyvsp[0].poly));

  (yyval.poly)=(yyvsp[-2].poly);
  delete (yyvsp[0].poly);

}
#line 1682 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 354 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].poly)->pow((yyvsp[0].dilVal));
  (yyval.poly)=(yyvsp[-2].poly);

}
#line 1692 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 362 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.poly)=new Poly_t(*((yyvsp[0].poly)));
}
#line 1700 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 369 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.poly)=new Poly_t(true);

  (yyval.poly)->add((yyvsp[0].dilVal));
}
#line 1710 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 376 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.poly)=new Poly_t(true);
  (yyval.poly)->add((yyvsp[0].dblVal));


}
#line 1721 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 384 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.poly)=new Poly_t(true);

  Poly_t::Term term;
  term.key.push_back(make_pair((yyvsp[0].dilVal),1));
  term.cf=1;
  (yyval.poly)->add_term(term);

}
#line 1735 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 398 "src/sosproblemparser.y" /* yacc.c:1646  */
    {(yyval.mVec)=(yyvsp[0].mVec); }
#line 1741 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 401 "src/sosproblemparser.y" /* yacc.c:1646  */
    {(yyval.mVec)=(yyvsp[0].mVec); }
#line 1747 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 406 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[-2].mVec)->push_back((yyvsp[0].mon));
  (yyval.mVec)=(yyvsp[-2].mVec);
}
#line 1756 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 412 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.mVec)=new vector<SOSMonomial*>();
  (yyval.mVec)->push_back((yyvsp[-2].mon));
  (yyval.mVec)->push_back((yyvsp[0].mon));
}
#line 1766 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 421 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[-1].mVec)->push_back((yyvsp[0].mon));
  (yyval.mVec)=(yyvsp[-1].mVec);
}
#line 1775 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 427 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.mVec)=new vector<SOSMonomial*>();
  (yyval.mVec)->push_back((yyvsp[0].mon));

}
#line 1785 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 436 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[-4].mon)->addIndices((yyvsp[-2].dilVal),(yyvsp[0].dilVal));
  (yyval.mon)=(yyvsp[-4].mon);
}
#line 1794 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 442 "src/sosproblemparser.y" /* yacc.c:1646  */
    {

  (yyvsp[-2].mon)->addIndices((yyvsp[0].dilVal),1);
  (yyval.mon)=(yyvsp[-2].mon);
}
#line 1804 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 448 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  int varNum=getVarTable<indice_t>().getAllVarNum();
  (yyval.mon)=new SOSMonomial(varNum);

  (yyval.mon)->addIndices((yyvsp[-2].dilVal),(yyvsp[0].dilVal));
}
#line 1815 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 456 "src/sosproblemparser.y" /* yacc.c:1646  */
    { 
  int varNum=getVarTable<indice_t>().getAllVarNum();
  (yyval.mon)=new SOSMonomial(varNum);
  (yyval.mon)->addIndices((yyvsp[0].dilVal),1);
}
#line 1825 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 462 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  int varNum=getVarTable<indice_t>().getAllVarNum();
  (yyval.mon)=new SOSMonomial(varNum);
}
#line 1834 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 481 "src/sosproblemparser.y" /* yacc.c:1646  */
    {

  (yyval.prob)=(yyvsp[-2].prob);

  
  if(NULL!=(yyval.prob)->getRhs()){
    
    if(NULL!= (yyvsp[0].poly)){
      (yyval.prob)->getRhs()->add_poly((yyvsp[0].poly));
      delete (yyvsp[0].poly);
    }
  }else{

    (yyval.prob)->setRhs( (yyvsp[0].poly));
  }
  
  
  if(problem.problem->size()>0){

    problem.solve();

  }
  else{
    yyerror(" constraints error");
  }
  
}
#line 1866 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 510 "src/sosproblemparser.y" /* yacc.c:1646  */
    {

  (yyval.prob)=(yyvsp[-3].prob);
  Poly_t *rhs=(yyval.prob)->getRhs();

  if(NULL!=rhs){
    
    if(NULL!= (yyvsp[0].poly)){
      (yyvsp[0].poly)->mult(-1);

      rhs->add_poly((yyvsp[0].poly));
      delete (yyvsp[0].poly);
    }

  }else{
    (yyvsp[0].poly)->mult(-1);
    (yyval.prob)->setRhs( (yyvsp[0].poly));
  }
  
  
  if(problem.problem->size()>0){

    problem.solve();

  }
  else{
    yyerror(" constraints error");
  }
  
}
#line 1901 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 543 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  
  (yyval.prob)=(yyvsp[-2].prob);

  Poly_t *lrhs=(yyval.prob)->getRhs();

  Poly_t *rrhs=(yyvsp[0].prob)->getRhs();
  
  if(NULL!=lrhs){
    
    if(NULL!= rrhs){
      lrhs->add_poly(rrhs);
    }

  }else{
    if(NULL!=rrhs){
      (yyval.prob)->setRhs(rrhs);
    }
  }
  
  int i;
  int left=problem.left_printMap.size();
  
  for(i=0;i< (yyvsp[0].prob)->size(); i++){
    (yyval.prob)->addElem((yyvsp[0].prob)->getPoly(i), (yyvsp[0].prob)->getPolyConstraint(i) );

    problem.left_printMap[left+i]=problem.right_printMap[i];
  }
  problem.right_printMap.clear();

  delete (yyvsp[0].prob);

  if(problem.problem->size()>0){
    problem.solve();

  }
  else{
    yyerror(" constraints error");
  }
}
#line 1946 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 585 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  problem.findSOS((yyvsp[-1].poly));
  delete (yyvsp[-1].poly);

}
#line 1956 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 593 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  int sep= (yyvsp[-3].prob)->size()+1;
  int i;
  for(i=0; i<(yyvsp[-1].prob)->size(); i++){
    
    (yyvsp[-3].prob)->addElem( (yyvsp[-1].prob)->getPoly(i), (yyvsp[-1].prob)->getPolyConstraint(i) );
  }
  for(i=0; i<(yyvsp[-3].prob)->size(); i++){
    (yyvsp[-3].prob)->getPoly(i)->mult(-1);

  }
  
  if(NULL==(yyvsp[-3].prob)->getRhs()){
    (yyvsp[-3].prob)->setRhs(new Poly_t());
  }
  if(NULL!=(yyvsp[-1].prob)->getRhs()){
    (yyvsp[-3].prob)->getRhs()->add_poly((yyvsp[-1].prob)->getRhs());
    (yyvsp[-1].prob)->deleteRhs();
  }
  (yyvsp[-3].prob)->getRhs()->add(1);
  (yyvsp[-3].prob)->getRhs()->mult(-1);
  
  problem.interpolant( (yyvsp[-3].prob), sep);
  
  delete (yyvsp[-1].prob);
  delete (yyvsp[-3].prob);


}
#line 1990 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 628 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-4].prob);

  PolyConstraint *pcons=new PolyConstraint((yyvsp[0].polycons));


  int index=(yyval.prob)-> addElem((yyvsp[-2].poly), pcons );
  problem.left_printMap[index]=unkown_name;
}
#line 2004 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 641 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-4].prob);
  PolyConstraint *pcons=new PolyConstraint((yyvsp[-2].polycons));

  int index=(yyval.prob)->addElem((yyvsp[0].poly), pcons );
  problem.left_printMap[index]=unkown_name;
}
#line 2016 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 652 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-4].prob);

  PolyConstraint *pcons=new PolyConstraint((yyvsp[0].polycons));

  (yyvsp[-2].poly)->mult(-1);

  int index=(yyval.prob)->addElem((yyvsp[-2].poly), pcons );
  problem.left_printMap[index]=unkown_name;
}
#line 2031 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 668 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-4].prob);
  (yyvsp[0].poly)->mult(-1);

  PolyConstraint *pcons=new PolyConstraint((yyvsp[-2].polycons));

  int index=(yyval.prob)->addElem( (yyvsp[0].poly), pcons );
  problem.left_printMap[index]=unkown_name;
}
#line 2045 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 680 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-2].prob);
  //  Poly_t *poly=copyPoly($3);
  (yyvsp[0].poly)->mult(-1);

  if(NULL==(yyval.prob)->getRhs()){
    (yyval.prob)->setRhs((yyvsp[0].poly));
  }else{
    (yyval.prob)->getRhs()->add_poly((yyvsp[0].poly));

  }
}
#line 2062 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 694 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-2].prob);

  if(NULL==(yyval.prob)->getRhs()){
    (yyval.prob)->setRhs((yyvsp[0].poly));
  }else{
    (yyval.prob)->getRhs()->add_poly((yyvsp[0].poly));

  }
}
#line 2077 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 707 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-2].prob);
  Poly_t * poly=new Poly_t();
  poly->add(1);

  PolyConstraint *pcons=new PolyConstraint( (yyvsp[0].polycons));

  int index=(yyval.prob)->addElem(poly, pcons );
  problem.left_printMap[index]=unkown_name;
}
#line 2092 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 719 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-2].prob);
  Poly_t * poly= new Poly_t();
  poly->add(-1);

  PolyConstraint *pcons=new PolyConstraint( (yyvsp[0].polycons));

  int index=(yyval.prob)->addElem(poly, pcons );
  problem.left_printMap[index]=unkown_name;
  
}
#line 2108 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 733 "src/sosproblemparser.y" /* yacc.c:1646  */
    {  (yyval.prob)=(yyvsp[-1].prob); }
#line 2114 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 737 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=problem.problem;
  PolyConstraint *pcons=new  PolyConstraint( (yyvsp[0].polycons));

  int index=(yyval.prob)->addElem((yyvsp[-2].poly), pcons);
  problem.left_printMap[index]=unkown_name;
  
}
#line 2127 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 747 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=problem.problem;
  PolyConstraint *pcons=new PolyConstraint((yyvsp[-2].polycons));

  int index=(yyval.prob)->addElem((yyvsp[0].poly), pcons );
  problem.left_printMap[index]=unkown_name;
  
}
#line 2140 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 759 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob) =problem.problem;
  Poly_t *poly=new Poly_t();
  poly->add(1);

  
  PolyConstraint *pcons=new PolyConstraint((yyvsp[0].polycons));

  int index=(yyval.prob)->addElem(poly, pcons );
  problem.left_printMap[index]=unkown_name;
}
#line 2156 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 776 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-4].prob);
  (yyvsp[-2].poly)->mult(-1);

  PolyConstraint *pcons=new PolyConstraint((yyvsp[0].polycons));

  int index=(yyval.prob)->addElem((yyvsp[-2].poly), pcons );
  problem.right_printMap[index]=unkown_name;
}
#line 2170 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 788 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyvsp[0].poly)->mult(-1);

  PolyConstraint *pcons=new PolyConstraint((yyvsp[-2].polycons));

  (yyval.prob)=(yyvsp[-4].prob);
  int index=(yyval.prob)->addElem((yyvsp[0].poly), pcons );
  problem.right_printMap[index]=unkown_name;
}
#line 2184 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 800 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-4].prob);

  PolyConstraint *pcons=new PolyConstraint((yyvsp[0].polycons));

  int index=(yyval.prob)->addElem((yyvsp[-2].poly), pcons );
  problem.right_printMap[index]=unkown_name;
}
#line 2197 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 812 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-4].prob);

  PolyConstraint *pcons= new PolyConstraint((yyvsp[-2].polycons));

  int index=(yyval.prob)->addElem( (yyvsp[0].poly), pcons );
  problem.right_printMap[index]=unkown_name;

}
#line 2211 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 825 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-2].prob);

  if(NULL==(yyval.prob)->getRhs()){
    (yyval.prob)->setRhs((yyvsp[0].poly));
  }else{
    (yyval.prob)->getRhs()->add_poly((yyvsp[0].poly));
  }
}
#line 2225 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 836 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-2].prob);
  (yyvsp[0].poly)->mult(-1);

  if(NULL==(yyval.prob)->getRhs()){
    (yyval.prob)->setRhs((yyvsp[0].poly));
  }else{
    (yyval.prob)->getRhs()->add_poly((yyvsp[0].poly));

  }
}
#line 2241 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 849 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-2].prob);
  Poly_t * poly= new Poly_t();
  poly->add(-1);

  PolyConstraint *pcons=new PolyConstraint((yyvsp[0].polycons));

  int index=(yyval.prob)->addElem(poly, pcons );
  problem.right_printMap[index]=unkown_name;
}
#line 2256 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 861 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob)=(yyvsp[-2].prob);
  Poly_t * poly=new Poly_t();
  poly->add(1);

  PolyConstraint *pcons=new PolyConstraint((yyvsp[0].polycons));
  int index=(yyval.prob)->addElem(poly, pcons );
  problem.right_printMap[index]=unkown_name;
}
#line 2270 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 872 "src/sosproblemparser.y" /* yacc.c:1646  */
    {  (yyval.prob)=(yyvsp[-1].prob); }
#line 2276 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 875 "src/sosproblemparser.y" /* yacc.c:1646  */
    {

  (yyval.prob) =new SOSProblem();
  PolyConstraint *pcons=new PolyConstraint((yyvsp[0].polycons));
  (yyvsp[-2].poly)->mult(-1);

  int index=(yyval.prob)->addElem((yyvsp[-2].poly), pcons );
  
  problem.right_printMap[index]=unkown_name;    
}
#line 2291 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 886 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob) =new SOSProblem();
  PolyConstraint *pcons=new PolyConstraint((yyvsp[-2].polycons));
  (yyvsp[0].poly)->mult(-1);

  int index=(yyval.prob)->addElem((yyvsp[0].poly), pcons );
  
  problem.right_printMap[index]=unkown_name;    
  
}
#line 2306 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 899 "src/sosproblemparser.y" /* yacc.c:1646  */
    {
  (yyval.prob) =new SOSProblem();
  
  Poly_t *poly=new Poly_t();
  poly->add(-1);

  PolyConstraint *pcons=new PolyConstraint((yyvsp[0].polycons));
  int index=(yyval.prob)->addElem(poly, pcons );
  problem.right_printMap[index]=unkown_name;
}
#line 2321 "src/sosproblemparser.cpp" /* yacc.c:1646  */
    break;


#line 2325 "src/sosproblemparser.cpp" /* yacc.c:1646  */
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
#line 912 "src/sosproblemparser.y" /* yacc.c:1906  */

#include "sosproblemscan.h"

int yywrap(void){
  return 1;
}


int yyerror(const char * what)
{
  fprintf(stderr, "Error line %d: %s\n", lineNum,what);
  err=true;
  return 1;
}

int yyerror(string what)
{
  cerr << "Error line "<<lineNum<<" "<<what<<endl;
  err=true;
  return 1;
}

void parseProblem(const char * filename){
  lineNum=1;
  static int init=0;
  if(init==0){
    init=1;
  }
  else{
    problem.clear();
  }
  yyin=fopen(filename,"r");
  yyparse();
  fclose(yyin);
}
void parseProblem(){
  std::cout<<"aiSat start:"<<std::endl;
  lineNum=1;
  static int init=0;
  if(init==0){
    init=1;
  }
  else{
    problem.clear();
  }

  yyparse();
  fclose(yyin);
}
