#ifndef AST_H
#define AST_H
#endif

typedef struct AST_Node
{
    int type; /* 0 -> token; 1 -> non-terminated */
    char *name;
    int row_index;
    int int_val;
    float float_val;
    struct AST_Node *parent;
    struct AST_Node *first_child;
    struct AST_Node *sibling;
} AST_Node;
