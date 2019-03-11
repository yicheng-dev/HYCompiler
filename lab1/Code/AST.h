#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern int line_num;

typedef struct AST_Node
{
    int term_type; /* 0 -> token; 1 -> non-terminated */
    int token_type; /* type of tokens (enum) */
    char *name;
    char *value;
    int row_index;
    struct AST_Node *parent;
    struct AST_Node *first_child;
    struct AST_Node *sibling;
} AST_Node;

AST_Node *create_node(char *name, char *value, int token_type){
    AST_Node *node = (AST_Node *)malloc(sizeof(AST_Node));
    node->name = (char *)malloc(sizeof(name));
    strcpy(node->name, name);
    node->value = (char *)malloc(sizeof(value));
    strcpy(node->value, value);
    node->row_index = line_num;
    node->token_type = token_type;
    node->term_type = token_type == -1 ? 1 : 0;
    /*printf("name: %s\tvalue: %s\trow_index: %d\ttoken_type: %d\tterm_type: %d\n", 
        node->name, node->value, node->row_index, node->token_type, node->term_type);*/
    return node;
}

void add_child_sibling(AST_Node *parent, const int count,  ...){
    /*printf("Parent --- name: %s\tvalue: %s\trow_index: %d\ttoken_type: %d\tterm_type: %d\n", 
        parent->name, parent->value, parent->row_index, parent->token_type, parent->term_type);*/
    va_list ap;
    int i;
    va_start(ap, count);
    AST_Node *last_node;
    //printf("count: %d\n", count);
    for (i = 0; i < count; i++){
        AST_Node *node = va_arg(ap, AST_Node*);
        /*printf("children --- name: %s\tvalue: %s\trow_index: %d\ttoken_type: %d\tterm_type: %d\n", 
            node->name, node->value, node->row_index, node->token_type, node->term_type);*/
        node->parent = parent;
        if (i == 0){
            parent->first_child = node;
            last_node = node;
        }
        else{
            last_node->sibling = node;
            last_node = node;
        }
    }
    va_end(ap);
}

void print_AST(AST_Node *node, int indent){
    for (int i = 0; i < indent; i++){
        printf(" ");
    }
    printf("%s (%d)\n", node->name, node->row_index);
    AST_Node *tmp = node->first_child;
    while (tmp){
        print_AST(tmp, indent + 2);
        tmp = tmp -> sibling;
    }
}

#endif