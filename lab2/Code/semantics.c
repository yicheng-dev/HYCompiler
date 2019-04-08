#include "semantics.h"
#include "utils.h"
#include "AST.h"
#include "debug.h"

Field_List *var_hash[MAX_HASH_TABLE_LEN];
Func *func_hash[MAX_HASH_TABLE_LEN];
Error_List *error_head = NULL;
extern int error_flag;
Type *return_type_list = NULL;

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
    check_undec_func();
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
    Type *type = sem_specifier(node->first_child, 0, 0); // get type from specifier
    if (strcmp(node->first_child->sibling->name, "ExtDecList") == 0)
        // usage of structure.first_field or TYPE
        sem_ext_dec_list(node->first_child->sibling, type); // set type to decList
    else if (strcmp(node->first_child->sibling->name, "SEMI") == 0){
        // Definition of structure
        if (!type && strcmp(node->first_child->first_child->first_child->sibling->name, "Tag") == 0){ // struct a; ('a' hasn't been defined)
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "structure '%s' hasn't been defined.\n", node->first_child->first_child->first_child->sibling->first_child->value);
            add_error_list(17, info, node->first_child->first_child->first_child->sibling->first_child->row_index);
        }
    }
    else{
        assert(strcmp(node->first_child->sibling->name, "FunDec") == 0);
        if (strcmp(node->first_child->sibling->sibling->name, "CompSt") == 0){ // Definition of functions
            Func *func = sem_fun_dec(node->first_child->sibling);
            if (!func)
                return;
            unsigned hash_index = hash_pjw(func->name);
            insert_func_hash_table(hash_index, type, func);
            sem_comp_st(node->first_child->sibling->sibling, 1);
            pop_local_var(1);
            Type *cur = return_type_list;
            while (cur){
                if (!check_equal_type(type, cur, 0)){
                    char info[MAX_ERROR_INFO_LEN];
                    sprintf(info, "Type mismatched for return.\n");
                    add_error_list(8, info, cur->line_num);
                    return;
                }
                cur = cur->next_ret_type;
            }
        }
        else{ // Declaration of functions
            assert(strcmp(node->first_child->sibling->sibling->name, "SEMI") == 0);
            Func *func = sem_fun_dec(node->first_child->sibling);
            if (!func)
                return;
            unsigned hash_index = hash_pjw(func->name);
            insert_func_dec_hash_table(hash_index, type, func);
            pop_local_var(1);
        }
    }
}

void sem_ext_dec_list(AST_Node *node, Type *type){
    assert(node && node->first_child);
    sem_var_dec(node->first_child, type, 0, 0);
    if (node->first_child->sibling)
        sem_ext_dec_list(node->first_child->sibling->sibling, type);
}

Type* sem_specifier(AST_Node *node, int wrapped_layer, int in_structure){
    assert(node);
    if (strcmp(node->first_child->name, "TYPE") == 0){
        Type *type = (Type *)malloc(sizeof(Type));
        type->kind = BASIC;
        if (strcmp(node->first_child->value, "int") == 0)
            type->u.basic = BASIC_INT;
        else
            type->u.basic = BASIC_FLOAT;
        return type;
    }
    assert(strcmp(node->first_child->name, "StructSpecifier") == 0);
    return sem_struct_specifier(node->first_child, wrapped_layer, in_structure);
}

Type* sem_struct_specifier(AST_Node *node, int wrapped_layer, int in_structure){
    assert(node && node->first_child && strcmp(node->first_child->name, "STRUCT") == 0);
    if (strcmp(node->first_child->sibling->name, "Tag") == 0){
        AST_Node *tag_node = node->first_child->sibling->first_child;
        unsigned hash_index = hash_pjw(tag_node->value);
        Field_List * field = query_field_hash_table(hash_index, tag_node, 1);
        if (field)
            return field->type;
        return NULL;
    }
    assert(strcmp(node->first_child->sibling->name, "OptTag") == 0);
    Type *structure_type = (Type *)malloc(sizeof(Type));
    structure_type->kind = STRUCTURE;
    structure_type->u.structure.first_field = sem_def_list(node->first_child->sibling->sibling->sibling, 1, wrapped_layer);
    structure_type->u.structure.first_flat = struct_type_to_list(structure_type->u.structure.first_field);
    structure_type->line_num = node->first_child->sibling->sibling->sibling->row_index;
    if (!check_duplicate_field(structure_type)){
        return NULL;
    }
    if (node->first_child->sibling->first_child){
        unsigned hash_index = hash_pjw(node->first_child->sibling->first_child->value);
        insert_field_hash_table(hash_index, structure_type, node->first_child->sibling->first_child, wrapped_layer, 1);
    }
    return structure_type;
}

