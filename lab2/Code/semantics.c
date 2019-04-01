#include "semantics.h"
#include "utils.h"
#include "AST.h"
#include "debug.h"

Field_List *var_hash[MAX_HASH_TABLE_LEN];
Func *func_hash[MAX_HASH_TABLE_LEN];
Error_List *error_head = NULL;
extern int error_flag;

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
    else{
        //TODO
    }
}

void sem_ext_dec_list(AST_Node *node, Type *type){
    assert(node && node->first_child);
    sem_var_dec(node->first_child, type);
    if (node->first_child->sibling)
        sem_ext_dec_list(node->first_child->sibling->sibling, type);
}

Type* sem_specifier(AST_Node *node){
    assert(node);
    if (strcmp(node->first_child->name, "TYPE") == 0){
        Type *type = (Type *)malloc(sizeof(Type));
        type->kind = BASIC;
        type->u.basic = BASIC_INT;
        return type;
    }
    assert(strcmp(node->first_child->name, "StructSpecifier") == 0);
    return sem_struct_specifier(node->first_child);
}

Type* sem_struct_specifier(AST_Node *node){
    //TODO
    return NULL;
}

void sem_var_dec(AST_Node *node, Type *type){
    assert(node && node->first_child);
    if (strcmp(node->first_child->name, "ID") == 0){
        unsigned hash_index = hash_pjw(node->first_child->value);
        insert_field_hash_table(hash_index, type, node->first_child, 0);
    }
    else{
        //TODO
    }
}


void insert_field_hash_table(unsigned hash_index, Type *type, AST_Node *node, int wrapped_layer){
    Field_List *field_head = var_hash[hash_index];
    if (field_head && field_head->wrapped_layer >= wrapped_layer){
        char info[MAX_ERROR_INFO_LEN];
        sprintf(info, "redefinition of %s\n", node->value);
        add_error_list(3, info, node->row_index);
    }
    else{
        Field_List *new_field = (Field_List *)malloc(sizeof(Field_List));
        strcpy(new_field->name, node->value);
        new_field->type = type;
        new_field->wrapped_layer = wrapped_layer;
        if (!field_head){
            field_head = new_field;
        }
        else{
            new_field->next = field_head;
            field_head = new_field;
        }
        var_hash[hash_index] = field_head;
    }
}

void add_error_list(int type, char *info, int line_num){
    error_flag = 1;
    Error_List *error = (Error_List *)malloc(sizeof(Error_List));
        error->type = type;
        strcpy(error->info, info);
        error->line_num = line_num;
        error->next = NULL;
    if (!error_head){
        error_head = error;
    }
    else{
        error_head->next = error;
    }
}

void print_error_list(){
    Error_List *cur = error_head;
    while (cur){
        printf("Error type %d at Line %d: %s", cur->type, cur->line_num, cur->info);
        cur = cur->next;
    }
}
