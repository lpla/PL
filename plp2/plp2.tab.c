/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 5 "plp2.y"


#define ERRLEXICO	1
#define ERRSINT		2
#define ERREOF		3
#define ERRYADECL	4
#define ERRNODECL	5
#define ERRDIM		6
#define ERRFALTAN	7
#define ERRSOBRAN	8
#define ERR_EXP_ENT	9
#define ERR_EXP_LOG	10
#define ERR_EXDER_LOG	11
#define ERR_EXDER_ENT	12
#define ERR_EXDER_RE	13
#define ERR_EXIZQ_LOG	14
#define ERR_EXIZQ_RE	15
#define ERR_NOCABE	16
#define ERR_MAXVAR	17
#define ERR_MAXTIPOS	18
#define ERR_MAXTMP	19

//Definimos estas constantes para los tipos
#define ENTERO		1
#define REAL		2
#define LOGICO		3


#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iostream>


typedef struct{
std::string lexema;
std::string cod;
std::string trad;

int dir;
int dbase;

int nlin;
int ncol;

int tipo;	//Nombra un tipo en base a la numeracion de la tabla de tipos
int tipoOp;	//Se usa para controlar que instruccion usar en caso de diferencia entre entero y real (xxxi/xxxr). Nombra el tipo entero o real, no el resto

}Atributos;

#define YYSTYPE Atributos

//Declaración de clases

//Definición de un tipo y sus elementos: el número del tipo, su tamaño, el tamaño total si forma un array y el tipo base de esa array
class Tipo {
	public:
		int tipo;
		int tam;
		int tamTotal;
		int tipoBase;

	Tipo(int tipo, int tam, int tamTotal, int tipoBase){
		this->tipo = tipo;
		this->tam = tam;
		this->tamTotal = tamTotal;
		this->tipoBase = tipoBase;
	}
};

//Definición de la tabla de tipos formada por un vector del tipo definido arriba y las funciones pedidas en clase de teoría
class TablaTipos{
	private:
		std::vector<Tipo*> tabla;

	public:
		TablaTipos(){
			nuevoTipo(1,1,1,1);
			nuevoTipo(2,1,1,2);
			nuevoTipo(3,1,1,3);
		}

		int nextId(){
			return this->tabla.size() + 1;
		}

		int tamTotal(int tipo){
			for(int i = 0; i < this->tabla.size(); i++){
				if(this->tabla[i]->tipo == tipo)
					return this->tabla[i]->tamTotal;
			}
		}

		void nuevoTipo(int tipo, int tam, int tamTotal, int tipoBase){
			this->tabla.push_back(new Tipo(tipo, tam, tamTotal, tipoBase));
		}
		
		int tam(int tipo){
			for(int i = 0; i < this->tabla.size(); i++){
				if(this->tabla[i]->tipo == tipo)
					return this->tabla[i]->tam;
			}
		}

		int tipoBase(int tipo){
			for(int i = 0; i < this->tabla.size(); i++){
				if(this->tabla[i]->tipo == tipo)
					return this->tabla[i]->tipoBase;
			}
		}
};

//Definición de las variables (símbolo) donde se almacena el tipo, el tipo de la operación (en caso de venir de una), la dirección y el tamaño
class Simbolo{
	public:
	std::string nombre;
	int tipo;
	int tipoOp;
	int dir;
	int tam;

	Simbolo(){}

	Simbolo(std::string nombre, int tipo, int tipoOp, int dir, int tam){
		this->nombre = nombre;
		this->tipo = tipo;
		this->tipoOp = tipoOp;
		this->dir = dir;
		this->tam = tam;
	}
};

//Definición de la tabla de símbolos formada por un vector del tipo definido arriba y las funciones pedidas en clase de teoría
class TablaSimbolos{
	private:
		std::vector<Simbolo*> tabla;
		int lastDir;
	
	public:
		TablaSimbolos(){
			this->lastDir = 100;
		}		
		
		int nextDir(){
			return lastDir;
		}

		void nuevoSimbolo(std::string id, int tipo, int tipoOp, int dir, int tam){
			this->tabla.push_back(new Simbolo(id, tipo, tipoOp, dir, tam));

			lastDir += tam;
		};

		Simbolo* buscarSimbolo(std::string id){
			for(int i = 0; i < tabla.size(); i++){
				if(this->tabla[i]->nombre == id)
					return this->tabla[i];
			}

			return NULL;
		}

		Simbolo* buscarPorDir(int dir){
			for(int i = 0; i < this->tabla.size(); i++){
				if(this->tabla[i]->dir == dir)
					return this->tabla[i];
			}

			return NULL;
		}
};

// Variables y funciones del A. Lexico
extern int ncol,nlin,findefichero;
extern int yylex();
extern char *yytext;
extern FILE *yyin;
int yyerror(std::string s);

//Definiciones de funciones declaradas al final del fichero
int NuevaTemporal();
void getTipoBaseArray(int &tipo);
bool isArray(int tipo);
void errorSemantico(int nerror, Atributos token);
void msgError(int nerror,int nlin,int ncol,const char *s);

//Variable para el conteo de las temporales y las etiquetas
int regTemporal = 10000;
int contadorEtiquetas = 0;
//Variables de reglas
char s[1000];	//Para los sprintf

//Variables auxiliares de las reglas
int i;		//Contador del for
int tipoArray;	//Variable para guardar el tipo base del array utilizando el método recursivo
int dir;	//Variable auxiliar para almacenar una nueva temporal
int diritor;	//Variable auxiliar para almacenar una nueva temporal donde se almacena una conversión itor


Simbolo* simbolo;

//Definición de las tablas
TablaSimbolos tsActual;
TablaTipos ttipos;



