%{
  #include <stdio.h>
  #include "AST.h"
  //#define YYERROR_VERBOSE
  //#define YYDEBUG 1
%}

%union{
  int type_int;
  float type_float;
  double type_double;
  AST_Node *node;
}

/* tokens */
%token <node> INT FLOAT ID 
%token <node> SEMI COMMA ASSIGNOP RELOP
%token <node> PLUS MINUS STAR DIV
%token <node> AND OR
%token <node> DOT
%token <node> NOT
%token <node> TYPE
%token <node> LP RP LB RB LC RC
%token <node> STRUCT RETURN IF ELSE WHILE

/* non-terminals */
%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier OptTag Tag
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def DecList Dec
%type <node> Exp Args

/* precedence & associativity */
%right  ASSIGNOP
%left   OR
%left   AND
%left   RELOP
%left   PLUS MINUS
%left   STAR DIV
%right  NOT
%left   DOT LP RP LB RB

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE STRUCT IF WHILE RETURN

%%

/* High-level Definitions */
Program : ExtDefList {}
  ;
ExtDefList : ExtDef ExtDefList {}
  | {}
  ;
ExtDef : Specifier ExtDecList SEMI {}
  | Specifier SEMI {}
  | Specifier FunDec CompSt {}
  | error SEMI {}
  ;
ExtDecList : VarDec {}
  | VarDec COMMA ExtDecList {}
  ;

/* Specifiers */
Specifier : TYPE {}
  | StructSpecifier {}
  ;
StructSpecifier : STRUCT OptTag LC DefList RC {}
  | STRUCT Tag {}
  | error RC {}
  ;
OptTag : ID {}
  | {}
  ;
Tag : ID {}
  ;

/* Declarators */
VarDec : ID {}
  | VarDec LB INT RB {}
  | error RB {}
  ;
FunDec : ID LP VarList RP {}
  | ID LP RP {}
  | error RP {}
  ;
VarList : ParamDec COMMA VarList {}
  | ParamDec {}
  ;
ParamDec : Specifier VarDec {}
  ;

/* Statements */
CompSt : LC DefList StmtList RC {}
  ;
StmtList : Stmt StmtList {}
  | {}
  ;
Stmt : Exp SEMI {}
  | CompSt {}
  | RETURN Exp SEMI {}
  | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {}
  | IF LP Exp RP Stmt ELSE Stmt {}
  | WHILE LP Exp RP Stmt {}
  ;

/* Local Definitions */
DefList : Def DefList {}
  | {}
  ;
Def : Specifier DecList SEMI {}
  | error SEMI {}
  ;
DecList : Dec {}
  | Dec COMMA DecList {}
  ;
Dec : VarDec {}
  | VarDec ASSIGNOP Exp {}
  ;

/* Expressions */
Exp : Exp ASSIGNOP Exp {}
  | Exp AND Exp {}
  | Exp OR Exp {}
  | Exp RELOP Exp {}
  | Exp PLUS Exp {}
  | Exp MINUS Exp {}
  | Exp STAR Exp {}
  | Exp DIV Exp {}
  | LP Exp RP {}
  | MINUS Exp {}
  | NOT Exp {}
  | ID LP Args RP {}
  | ID LP RP {}
  | Exp LB Exp RB {}
  | Exp DOT ID {}
  | ID {}
  | INT {}
  | FLOAT {}
  ;
Args : Exp COMMA Args {}
  | Exp {}
  ;

%%

#include "lex.yy.c"

yyerror(char *msg){
  fprintf(stderr, "line %d: %s\n", line_num, msg);
}