%{
  #include "syntax.tab.h"
  #include <stdio.h>
  FILE *in;
  yydebug = 1;
  int error_flag = 0;
  int line_num = 1;
%}

%option yylineno
SPACE			[ \t]+
CHLINE			\n+
COMMENT_LINE	"//"[^\n]*
COMMENT			"/*"[^"*/""/*"]*"*/"
COMMENT_ERROR	("/*"?[^"*/"]*"*/"[^"*/"]*"*/")|("/*"[^"/*"]*"/*"[^"/*"]*"*/"?)
STRUCT			struct
RETURN			return
IF				if
ELSE			else
WHILE			while
TYPE			int|float
OCTINT			([+-]?0[0-7]+)
OCTINT_ERROR	([+-]?0[0-9a-zA-Z]+)
HEXINT			([+-]?0[Xx][0-9a-fA-F]+)
HEXINT_ERROR	([+-]?0[Xx][0-9a-zA-Z]+)
INT				([+-]?(0|[1-9][0-9]*))
FLOAT			[+-]?([0-9]*\.?[0-9]+|[0-9]+\.?)
FLOAT_E			{FLOAT}[Ee][+-]?[0-9]+
FLOAT_ERROR		({FLOAT}[Ee][+-]?)|({FLOAT}[Ee][+-]?[0-9]*\.[0-9]*)
ID				[_a-zA-Z][_a-zA-Z0-9]*
SEMI 			;
COMMA			,
ASSIGNOP		=
RELOP			>|<|>=|<=|==|!=
PLUS			\+
MINUS			\-
STAR			\*
DIV				\/
AND				&&
OR				\|\|
DOT				\.
NOT				!
LP				\(
RP				\)
LB				\[
RB				\]
LC				\{
RC				\}


%%

{SPACE}			{ }
{CHLINE}		{ line_num ++; }
{COMMENT}		{ }
{COMMENT_LINE}	{ }
{COMMENT_ERROR}	{ printf("Error type A at Line %d: Invalid comment \'%s\'.\n", yylineno, yytext); error_flag = 1; }
{STRUCT}		{ return STRUCT; }
{RETURN}		{ return RETURN; }
{IF}			{ return IF; }
{ELSE}			{ return ELSE; }
{WHILE}			{ return WHILE; }
{TYPE}			{ return TYPE; }
{OCTINT}		{ return INT; }
{HEXINT}		{ return INT; }
{INT}			{ return INT; }
{HEXINT_ERROR}	{ printf("Error type A at Line %d: Invalid Hex number \'%s\'.\n", yylineno, yytext); error_flag = 1; }
{OCTINT_ERROR}	{ printf("Error type A at Line %d: Invalid Oct number \'%s\'.\n", yylineno, yytext); error_flag = 1; }
{FLOAT}			{ return FLOAT; }
{FLOAT_E}		{ return FLOAT; }
{FLOAT_ERROR}	{ printf("Error type A at Line %d: Invalid FLOAT \'%s\'.\n", yylineno, yytext); error_flag = 1; }
{ID}			{ return ID; }
{SEMI} 			{ return SEMI; }
{COMMA}			{ return COMMA; }
{ASSIGNOP}		{ return ASSIGNOP; }
{RELOP}			{ return RELOP; }
{PLUS}			{ return PLUS; }
{MINUS}			{ return MINUS; }
{STAR}			{ return STAR; }
{DIV}			{ return DIV; }
{AND}			{ return AND; }
{OR}			{ return OR; }
{DOT}			{ return DOT; }
{NOT}			{ return NOT; }
{LP}			{ return LP; }
{RP}			{ return RP; }
{LB}			{ return LB; }
{RB}			{ return RB; }
{LC}			{ return LC; }
{RC}			{ return RC; }
.				{ printf("Error type A at Line %d: Mysterious characters \'%s\'.\n", yylineno, yytext); error_flag = 1; }

%%

int main(int argc, char **argv) {
  if (argc > 1){
	  if (!(in = fopen(argv[1], "r"))){
	    perror(argv[1]);
	    return 1;
	  }
	  yyrestart(in);
    if (error_flag == 0){
      AST_Node *root = malloc(sizeof(AST_Node));
	    yyparse();
    }
	  return 0;
  }
  if (error_flag == 0)
    yyparse();
  return 0;
}