/* Line 268 of yacc.c  */
#line 280 "plp2.tab.c"

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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     algoritmo = 258,
     falgoritmo = 259,
     var = 260,
     fvar = 261,
     entero = 262,
     real = 263,
     logico = 264,
     tabla = 265,
     de = 266,
     escribe = 267,
     lee = 268,
     si = 269,
     entonces = 270,
     sino = 271,
     mientras = 272,
     hacer = 273,
     blq = 274,
     fblq = 275,
     cierto = 276,
     falso = 277,
     id = 278,
     numentero = 279,
     numreal = 280,
     opas = 281,
     opmd = 282,
     oprel = 283,
     obool = 284,
     ybool = 285,
     nobool = 286,
     pari = 287,
     pard = 288,
     pyc = 289,
     coma = 290,
     opasig = 291,
     dospto = 292,
     lcor = 293,
     rcor = 294
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 361 "plp2.tab.c"

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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
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
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   89

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNRULES -- Number of states.  */
#define YYNSTATES  90

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,    10,    12,    13,    18,    19,    26,    29,
      30,    31,    36,    37,    39,    41,    43,    48,    49,    54,
      56,    59,    62,    67,    74,    79,    80,    85,    89,    93,
      95,    99,   101,   105,   107,   111,   113,   116,   120,   122,
     124,   126,   128,   132,   135,   137,   139,   141,   142
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      41,     0,    -1,     3,    37,    23,    42,    50,     4,    -1,
      43,    -1,    -1,     5,    44,    46,     6,    -1,    -1,    49,
      37,    23,    45,    47,    34,    -1,    44,    46,    -1,    -1,
      -1,    35,    23,    48,    47,    -1,    -1,     7,    -1,     8,
      -1,     9,    -1,    10,    24,    11,    49,    -1,    -1,    50,
      34,    51,    52,    -1,    52,    -1,    12,    54,    -1,    13,
      60,    -1,    14,    54,    15,    52,    -1,    14,    54,    15,
      52,    16,    52,    -1,    17,    54,    18,    52,    -1,    -1,
      60,    36,    53,    54,    -1,    19,    50,    20,    -1,    54,
      29,    55,    -1,    55,    -1,    55,    30,    56,    -1,    56,
      -1,    57,    28,    57,    -1,    57,    -1,    57,    26,    58,
      -1,    58,    -1,    26,    58,    -1,    58,    27,    59,    -1,
      59,    -1,    60,    -1,    24,    -1,    25,    -1,    32,    54,
      33,    -1,    31,    59,    -1,    21,    -1,    22,    -1,    23,
      -1,    -1,    60,    38,    61,    57,    39,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   216,   216,   222,   223,   226,   229,   229,   287,   288,
     291,   291,   323,   326,   327,   328,   329,   337,   337,   341,
     346,   371,   386,   398,   414,   430,   430,   458,   461,   498,
     508,   547,   557,   593,   603,   638,   646,   679,   713,   723,
     739,   753,   767,   774,   793,   806,   821,   837,   837
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "algoritmo", "falgoritmo", "var", "fvar",
  "entero", "real", "logico", "tabla", "de", "escribe", "lee", "si",
  "entonces", "sino", "mientras", "hacer", "blq", "fblq", "cierto",
  "falso", "id", "numentero", "numreal", "opas", "opmd", "oprel", "obool",
  "ybool", "nobool", "pari", "pard", "pyc", "coma", "opasig", "dospto",
  "lcor", "rcor", "$accept", "S", "SDec", "Dec", "DVar", "@1", "MDVar",
  "LId", "@2", "Tipo", "SInstr", "$@3", "Instr", "$@4", "Expr", "Econj",
  "Ecomp", "Esimple", "Term", "Factor", "Ref", "$@5", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    45,    44,    46,    46,
      48,    47,    47,    49,    49,    49,    49,    51,    50,    50,
      52,    52,    52,    52,    52,    53,    52,    52,    54,    54,
      55,    55,    56,    56,    57,    57,    57,    58,    58,    59,
      59,    59,    59,    59,    59,    59,    60,    61,    60
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     6,     1,     0,     4,     0,     6,     2,     0,
       0,     4,     0,     1,     1,     1,     4,     0,     4,     1,
       2,     2,     4,     6,     4,     0,     4,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     2,     3,     1,     1,
       1,     1,     3,     2,     1,     1,     1,     0,     5
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     4,     0,     0,     3,    13,
      14,    15,     0,     9,     0,     0,     0,     0,     0,     0,
      46,     0,    19,     0,     0,     9,     0,     0,    44,    45,
      40,    41,     0,     0,     0,    20,    29,    31,    33,    35,
      38,    39,    21,     0,     0,     0,     2,    17,    25,    47,
       0,     8,     5,     6,    36,    43,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,    16,    12,
      42,    28,    30,    34,    32,    37,    22,    24,    18,    26,
       0,     0,     0,     0,    48,    10,     7,    23,    12,    11
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     7,     8,    25,    69,    26,    82,    88,    14,
      21,    65,    22,    66,    35,    36,    37,    38,    39,    40,
      41,    67
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -58
static const yytype_int8 yypact[] =
{
      13,   -20,    28,     8,   -58,    46,    58,    30,   -58,   -58,
     -58,   -58,    24,    58,    20,    14,    36,    14,    14,    30,
     -58,    -1,   -58,   -23,    51,    58,    57,    47,   -58,   -58,
     -58,   -58,    -2,    -2,    14,    44,    42,   -58,    43,    48,
     -58,    39,    39,    -5,    23,    -9,   -58,   -58,   -58,   -58,
      58,   -58,   -58,   -58,    48,   -58,    31,    14,    14,    -2,
      14,    -2,    30,    30,   -58,    30,    14,    14,   -58,    45,
     -58,    42,   -58,    48,    52,   -58,    63,   -58,   -58,    44,
     -12,    59,    40,    30,   -58,   -58,   -58,   -58,    45,   -58
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -58,   -58,   -58,   -58,    75,   -58,    60,    -4,   -58,    33,
      67,   -58,   -57,   -58,   -16,    32,    29,    -6,   -25,   -29,
      -7,   -58
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      23,    43,    44,    46,    55,    76,    77,    54,    78,    42,
      62,    64,    23,    48,    59,    49,     1,     3,    56,    28,
      29,    20,    30,    31,    57,    47,    87,    84,     4,    33,
      34,     5,    75,    47,    73,    28,    29,    20,    30,    31,
      32,    63,    15,    16,    17,    33,    34,    18,    24,    19,
      79,     6,    57,    20,    74,    23,    23,    27,    23,    20,
      57,    80,    50,    52,    70,     9,    10,    11,    12,    59,
      53,    60,    58,    57,    86,    61,    23,    49,    59,    83,
      81,    13,    85,    68,    89,    51,    45,    72,     0,    71
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-58))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
       7,    17,    18,     4,    33,    62,    63,    32,    65,    16,
      15,    20,    19,    36,    26,    38,     3,    37,    34,    21,
      22,    23,    24,    25,    29,    34,    83,    39,     0,    31,
      32,    23,    61,    34,    59,    21,    22,    23,    24,    25,
      26,    18,    12,    13,    14,    31,    32,    17,    24,    19,
      66,     5,    29,    23,    60,    62,    63,    37,    65,    23,
      29,    67,    11,     6,    33,     7,     8,     9,    10,    26,
      23,    28,    30,    29,    34,    27,    83,    38,    26,    16,
      35,     6,    23,    50,    88,    25,    19,    58,    -1,    57
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    41,    37,     0,    23,     5,    42,    43,     7,
       8,     9,    10,    44,    49,    12,    13,    14,    17,    19,
      23,    50,    52,    60,    24,    44,    46,    37,    21,    22,
      24,    25,    26,    31,    32,    54,    55,    56,    57,    58,
      59,    60,    60,    54,    54,    50,     4,    34,    36,    38,
      11,    46,     6,    23,    58,    59,    54,    29,    30,    26,
      28,    27,    15,    18,    20,    51,    53,    61,    49,    45,
      33,    55,    56,    58,    57,    59,    52,    52,    52,    54,
      57,    35,    47,    16,    39,    23,    34,    52,    48,    47
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
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


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
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

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

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


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


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
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  *++yyvsp = yylval;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 216 "plp2.y"
    {
								(yyval).cod = (yyvsp[(4) - (6)]).cod + (yyvsp[(5) - (6)]).cod + "halt\n";
								printf("%s", (yyval).cod.c_str());
							}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 222 "plp2.y"
    {	(yyval).cod = (yyvsp[(1) - (1)]).cod;	}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 223 "plp2.y"
    { 	/*No se hace nada */ 	}
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 226 "plp2.y"
    {	(yyval).cod = (yyvsp[(2) - (4)]).cod + (yyvsp[(3) - (4)]).cod;	}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 229 "plp2.y"
    {
					dir = tsActual.nextDir();

					if(!isArray((yyvsp[(1) - (3)]).tipo)){
						if((yyvsp[(1) - (3)]).tipo == LOGICO){
							sprintf(s, "mov #0 %d;Tipo dospto id\n\n", dir);
							(yyval).cod = s;
							tsActual.nuevoSimbolo((yyvsp[(3) - (3)]).lexema, (yyvsp[(1) - (3)]).tipo, ENTERO, dir, ttipos.tamTotal((yyvsp[(1) - (3)]).tipo));
						}
						else{
							if((yyvsp[(1) - (3)]).tipo == ENTERO)
								sprintf(s, "mov #0 %d;Tipo dospto id\n\n", dir);

							else if((yyvsp[(1) - (3)]).tipo == REAL)
								sprintf(s, "mov $0 %d;Tipo dospto id\n\n", dir);

							(yyval).cod = s;
							tsActual.nuevoSimbolo((yyvsp[(3) - (3)]).lexema, (yyvsp[(1) - (3)]).tipo, (yyvsp[(1) - (3)]).tipo, dir, ttipos.tamTotal((yyvsp[(1) - (3)]).tipo));
						}
					}
					else{
						tipoArray = (yyvsp[(1) - (3)]).tipo;
						getTipoBaseArray(tipoArray);
						
						if(tipoArray == LOGICO){
							for(i = 0; i < ttipos.tamTotal((yyvsp[(1) - (3)]).tipo); i++){
								sprintf(s, "mov #0 %d;Tipo dospto id\n\n", dir+i);
								(yyval).cod = s;
							}
							tsActual.nuevoSimbolo((yyvsp[(3) - (3)]).lexema, (yyvsp[(1) - (3)]).tipo, ENTERO, dir, ttipos.tamTotal((yyvsp[(1) - (3)]).tipo));
						}
						else{
							if(tipoArray == ENTERO){
								for(i = 0; i < ttipos.tamTotal((yyvsp[(1) - (3)]).tipo); i++){
									sprintf(s, "mov #0 %d;Tipo dospto id\n\n", dir+i);
									(yyval).cod = s;
								}
							}
							else if(tipoArray == REAL){
								for(i = 0; i < ttipos.tamTotal((yyvsp[(1) - (3)]).tipo); i++){
									sprintf(s, "mov $0 %d;Tipo dospto id\n\n", dir+i);
									(yyval).cod = s;
								}
							}
							tsActual.nuevoSimbolo((yyvsp[(3) - (3)]).lexema, (yyvsp[(1) - (3)]).tipo, (yyvsp[(1) - (3)]).tipo, dir, ttipos.tamTotal((yyvsp[(1) - (3)]).tipo));
						}				
					}

					dir = tsActual.nextDir();
					if(dir >= 10000)
						errorSemantico(ERR_NOCABE, (yyvsp[(3) - (3)]));
				}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 281 "plp2.y"
    {
					(yyval).cod = (yyvsp[(4) - (6)]).cod + (yyvsp[(5) - (6)]).cod;
					regTemporal = 10000;
				}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 287 "plp2.y"
    {	(yyval).cod = (yyvsp[(1) - (2)]).cod + (yyvsp[(2) - (2)]).cod;	}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 288 "plp2.y"
    { 	/*No se hace nada */ 		}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 291 "plp2.y"
    {	
				simbolo = new Simbolo();

				if((simbolo = tsActual.buscarSimbolo((yyvsp[(2) - (2)]).lexema)) != NULL)
					errorSemantico(ERRYADECL, (yyvsp[(2) - (2)]));
				else{
					simbolo = tsActual.buscarSimbolo((yyvsp[(-1) - (2)]).lexema);
					dir = tsActual.nextDir();

					if(simbolo->tipo == LOGICO){
						sprintf(s, "mov #0 %d;coma id\n\n", dir);
						(yyval).cod = s;
						tsActual.nuevoSimbolo((yyvsp[(2) - (2)]).lexema, simbolo->tipo, ENTERO, dir, ttipos.tamTotal(simbolo->tipo));
					}
					else {
						if(simbolo->tipo == ENTERO)
							sprintf(s, "mov #0 %d;coma id\n\n", dir);
						else if(simbolo->tipo == REAL)
							sprintf(s, "mov $0 %d;coma id\n\n", dir);

						(yyval).cod = s;
						tsActual.nuevoSimbolo((yyvsp[(2) - (2)]).lexema, simbolo->tipo, simbolo->tipo, dir, ttipos.tamTotal(simbolo->tipo));
					}
				}

				dir = tsActual.nextDir();
				if(dir >= 10000)
					errorSemantico(ERR_NOCABE, (yyvsp[(2) - (2)]));
			}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 320 "plp2.y"
    {
				(yyval).cod = (yyvsp[(3) - (4)]).cod + (yyvsp[(4) - (4)]).cod;
			}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 323 "plp2.y"
    {	(yyval).cod = "";	}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 326 "plp2.y"
    {	(yyval).tipo = ENTERO;	}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 327 "plp2.y"
    {	(yyval).tipo = REAL;		}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 328 "plp2.y"
    {	(yyval).tipo = LOGICO;	}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 329 "plp2.y"
    {
						if((yyvsp[(2) - (4)]).lexema == "0")
							errorSemantico(ERRDIM, (yyvsp[(2) - (4)]));
						(yyval).tipo = ttipos.nextId();
						ttipos.nuevoTipo((yyval).tipo, atoi((yyvsp[(2) - (4)]).lexema.c_str()), (atoi((yyvsp[(2) - (4)]).lexema.c_str())*ttipos.tamTotal((yyvsp[(4) - (4)]).tipo)), (yyvsp[(4) - (4)]).tipo);
					}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 337 "plp2.y"
    {	regTemporal = 10000;		}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 338 "plp2.y"
    {
				(yyval).cod = (yyvsp[(1) - (4)]).cod + (yyvsp[(4) - (4)]).cod;
			}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 341 "plp2.y"
    {
				(yyval).cod = (yyvsp[(1) - (1)]).cod;
			}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 346 "plp2.y"
    {

							if((yyvsp[(2) - (2)]).tipo == ENTERO){
								sprintf(s, "wri %d;escribe Ref\nwrl\n\n", (yyvsp[(2) - (2)]).dir);
								(yyval).cod = s;
							}
							else if((yyvsp[(2) - (2)]).tipo == REAL){
								sprintf(s, "wrr %d;escribe Ref\nwrl\n\n", (yyvsp[(2) - (2)]).dir);
								(yyval).cod = s;
							}
							else if((yyvsp[(2) - (2)]).tipo == LOGICO){								
								contadorEtiquetas++;
								sprintf(s, "mov %d A;escribe Ref\njz L%d\nwrc #99\nwrl\n", (yyvsp[(2) - (2)]).dir, contadorEtiquetas);
								(yyval).cod = (yyvsp[(2) - (2)]).cod + s;
								
								contadorEtiquetas++;
								sprintf(s, "jmp L%d\nL%d wrc #102\nwrl\n", contadorEtiquetas, contadorEtiquetas-1);
								(yyval).cod = (yyval).cod + s;

								sprintf(s, "L%d ", contadorEtiquetas);
								(yyval).cod = (yyval).cod + s;
							}
							
							(yyval).cod = (yyvsp[(2) - (2)]).cod + (yyval).cod;
						}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 371 "plp2.y"
    {

							if((yyvsp[(2) - (2)]).tipo == ENTERO)
								sprintf(s, "mov %d A;lee Ref\nmuli #%d\naddi #%d\nrdi @A\n\n", (yyvsp[(2) - (2)]).dir, ttipos.tam((yyvsp[(2) - (2)]).tipo), (yyvsp[(2) - (2)]).dbase);
							else if((yyvsp[(2) - (2)]).tipo == REAL)
								sprintf(s, "mov %d A;lee Ref\nmuli #%d\naddi #%d\nrdr @A\n\n", (yyvsp[(2) - (2)]).dir, ttipos.tam((yyvsp[(2) - (2)]).tipo), (yyvsp[(2) - (2)]).dbase);
							else if((yyvsp[(2) - (2)]).tipo == LOGICO) {
								dir = NuevaTemporal();
								if(dir == 16383)
									errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (2)]));
								sprintf(s, "rdc A;lee Ref\neqli #99\nmov A %d\nmov %d A\nmuli #%d\naddi #%d\nmov %d @A\n\n", dir, (yyvsp[(2) - (2)]).dir, ttipos.tam((yyvsp[(2) - (2)]).tipo), (yyvsp[(2) - (2)]).dbase,dir);
							}
							
							(yyval).cod = (yyvsp[(2) - (2)]).cod + s;
						}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 386 "plp2.y"
    {
							if((yyvsp[(2) - (4)]).tipo != LOGICO)
								errorSemantico(ERR_EXP_LOG,(yyvsp[(1) - (4)]));
							else{
								contadorEtiquetas++;
								sprintf(s, "mov %d A;si\njz L%d\n", (yyvsp[(2) - (4)]).dir, contadorEtiquetas);
								(yyval).cod = (yyvsp[(2) - (4)]).cod + s + (yyvsp[(4) - (4)]).cod;
								
								sprintf(s, "L%d ", contadorEtiquetas);
								(yyval).cod = (yyval).cod + s;
							}
						}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 398 "plp2.y"
    {
							if((yyvsp[(2) - (6)]).tipo != LOGICO)
								errorSemantico(ERR_EXP_LOG,(yyvsp[(1) - (6)]));
							else{
								contadorEtiquetas++;
								sprintf(s, "mov %d A;si sino\njz L%d\n", (yyvsp[(2) - (6)]).dir, contadorEtiquetas);
								(yyval).cod = (yyvsp[(2) - (6)]).cod + s + (yyvsp[(4) - (6)]).cod;
								
								contadorEtiquetas++;
								sprintf(s, "jmp L%d\nL%d ", contadorEtiquetas, contadorEtiquetas-1);
								(yyval).cod = (yyval).cod + s + (yyvsp[(6) - (6)]).cod;

								sprintf(s, "L%d ", contadorEtiquetas);
								(yyval).cod = (yyval).cod + s;
							}
						}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 414 "plp2.y"
    {
							if((yyvsp[(2) - (4)]).tipo != LOGICO)
								errorSemantico(ERR_EXP_LOG,(yyvsp[(1) - (4)]));
							else{
								contadorEtiquetas++;
								sprintf(s, "L%d ", contadorEtiquetas);
								(yyval).cod = s + (yyvsp[(2) - (4)]).cod;
								
								contadorEtiquetas++;
								sprintf(s, "mov %d A;mientras\njz L%d\n", (yyvsp[(2) - (4)]).dir, contadorEtiquetas);
								(yyval).cod = (yyval).cod + s + (yyvsp[(4) - (4)]).cod;
								
								sprintf(s, "jmp L%d\nL%d ", contadorEtiquetas-1, contadorEtiquetas);
								(yyval).cod = (yyval).cod + s;
							}
						}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 430 "plp2.y"
    {
							if(isArray((yyvsp[(1) - (2)]).tipo))
								errorSemantico(ERRFALTAN, (yyvsp[(1) - (2)]));
						}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 435 "plp2.y"
    {	
							if(isArray((yyvsp[(4) - (4)]).tipo))
								errorSemantico(ERRFALTAN, (yyvsp[(4) - (4)]));

							if((yyvsp[(1) - (4)]).tipo == LOGICO && (yyvsp[(1) - (4)]).tipo != (yyvsp[(4) - (4)]).tipo)
								errorSemantico(ERR_EXDER_LOG, (yyvsp[(2) - (4)]));
							else if((yyvsp[(1) - (4)]).tipo == ENTERO && (yyvsp[(1) - (4)]).tipo != (yyvsp[(4) - (4)]).tipo)
								errorSemantico(ERR_EXDER_ENT, (yyvsp[(2) - (4)]));
							else if((yyvsp[(1) - (4)]).tipo == REAL && !((yyvsp[(4) - (4)]).tipo == REAL || (yyvsp[(4) - (4)]).tipo == ENTERO))
								errorSemantico(ERR_EXDER_RE, (yyvsp[(2) - (4)]));
							else{
								if((yyvsp[(1) - (4)]).tipo == REAL && (yyvsp[(4) - (4)]).tipo == ENTERO){
									dir = NuevaTemporal();
									if(dir == 16383)
										errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (4)]));
									sprintf(s, "mov %d A;Ref opasig Expr\nitor\nmov A %d\nmov %d A\nmuli #%d\naddi #%d\nmov %d @A\n\n", (yyvsp[(4) - (4)]).dir, dir, (yyvsp[(1) - (4)]).dir, ttipos.tam((yyvsp[(1) - (4)]).tipo), (yyvsp[(1) - (4)]).dbase, dir);
								}
								else
									sprintf(s, "mov %d A;Ref opasig Expr\nmuli #%d\naddi #%d\nmov %d @A\n\n", (yyvsp[(1) - (4)]).dir, ttipos.tam((yyvsp[(1) - (4)]).tipo), (yyvsp[(1) - (4)]).dbase, (yyvsp[(4) - (4)]).dir);
							}

							(yyval).cod = (yyvsp[(1) - (4)]).cod + (yyvsp[(4) - (4)]).cod + s;
						}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 458 "plp2.y"
    {	(yyval).cod = (yyvsp[(2) - (3)]).cod;	}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 461 "plp2.y"
    {
					if((yyvsp[(3) - (3)]).tipo != LOGICO)
						errorSemantico(ERR_EXDER_LOG, (yyvsp[(2) - (3)]));
					else if((yyvsp[(1) - (3)]).tipo != LOGICO)
						errorSemantico(ERR_EXIZQ_LOG, (yyvsp[(2) - (3)]));
					else{
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (3)]));
						if((yyvsp[(1) - (3)]).tipoOp == ENTERO && (yyvsp[(3) - (3)]).tipoOp == ENTERO){
							sprintf(s, "mov %d A;Expr obool Econj\n%si %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
							(yyval).tipoOp = ENTERO;
							(yyval).tipo = LOGICO;
						}
						else if((yyvsp[(1) - (3)]).tipoOp == REAL && (yyvsp[(3) - (3)]).tipoOp == ENTERO){
							diritor = NuevaTemporal();
							if(diritor == 16383)
								errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (3)]));
							sprintf(s, "mov %d A;Expr obool Econj\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", (yyvsp[(3) - (3)]).dir, diritor, (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), diritor, dir);
							(yyval).tipoOp = REAL;
							(yyval).tipo = LOGICO;
						}
						else if((yyvsp[(1) - (3)]).tipoOp == ENTERO && (yyvsp[(3) - (3)]).tipoOp == REAL){
							sprintf(s, "mov %d A;Expr obool Econj\nitor\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
							(yyval).tipoOp = REAL;
							(yyval).tipo = LOGICO;
						}
						else if((yyvsp[(1) - (3)]).tipoOp == REAL && (yyvsp[(3) - (3)]).tipoOp == REAL){
							sprintf(s, "mov %d A;Expr obool Econj\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
							(yyval).tipoOp = REAL;
							(yyval).tipo = LOGICO;
						}

						(yyval).dir = dir;
						(yyval).cod = (yyvsp[(1) - (3)]).cod + (yyvsp[(3) - (3)]).cod + s;
					}
				}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 498 "plp2.y"
    {
					(yyval).cod = (yyvsp[(1) - (1)]).cod;
					(yyval).dir = (yyvsp[(1) - (1)]).dir;
					(yyval).tipoOp = (yyvsp[(1) - (1)]).tipoOp;
					(yyval).tipo = (yyvsp[(1) - (1)]).tipo;
					(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
					(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
				}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 508 "plp2.y"
    {
					if((yyvsp[(3) - (3)]).tipo != LOGICO){
						errorSemantico(ERR_EXDER_LOG, (yyvsp[(2) - (3)]));
					}
					else if((yyvsp[(1) - (3)]).tipo != LOGICO){
						errorSemantico(ERR_EXIZQ_LOG, (yyvsp[(2) - (3)]));
					}
					else{
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (3)]));
						if((yyvsp[(1) - (3)]).tipoOp == ENTERO && (yyvsp[(3) - (3)]).tipoOp == ENTERO){
							sprintf(s, "mov %d A;Econj ybool Ecomp\n%si %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
							(yyval).tipoOp = ENTERO;
							(yyval).tipo = LOGICO;
						}
						else if((yyvsp[(1) - (3)]).tipoOp == REAL && (yyvsp[(3) - (3)]).tipoOp == ENTERO){
							diritor = NuevaTemporal();
							if(diritor == 16383)
								errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (3)]));
							sprintf(s, "mov %d A;Econj ybool Ecomp\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", (yyvsp[(3) - (3)]).dir, diritor, (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), diritor, dir);
							(yyval).tipoOp = REAL;
							(yyval).tipo = LOGICO;
						}
						else if((yyvsp[(1) - (3)]).tipoOp == ENTERO && (yyvsp[(3) - (3)]).tipoOp == REAL){
							sprintf(s, "mov %d A;Econj ybool Ecomp\nitor\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
							(yyval).tipoOp = REAL;
							(yyval).tipo = LOGICO;
						}
						else if((yyvsp[(1) - (3)]).tipoOp == REAL && (yyvsp[(3) - (3)]).tipoOp == REAL){
							sprintf(s, "mov %d A;Econj ybool Ecomp\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
							(yyval).tipoOp = REAL;
							(yyval).tipo = LOGICO;
						}

						(yyval).dir = dir;
						(yyval).cod = (yyvsp[(1) - (3)]).cod + (yyvsp[(3) - (3)]).cod + s;
					}
				}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 547 "plp2.y"
    {
					(yyval).cod = (yyvsp[(1) - (1)]).cod;
					(yyval).dir = (yyvsp[(1) - (1)]).dir;
					(yyval).tipoOp = (yyvsp[(1) - (1)]).tipoOp;
					(yyval).tipo = (yyvsp[(1) - (1)]).tipo;
					(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
					(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
				}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 557 "plp2.y"
    {
					dir = NuevaTemporal();
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (3)]));
					(yyval).dir = dir;
					
					if((yyvsp[(1) - (3)]).tipo == LOGICO)
						errorSemantico(ERR_EXIZQ_RE, (yyvsp[(2) - (3)]));
					if((yyvsp[(1) - (3)]).tipo == ENTERO && (yyvsp[(3) - (3)]).tipo == ENTERO){
						sprintf(s, "mov %d A;Esimple oprel Esimple\n%si %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
						(yyval).tipoOp = ENTERO;
						(yyval).tipo = LOGICO;
					}
					else if((yyvsp[(1) - (3)]).tipo == REAL && (yyvsp[(3) - (3)]).tipo == ENTERO){
						diritor = NuevaTemporal();
						if(diritor == 16383)
							errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (3)]));
						sprintf(s, "mov %d A;Esimple oprel Esimple\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", (yyvsp[(3) - (3)]).dir, diritor, (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), diritor, dir);
						(yyval).tipoOp = REAL;
						(yyval).tipo = LOGICO;
					}
					else if((yyvsp[(1) - (3)]).tipo == ENTERO && (yyvsp[(3) - (3)]).tipo == REAL){
						sprintf(s, "mov %d A;Esimple oprel Esimple\nitor\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
						(yyval).tipoOp = REAL;
						(yyval).tipo = LOGICO;
					}
					else if((yyvsp[(1) - (3)]).tipo == REAL && (yyvsp[(3) - (3)]).tipo == REAL){
						sprintf(s, "mov %d A;Esimple oprel Esimple\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
						(yyval).tipoOp = REAL;
						(yyval).tipo = LOGICO;
					}
					else
						errorSemantico(ERR_EXDER_RE, (yyvsp[(2) - (3)]));

					(yyval).cod = (yyvsp[(1) - (3)]).cod + (yyvsp[(3) - (3)]).cod + s;
				}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 593 "plp2.y"
    {
					(yyval).cod = (yyvsp[(1) - (1)]).cod;
					(yyval).dir = (yyvsp[(1) - (1)]).dir;
					(yyval).tipoOp = (yyvsp[(1) - (1)]).tipoOp;
					(yyval).tipo = (yyvsp[(1) - (1)]).tipo;
					(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
					(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
				}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 603 "plp2.y"
    {
					dir = NuevaTemporal();
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (3)]));
					(yyval).dir = dir;
					if((yyvsp[(1) - (3)]).tipo == LOGICO)
						errorSemantico(ERR_EXIZQ_RE, (yyvsp[(2) - (3)]));
					if((yyvsp[(1) - (3)]).tipo == ENTERO && (yyvsp[(3) - (3)]).tipo == ENTERO){
						sprintf(s, "mov %d A;Esimple opas Term\n%si %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
						(yyval).tipo = ENTERO;
						(yyval).tipoOp = ENTERO;
					}
					else if((yyvsp[(1) - (3)]).tipo == REAL && (yyvsp[(3) - (3)]).tipo == ENTERO){
						diritor = NuevaTemporal();
						if(diritor == 16383)
							errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (3)]));
						sprintf(s, "mov %d A;Esimple opas Term\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", (yyvsp[(3) - (3)]).dir, diritor, (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), diritor, dir);
						(yyval).tipo = REAL;
						(yyval).tipoOp = REAL;
					}
					else if((yyvsp[(1) - (3)]).tipo == ENTERO && (yyvsp[(3) - (3)]).tipo == REAL){
						sprintf(s, "mov %d A;Esimple opas Term\nitor\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
						(yyval).tipo = REAL;
						(yyval).tipoOp = REAL;
					}
					else if((yyvsp[(1) - (3)]).tipo == REAL && (yyvsp[(3) - (3)]).tipo == REAL){
						sprintf(s, "mov %d A;Esimple opas Term\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
						(yyval).tipo = REAL;
						(yyval).tipoOp = REAL;
					}
					else
						errorSemantico(ERR_EXDER_RE, (yyvsp[(2) - (3)]));

					(yyval).cod = (yyvsp[(1) - (3)]).cod + (yyvsp[(3) - (3)]).cod + s;
				}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 638 "plp2.y"
    {
					(yyval).cod = (yyvsp[(1) - (1)]).cod;
					(yyval).dir = (yyvsp[(1) - (1)]).dir;
					(yyval).tipoOp = (yyvsp[(1) - (1)]).tipoOp;
					(yyval).tipo = (yyvsp[(1) - (1)]).tipo;
					(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
					(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
				}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 646 "plp2.y"
    {
					dir = NuevaTemporal();
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (2)]));
					(yyval).dir = dir;
	
					if((yyvsp[(1) - (2)]).trad == "sub"){
						if((yyvsp[(2) - (2)]).tipo == ENTERO){
							sprintf(s, "mov #0 A;opas Term\n%si %d\nmov A %d\n\n", (yyvsp[(1) - (2)]).trad.c_str(), (yyvsp[(2) - (2)]).dir, dir);
							(yyval).tipo = ENTERO;
						}
						else if((yyvsp[(2) - (2)]).tipo == REAL){
							sprintf(s, "mov #0 A;opas Term\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (2)]).trad.c_str(), (yyvsp[(2) - (2)]).dir, dir);
							(yyval).tipo = REAL;
						}
						else
							errorSemantico(ERR_EXDER_RE, (yyvsp[(2) - (2)]));
						
					}
					else{
						sprintf(s, "mov %d %d;opas Term\n\n", (yyvsp[(2) - (2)]).dir, dir);
						if((yyvsp[(2) - (2)]).tipo == ENTERO)
							(yyval).tipo = ENTERO;
						else if((yyvsp[(2) - (2)]).tipo == REAL)
							(yyval).tipo = REAL;
						else
							errorSemantico(ERR_EXDER_RE, (yyvsp[(2) - (2)]));
					}
	
					(yyval).cod = (yyvsp[(2) - (2)]).cod + s;
				}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 679 "plp2.y"
    {
					dir = NuevaTemporal(); 
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (3)]));
					(yyval).dir = dir;
	
					if((yyvsp[(1) - (3)]).tipo == ENTERO && (yyvsp[(3) - (3)]).tipo == ENTERO){
						sprintf(s, "mov %d A;Term opmd Factor\n%si %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
						(yyval).tipo = ENTERO;
						(yyval).tipoOp = ENTERO;
					}
					else if((yyvsp[(1) - (3)]).tipo == REAL && (yyvsp[(3) - (3)]).tipo == ENTERO){
						diritor = NuevaTemporal();
						if(diritor == 16383)
							errorSemantico(ERR_MAXTMP,(yyvsp[(1) - (3)]));
						sprintf(s, "mov %d A;Term opmd Factor\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", (yyvsp[(3) - (3)]).dir, diritor, (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), diritor, dir);
						(yyval).tipo = REAL;
						(yyval).tipoOp = REAL;
					}
					else if((yyvsp[(1) - (3)]).tipo == ENTERO && (yyvsp[(3) - (3)]).tipo == REAL){
						sprintf(s, "mov %d A;Term opmd Factor\nitor\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
						(yyval).tipo = REAL;
						(yyval).tipoOp = REAL;
					}
					else if((yyvsp[(1) - (3)]).tipo == REAL && (yyvsp[(3) - (3)]).tipo == REAL) {
						sprintf(s, "mov %d A;Term opmd Factor\n%sr %d\nmov A %d\n\n", (yyvsp[(1) - (3)]).dir, (yyvsp[(2) - (3)]).trad.c_str(), (yyvsp[(3) - (3)]).dir, dir);
						(yyval).tipo = REAL;
						(yyval).tipoOp = REAL;
					}
					else
						errorSemantico(ERR_EXDER_RE, (yyvsp[(2) - (3)]));

					(yyval).cod = (yyvsp[(1) - (3)]).cod + (yyvsp[(3) - (3)]).cod + s;
				}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 713 "plp2.y"
    {
					(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
					(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
					(yyval).dir = (yyvsp[(1) - (1)]).dir;
					(yyval).cod = (yyvsp[(1) - (1)]).cod;
					(yyval).tipo = (yyvsp[(1) - (1)]).tipo;
					(yyval).tipoOp = (yyvsp[(1) - (1)]).tipoOp;
				}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 723 "plp2.y"
    {
					if(isArray((yyvsp[(1) - (1)]).tipo))
						errorSemantico(ERRFALTAN, (yyvsp[(1) - (1)]));
					else{
						(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
						(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (1)]));
						(yyval).dir = dir;
						sprintf(s, "mov %d A;Ref\nmuli #%d\naddi #%d\nmov @A %d\n\n", (yyvsp[(1) - (1)]).dir, ttipos.tam((yyvsp[(1) - (1)]).tipo), (yyvsp[(1) - (1)]).dbase, dir);
						(yyval).cod = (yyvsp[(1) - (1)]).cod + s;
						(yyval).tipo = (yyvsp[(1) - (1)]).tipo;
						(yyval).tipoOp = (yyvsp[(1) - (1)]).tipoOp;
					}
				}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 739 "plp2.y"
    {
					(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
					(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
					dir = NuevaTemporal();
					(yyval).dir = dir;
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP,(yyvsp[(1) - (1)]));
					(yyval).tipo = ENTERO;
					(yyval).tipoOp = ENTERO;
					sprintf(s, "#%s", (yyvsp[(1) - (1)]).lexema.c_str());
					(yyval).trad = s;
					sprintf(s, "mov %s %d;numentero\n\n", (yyval).trad.c_str(), dir);
					(yyval).cod = s;
				}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 753 "plp2.y"
    {
					(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
					(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
					dir = NuevaTemporal();
					(yyval).dir = dir;
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP,(yyvsp[(1) - (1)]));
					(yyval).tipo = REAL;
					(yyval).tipoOp = REAL;
					sprintf(s, "$%s", (yyvsp[(1) - (1)]).lexema.c_str());
					(yyval).trad = s;
					sprintf(s, "mov %s %d;numreal\n\n", (yyval).trad.c_str(), dir);
					(yyval).cod = s;
				}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 767 "plp2.y"
    {
					(yyval).nlin = (yyvsp[(2) - (3)]).nlin;
					(yyval).ncol = (yyvsp[(2) - (3)]).ncol;
					(yyval).dir = (yyvsp[(2) - (3)]).dir;
					(yyval).cod = (yyvsp[(2) - (3)]).cod;
					(yyval).tipo = (yyvsp[(2) - (3)]).tipo;
				}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 774 "plp2.y"
    {
					dir = NuevaTemporal();
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP,(yyvsp[(1) - (2)]));
					(yyval).dir = dir;

					if((yyvsp[(2) - (2)]).tipoOp == ENTERO){
						sprintf(s, "mov %d A;nobool Factor\n%si\nmov A %d\n\n", (yyvsp[(2) - (2)]).dir, (yyvsp[(1) - (2)]).trad.c_str(), dir);
						(yyval).tipoOp = ENTERO;
						(yyval).tipo = LOGICO;
					}
					else if((yyvsp[(2) - (2)]).tipoOp == REAL){
						sprintf(s, "mov %d A;nobool Factor\n%sr\nmov A %d\n\n", (yyvsp[(2) - (2)]).dir, (yyvsp[(1) - (2)]).trad.c_str(), dir);
						(yyval).tipoOp = REAL;
						(yyval).tipo = LOGICO;
					}
	
					(yyval).cod = (yyvsp[(2) - (2)]).cod + s;
				}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 793 "plp2.y"
    {
					(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
					(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
					dir = NuevaTemporal();
					(yyval).dir = dir;
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (1)]));
					(yyval).tipo = LOGICO;
					(yyval).tipoOp = ENTERO;
					(yyval).trad = "#1";
					sprintf(s, "mov %s %d;cierto\n\n", "#1", dir);
					(yyval).cod = s;
				}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 806 "plp2.y"
    {
					(yyval).nlin = (yyvsp[(1) - (1)]).nlin;
					(yyval).ncol = (yyvsp[(1) - (1)]).ncol;
					dir = NuevaTemporal();
					(yyval).dir = dir;
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (1)]));
					(yyval).tipo = LOGICO;
					(yyval).tipoOp = ENTERO;
					(yyval).trad = "#0";
					sprintf(s, "mov %s %d;falso\n\n", "#0", dir);
					(yyval).cod = s;
				}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 821 "plp2.y"
    {
					simbolo = new Simbolo();
					if((simbolo = tsActual.buscarSimbolo((yyvsp[(1) - (1)]).lexema)) == NULL)
						errorSemantico(ERRNODECL, (yyvsp[(1) - (1)]));
					else{
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (1)]));
						(yyval).dir = dir;
						(yyval).dbase = simbolo->dir;
						sprintf(s, "mov #0 %d;id\n\n",dir);
						(yyval).cod = s;
						(yyval).tipo = simbolo->tipo;
						(yyval).tipoOp = simbolo->tipoOp;
					}
				}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 837 "plp2.y"
    {
					if(!isArray((yyvsp[(1) - (2)]).tipo))
						errorSemantico(ERRSOBRAN, (yyvsp[(2) - (2)]));
				}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 841 "plp2.y"
    {
					if((yyvsp[(4) - (5)]).tipo != ENTERO)
						errorSemantico(ERR_EXP_ENT, (yyvsp[(4) - (5)]));
					else{
						(yyval).tipo = ttipos.tipoBase((yyvsp[(1) - (5)]).tipo);
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, (yyvsp[(1) - (5)]));
						(yyval).dir = dir;
						(yyval).dbase = (yyvsp[(1) - (5)]).dbase;
						sprintf(s, "mov %d A;Ref [E]\nmuli #%d\naddi %d\nmov A %d\n\n", (yyvsp[(1) - (5)]).dir, ttipos.tam((yyvsp[(1) - (5)]).tipo), (yyvsp[(4) - (5)]).dir, dir);
						(yyval).cod = (yyvsp[(1) - (5)]).cod + (yyvsp[(4) - (5)]).cod + s;
						(yyval).ncol = (yyvsp[(5) - (5)]).ncol;
					}		
				}
    break;