Field_List *sem_var_dec(AST_Node *node, Type *type, int in_structure, int wrapped_layer){
    assert(node && node->first_child);
    if (!type){
        char info[MAX_ERROR_INFO_LEN];
        sprintf(info, "Undefined structure.\n");
        add_error_list(17, info, node->first_child->row_index);
        return NULL;
    }
    if (strcmp(node->first_child->name, "ID") == 0){
        if (in_structure){
            Field_List *new_field = (Field_List *)malloc(sizeof(Field_List));
            new_field->type = type;
            new_field->is_structure = 0;
            new_field->wrapped_layer = wrapped_layer;
            new_field->line_num = node->first_child->row_index;
            strcpy(new_field->name, node->first_child->value);
            return new_field;
        }
        else{
            unsigned hash_index = hash_pjw(node->first_child->value);
            return insert_field_hash_table(hash_index, type, node->first_child, wrapped_layer, 0);
        }
    }
    else{
        Type *array_type = (Type *)malloc(sizeof(Type));
        array_type->kind = ARRAY;
        array_type->u.array.size = str_to_int(node->first_child->sibling->sibling->value, node->first_child->sibling->sibling->int_type);
        array_type->u.array.elem = type;
        return sem_var_dec(node->first_child, array_type, in_structure, wrapped_layer);
    }
}

Func *sem_fun_dec(AST_Node *node){
    assert(node && node->first_child);
    Func *func = (Func *)malloc(sizeof(Func));
    strcpy(func->name, node->first_child->value);
    func->line_num = node->first_child->row_index;
    if (strcmp(node->first_child->sibling->sibling->name, "VarList") == 0){
        Field_List *params = sem_var_list(node->first_child->sibling->sibling);
        if (!params)
            return NULL;
        func->first_param = params;
        Field_List *cur = func->first_param;
        func->param_size = 0;
        while (cur){
            func->param_size ++;
            cur = cur->next;
        }

        return func;
    }
    else{
        func->param_size = 0;
        func->first_param = NULL;
        return func;
    }
}

Field_List *sem_var_list(AST_Node *node){
    assert(node && node->first_child);
    Field_List *field = sem_param_dec(node->first_child);
    if (!field)
        return NULL;
    if (node->first_child->sibling){
        field->next = sem_var_list(node->first_child->sibling->sibling);
    }
    return field;
}

Field_List *sem_param_dec(AST_Node *node){
    assert(node && node->first_child && node->first_child->sibling);
    Type *type = sem_specifier(node->first_child, 1, 0);
    return sem_var_dec(node->first_child->sibling, type, 0, 1);
}

void sem_comp_st(AST_Node *node, int wrapped_layer){
    assert(node && node->first_child && node->first_child->sibling && node->first_child->sibling->sibling);
    return_type_list = NULL;
    sem_def_list(node->first_child->sibling, 0, wrapped_layer);
    sem_stmt_list(node->first_child->sibling->sibling, wrapped_layer);
    pop_local_var(wrapped_layer);
}

void sem_stmt_list(AST_Node *node, int wrapped_layer){
    assert(node);
    if (node->first_child){
        sem_stmt(node->first_child, wrapped_layer);
        sem_stmt_list(node->first_child->sibling, wrapped_layer);
    }
}

