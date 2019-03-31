/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     FLOAT = 259,
     ID = 260,
     SEMI = 261,
     COMMA = 262,
     ASSIGNOP = 263,
     RELOP = 264,
     PLUS = 265,
     MINUS = 266,
     STAR = 267,
     DIV = 268,
     AND = 269,
     OR = 270,
     DOT = 271,
     NOT = 272,
     TYPE = 273,
     LP = 274,
     RP = 275,
     LB = 276,
     RB = 277,
     LC = 278,
     RC = 279,
     STRUCT = 280,
     RETURN = 281,
     IF = 282,
     ELSE = 283,
     WHILE = 284,
     UMINUS = 285,
     LOWER_THAN_ELSE = 286
   };
#endif
/* Tokens.  */
#define INT 258
#define FLOAT 259
#define ID 260
#define SEMI 261
#define COMMA 262
#define ASSIGNOP 263
#define RELOP 264
#define PLUS 265
#define MINUS 266
#define STAR 267
#define DIV 268
#define AND 269
#define OR 270
#define DOT 271
#define NOT 272
#define TYPE 273
#define LP 274
#define RP 275
#define LB 276
#define RB 277
#define LC 278
#define RC 279
#define STRUCT 280
#define RETURN 281
#define IF 282
#define ELSE 283
#define WHILE 284
#define UMINUS 285
#define LOWER_THAN_ELSE 286