/* Line 1806 of yacc.c  */
#line 2548 "plp2.tab.c"
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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 858 "plp2.y"


int NuevaTemporal(){
	int newDir = regTemporal;
	regTemporal++;
	return newDir;
}

void getTipoBaseArray(int &tipo){
	if(tipo != ENTERO && tipo != REAL && tipo != LOGICO){
		tipo = ttipos.tipoBase(tipo);
		getTipoBaseArray(tipo);
	}
	else 
		return;
}

bool isArray(int tipo){
	return tipo > 3;
}

int yyerror(std::string s)
{
	extern int findefichero;		// de plp2.l
	if (findefichero){
		msgError(ERREOF,0,0,"");
	}
	else{
		msgError(ERRSINT,nlin,ncol-strlen(yytext),yytext);
	}
}

void errorSemantico(int nerror, Atributos token){
	msgError(nerror,token.nlin,token.ncol,token.lexema.c_str());
}

void msgError(int nerror,int nlin,int ncol,const char *s){
	if (nerror != ERREOF){
		fprintf(stderr,"Error %d (%d:%d) ",nerror,nlin,ncol);
		switch (nerror){
			case ERRLEXICO:fprintf(stderr,"caracter '%s' incorrecto\n",s);
				break;
			case ERRSINT:fprintf(stderr,"en '%s'\n",s);
				break;
			case ERRYADECL:fprintf(stderr,"variable '%s' ya declarada\n",s);
				break;
			case ERRNODECL:fprintf(stderr,"variable '%s' no declarada\n",s);
				break;
			case ERRDIM:fprintf(stderr,"la dimension debe ser mayor que cero\n");
				break;
			case ERRFALTAN:fprintf(stderr,"faltan indices\n");
				break;
			case ERRSOBRAN:fprintf(stderr,"sobran indices\n");
				break;
			case ERR_EXP_ENT:fprintf(stderr,"la expresion entre corchetes debe ser de tipo entero\n");
				break;
			case ERR_EXP_LOG:fprintf(stderr,"la expresion debe ser de tipo logico\n");
				break;
			case ERR_EXDER_LOG:fprintf(stderr,"la expresion a la derecha de '%s' debe ser de tipo logico\n",s);
				break;
			case ERR_EXDER_ENT:fprintf(stderr,"la expresion a la derecha de '%s' debe ser de tipo entero\n",s);
				break;
			case ERR_EXDER_RE:fprintf(stderr,"la expresion a la derecha de '%s' debe ser de tipo real o entero\n",s);
				break;
			case ERR_EXIZQ_LOG:fprintf(stderr,"la expresion a la izquierda de '%s' debe ser de tipo logico\n",s);
				break;
			case ERR_EXIZQ_RE:fprintf(stderr,"la expresion a la izquierda de '%s' debe ser de tipo real o entero\n",s);
				break;
			case ERR_NOCABE:fprintf(stderr,"la variable '%s' ya no cabe en memoria\n",s);
				break;
			case ERR_MAXVAR:fprintf(stderr,"en la variable '%s', hay demasiadas variables declaradas\n",s);
				break;
			case ERR_MAXTIPOS:fprintf(stderr,"hay demasiados tipos definidos\n");
				break;
			case ERR_MAXTMP:fprintf(stderr,"no hay espacio para variables temporales\n");
				break;
		}
	}
	else
		fprintf(stderr,"Error al final del fichero\n");
	exit(1);
}

int main(int argc, char *argv[]){
	FILE *fent;
	if (argc == 2){
		fent = fopen(argv[1], "rt");
		if (fent) {
			yyin = fent;
			yyparse();
			fclose(fent);
		}
		else
			fprintf(stderr, "No puedo abrir el fichero\n");
	}
	else
		fprintf(stderr, "Uso: ejemplo <nombre de fichero>\n");
}