void sem_stmt(AST_Node *node, int wrapped_layer){
    assert(node && node->first_child);
    if (strcmp(node->first_child->name, "CompSt") == 0){
        sem_comp_st(node->first_child, wrapped_layer + 1);
    }
    if (strcmp(node->first_child->name, "Exp") == 0){
        sem_exp(node->first_child);
    }
    if (strcmp(node->first_child->name, "RETURN") == 0){
        Type *type = sem_exp(node->first_child->sibling);
        if (!type)
            return;
        type->line_num = node->first_child->sibling->row_index;
        if (return_type_list){
            type->next_ret_type = return_type_list;
            return_type_list = type;
        }
        else{
            return_type_list = type;
        }
    }
    if (strcmp(node->first_child->name, "IF") == 0){
        sem_exp(node->first_child->sibling->sibling);
        sem_stmt(node->first_child->sibling->sibling->sibling->sibling, wrapped_layer);
        if (node->first_child->sibling->sibling->sibling->sibling->sibling){
            sem_stmt(node->first_child->sibling->sibling->sibling->sibling->sibling->sibling, wrapped_layer);
        }
    }
    if (strcmp(node->first_child->name, "WHILE") == 0){
        sem_exp(node->first_child->sibling->sibling);
        sem_stmt(node->first_child->sibling->sibling->sibling->sibling, wrapped_layer);
    }
}

Field_List *sem_def_list(AST_Node *node, int in_structure, int wrapped_layer){
    assert(node);
    if (node->first_child){
        Field_List *field = sem_def(node->first_child, in_structure, wrapped_layer);
        Field_List *cur = field; // insert field_lists of a def after the front def
        if (!cur)
            return NULL;
        if (in_structure){
            while(cur && cur->next){
                cur = cur->next;
            }
            cur->next = sem_def_list(node->first_child->sibling, in_structure, wrapped_layer);
        }
        else
            sem_def_list(node->first_child->sibling, in_structure, wrapped_layer);
        return field;
    }
    return NULL;
}

Field_List *sem_def(AST_Node *node, int in_structure, int wrapped_layer){
    assert(node);
    Type *type = sem_specifier(node->first_child, wrapped_layer, in_structure);
    return sem_dec_list(node->first_child->sibling, type, in_structure, wrapped_layer);
}

Field_List *sem_dec_list(AST_Node *node, Type *type, int in_structure, int wrapped_layer){
    assert(node && node->first_child);
    Field_List *field = sem_dec(node->first_child, type, in_structure, wrapped_layer);
    if (field && node->first_child->sibling){
        if (in_structure)
            field->next = sem_dec_list(node->first_child->sibling->sibling, type, in_structure, wrapped_layer);
        else
            sem_dec_list(node->first_child->sibling->sibling, type, in_structure, wrapped_layer);
    }
    return field;
}

Field_List *sem_dec(AST_Node *node, Type *type, int in_structure, int wrapped_layer){
    assert(node && node->first_child);
    if (node->first_child->sibling){
        if (in_structure){
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "initialization of fields in a structure.\n");
            add_error_list(15, info, node->first_child->sibling->row_index);
            return NULL;
        }
        Field_List *var_dec_field = sem_var_dec(node->first_child, type, in_structure, wrapped_layer);
        if (var_dec_field){
            if (check_equal_type(var_dec_field->type, sem_exp(node->first_child->sibling->sibling), 0)){
                return var_dec_field;
            }
            else{
                char info[MAX_ERROR_INFO_LEN];
                sprintf(info, "Type mismatched for assignment.\n");
                add_error_list(5, info, node->first_child->sibling->row_index);
                return NULL;
            }
        }
        return NULL;
    }
    else{
        Field_List *var_dec_field = sem_var_dec(node->first_child, type, in_structure, wrapped_layer);
        return var_dec_field;
    }
}