/* Copy the first part of user declarations.  */
#line 1 ".//syntax.y"

    #include <stdio.h>
    #include "AST.h"
    #define YYERROR_VERBOSE
    //#define YYDEBUG 1
    extern AST_Node *root;
    extern int error_flag;
    extern int empty_flag;


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 11 ".//syntax.y"
{
    AST_Node *node;
}
/* Line 193 of yacc.c.  */
#line 172 ".//syntax.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 197 ".//syntax.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   340

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNRULES -- Number of states.  */
#define YYNSTATES  141

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
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
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    13,    16,    20,    24,
      27,    29,    33,    35,    37,    43,    46,    52,    54,    55,
      57,    59,    64,    69,    74,    78,    83,    87,    89,    92,
      95,    98,   103,   107,   110,   111,   114,   116,   120,   124,
     130,   136,   144,   150,   156,   159,   160,   164,   167,   169,
     173,   175,   179,   183,   187,   191,   195,   199,   203,   207,
     211,   215,   219,   222,   225,   230,   234,   239,   244,   249,
     253,   255,   257,   259,   263
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      33,     0,    -1,    34,    -1,    35,    34,    -1,    -1,    37,
      36,     6,    -1,    37,     6,    -1,    37,    42,    45,    -1,
      37,    42,     6,    -1,     1,     6,    -1,    41,    -1,    41,
       7,    36,    -1,    18,    -1,    38,    -1,    25,    39,    23,
      48,    24,    -1,    25,    40,    -1,    25,    39,    23,     1,
      24,    -1,     5,    -1,    -1,     5,    -1,     5,    -1,    41,
      21,     3,    22,    -1,    41,    21,     1,    22,    -1,     5,
      19,    43,    20,    -1,     5,    19,    20,    -1,     5,    19,
       1,    20,    -1,    44,     7,    43,    -1,    44,    -1,    37,
      41,    -1,     1,    20,    -1,     1,     6,    -1,    23,    48,
      46,    24,    -1,    23,     1,    24,    -1,    47,    46,    -1,
      -1,    52,     6,    -1,    45,    -1,    26,    52,     6,    -1,
      26,     1,     6,    -1,    27,    19,    52,    20,    47,    -1,
      27,    19,     1,    20,    47,    -1,    27,    19,    52,    20,
      47,    28,    47,    -1,    29,    19,    52,    20,    47,    -1,
      29,    19,     1,    20,    47,    -1,    49,    48,    -1,    -1,
      37,    50,     6,    -1,     1,     6,    -1,    51,    -1,    51,
       7,    50,    -1,    41,    -1,    41,     8,    52,    -1,    52,
       8,    52,    -1,    52,    14,    52,    -1,    52,    15,    52,
      -1,    52,     9,    52,    -1,    52,    10,    52,    -1,    52,
      11,    52,    -1,    52,    12,    52,    -1,    52,    13,    52,
      -1,    19,    52,    20,    -1,    19,     1,    20,    -1,    11,
      52,    -1,    17,    52,    -1,     5,    19,    53,    20,    -1,
       5,    19,    20,    -1,     5,    19,     1,    20,    -1,    52,
      21,    52,    22,    -1,    52,    21,     1,    22,    -1,    52,
      16,     5,    -1,     5,    -1,     3,    -1,     4,    -1,    52,
       7,    53,    -1,    52,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    51,    51,    58,    63,    68,    72,    76,    80,    84,
      88,    92,    99,   103,   109,   113,   117,   121,   125,   130,
     137,   141,   145,   149,   153,   157,   161,   165,   171,   175,
     177,   182,   186,   190,   194,   199,   203,   207,   211,   213,
     217,   219,   223,   227,   232,   236,   241,   245,   249,   253,
     259,   263,   270,   274,   278,   282,   286,   290,   294,   298,
     302,   306,   308,   312,   316,   320,   324,   326,   330,   332,
     336,   340,   344,   350,   354
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT", "ID", "SEMI", "COMMA",
  "ASSIGNOP", "RELOP", "PLUS", "MINUS", "STAR", "DIV", "AND", "OR", "DOT",
  "NOT", "TYPE", "LP", "RP", "LB", "RB", "LC", "RC", "STRUCT", "RETURN",
  "IF", "ELSE", "WHILE", "UMINUS", "LOWER_THAN_ELSE", "$accept", "Program",
  "ExtDefList", "ExtDef", "ExtDecList", "Specifier", "StructSpecifier",
  "OptTag", "Tag", "VarDec", "FunDec", "VarList", "ParamDec", "CompSt",
  "StmtList", "Stmt", "DefList", "Def", "DecList", "Dec", "Exp", "Args", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    35,    35,    35,
      36,    36,    37,    37,    38,    38,    38,    39,    39,    40,
      41,    41,    41,    42,    42,    42,    43,    43,    44,    44,
      44,    45,    45,    46,    46,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    48,    48,    49,    49,    50,    50,
      51,    51,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    53,    53
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     3,     2,
       1,     3,     1,     1,     5,     2,     5,     1,     0,     1,
       1,     4,     4,     4,     3,     4,     3,     1,     2,     2,
       2,     4,     3,     2,     0,     2,     1,     3,     3,     5,
       5,     7,     5,     5,     2,     0,     3,     2,     1,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     4,     3,     4,     4,     4,     3,
       1,     1,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    12,    18,     0,     2,     0,     0,    13,     9,
      19,     0,    15,     1,     3,    20,     6,     0,    10,     0,
       0,     0,     5,     0,     0,     8,     0,     7,     0,     0,
       0,     0,     0,    24,     0,     0,    27,    20,    11,     0,
       0,     0,    34,    47,    16,    50,     0,    48,    14,     0,
      44,    30,    25,    28,    23,     0,    22,    21,    32,    71,
      72,    70,     0,     0,     0,     0,     0,     0,    36,     0,
      34,     0,     0,    46,     0,     0,    26,     0,    62,    63,
       0,     0,     0,     0,     0,     0,    31,    33,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
      49,    29,     0,    65,    74,     0,    61,    60,    38,    37,
       0,     0,     0,     0,    52,    55,    56,    57,    58,    59,
      53,    54,    69,     0,     0,    66,     0,    64,     0,     0,
       0,     0,    68,    67,    73,    40,    39,    43,    42,     0,
      41
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,    17,    29,     8,    11,    12,    18,
      19,    35,    36,    68,    69,    70,    30,    31,    46,    47,
      71,   105
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -63
static const yytype_int16 yypact[] =
{
      17,    10,   -63,    42,    56,   -63,    17,    81,   -63,   -63,
      27,    36,   -63,   -63,   -63,    43,   -63,    59,    45,    40,
      19,    20,   -63,    66,    11,   -63,    50,   -63,     1,    66,
      65,    77,    52,   -63,    66,    63,    83,   -63,   -63,    75,
      87,    33,    88,   -63,   -63,    -2,   104,   105,   -63,   116,
     -63,   -63,    78,   106,   -63,    23,   -63,   -63,   -63,   -63,
     -63,   109,   168,   168,   120,   137,   110,   111,   -63,   112,
      88,   180,   168,   -63,    66,    64,   -63,   115,    -8,    -8,
     113,   238,   138,   194,   142,   159,   -63,   -63,   -63,   168,
     168,   168,   168,   168,   168,   168,   168,   144,   164,   280,
     -63,   -63,   130,   -63,   209,   131,   -63,   -63,   -63,   -63,
     132,   252,   135,   266,   280,   313,   319,   319,    -8,    -8,
     306,   293,   -63,   136,   223,   -63,   168,   -63,    88,    88,
      88,    88,   -63,   -63,   -63,   -63,   129,   -63,   -63,    88,
     -63
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -63,   -63,   160,   -63,   151,     5,   -63,   -63,   -63,   -25,
     -63,   122,   -63,   161,   114,   169,    82,   -63,   108,   -63,
     -62,    71
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -46
static const yytype_int16 yytable[] =
{
      78,    79,    81,    83,    45,     7,    72,    43,    97,    53,
      99,     7,    39,    98,    40,   104,     9,    -4,     1,    24,
      28,    32,   111,   113,    75,    44,    34,   114,   115,   116,
     117,   118,   119,   120,   121,     2,   124,     2,     2,    43,
      33,     2,     3,   -45,     3,     3,    25,    10,     3,    45,
     -17,    41,    23,   -45,   -45,   -45,    13,    58,    51,    20,
      34,   -45,    21,    26,   104,    22,    24,   -45,     2,   -45,
      51,    37,    52,   -45,   -45,     3,   -45,   -45,    49,   -45,
     -45,   -45,   -45,    54,   101,   -29,    15,    16,   -45,    48,
      55,    59,    60,    61,   -45,     2,   -45,    56,   -29,    62,
     -45,   -45,     3,   -45,   -45,    63,   -45,    64,    42,    57,
      73,    26,    74,    50,    65,    66,   102,    67,    59,    60,
      61,    80,    43,    59,    60,    61,    62,    24,    77,    84,
      85,    62,    63,   106,    64,   103,    86,    63,    82,    64,
      59,    60,    61,   110,   108,    59,    60,    61,    62,   122,
     125,   127,   128,    62,    63,   130,    64,   139,   132,    63,
     112,    64,    59,    60,    61,   123,    14,    59,    60,    61,
      62,    59,    60,    61,    38,    62,    63,    76,    64,    62,
      27,    63,   100,    64,    87,    63,    88,    64,    89,    90,
      91,    92,    93,    94,    95,    96,    97,   134,     0,     0,
     109,    98,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,   126,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,   133,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,   107,    98,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,   129,    98,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,     0,     0,   131,    98,    89,    90,
      91,    92,    93,    94,    95,    96,    97,   135,   136,   137,
     138,    98,    90,    91,    92,    93,    94,    95,   140,    97,
       0,     0,     0,     0,    98,    90,    91,    92,    93,    94,
       0,     0,    97,    91,    92,    93,    94,    98,     0,    97,
       0,    93,    94,     0,    98,    97,     0,     0,     0,     0,
      98
};

static const yytype_int16 yycheck[] =
{
      62,    63,    64,    65,    29,     0,     8,     6,    16,    34,
      72,     6,     1,    21,     3,    77,     6,     0,     1,    21,
       1,     1,    84,    85,     1,    24,    21,    89,    90,    91,
      92,    93,    94,    95,    96,    18,    98,    18,    18,     6,
      20,    18,    25,    24,    25,    25,     6,     5,    25,    74,
      23,     1,     7,     3,     4,     5,     0,    24,     6,    23,
      55,    11,    19,    23,   126,     6,    21,    17,    18,    19,
       6,     5,    20,    23,    24,    25,    26,    27,     1,    29,
       3,     4,     5,    20,    20,     7,     5,     6,    11,    24,
       7,     3,     4,     5,    17,    18,    19,    22,    20,    11,
      23,    24,    25,    26,    27,    17,    29,    19,    26,    22,
       6,    23,     7,    31,    26,    27,     1,    29,     3,     4,
       5,     1,     6,     3,     4,     5,    11,    21,    19,    19,
      19,    11,    17,    20,    19,    20,    24,    17,     1,    19,
       3,     4,     5,     1,     6,     3,     4,     5,    11,     5,
      20,    20,    20,    11,    17,    20,    19,    28,    22,    17,
       1,    19,     3,     4,     5,     1,     6,     3,     4,     5,
      11,     3,     4,     5,    23,    11,    17,    55,    19,    11,
      19,    17,    74,    19,    70,    17,     6,    19,     8,     9,
      10,    11,    12,    13,    14,    15,    16,   126,    -1,    -1,
       6,    21,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    -1,    21,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      21,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    21,    22,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    20,    21,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    -1,
      -1,    -1,    20,    21,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    -1,    -1,    -1,    20,    21,     8,     9,
      10,    11,    12,    13,    14,    15,    16,   128,   129,   130,
     131,    21,     9,    10,    11,    12,    13,    14,   139,    16,
      -1,    -1,    -1,    -1,    21,     9,    10,    11,    12,    13,
      -1,    -1,    16,    10,    11,    12,    13,    21,    -1,    16,
      -1,    12,    13,    -1,    21,    16,    -1,    -1,    -1,    -1,
      21
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    18,    25,    33,    34,    35,    37,    38,     6,
       5,    39,    40,     0,    34,     5,     6,    36,    41,    42,
      23,    19,     6,     7,    21,     6,    23,    45,     1,    37,
      48,    49,     1,    20,    37,    43,    44,     5,    36,     1,
       3,     1,    48,     6,    24,    41,    50,    51,    24,     1,
      48,     6,    20,    41,    20,     7,    22,    22,    24,     3,
       4,     5,    11,    17,    19,    26,    27,    29,    45,    46,
      47,    52,     8,     6,     7,     1,    43,    19,    52,    52,
       1,    52,     1,    52,    19,    19,    24,    46,     6,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    21,    52,
      50,    20,     1,    20,    52,    53,    20,    20,     6,     6,
       1,    52,     1,    52,    52,    52,    52,    52,    52,    52,
      52,    52,     5,     1,    52,    20,     7,    20,    20,    20,
      20,    20,    22,    22,    53,    47,    47,    47,    47,    28,
      47
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 51 ".//syntax.y"
    { 
    (yyval.node) = create_node("Program", "", -1, (yyloc).first_line); 
    root = (yyval.node); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 3:
#line 58 ".//syntax.y"
    { 
    empty_flag = 0; 
    (yyval.node) = create_node("ExtDefList", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 2, (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); 
  ;}
    break;

  case 4:
#line 63 ".//syntax.y"
    { 
    (yyval.node) = create_node("ExtDefList", "", -1, (yyloc).first_line); 
  ;}
    break;

  case 5:
#line 68 ".//syntax.y"
    { 
    (yyval.node) = create_node("ExtDef", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 6:
#line 72 ".//syntax.y"
    { 
    (yyval.node) = create_node("ExtDef", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 2, (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); 
  ;}
    break;

  case 7:
#line 76 ".//syntax.y"
    { 
    (yyval.node) = create_node("ExtDef", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 8:
#line 80 ".//syntax.y"
    {
    (yyval.node) = create_node("ExtDef", "", -1, (yyloc).first_line);
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node));
  ;}
    break;

  case 9:
#line 84 ".//syntax.y"
    {
  ;}
    break;

  case 10:
#line 88 ".//syntax.y"
    { 
    (yyval.node) = create_node("ExtDecList", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 11:
#line 92 ".//syntax.y"
    { 
    (yyval.node) = create_node("ExtDecList", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 12:
#line 99 ".//syntax.y"
    { 
    (yyval.node) = create_node("Specifier", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 13:
#line 103 ".//syntax.y"
    { 
    (yyval.node) = create_node("Specifier", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 14:
#line 109 ".//syntax.y"
    { 
    (yyval.node) = create_node("StructSpecifier", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 5, (yyvsp[(1) - (5)].node), (yyvsp[(2) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(4) - (5)].node), (yyvsp[(5) - (5)].node)); 
  ;}
    break;

  case 15:
#line 113 ".//syntax.y"
    { 
    (yyval.node) = create_node("StructSpecifier", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 2 , (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); 
  ;}
    break;

  case 16:
#line 117 ".//syntax.y"
    {
  ;}
    break;

  case 17:
#line 121 ".//syntax.y"
    { 
    (yyval.node) = create_node("OptTag", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 18:
#line 125 ".//syntax.y"
    { 
    (yyval.node) = create_node("OptTag", "", -1, (yyloc).first_line); 
  ;}
    break;

  case 19:
#line 130 ".//syntax.y"
    { 
    (yyval.node) = create_node("Tag", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 20:
#line 137 ".//syntax.y"
    { 
    (yyval.node) = create_node("VarDec", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 21:
#line 141 ".//syntax.y"
    { 
    (yyval.node) = create_node("VarDec", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 4, (yyvsp[(1) - (4)].node), (yyvsp[(2) - (4)].node), (yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].node)); 
  ;}
    break;

  case 22:
#line 145 ".//syntax.y"
    {
  ;}
    break;

  case 23:
#line 149 ".//syntax.y"
    { 
    (yyval.node) = create_node("FunDec", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 4, (yyvsp[(1) - (4)].node), (yyvsp[(2) - (4)].node), (yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].node)); 
  ;}
    break;

  case 24:
#line 153 ".//syntax.y"
    { 
    (yyval.node) = create_node("FunDec", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 25:
#line 157 ".//syntax.y"
    {
  ;}
    break;

  case 26:
#line 161 ".//syntax.y"
    { 
    (yyval.node) = create_node("VarList", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 27:
#line 165 ".//syntax.y"
    { 
    (yyval.node) = create_node("VarList", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 28:
#line 171 ".//syntax.y"
    { 
    (yyval.node) = create_node("ParamDec", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 2, (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); 
  ;}
    break;

  case 29:
#line 175 ".//syntax.y"
    {
  ;}
    break;

  case 30:
#line 177 ".//syntax.y"
    {
  ;}
    break;

  case 31:
#line 182 ".//syntax.y"
    { 
    (yyval.node) = create_node("CompSt", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 4, (yyvsp[(1) - (4)].node), (yyvsp[(2) - (4)].node), (yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].node)); 
  ;}
    break;

  case 32:
#line 186 ".//syntax.y"
    {
  ;}
    break;

  case 33:
#line 190 ".//syntax.y"
    { 
    (yyval.node) = create_node("StmtList", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 2, (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); 
  ;}
    break;

  case 34:
#line 194 ".//syntax.y"
    { 
    (yyval.node) = create_node("StmtList", "", -1, (yyloc).first_line); 
  ;}
    break;

  case 35:
#line 199 ".//syntax.y"
    { 
    (yyval.node) = create_node("Stmt", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 2, (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); 
  ;}
    break;

  case 36:
#line 203 ".//syntax.y"
    { 
    (yyval.node) = create_node("Stmt", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 37:
#line 207 ".//syntax.y"
    { 
    (yyval.node) = create_node("Stmt", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 38:
#line 211 ".//syntax.y"
    {
  ;}
    break;

  case 39:
#line 213 ".//syntax.y"
    { 
    (yyval.node) = create_node("Stmt", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 5, (yyvsp[(1) - (5)].node), (yyvsp[(2) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(4) - (5)].node), (yyvsp[(5) - (5)].node)); 
  ;}
    break;

  case 40:
#line 217 ".//syntax.y"
    {
  ;}
    break;

  case 41:
#line 219 ".//syntax.y"
    { 
    (yyval.node) = create_node("Stmt", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 7, (yyvsp[(1) - (7)].node), (yyvsp[(2) - (7)].node), (yyvsp[(3) - (7)].node), (yyvsp[(4) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(6) - (7)].node), (yyvsp[(7) - (7)].node)); 
  ;}
    break;

  case 42:
#line 223 ".//syntax.y"
    { 
    (yyval.node) = create_node("Stmt", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 5, (yyvsp[(1) - (5)].node), (yyvsp[(2) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(4) - (5)].node), (yyvsp[(5) - (5)].node)); 
  ;}
    break;

  case 43:
#line 227 ".//syntax.y"
    {
  ;}
    break;

  case 44:
#line 232 ".//syntax.y"
    { 
    (yyval.node) = create_node("DefList", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 2, (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); 
  ;}
    break;

  case 45:
#line 236 ".//syntax.y"
    { 
    (yyval.node) = create_node("DefList", "", -1, (yyloc).first_line); 
  ;}
    break;

  case 46:
#line 241 ".//syntax.y"
    { 
    (yyval.node) = create_node("Def", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 47:
#line 245 ".//syntax.y"
    {
  ;}
    break;

  case 48:
#line 249 ".//syntax.y"
    { 
    (yyval.node) = create_node("DecList", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 49:
#line 253 ".//syntax.y"
    { 
    (yyval.node) = create_node("DecList", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 50:
#line 259 ".//syntax.y"
    { 
    (yyval.node) = create_node("Dec", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 51:
#line 263 ".//syntax.y"
    { 
    (yyval.node) = create_node("Dec", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 52:
#line 270 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 53:
#line 274 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 54:
#line 278 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 55:
#line 282 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 56:
#line 286 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 57:
#line 290 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 58:
#line 294 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 59:
#line 298 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 60:
#line 302 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 61:
#line 306 ".//syntax.y"
    {
  ;}
    break;

  case 62:
#line 308 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 2, (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); 
  ;}
    break;

  case 63:
#line 312 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 2, (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); 
  ;}
    break;

  case 64:
#line 316 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 4, (yyvsp[(1) - (4)].node), (yyvsp[(2) - (4)].node), (yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].node)); 
  ;}
    break;

  case 65:
#line 320 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 66:
#line 324 ".//syntax.y"
    {
  ;}
    break;

  case 67:
#line 326 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 4, (yyvsp[(1) - (4)].node), (yyvsp[(2) - (4)].node), (yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].node)); 
  ;}
    break;

  case 68:
#line 330 ".//syntax.y"
    {
  ;}
    break;

  case 69:
#line 332 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 70:
#line 336 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 71:
#line 340 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 72:
#line 344 ".//syntax.y"
    { 
    (yyval.node) = create_node("Exp", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;

  case 73:
#line 350 ".//syntax.y"
    { 
    (yyval.node) = create_node("Args", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 3, (yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].node)); 
  ;}
    break;

  case 74:
#line 354 ".//syntax.y"
    { 
    (yyval.node) = create_node("Args", "", -1, (yyloc).first_line); 
    add_child_sibling((yyval.node), 1, (yyvsp[(1) - (1)].node)); 
  ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2133 ".//syntax.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
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

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 359 ".//syntax.y"


#include "lex.yy.c"

int yyerror(char *msg){
    error_flag = 1;
    printf("Error type B at Line %d: %s.\n", yylineno, msg);
    return 0;
}
