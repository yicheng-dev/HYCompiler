#include "AST.h"

extern int yylineno;
extern int empty_flag;

int str_to_int(char *str, int type){
    if (type == 0)
        return atoi(str);
    else if (type == 1)
        return (int)strtol(str, NULL, 8); 
    else
        return (int)strtol(str, NULL, 16);
}

AST_Node *create_node(char *name, char *value, int token_type, int lineno){
    AST_Node *node = (AST_Node *)malloc(sizeof(AST_Node));
    node->name = (char *)malloc(sizeof(name));
    strcpy(node->name, name);
    node->value = (char *)malloc(sizeof(value));
    strcpy(node->value, value);
    node->row_index = lineno;
    node->token_type = token_type;
    node->term_type = token_type == -1 ? 1 : 0;
    // printf("name: %s\tvalue: %s\trow_index: %d\ttoken_type: %d\tterm_type: %d\n", 
    //     node->name, node->value, node->row_index, node->token_type, node->term_type);
    return node;
}

void add_child_sibling(AST_Node *parent, const int count,  ...){
    va_list ap;
    int i;
    va_start(ap, count);
    AST_Node *last_node;
    for (i = 0; i < count; i++){
        AST_Node *node = va_arg(ap, AST_Node*);
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
    if (!node)
        return;
    if (empty_flag == 1){
        printf("Empty file!\n");
        return;
    }
    if (node->term_type == 1 && !node->first_child)
        return;
    for (int i = 0; i < indent; i++){
        printf(" ");
    }
    if (node->term_type == 1){
        printf("%s (%d)\n", node->name, node->row_index);
    }
    else{
        if (strcmp(node->name, "INT") == 0)
            printf("%s: %d\n", node->name, str_to_int(node->value, node->int_type));
        else if (strcmp(node->name, "FLOAT") == 0)
            printf("%s: %f\n", node->name, atof(node->value));
        else if (strcmp(node->name, "ID") == 0)
            printf("%s: %s\n", node->name, node->value);
        else if (strcmp(node->name, "TYPE") == 0)
            printf("%s: %s\n", node->name, node->value);
        else
            printf("%s\n", node->name);
    }
    AST_Node *tmp = node->first_child;
    while (tmp){
        print_AST(tmp, indent + 2);
        tmp = tmp -> sibling;
    }
}