Type *sem_exp(AST_Node *node){
    assert(node && node->first_child);
    if (strcmp(node->first_child->name, "Exp") == 0){
        if (strcmp(node->first_child->sibling->name, "ASSIGNOP") == 0){
            Type *type1 = sem_exp(node->first_child);
            Type *type2 = sem_exp(node->first_child->sibling->sibling);
            if (!(type1 && type2))
                return NULL;
            if (check_equal_type(type1, type2, 0)){
                //check left-value
                AST_Node *child_node = node->first_child;
                if ((strcmp(child_node->first_child->name, "ID") == 0 && !child_node->first_child->sibling) 
                    || (strcmp(child_node->first_child->name, "Exp") == 0 && strcmp(child_node->first_child->sibling->name, "LB") == 0) 
                    || (strcmp(child_node->first_child->name, "Exp") == 0 && strcmp(child_node->first_child->sibling->name, "DOT") == 0)){
                    return type1;    
                }
                else{
                    char info[MAX_ERROR_INFO_LEN];
                    sprintf(info, "The left-hand side of an assignment must be a variable.\n");
                    add_error_list(6, info, node->first_child->sibling->row_index);
                    return NULL;
                }
            }
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "Type mismatched for assignment.\n");
            add_error_list(5, info, node->first_child->sibling->row_index);
            return NULL;
        }
        else if (strcmp(node->first_child->sibling->name, "AND") == 0
            || strcmp(node->first_child->sibling->name, "OR") == 0){
            Type *type1 = sem_exp(node->first_child);
            Type *type2 = sem_exp(node->first_child->sibling->sibling);
            if (!(type1 && type2))
                return NULL;
            if (check_equal_type(type1, type2, 0)){
                //only INT can do logical computation
                if (type1->kind == BASIC && type1->u.basic == BASIC_INT){
                    return type1;
                }
                else{
                    char info[MAX_ERROR_INFO_LEN];
                    sprintf(info, "Only INT varaibles can do logical computation.\n");
                    add_error_list(98, info, node->first_child->sibling->row_index);
                    return NULL;
                }
            }
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "Type mismatched for logical operation.\n");
            add_error_list(7, info, node->first_child->sibling->row_index);
            return NULL;
        }
        else if (strcmp(node->first_child->sibling->name, "PLUS") == 0
            || strcmp(node->first_child->sibling->name, "MINUS") == 0
            || strcmp(node->first_child->sibling->name, "STAR") == 0
            || strcmp(node->first_child->sibling->name, "DIV") == 0
            || strcmp(node->first_child->sibling->name, "RELOP") == 0){
            Type *type1 = sem_exp(node->first_child);
            Type *type2 = sem_exp(node->first_child->sibling->sibling);
            if (!(type1 && type2))
                return NULL;
            if (check_equal_type(type1, type2, 0)){
                Type *type = (Type *)malloc(sizeof(Type));
                type->kind = BASIC;
                type->u.basic = BASIC_INT;
                return type;
            }
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "Type mismatched for arithmatic operation.\n");
            add_error_list(7, info, node->first_child->sibling->row_index);
            return NULL;
        }
        else if (strcmp(node->first_child->sibling->name, "LB") == 0 && strcmp(node->first_child->sibling->sibling->name, "Exp") == 0){
            Type *index_type = sem_exp(node->first_child->sibling->sibling);
            if (!index_type)
                return NULL;
            if (index_type->kind == BASIC && index_type->u.basic == BASIC_INT){
                Type *type = sem_exp(node->first_child);
                if (!type)
                    return NULL;
                if (type->kind == ARRAY){
                    return type->u.array.elem;
                }
                else{
                    char info[MAX_ERROR_INFO_LEN];
                    sprintf(info, "The variable is not an array.\n");
                    add_error_list(10, info, node->first_child->row_index);
                    return NULL; 
                }
            }
            else{
                char info[MAX_ERROR_INFO_LEN];
                sprintf(info, "The size of array can only be an integer.\n");
                add_error_list(12, info, node->first_child->sibling->row_index);
                return NULL;                
            }
        }
        else if (strcmp(node->first_child->sibling->name, "DOT") == 0){
            Type *type = sem_exp(node->first_child);
            if (!type)
                return NULL;
            if (type->kind == STRUCTURE){
                Field_List *cur = type->u.structure.first_field;
                while (cur){
                    if (strcmp(cur->name, node->first_child->sibling->sibling->value) == 0){
                        if (strcmp("b7p6y_i", cur->name) == 0){
                        }
                        return cur->type;
                    }
                    cur = cur->next;
                }
                char info[MAX_ERROR_INFO_LEN];
                sprintf(info, "Non-existent field '%s'.\n", node->first_child->sibling->sibling->value);
                add_error_list(14, info, node->first_child->sibling->sibling->row_index);
                return NULL;    
            }
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "Illegal use of '.'.\n");
            add_error_list(13, info, node->first_child->sibling->row_index);
            return NULL; 
        }
    }
    else if (strcmp(node->first_child->name, "LP") == 0){
        return sem_exp(node->first_child->sibling);
    }
    else if (strcmp(node->first_child->name, "MINUS") == 0){
        return sem_exp(node->first_child->sibling);
    }
    else if (strcmp(node->first_child->name, "NOT") == 0){
        Type *type = sem_exp(node->first_child->sibling);
        if (!type)
            return NULL;
        if (type->kind == BASIC && type->u.basic == BASIC_INT){
            Type *new_type = (Type *)malloc(sizeof(Type));
            new_type->kind = BASIC;
            new_type->u.basic = BASIC_INT;
            return new_type;
        }
        char info[MAX_ERROR_INFO_LEN];
        sprintf(info, "Only INT varaibles can do logical computation.\n");
        add_error_list(98, info, node->first_child->sibling->row_index);
        return NULL;
    }
    else if (strcmp(node->first_child->name, "ID") == 0){
        if (!node->first_child->sibling){
            unsigned hash_index = hash_pjw(node->first_child->value);
            Field_List *field = query_field_hash_table(hash_index, node->first_child, 0);
            if (!field){
                char info[MAX_ERROR_INFO_LEN];
                sprintf(info, "Undefined variable '%s'.\n", node->first_child->value);
                add_error_list(1, info, node->first_child->row_index);
                return NULL;
            }
            return field->type;
        }
        unsigned hash_index = hash_pjw(node->first_child->value);
        Func *func = query_func_hash_table(hash_index);
        Field_List *var = query_field_hash_table(hash_index, NULL, 0);
        if (!func || func->defined == 0){
            if (var){
                char info[MAX_ERROR_INFO_LEN];
                sprintf(info, "'%s' is not a function.\n", node->first_child->value);
                add_error_list(11, info, node->first_child->sibling->row_index);
                return NULL;
            }
            else{
                char info[MAX_ERROR_INFO_LEN];
                sprintf(info, "Undefined function '%s'\n", node->first_child->value);
                add_error_list(2, info, node->first_child->sibling->row_index);
                return NULL;
            }
        }
        if (strcmp(node->first_child->sibling->sibling->name, "Args") == 0){
            Type *true_params_type = sem_args(node->first_child->sibling->sibling);
            if (!true_params_type)
                return NULL;
            if (check_equal_params(func->first_param, true_params_type)){
                return func->return_type;
            }
            return NULL;
        }
        else{
            assert(strcmp(node->first_child->sibling->sibling->name, "RP") == 0);
            if (func->param_size == 0){
                return func->return_type;
            }
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "Function arguments are not applicable.\n");
            add_error_list(9, info, node->first_child->row_index);
            return NULL;
        }
    }
    else if (strcmp(node->first_child->name, "INT") == 0){
        Type *type = (Type *)malloc(sizeof(Type));
        type->kind = BASIC;
        type->u.basic = BASIC_INT;
        return type;
    }
    else if (strcmp(node->first_child->name, "FLOAT") == 0){
        Type *type = (Type *)malloc(sizeof(Type));
        type->kind = BASIC;
        type->u.basic = BASIC_FLOAT;
        return type;
    }
    return NULL;
}

