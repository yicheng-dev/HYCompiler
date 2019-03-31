#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

int yylex();
void yyrestart(FILE*);
int yyparse();
int yyerror(char*);

typedef struct AST_Node
{
    int term_type; /* 0 -> token; 1 -> non-terminated */
    int token_type; /* type of tokens (enum) */
    int int_type; /* 0 -> Dec; 1 -> Oct; 2 -> Hex */
    char *name;
    char *value;
    int row_index;
    struct AST_Node *parent;
    struct AST_Node *first_child;
    struct AST_Node *sibling;
} AST_Node;

int str_to_int(char *str, int type);
AST_Node *create_node(char *name, char *value, int token_type, int lineno);
void add_child_sibling(AST_Node *parent, const int count,  ...);
void print_AST(AST_Node *node, int indent);

#endif