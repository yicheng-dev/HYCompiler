#include "semantics.h"
#include "utils.h"
#include "AST.h"
#include "debug.h"
#include "ir.h"

static InterCode *ir_head = NULL;
static Field_List *var_hash[MAX_HASH_TABLE_LEN];
static Func *func_hash[MAX_HASH_TABLE_LEN];

void ir_generate(AST_Node *root) {
    ir_program(root);
}

void ir_program(AST_Node *node) {
    ir_ext_def_list(node->first_child);
}

InterCode *ir_ext_def_list(AST_Node *node) {
    assert(node);
    if (node->first_child) {
        InterCode *code1 = ir_ext_def(node->first_child);
        InterCode *code2 = ir_ext_def_list(node->first_child->sibling);
        return bind(code1, code2);
    }
}

InterCode *ir_ext_def(AST_Node *node) {
    /* not consider global variables */
    assert(node && node->first_child && node->first_child->sibling);
    if (strcmp(node->first_child->sibling->name, "SEMI") == 0) {
        ir_specifier(node->first_child);
        return NULL;
    }
    if (strcmp(node->first_child->sibling->name, "FunDec") == 0) {
        Type *ret_type = ir_specifier(node->first_child);
        InterCode *fun_code = ir_fun_dec(node->first_child->sibling);
        InterCode *comp_code = ir_comp_st(node->first_child->sibling->sibling);
        return bind(fun_code, comp_code);
    }
}

InterCode *ir_fun_dec(AST_Node *node) {
    assert(node && node->first_child && node->first_child->sibling && node->first_child->sibling->sibling);
    Func *func = ir_query_func_hash_table(hash_pjw(node->first_child->value), node->first_child->value);
    assert(func);
    if (node->first_child->sibling->sibling->sibling != NULL) {
        if (strcmp(func->name, "read") == 0) {

        }
    }
    else {

    }
}

InterCode *bind(InterCode *code1, InterCode *code2) {
    InterCode *ret_code = code1;
    InterCode *code1_tail = code1;
    if (code1 == NULL && code2 == NULL) {
        return NULL;
    }
    if (code1 == NULL && code2 != NULL) {
        return code2;
    }
    if (code1 != NULL && code2 == NULL) {
        return code1;
    }
    if (code1 == NULL && code1 != NULL) {
        ir_head = code1;
    }
    while (code1_tail != NULL) {
        code1_tail = code1_tail->next;
    }
    code1_tail->next = code2;
    code2->prev = code1_tail;
    return ret_code;
}

Field_List *ir_insert_field_hash_table(unsigned hash_index, char *str, Type *type, AST_Node *node, int wrapped_layer, int is_structure){
    Field_List *new_field;
    new_field = malloc(sizeof(Field_List));
    strcpy(new_field->name, node->value);
    new_field->type = type;
    new_field->wrapped_layer = wrapped_layer;
    new_field->is_structure = is_structure;
    new_field->line_num = node->row_index;
    new_field->next = var_hash[hash_index];
    var_hash[hash_index] = new_field;
    return new_field;            
}

Func *ir_insert_func_hash_table(unsigned hash_index, char *str, Type *return_type, Func *func){
    Func *cur = func_hash[hash_index];
    if (cur == NULL){
        func->return_type = return_type;
        func->defined = 1;
        func_hash[hash_index] = func;
        return func;
    }
    func->return_type = return_type;
    func->defined = 1;
    func->next = func_hash[hash_index];
    func_hash[hash_index] = func;
    return func;
}

Func *ir_query_func_hash_table(unsigned hash_index, char *str){
    Func *cur = func_hash[hash_index];
    if (cur == NULL)
        return NULL;
    while (cur){
        if (strcmp(str, cur->name) == 0)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

Field_List *ir_query_field_hash_table(unsigned hash_index, char *str, AST_Node *node, int look_for_structure){
    Field_List *field_now = var_hash[hash_index];
    while(field_now != NULL){
        if (strcmp(field_now->name, str) == 0 && field_now->is_structure == look_for_structure)
            return field_now;
        field_now = field_now->next;
    }
    return NULL;
}