Type *sem_args(AST_Node *node){
    Type *type = sem_exp(node->first_child);
    if (!type)
        return NULL;
    type->line_num = node->first_child->row_index;
    if (node->first_child->sibling){
        type->next_actual_param = sem_args(node->first_child->sibling->sibling);
    }
    return type;
}

Field_List *query_field_hash_table(unsigned hash_index, AST_Node *node, int look_for_structure){
    Field_List *field_now = var_hash[hash_index];
    while(field_now != NULL){
        if (field_now->is_structure == look_for_structure)
            return field_now;
        field_now = field_now->next;
    }
    return NULL;
}

Field_List *insert_field_hash_table(unsigned hash_index, Type *type, AST_Node *node, int wrapped_layer, int is_structure){
    Field_List *new_field;
    if (var_hash[hash_index] != NULL && var_hash[hash_index]->wrapped_layer >= wrapped_layer){
        char info[MAX_ERROR_INFO_LEN];
        if (is_structure){
            sprintf(info, "redefinition of '%s'.\n", node->value);
            add_error_list(16, info, node->row_index);
        }
        else{
            sprintf(info, "redefinition of '%s'.\n", node->value);
            add_error_list(3, info, node->row_index);
        }
        return NULL;
    }
    else{
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
}

Func *insert_func_hash_table(unsigned hash_index, Type *return_type, Func *func){
    Func *func_head = func_hash[hash_index];
    if (func_head){
        if (func_head->defined){
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "Redefined function '%s'.\n", func->name);
            add_error_list(4, info, func->line_num);
            return NULL;
        }
        else{
            if (!check_equal_type(return_type, func_head->return_type, 0) || !check_twofunc_equal_params(func->first_param, func_head->first_param)){
                char info[MAX_ERROR_INFO_LEN];
                sprintf(info, "Inconsistent declaration of function '%s'.\n", func->name);
                add_error_list(19, info, func->line_num);
                return NULL;
            }
            func_hash[hash_index]->defined = 1;
            return func_hash[hash_index];
        }
    }
    func->return_type = return_type;
    func->defined = 1;
    func_hash[hash_index] = func;
    return func;
}

