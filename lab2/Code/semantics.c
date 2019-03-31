#include "semantics.h"
#include "utils.h"
#include "AST.h"
#include "debug.h"

Var *var_hash[MAX_HASH_TABLE_LEN];
Func *func_hash[MAX_HASH_TABLE_LEN];

void init_hash_table() {
    int i;
    for (i = 0; i < MAX_HASH_TABLE_LEN; i ++){
        var_hash[i] = NULL;
        func_hash[i] = NULL;
    }
}

void semantics_analysis(AST_Node *root){
    init_hash_table();
    sem_program(root);
}

void sem_program(AST_Node *root){
    sem_ext_def_list(root->first_child);
}

void sem_ext_def_list(AST_Node *node){
    if (node && node->first_child){
        sem_ext_def(node->first_child);
        sem_ext_def_list(node->first_child->sibling);
    }
}

void sem_ext_def(AST_Node *node){
    assert(node);
    Type *type = sem_specifier(node->first_child); // get type from specifier
    if (strcmp(node->first_child->sibling->name, "ExtDecList") == 0)
        sem_ext_dec_list(node->first_child->sibling, type); // set type to decList
}

void sem_ext_dec_list(AST_Node *node, Type *type){
    
}

Type* sem_specifier(AST_Node *node){
    return NULL;
}