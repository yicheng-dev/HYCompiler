%{
  #include "syntax.tab.h"
  #include <stdio.h>
  #include "AST.h"
  FILE *in;
  yydebug = 1;
  int error_flag = 0;
  int line_num = 1;
  AST_Node *root;
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
{STRUCT}		{ yylval.node = create_node("STRUCT", yytext, STRUCT); return STRUCT; }
{RETURN}		{ yylval.node = create_node("RETURN", yytext, RETURN); return RETURN; }
{IF}			{ yylval.node = create_node("IF", yytext, IF); return IF; }
{ELSE}			{ yylval.node = create_node("ELSE", yytext, ELSE); return ELSE; }
{WHILE}			{ yylval.node = create_node("WHILE", yytext, WHILE); return WHILE; }
{TYPE}			{ yylval.node = create_node("TYPE", yytext, TYPE); return TYPE; }
{OCTINT}		{ yylval.node = create_node("INT", yytext, INT); return INT; }
{HEXINT}		{ yylval.node = create_node("INT", yytext, INT); return INT; }
{INT}			{ yylval.node = create_node("INT", yytext, INT); return INT; }
{HEXINT_ERROR}	{ printf("Error type A at Line %d: Invalid Hex number \'%s\'.\n", yylineno, yytext); error_flag = 1; }
{OCTINT_ERROR}	{ printf("Error type A at Line %d: Invalid Oct number \'%s\'.\n", yylineno, yytext); error_flag = 1; }
{FLOAT}			{ yylval.node = create_node("FLOAT", yytext, FLOAT); return FLOAT; }
{FLOAT_E}		{ yylval.node = create_node("FLOAT", yytext, FLOAT); return FLOAT; }
{FLOAT_ERROR}	{ printf("Error type A at Line %d: Invalid FLOAT \'%s\'.\n", yylineno, yytext); error_flag = 1; }
{ID}			{ yylval.node = create_node("ID", yytext, ID); return ID; }
{SEMI} 			{ yylval.node = create_node("SEMI", yytext, SEMI); return SEMI; }
{COMMA}			{ yylval.node = create_node("COMMA", yytext, COMMA); return COMMA; }
{ASSIGNOP}		{ yylval.node = create_node("ASSIGNOP", yytext, ASSIGNOP); return ASSIGNOP; }
{RELOP}			{ yylval.node = create_node("RELOP", yytext, RELOP); return RELOP; }
{PLUS}			{ yylval.node = create_node("PLUS", yytext, PLUS); return PLUS; }
{MINUS}			{ yylval.node = create_node("MINUS", yytext, MINUS); return MINUS; }
{STAR}			{ yylval.node = create_node("STAR", yytext, STAR); return STAR; }
{DIV}			{ yylval.node = create_node("DIV", yytext, DIV); return DIV; }
{AND}			{ yylval.node = create_node("AND", yytext, AND); return AND; }
{OR}			{ yylval.node = create_node("OR", yytext, OR); return OR; }
{DOT}			{ yylval.node = create_node("DOT", yytext, DOT); return DOT; }
{NOT}			{ yylval.node = create_node("NOT", yytext, NOT); return NOT; }
{LP}			{ yylval.node = create_node("LP", yytext, LP); return LP; }
{RP}			{ yylval.node = create_node("RP", yytext, RP); return RP; }
{LB}			{ yylval.node = create_node("LB", yytext, LB); return LB; }
{RB}			{ yylval.node = create_node("RB", yytext, RB); return RB; }
{LC}			{ yylval.node = create_node("LC", yytext, LC); return LC; }
{RC}			{ yylval.node = create_node("RC", yytext, RC); return RC; }
.				{ printf("Error type A at Line %d: Mysterious characters \'%s\'.\n", yylineno, yytext); error_flag = 1; }

%%

int main(int argc, char **argv) {
  if (argc > 1){
	  if (!(in = fopen(argv[1], "r"))){
	    perror(argv[1]);
	    return 1;
	  }
	  yyrestart(in);
  }
  root = malloc(sizeof(AST_Node));
	yyparse();
  if (error_flag == 0){
    print_AST(root, 0);
  }
	return 0;
}