Func *query_func_hash_table(unsigned hash_index){
    return func_hash[hash_index];
}

Func *insert_func_dec_hash_table(unsigned hash_index, Type *return_type, Func *func){
    Func *func_head = func_hash[hash_index];
    if (func_head){
        if (!check_equal_type(return_type, func_head->return_type, 0) || !check_twofunc_equal_params(func->first_param, func_head->first_param)){
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "Inconsistent declaration of function '%s'.\n", func->name);
            add_error_list(19, info, func->line_num);
            return NULL;
        }
        return func_hash[hash_index];
    }
    func->defined = 0;
    func->return_type = return_type;
    func_hash[hash_index] = func;
    return func;
}

int check_equal_type(Type *type1, Type *type2, int in_structure){
    if (!type1 || !type2)
        return 1; // if many errors happen in an expression, only one error will be reported.
    if (type1->kind == BASIC && type2->kind == BASIC){
        if (type1->u.basic == BASIC_INT && type2->u.basic == BASIC_INT){
            return 1;
        }
        if (type1->u.basic == BASIC_FLOAT && type2->u.basic == BASIC_FLOAT){
            return 1;
        }
        return 0;
    }
    if (type1->kind == STRUCTURE && type2->kind == STRUCTURE){
        return check_struct_equal_type(type1, type2);
    }
    if (type1->kind == ARRAY && type2->kind == ARRAY){
        int dim1 = 0, dim2 = 0;
        Type *type11 = type1->u.array.elem;
        Type *type22 = type2->u.array.elem;
        while(type11->kind == ARRAY){
            type11 = type11->u.array.elem;
            dim1 ++;
        }
        while(type22->kind == ARRAY){
            type22 = type22->u.array.elem;
            dim2 ++;
        }
        if (in_structure && dim1 != dim2)
            return 0;
        assert(type11->kind != ARRAY && type22->kind != ARRAY);
        return check_equal_type(type11, type22, in_structure);
    }
    return 0;
}

int check_struct_equal_type(Type *type1, Type *type2){
    Type *type_list1 = type1->u.structure.first_flat;
    Type *type_list2 = type2->u.structure.first_flat;
    
    Type *cur1 = type_list1;
    Type *cur2 = type_list2;

    while (cur1 && cur2){
        assert(cur1->kind != STRUCTURE && cur2->kind != STRUCTURE);
        if (!check_equal_type(cur1, cur2, 1))
            return 0;
        cur1 = cur1->next_flat;
        cur2 = cur2->next_flat;
    }
    if (cur1 || cur2){
        return 0;
    }
    return 1;
}

int check_duplicate_field(Type *structure_type){
    assert(structure_type && structure_type->kind == STRUCTURE);
    Field_List *cur = structure_type->u.structure.first_field;
    char field_name[MAX_FIELD_NUM][MAX_NAME_LEN];
    int field_num = 0;
    while(cur){
        int i;
        for (i = 0; i < field_num; i ++){
            if (strcmp(field_name[i], cur->name) == 0){
                char info[MAX_ERROR_INFO_LEN];
                sprintf(info, "The field name in the structure is repeatedly defined.\n");
                add_error_list(15, info, cur->line_num);
                return 0;
            }
        }
        strcpy(field_name[field_num++], cur->name);
        cur = cur->next;
    }
    return 1;
}

int check_equal_params(Field_List *field_list, Type *type){
    Type *true_cur = type;
    Type *true_first = type;
    Field_List *formal_cur = field_list;
    while (true_cur && formal_cur){
        if (!check_equal_type(true_cur, formal_cur->type, 0)){
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "Function arguments are not applicable.\n");
            add_error_list(9, info, true_cur->line_num);
            return 0;
        }
        formal_cur = formal_cur->next;
        true_cur = true_cur->next_actual_param;
    }
    if (true_cur || formal_cur){
        char info[MAX_ERROR_INFO_LEN];
        sprintf(info, "Function arguments are not applicable.\n");
        add_error_list(9, info, true_first->line_num);
        return 0;
    }
    return 1;
}

int check_twofunc_equal_params(Field_List *field1, Field_List *field2){
    Field_List *cur1 = field1;
    Field_List *cur2 = field2;
    while (cur1 && cur2){
        if (!check_equal_type(cur1->type, cur2->type, 0)){
            return 0;
        }
        cur1 = cur1->next;
        cur2 = cur2->next;
    }
    if (cur1 || cur2)
        return 0;
    return 1;
}

void check_undec_func(){
    int i;
    for (i = 0; i < MAX_HASH_TABLE_LEN; i ++){
        if (func_hash[i] && func_hash[i]->defined == 0){
            char info[MAX_ERROR_INFO_LEN];
            sprintf(info, "Undefined function '%s'.\n", func_hash[i]->name);
            add_error_list(18, info, func_hash[i]->line_num);
        }
    }
}

void pop_local_var(int wrapped_layer){
    int i;
    Field_List *new_head;
    for (i = 0; i < MAX_HASH_TABLE_LEN; i ++){
        if (var_hash[i] != NULL && var_hash[i]->wrapped_layer == wrapped_layer){
            new_head = var_hash[i]->next;
            if (new_head != NULL){
                var_hash[i] = new_head;
            }
            else
                var_hash[i] = NULL;
        }
    }
}

Type *struct_type_to_list(Field_List *cur_field){
    if (!cur_field){
        return NULL;
    }
    Type *flat_type_list = NULL;
    if (cur_field->type->kind != STRUCTURE){
        flat_type_list = cur_field->type;
    }
    else{
        if (cur_field->type->u.structure.first_flat)
            flat_type_list = cur_field->type->u.structure.first_flat;
        else
            flat_type_list = struct_type_to_list(cur_field->type->u.structure.first_field);
    }
    if (flat_type_list){
        Type *tail = flat_type_list;
        while (tail && tail->next_flat)
            tail = tail->next_flat;
        tail->next_flat = struct_type_to_list(cur_field->next);
    }

    return flat_type_list;
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
        error->next = error_head;
        error_head = error;
    }
}

void print_error_list(){
    Error_List *cur = error_head;
    while (cur){
        printf("Error type %d at Line %d: %s", cur->type, cur->line_num, cur->info);
        cur = cur->next;
    }
}

void print_field_list(int hash_index){
    TestLog("Begin printing var_hash[%d]", hash_index);
    Field_List *field = var_hash[hash_index];
    while (field) {
        TestLog("%s %d", field->name, field->wrapped_layer);
        field = field->next;
    }
    TestLog("End printing var_hash[%d]", hash_index);    
}