#include "semantics.h"
#include "utils.h"
#include "AST.h"
#include "debug.h"
#include "ir.h"
#include "math.h"

static InterCode *ir_head = NULL;
static Operand *op_head = NULL;
static Operand *arg_list_head = NULL;
static Operand *fall_label = NULL;
static Field_List *var_hash[MAX_HASH_TABLE_LEN];
static Func *func_hash[MAX_HASH_TABLE_LEN];

static int label_no;
static int temp_no;
static int var_no;

void ir_init_hash_table() {
    int i;
    label_no = 1;
    temp_no = 1;
    var_no = 1;
    for (i = 0; i < MAX_HASH_TABLE_LEN; i ++){
        var_hash[i] = NULL;
        func_hash[i] = NULL;
    }
    ir_insert_read_write_func("read");
    ir_insert_read_write_func("write");
    fall_label = make_fall_label();
}

void ir_insert_read_write_func(char *name) {
    Func *func = malloc(sizeof(Func));
    func->line_num = 0;
    Type *type = malloc(sizeof(type));
    type->kind = BASIC;
    type->u.basic = BASIC_INT;
    if (strcmp(name, "write") == 0){
        func->param_size = 1;
        Field_List *field = malloc(sizeof(Field_List));
        field->is_structure = 0;
        field->wrapped_layer = 1;
        field->type = type;
        field->line_num = 0;
        func->first_param = field;
    }
    else {
        assert(strcmp(name, "read") == 0);
        func->param_size = 0;
    }
    strcpy(func->name, name);
    ir_insert_func_hash_table(hash_pjw(func->name), func->name, type, func);
}

void ir_generate(AST_Node *root) {
    ir_init_hash_table();
    ir_program(root);
    post_optimize();
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
    return NULL;
}

InterCode *ir_ext_def(AST_Node *node) {
    /* not consider global variables */
    assert(node && node->first_child && node->first_child->sibling);
    if (strcmp(node->first_child->sibling->name, "SEMI") == 0) {
        ir_specifier(node->first_child, 0, 0);
        return NULL;
    }
    if (strcmp(node->first_child->sibling->name, "FunDec") == 0) {
        Type *ret_type = ir_specifier(node->first_child, 0, 0);
        InterCode *fun_code = ir_fun_dec(node->first_child->sibling, ret_type);
        InterCode *comp_code = ir_comp_st(node->first_child->sibling->sibling, 1);
        return bind(fun_code, comp_code);
    }
    return NULL;
}

Type* ir_specifier(AST_Node *node, int wrapped_layer, int in_structure){
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
    return ir_struct_specifier(node->first_child, wrapped_layer, in_structure);
}

InterCode *ir_fun_dec(AST_Node *node, Type *ret_type) {
    assert(node && node->first_child && node->first_child->sibling && node->first_child->sibling->sibling);
    Func *func = malloc(sizeof(Func));
    strcpy(func->name, node->first_child->value);
    func->line_num = node->first_child->row_index;
    if (strcmp(node->first_child->sibling->sibling->name, "VarList") == 0){
        Field_List *params = ir_var_list(node->first_child->sibling->sibling);
        if (!params)
            return NULL;
        func->first_param = params;
        Field_List *cur = func->first_param;
        func->param_size = 0;
        while (cur){
            func->param_size ++;
            cur = cur->next;
        }
        ir_insert_func_hash_table(hash_pjw(func->name), func->name, ret_type, func);
        InterCode *code1 = make_ir(IR_FUNCTION, func->op, NULL, NULL, NULL);
        cur = func->first_param;
        while (cur) {
            code1 = bind(code1, make_ir(IR_PARAM, cur->op, NULL, NULL, NULL));
            cur = cur->next;
        }
        return code1;
    }
    else{
        func->param_size = 0;
        func->first_param = NULL;
        ir_insert_func_hash_table(hash_pjw(func->name), func->name, ret_type, func);
        return make_ir(IR_FUNCTION, func->op, NULL, NULL, NULL);
    }
}

Field_List *ir_var_list(AST_Node *node) {
    assert(node && node->first_child);
    Field_List *field = ir_param_dec(node->first_child);
    if (!field)
        return NULL;
    if (node->first_child->sibling){
        field->next = ir_var_list(node->first_child->sibling->sibling);
    }
    return field;
}

Field_List *ir_param_dec(AST_Node *node) {
    assert(node && node->first_child && node->first_child->sibling);
    Type *type = ir_specifier(node->first_child, 1, 0);
    return ir_var_dec(node->first_child->sibling, type, 0, 1);
}

InterCode *ir_comp_st(AST_Node *node, int wrapped_layer) {
    assert(node && node->first_child && node->first_child->sibling && node->first_child->sibling->sibling);
    InterCode *code1 = ir_def_list(node->first_child->sibling, wrapped_layer);
    InterCode *code2 = ir_stmt_list(node->first_child->sibling->sibling, wrapped_layer);
    return bind(code1, code2);
    // pop_local_var(wrapped_layer);
}

InterCode *ir_def_list(AST_Node *node, int wrapped_layer) {
    assert(node);
    if (node->first_child){
        InterCode *code1 = ir_def(node->first_child, wrapped_layer);
        InterCode *code2 = ir_def_list(node->first_child->sibling, wrapped_layer);
        return bind(code1, code2);
    }
    return NULL;
}

InterCode *ir_def(AST_Node *node, int wrapped_layer) {
    assert(node);
    Type *type = ir_specifier(node->first_child, wrapped_layer, 0);
    return ir_dec_list(node->first_child->sibling, type, wrapped_layer);
}

InterCode *ir_dec_list(AST_Node *node, Type *type, int wrapped_layer) {
    assert(node && node->first_child);
    InterCode *code1 = ir_dec(node->first_child, type, wrapped_layer);
    if (node->first_child->sibling){
        InterCode *code2 = ir_dec_list(node->first_child->sibling->sibling, type, wrapped_layer);
        return bind(code1, code2);
    }
    return code1;
}

InterCode *ir_dec(AST_Node *node, Type *type, int wrapped_layer) {
    assert(node && node->first_child);
    assert(type);
    if (node->first_child->sibling){
        Field_List *variable = ir_var_dec(node->first_child, type, 0, wrapped_layer);
        if (all_constant(node->first_child->sibling->sibling)) {
            return make_ir(IR_ASSIGN, variable->op, make_constant(get_constant(node->first_child->sibling->sibling)), NULL, NULL);
        }
        if (is_id(node->first_child->sibling->sibling)) {
            return make_ir(IR_ASSIGN, variable->op, get_id(node->first_child->sibling->sibling), NULL, NULL);
        }
        Operand *t1 = make_temp();
        InterCode *code1 = ir_exp(node->first_child->sibling->sibling, t1);
        InterCode *code2 = make_ir(IR_ASSIGN, variable->op, t1, NULL, NULL);
        return bind(code1, code2);
    }
    else{
        Field_List *variable = ir_var_dec(node->first_child, type, 0, wrapped_layer);
        if (variable->type->kind == STRUCTURE) {
            variable->op = make_addr(variable->op, 0);
            int size = variable->type->u.structure.size;
            Operand *op_size = make_constant(size);
            InterCode *code = make_ir(IR_DEC, variable->op, op_size, NULL, NULL);
            return code;
        }
        else if (variable->type->kind == ARRAY) {
            int size = 1;
            variable->op = make_addr(variable->op, 0);
            Type *cur_type = variable->type;
            while (cur_type && cur_type->kind == ARRAY) {
                size *= cur_type->u.array.size;
                cur_type = cur_type->u.array.elem;
            }
            assert(cur_type);
            if (cur_type->kind == BASIC) {
                size *= 4;
            }
            else {
                assert(cur_type->kind == STRUCTURE);
                size *= cur_type->u.structure.size;
            }
            Operand *op_size = make_constant(size);
            InterCode *code = make_ir(IR_DEC, variable->op, op_size, NULL, NULL);
            return code;
        }
        return NULL;
    }
}

InterCode *ir_stmt_list(AST_Node *node, int wrapped_layer){
    assert(node);
    if (node->first_child){
        InterCode *code1 = ir_stmt(node->first_child, wrapped_layer);
        InterCode *code2 = ir_stmt_list(node->first_child->sibling, wrapped_layer);
        return bind(code1, code2);
    }
    return NULL;
}

InterCode *ir_stmt(AST_Node *node, int wrapped_layer) {
    assert(node && node->first_child);
    if (strcmp(node->first_child->name, "CompSt") == 0) {
        return ir_comp_st(node->first_child, wrapped_layer + 1);
    }
    if (strcmp(node->first_child->name, "Exp") == 0) {
        return ir_exp(node->first_child, NULL);
    }
    if (strcmp(node->first_child->name, "RETURN") == 0) {
        if (all_constant(node->first_child->sibling)) {
            return make_ir(IR_RETURN, make_constant(get_constant(node->first_child->sibling)), NULL, NULL, NULL);
        }
        if (is_id(node->first_child->sibling)) {
            return make_ir(IR_RETURN, get_id(node->first_child->sibling), NULL, NULL, NULL);
        }
        Operand *t1 = make_temp();
        InterCode *code1 = ir_exp(node->first_child->sibling, t1);
        InterCode *code2 = make_ir(IR_RETURN, t1, NULL, NULL, NULL);
        return bind(code1, code2);
    }
    if (strcmp(node->first_child->name, "IF") == 0) {
        if (node->first_child->sibling->sibling->sibling->sibling->sibling) {
            Operand *label1 = make_label();
            Operand *label2 = make_label();
            InterCode *code1 = ir_cond(node->first_child->sibling->sibling, fall_label, label1);
            InterCode *code2 = ir_stmt(node->first_child->sibling->sibling->sibling->sibling, wrapped_layer);
            InterCode *code3 = ir_stmt(node->first_child->sibling->sibling->sibling->sibling->sibling->sibling, wrapped_layer);
            return
                bind(
                    bind(
                        bind(
                            bind(
                                bind(
                                    code1,
                                    code2
                                ),
                                make_ir(IR_GOTO, label2, NULL, NULL, NULL)
                            ),
                            make_ir(IR_LABEL, label1, NULL, NULL, NULL)
                        ),
                        code3
                    ),
                    make_ir(IR_LABEL, label2, NULL, NULL, NULL)
                );
        }
        else {
            Operand *label1 = make_label();
            InterCode *code1 = ir_cond(node->first_child->sibling->sibling, fall_label, label1);
            InterCode *code2 = ir_stmt(node->first_child->sibling->sibling->sibling->sibling, wrapped_layer);
            return 
                bind(
                    bind(
                        code1,
                        code2
                    ), 
                    make_ir(IR_LABEL, label1, NULL, NULL, NULL)
                );
        }
    }
    if (strcmp(node->first_child->name, "WHILE") == 0) {
        Operand *label1 = make_label();
        Operand *label3 = make_label();
        InterCode *code1 = ir_cond(node->first_child->sibling->sibling, fall_label, label3);
        InterCode *code2 = ir_stmt(node->first_child->sibling->sibling->sibling->sibling, wrapped_layer);
        return
            bind(
                bind(
                    bind(
                        bind(
                            make_ir(IR_LABEL, label1, NULL, NULL, NULL),
                            code1
                        ),
                        code2
                    ),
                    make_ir(IR_GOTO, label1, NULL, NULL, NULL)
                ),
                make_ir(IR_LABEL, label3, NULL, NULL, NULL)
            );
    }
    assert(0);
    return NULL;
}

InterCode *ir_exp(AST_Node *node, Operand *place) {
    assert(node && node->first_child);
    if (all_constant(node)) {
        if (place == NULL)
            return NULL;
        Operand *constant = make_constant(get_constant(node));
        return make_ir(IR_ASSIGN, place, constant, NULL, NULL);
    }
    if (strcmp(node->first_child->name, "Exp") == 0 || strcmp(node->first_child->name, "NOT") == 0) {
        if (strcmp(node->first_child->name, "Exp") == 0) {
            if (strcmp(node->first_child->sibling->name, "ASSIGNOP") == 0){
                Field_List *variable = NULL;
                if (strcmp(node->first_child->first_child->name, "ID") == 0 && node->first_child->first_child->sibling == NULL) {
                    // ID
                    variable = ir_query_field_hash_table(hash_pjw(node->first_child->first_child->value), 
                        node->first_child->first_child->value, 
                        node->first_child->first_child,
                        0);
                    assert(variable);
                    if (all_constant(node->first_child->sibling->sibling)) {
                        if (place == NULL) {
                            return make_ir(IR_ASSIGN, variable->op, make_constant(get_constant(node->first_child->sibling->sibling)), NULL, NULL);
                        }
                        else {
                            return bind(make_ir(IR_ASSIGN, variable->op, make_constant(get_constant(node->first_child->sibling->sibling)), NULL, NULL), make_ir(IR_ASSIGN, place, variable->op, NULL, NULL));
                        }
                    }
                    if (is_id(node->first_child->sibling->sibling)) {
                        if (place == NULL) {
                            return make_ir(IR_ASSIGN, variable->op, get_id(node->first_child->sibling->sibling), NULL, NULL);
                        }
                        else {
                            return bind(make_ir(IR_ASSIGN, variable->op, get_id(node->first_child->sibling->sibling), NULL, NULL), make_ir(IR_ASSIGN, place, variable->op, NULL, NULL));
                        }
                    }
                    Operand *t1 = make_temp();
                    InterCode *code1 = ir_exp(node->first_child->sibling->sibling, t1);
                    InterCode *code2 = NULL;
                    if (place == NULL) {
                        code2 = make_ir(IR_ASSIGN, variable->op, t1, NULL, NULL);
                    }
                    else {
                        code2 = bind(make_ir(IR_ASSIGN, variable->op, t1, NULL, NULL), make_ir(IR_ASSIGN, place, variable->op, NULL, NULL));
                    }
                    return bind(code1, code2);
                }
                else {
                    // Arrays and Structures
                    if (place != NULL) {
                        Operand *t1 = make_temp();
                        InterCode *code1 = ir_exp(node->first_child, t1);
                        InterCode *code2 = make_ir(IR_ASSIGN, place, t1, NULL, NULL);
                        return bind(code1, code2);
                    }
                    else {
                        InterCode *code1 = ir_exp(node->first_child, NULL);
                        return code1;
                    }
                }
            }
            else if (strcmp(node->first_child->sibling->name, "PLUS") == 0
                || strcmp(node->first_child->sibling->name, "MINUS") == 0
                || strcmp(node->first_child->sibling->name, "STAR") == 0
                || strcmp(node->first_child->sibling->name, "DIV") == 0){
                int kind = 0;
                if (strcmp(node->first_child->sibling->name, "PLUS") == 0) {
                    kind = IR_ADD;
                }
                else if (strcmp(node->first_child->sibling->name, "MINUS") == 0) {
                    kind = IR_SUB;
                }
                else if (strcmp(node->first_child->sibling->name, "STAR") == 0) {
                    kind = IR_MUL;
                }
                else {
                    kind = IR_DIV;
                }
                if (all_constant(node->first_child) && all_constant(node->first_child->sibling->sibling)) {
                    if (place == NULL)
                        return NULL;
                    int result = get_constant(node->first_child);
                    switch (kind) {
                        case IR_ADD: result += get_constant(node->first_child->sibling->sibling); break;
                        case IR_SUB: result -= get_constant(node->first_child->sibling->sibling); break;
                        case IR_MUL: result *= get_constant(node->first_child->sibling->sibling); break;
                        case IR_DIV: result /= get_constant(node->first_child->sibling->sibling);  break;
                        default: assert(0);
                    }
                    // place = make_constant(result);
                    return make_ir(IR_ASSIGN, place, make_constant(result), NULL, NULL);
                }
                Operand *t1 = NULL;
                Operand *t2 = NULL;
                InterCode *code1 = NULL;
                InterCode *code2 = NULL;
                if (all_constant(node->first_child)) {
                    Operand *constant = make_constant(get_constant(node->first_child));
                    if (is_id(node->first_child->sibling->sibling))
                        t1 = get_id(node->first_child->sibling->sibling);
                    else {
                        t1 = make_temp();
                        code1 = ir_exp(node->first_child->sibling->sibling, t1);
                    }
                    if (place != NULL)
                        code2 = make_ir(kind, place, constant, t1, NULL);
                    return bind(code1, code2);
                }
                else if (all_constant(node->first_child->sibling->sibling)){
                    if (is_id(node->first_child)) {
                        t1 = get_id(node->first_child);
                    }
                    else {
                        t1 = make_temp();
                        code1 = ir_exp(node->first_child, t1);
                    }
                    Operand *constant = make_constant(get_constant(node->first_child->sibling->sibling));
                    if (place != NULL)
                        code2 = make_ir(kind, place, t1, constant, NULL);
                    return bind(code1, code2);
                }
                if (is_id(node->first_child)) {
                    t1 = get_id(node->first_child);
                }
                else {
                    t1 = make_temp();
                    code1 = ir_exp(node->first_child, t1);
                }
                if (is_id(node->first_child->sibling->sibling)) {
                    t2 = get_id(node->first_child->sibling->sibling);
                }
                else {
                    t2 = make_temp();
                    code2 = ir_exp(node->first_child->sibling->sibling, t2);
                }
                InterCode *code3 = NULL;
                if (place != NULL)
                    code3 = make_ir(kind, place, t1, t2, NULL);
                return bind(bind(code1, code2), code3);
            }
            else if (strcmp(node->first_child->sibling->name, "LB") == 0 && strcmp(node->first_child->sibling->sibling->name, "Exp") == 0){
                Operand *t1 = make_temp();
                InterCode *code1 = ir_exp(node->first_child, t1);
                if (all_constant(node->first_child->sibling->sibling)) {
                    Operand *array_offset = make_constant(get_constant(node->first_child->sibling->sibling) * size_of_array(node));
                    Type *type = ir_exp_type(node);
                    InterCode *code2;
                    assert(type);
                    if (type->kind != BASIC && place != NULL){
                        code2 = make_ir(IR_ADD, place, t1, array_offset, NULL);
                    }
                    else if (node->sibling && strcmp(node->sibling->name, "ASSIGNOP") == 0){
                        Operand *t4 = make_temp();
                        Operand *t5 = make_temp();
                        if (place == NULL)
                            code2 = bind(
                                        bind(
                                            make_ir(IR_ADD, t4, t1, array_offset, NULL), 
                                            ir_exp(node->sibling->sibling, t5)
                                        ), 
                                        make_ir(IR_ASSIGN_TO_DEREF, t4, t5, NULL, NULL)
                                    );
                        else
                            code2 = bind(
                                        bind(
                                            bind(
                                                make_ir(IR_ADD, t4, t1, array_offset, NULL), 
                                                ir_exp(node->sibling->sibling, t5)
                                            ), 
                                            make_ir(IR_ASSIGN_TO_DEREF, t4, t5, NULL, NULL)
                                        ), 
                                        make_ir(IR_DEREF_ASSIGN, place, t4, NULL, NULL)
                                    );
                    }
                    else if (place != NULL){
                        Operand *t4 = make_temp();
                        code2 = bind(make_ir(IR_ADD, t4, t1, array_offset, NULL), make_ir(IR_DEREF_ASSIGN, place, t4, NULL, NULL));
                    }
                    return bind(code1, code2);
                }
                Operand *t2 = NULL;
                InterCode *code2 = NULL;
                if (is_id(node->first_child->sibling->sibling)) {
                    t2 = get_id(node->first_child->sibling->sibling);
                }
                else {
                    t2 = make_temp();
                    code2 = ir_exp(node->first_child->sibling->sibling, t2);
                }
                Operand *t3 = make_temp();
                InterCode *code3 = make_ir(IR_MUL, t3, t2, make_constant(size_of_array(node)), NULL);
                InterCode *code4 = NULL;
                Type *type = ir_exp_type(node);
                assert(type);
                if (type->kind != BASIC && place != NULL){
                    code4 = make_ir(IR_ADD, place, t1, t3, NULL);
                }
                else if (node->sibling && strcmp(node->sibling->name, "ASSIGNOP") == 0){
                    Operand *t4 = make_temp();
                    Operand *t5 = make_temp();
                    if (place == NULL)
                        code4 = bind(
                                    bind(
                                        make_ir(IR_ADD, t4, t1, t3, NULL), 
                                        ir_exp(node->sibling->sibling, t5)
                                    ), 
                                    make_ir(IR_ASSIGN_TO_DEREF, t4, t5, NULL, NULL)
                                );
                    else
                        code4 = bind(
                                    bind(
                                        bind(
                                            make_ir(IR_ADD, t4, t1, t3, NULL), 
                                            ir_exp(node->sibling->sibling, t5)
                                        ), 
                                        make_ir(IR_ASSIGN_TO_DEREF, t4, t5, NULL, NULL)
                                    ), 
                                    make_ir(IR_DEREF_ASSIGN, place, t4, NULL, NULL)
                                );
                }
                else if (place != NULL){
                    Operand *t4 = make_temp();
                    code4 = bind(make_ir(IR_ADD, t4, t1, t3, NULL), make_ir(IR_DEREF_ASSIGN, place, t4, NULL, NULL));
                }
                return
                    bind(
                        bind(
                            bind(
                                code1,
                                code2
                            ),
                            code3
                        ),
                        code4
                    );
            }
            else if (strcmp(node->first_child->sibling->name, "DOT") == 0){
                Type *structure_type = ir_exp_type(node->first_child);
                Type *type = ir_exp_type(node);
                assert(structure_type && structure_type->kind == STRUCTURE);
                assert(type);
                Operand *t1 = make_temp();
                InterCode *code1 = ir_exp(node->first_child, t1);
                Field_List *cur_field = structure_type->u.structure.first_field;
                while (cur_field != NULL) {
                    if (strcmp(cur_field->name, node->first_child->sibling->sibling->value) == 0) {
                        break;
                    }
                    cur_field = cur_field->next;
                }
                assert(cur_field);
                Operand *offset = make_constant(cur_field->offset);
                InterCode *code2 = NULL;
                if (type->kind != BASIC && place != NULL) {
                    code2 = make_ir(IR_ADD, place, t1, offset, NULL);
                }
                else if (node->sibling && strcmp(node->sibling->name, "ASSIGNOP") == 0) {
                    Operand *t2 = make_temp();
                    Operand *t3 = make_temp();
                    if (place == NULL) {
                        code2 = bind(bind(make_ir(IR_ADD, t2, t1, offset, NULL), ir_exp(node->sibling->sibling, t3)), make_ir(IR_ASSIGN_TO_DEREF, t2, t3, NULL, NULL));
                    }
                    else {
                        code2 = bind(bind(bind(make_ir(IR_ADD, t2, t1, offset, NULL), ir_exp(node->sibling->sibling, t3)), make_ir(IR_ASSIGN_TO_DEREF, t2, t3, NULL, NULL)), make_ir(IR_DEREF_ASSIGN, place, t2, NULL, NULL));
                    }
                }
                else if (place != NULL){
                    Operand *t2 = make_temp();
                    code2 = bind(make_ir(IR_ADD, t2, t1, offset, NULL), make_ir(IR_DEREF_ASSIGN, place, t2, NULL, NULL));
                }
                return bind(code1, code2);
            }
        }
        if ((strcmp(node->first_child->name, "Exp") == 0 && strcmp(node->first_child->sibling->name, "AND") == 0)
            || (strcmp(node->first_child->name, "Exp") == 0 && strcmp(node->first_child->sibling->name, "OR") == 0)
            || (strcmp(node->first_child->name, "Exp") == 0 && strcmp(node->first_child->sibling->name, "RELOP") == 0)
            || (strcmp(node->first_child->name, "NOT") == 0)){
            if (place == NULL)
                return NULL;
            Operand *label1 = make_label();
            Operand *label2 = make_label();
            InterCode *code0 = make_ir(IR_ASSIGN, place, make_constant(0), NULL, NULL);
            InterCode *code1 = ir_cond(node, label1, label2);
            InterCode *code2 = bind(make_ir(IR_LABEL, label1, NULL, NULL, NULL), make_ir(IR_ASSIGN, place, make_constant(1), NULL, NULL));
            return
                bind(
                    bind(
                        bind(
                            code0,
                            code1
                        ),
                        code2
                    ),
                    make_ir(IR_LABEL, label2, NULL, NULL, NULL)
                );
        }
        assert(0);
        return NULL;
    }
    else if (strcmp(node->first_child->name, "LP") == 0){
        return ir_exp(node->first_child->sibling, place);
    }
    else if (strcmp(node->first_child->name, "MINUS") == 0){
        if (all_constant(node->first_child->sibling)) {
            if (place != NULL) {
                return make_ir(IR_ASSIGN, place, make_constant(-get_constant(node->first_child->sibling)), NULL, NULL);
            }
            return NULL;
        }
        Operand *t1 = make_temp();
        InterCode *code1 = ir_exp(node->first_child->sibling, t1);
        InterCode *code2 = NULL;
        if (place != NULL)
            code2 = make_ir(IR_SUB, place, make_constant(0), t1, NULL);
        return bind(code1, code2);
    }
    else if (strcmp(node->first_child->name, "ID") == 0){
        if (node->first_child->sibling == NULL) {
            Field_List *variable = ir_query_field_hash_table(hash_pjw(node->first_child->value), node->first_child->value, node->first_child, 0);
            assert(variable);
            if (place != NULL)
                return make_ir(IR_ASSIGN, place, variable->op, NULL, NULL);
            return NULL;
        }
        if (strcmp(node->first_child->sibling->sibling->name, "Args") == 0){
            Func *function = ir_query_func_hash_table(hash_pjw(node->first_child->value), node->first_child->value);
            arg_list_head = NULL;
            InterCode *code1 = ir_args(node->first_child->sibling->sibling);
            if (strcmp(function->name, "write") == 0) {
                assert(arg_list_head);
                return bind(code1, make_ir(IR_WRITE, arg_list_head, NULL, NULL, NULL));
            }
            Operand *cur = arg_list_head;
            InterCode *code2 = NULL;
            while (cur != NULL) {
                code2 = bind(code2, make_ir(IR_ARG, cur, NULL, NULL, NULL));
                cur = cur->next;
            }
            InterCode *code3 = NULL;
            if (place != NULL) {
                code3 = make_ir(IR_CALL, place, function->op, NULL, NULL);
            }
            else {
                Operand *tmp = make_temp();
                code3 = make_ir(IR_CALL, tmp, function->op, NULL, NULL);
            }
            return bind(bind(code1, code2), code3);
        }
        else {
            Func *function = ir_query_func_hash_table(hash_pjw(node->first_child->value), node->first_child->value);
            if (strcmp(function->name, "read") == 0) {
                if (place != NULL)
                    return make_ir(IR_READ, place, NULL, NULL, NULL);
                else {
                    Operand *tmp = make_temp();
                    return make_ir(IR_READ, tmp, NULL, NULL, NULL);
                }
            }
            if (place != NULL)
                return make_ir(IR_CALL, place, function->op, NULL, NULL);
            else {
                Operand *tmp = make_temp();
                return make_ir(IR_CALL, tmp, function->op, NULL, NULL);
            }
        }
    }
    else if (strcmp(node->first_child->name, "INT") == 0){
        if (place != NULL) {
            Operand *value = make_constant(atoi(node->first_child->value));
            return make_ir(IR_ASSIGN, place, value, NULL, NULL);
        }
        return NULL;
    }
    else if (strcmp(node->first_child->name, "FLOAT") == 0){
        assert(0); // undefined
        return NULL;
    }
    return NULL;
}

Type *ir_exp_type(AST_Node *node){
    assert(node && node->first_child);
    if (strcmp(node->first_child->name, "Exp") == 0){
        if (strcmp(node->first_child->sibling->name, "ASSIGNOP") == 0){
            Type *type1 = ir_exp_type(node->first_child);
            return type1;
        }
        else if (strcmp(node->first_child->sibling->name, "AND") == 0
            || strcmp(node->first_child->sibling->name, "OR") == 0){
            Type *type1 = ir_exp_type(node->first_child);
            return type1;
        }
        else if (strcmp(node->first_child->sibling->name, "PLUS") == 0
            || strcmp(node->first_child->sibling->name, "MINUS") == 0
            || strcmp(node->first_child->sibling->name, "STAR") == 0
            || strcmp(node->first_child->sibling->name, "DIV") == 0
            || strcmp(node->first_child->sibling->name, "RELOP") == 0){
            Type *type1 = ir_exp_type(node->first_child);
            return type1;
        }
        else if (strcmp(node->first_child->sibling->name, "LB") == 0 && strcmp(node->first_child->sibling->sibling->name, "Exp") == 0){
            Type *type = ir_exp_type(node->first_child);
            assert(type);
            return type->u.array.elem;
        }
        else if (strcmp(node->first_child->sibling->name, "DOT") == 0){
            Type *type = ir_exp_type(node->first_child);
            assert(type && type->kind == STRUCTURE);
            Field_List *cur = type->u.structure.first_field;
            while (cur){
                if (strcmp(cur->name, node->first_child->sibling->sibling->value) == 0){
                    return cur->type;
                }
                cur = cur->next;
            }
            assert(0);
            return NULL;
        }
    }
    else if (strcmp(node->first_child->name, "LP") == 0){
        return ir_exp_type(node->first_child->sibling);
    }
    else if (strcmp(node->first_child->name, "MINUS") == 0){
        return ir_exp_type(node->first_child->sibling);
    }
    else if (strcmp(node->first_child->name, "NOT") == 0){
        Type *type = ir_exp_type(node->first_child->sibling);
        assert(type);
        Type *new_type = (Type *)malloc(sizeof(Type));
        new_type->kind = BASIC;
        new_type->u.basic = BASIC_INT;
        return new_type;
    }
    else if (strcmp(node->first_child->name, "ID") == 0){
        if (!node->first_child->sibling){
            unsigned hash_index = hash_pjw(node->first_child->value);
            Field_List *field = ir_query_field_hash_table(hash_index, node->first_child->value, node->first_child, 0);
            assert(field);
            return field->type;
        }
        unsigned hash_index = hash_pjw(node->first_child->value);
        Func *func = ir_query_func_hash_table(hash_index, node->first_child->value);
        return func->return_type;
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

InterCode *ir_cond(AST_Node *node, Operand *label_true, Operand *label_false) {
    assert(node);
    if (strcmp(node->first_child->name, "Exp") == 0) {
        if (strcmp(node->first_child->sibling->name, "RELOP") == 0) {
            Operand *t1 = NULL;
            Operand *t2 = NULL;
            InterCode *code1 = NULL;
            InterCode *code2 = NULL;
            if (all_constant(node->first_child)) {
                t1 = make_constant(get_constant(node->first_child));
            }
            else if (is_id(node->first_child)) {
                t1 = get_id(node->first_child);
            }
            else {
                t1 = make_temp();
                code1 = ir_exp(node->first_child, t1);
            }
            if (all_constant(node->first_child->sibling->sibling)) {
                t2 = make_constant(get_constant(node->first_child->sibling->sibling));
            }
            else if (is_id(node->first_child->sibling->sibling)) {
                t2 = get_id(node->first_child->sibling->sibling);
            }
            else {
                t2 = make_temp();
                code2 = ir_exp(node->first_child->sibling->sibling, t2);
            }
            /* fall optimization */
            InterCode *code3 = NULL;
            int relop_kind = -1;
            if (strcmp(node->first_child->sibling->value, ">") == 0) relop_kind = RELOP_G;
            else if (strcmp(node->first_child->sibling->value, "<") == 0) relop_kind = RELOP_L;
            else if (strcmp(node->first_child->sibling->value, ">=") == 0) relop_kind = RELOP_GE;
            else if (strcmp(node->first_child->sibling->value, "<=") == 0) relop_kind = RELOP_LE;
            else if (strcmp(node->first_child->sibling->value, "==") == 0) relop_kind = RELOP_E;
            else if (strcmp(node->first_child->sibling->value, "!=") == 0) relop_kind = RELOP_NE;
            Operand *relop = make_relop(relop_kind);
            if (label_true->u.label.no != -1 && label_false->u.label.no != -1) {
                code3 = bind(make_ir(IR_IF, label_true, t1, t2, relop), make_ir(IR_GOTO, label_false, NULL, NULL, NULL));
            }
            else if (label_true->u.label.no != -1) {
                code3 = make_ir(IR_IF, label_true, t1, t2, relop);
            }
            else if (label_false->u.label.no != -1) {
                code3 = make_ir(IR_IF, label_false, t1, t2, relop_reverse(relop));
            }
            return bind(bind(code1, code2), code3);
        }
        else if (strcmp(node->first_child->sibling->name, "AND") == 0) {
            InterCode *code1 = NULL;
            Operand *label1 = NULL;
            if (label_false->u.label.no != -1) {
                code1 = ir_cond(node->first_child, fall_label, label_false);
            }
            else {
                label1 = make_label();
                code1 = ir_cond(node->first_child, fall_label, label1);
            }
            InterCode *code2 = ir_cond(node->first_child->sibling->sibling, label_true, label_false);
            if (label_false->u.label.no != -1) {
                return bind(code1, code2);
            }
            else {
                return bind(bind(code1, code2), make_ir(IR_LABEL, label1, NULL, NULL, NULL));
            }
        }
        else if (strcmp(node->first_child->sibling->name, "OR") == 0) {
            InterCode *code1 = NULL;
            Operand *label1 = NULL;
            if (label_true->u.label.no != -1) {
                code1 = ir_cond(node->first_child, label_true, fall_label);
            }
            else {
                label1 = make_label();
                code1 = ir_cond(node->first_child, label1, fall_label);
            }
            InterCode *code2 = ir_cond(node->first_child->sibling->sibling, label_true, label_false);
            if (label_true->u.label.no != -1) {
                return bind(code1, code2);
            }
            else {
                return bind(bind(code1, code2), make_ir(IR_LABEL, label1, NULL, NULL, NULL));
            }
        }
    }
    if (strcmp(node->first_child->name, "NOT") == 0) {
        return ir_cond(node->first_child->sibling, label_false, label_true);
    }
    else {
        Operand *t1 = make_temp();
        InterCode *code1 = ir_exp(node, t1);
        InterCode *code3 = NULL;
            if (label_true->u.label.no != -1 && label_false->u.label.no != -1) {
                code3 = bind(make_ir(IR_IF, label_true, t1, make_constant(0), 
                    make_relop(RELOP_NE)), make_ir(IR_GOTO, label_false, NULL, NULL, NULL));
            }
            else if (label_true->u.label.no != -1) {
                code3 = make_ir(IR_IF, label_true, t1, make_constant(0), make_relop(RELOP_NE));
            }
            else if (label_false->u.label.no != -1) {
                code3 = make_ir(IR_IF, label_false, t1, make_constant(0), make_relop(RELOP_E));
            }
            return bind(code1, code3);
    }
}

InterCode *ir_args(AST_Node *node) {
    assert(node && node->first_child);
    Operand *t1 = NULL;
    InterCode *code1 = NULL;
    if (all_constant(node->first_child)) {
        t1 = make_constant(get_constant(node->first_child));
    }
    else if (is_id(node->first_child)) {
        t1 = get_id(node->first_child);
    }
    else {
        t1 = make_temp();
        code1 = ir_exp(node->first_child, t1);
    }
    t1->next = arg_list_head;
    arg_list_head = t1;
    assert(arg_list_head);    
    if (node->first_child->sibling == NULL) {
        return code1;
    }
    else {
        InterCode *code2 = ir_args(node->first_child->sibling->sibling);
        return bind(code1, code2);
    }
}

Type* ir_struct_specifier(AST_Node *node, int wrapped_layer, int in_structure){
    assert(node && node->first_child && strcmp(node->first_child->name, "STRUCT") == 0);
    if (strcmp(node->first_child->sibling->name, "Tag") == 0){
        AST_Node *tag_node = node->first_child->sibling->first_child;
        unsigned hash_index = hash_pjw(tag_node->value);
        Field_List * field = ir_query_field_hash_table(hash_index, tag_node->value, tag_node, 1);
        if (field)
            return field->type;
        return NULL;
    }
    assert(strcmp(node->first_child->sibling->name, "OptTag") == 0);
    Type *structure_type = (Type *)malloc(sizeof(Type));
    structure_type->kind = STRUCTURE;
    structure_type->u.structure.first_field = ir_def_list_structure(node->first_child->sibling->sibling->sibling, wrapped_layer);
    structure_type->u.structure.size = build_size_offset(structure_type);
    structure_type->line_num = node->first_child->sibling->sibling->sibling->row_index;
    if (node->first_child->sibling->first_child){
        unsigned hash_index = hash_pjw(node->first_child->sibling->first_child->value);
        ir_insert_field_hash_table(hash_index, node->first_child->sibling->first_child->value, structure_type, node->first_child->sibling->first_child, wrapped_layer, 1);
    }
    return structure_type;
}


Field_List *ir_def_list_structure(AST_Node *node, int wrapped_layer) {
    assert(node);
    if (node->first_child){
        Field_List *field = ir_def_structure(node->first_child, wrapped_layer);
        Field_List *cur = field; // insert field_lists of a def after the front def
        if (!cur)
            return NULL;
        while(cur && cur->next){
            cur = cur->next;
        }
        cur->next = ir_def_list_structure(node->first_child->sibling, wrapped_layer);
        return field;
    }
    return NULL;
}

Field_List *ir_def_structure(AST_Node *node, int wrapped_layer) {
    assert(node);
    Type *type = ir_specifier(node->first_child, wrapped_layer, 1);
    return ir_dec_list_structure(node->first_child->sibling, type, wrapped_layer);
}

Field_List *ir_dec_list_structure(AST_Node *node, Type *type, int wrapped_layer) {
    assert(node && node->first_child);
    Field_List *field = ir_dec_structure(node->first_child, type, wrapped_layer);
    if (node->first_child->sibling){
        if (field)
            field->next = ir_dec_list_structure(node->first_child->sibling->sibling, type, wrapped_layer);
    }
    return field;
}

Field_List *ir_dec_structure(AST_Node *node, Type *type, int wrapped_layer) {
    assert(node && node->first_child);
    if (node->first_child->sibling){
        Field_List *var_dec_field = ir_var_dec(node->first_child, type, 1, wrapped_layer);
        return var_dec_field;
    }
    else{
        Field_List *var_dec_field = ir_var_dec(node->first_child, type, 1, wrapped_layer);
        return var_dec_field;
    }
}

Field_List *ir_var_dec(AST_Node *node, Type *type, int in_structure, int wrapped_layer) {
    assert(node && node->first_child);
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
            return ir_insert_field_hash_table(hash_index, node->first_child->value, type, node->first_child, wrapped_layer, 0);
        }
    }
    else{
        Type *array_type = (Type *)malloc(sizeof(Type));
        array_type->kind = ARRAY;
        array_type->u.array.size = str_to_int(node->first_child->sibling->sibling->value, node->first_child->sibling->sibling->int_type);
        array_type->u.array.elem = type;
        return ir_var_dec(node->first_child, array_type, in_structure, wrapped_layer);
    }
}

int build_size_offset(Type *structure_type) {
    assert(structure_type && structure_type->kind == STRUCTURE);
    Field_List *field = structure_type->u.structure.first_field;
    int offset = 0;
    while (field != NULL) {
        field->offset = offset;
        if (field->type->kind == BASIC) {
            offset += 4;
        }
        else if (field->type->kind == ARRAY) {
            offset += size_of_array_type(field->type);
        }
        else {
            assert(field->type->kind == STRUCTURE);
            field->type->u.structure.size = build_size_offset(field->type);
            offset += field->type->u.structure.size;
        }
        field = field->next;
    }
    return offset;
}

int size_of_array_type(Type *type) {
    int size = 1;
    Type *cur_type = type;
    while (cur_type && cur_type->kind == ARRAY) {
        size *= cur_type->u.array.size;
        cur_type = cur_type->u.array.elem;
    }
    assert(cur_type);
    if (cur_type->kind == BASIC) {
        size *= 4;
    }
    else {
        assert(cur_type->kind == STRUCTURE);
        size *= cur_type->u.structure.size;
    }
    return size;
}

InterCode *bind(InterCode *code1, InterCode *code2) {
    InterCode *ret_code = code1;
    InterCode *code1_tail = code1;
    if (code1 == NULL && code2 == NULL) {
        return NULL;
    }
    if (code1 == NULL && code2 != NULL) {
        if (ir_head == NULL)
            ir_head = code2;
        return code2;
    }
    if (code1 != NULL && code2 == NULL) {
        if (ir_head == NULL)
            ir_head = code1;
        return code1;
    }
    if (ir_head == NULL) {
        ir_head = code1;
    }
    while (code1_tail != NULL && code1_tail->next != NULL) {
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
    Operand *op = make_var(new_field);
    if (type->kind != BASIC)
        op = make_addr(op, 1);
    new_field->op = op;
    new_field->next = var_hash[hash_index];
    var_hash[hash_index] = new_field;
    return new_field;            
}

Func *ir_insert_func_hash_table(unsigned hash_index, char *str, Type *return_type, Func *func){
    Func *cur = func_hash[hash_index];
    if (cur == NULL){
        func->return_type = return_type;
        func->defined = 1;
        Operand *op = make_func(func);
        func->op = op;
        func_hash[hash_index] = func;
        return func;
    }
    func->return_type = return_type;
    func->defined = 1;
    Operand *op = make_func(func);
    func->op = op;
    func->next = func_hash[hash_index];
    func_hash[hash_index] = func;
    return func_hash[hash_index];
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

void insert_operand(Operand *op) {
    op->next_list_op = op_head;
    op_head = op;
}

Operand *make_temp() {
    Operand *new_temp = malloc(sizeof(Operand));
    new_temp->kind = OP_TEMP;
    new_temp->u.temp.no = temp_no ++;
    insert_operand(new_temp);
    return new_temp;
}

Operand *make_var(Field_List *field) {
    Operand *new_var = malloc(sizeof(Operand));
    new_var->kind = OP_VARIABLE;
    new_var->u.var.no = var_no ++;
    new_var->u.var.field = field;
    insert_operand(new_var);
    return new_var;
}

Operand *make_addr(Operand *var, int ref_hidden) {
    Operand *new_addr = malloc(sizeof(Operand));
    new_addr->kind = OP_ADDRESS;
    new_addr->u.address.no = var->u.var.no;
    new_addr->u.address.field = var->u.var.field;
    new_addr->u.address.val_kind = var->kind;
    new_addr->u.address.ref_hidden = ref_hidden;
    insert_operand(new_addr);
    return new_addr;
}

Operand *make_func(Func *func) {
    Operand *new_func = malloc(sizeof(Operand));
    new_func->kind = OP_FUNC;
    new_func->u.func.func = func;
    insert_operand(new_func);
    return new_func;
}

Operand *make_fall_label() {
    Operand *new_label = malloc(sizeof(Operand));
    new_label->kind = OP_LABEL;
    new_label->u.label.no = -1;
    return new_label;
}

Operand *make_label() {
    Operand *new_label = malloc(sizeof(Operand));
    new_label->kind = OP_LABEL;
    new_label->u.label.no = label_no ++;
    insert_operand(new_label);
    return new_label;
}

Operand *make_constant(int val) {
    Operand *new_constant = malloc(sizeof(Operand));
    new_constant->kind = OP_CONSTANT;
    new_constant->u.constant.val = val;
    insert_operand(new_constant);
    return new_constant;
}

Operand *make_relop(int kind) {
    Operand *new_relop = malloc(sizeof(Operand));
    new_relop->u.relop.kind = kind;
    new_relop->kind = OP_RELOP;
    insert_operand(new_relop);
    return new_relop;
}

InterCode *make_ir(int kind, Operand *result, Operand *op1, Operand *op2, Operand *relop) {
    InterCode *code = malloc(sizeof(InterCode));
    code->kind = kind;
    switch (kind) {
    case IR_LABEL:
    case IR_FUNCTION:
    case IR_GOTO:
    case IR_RETURN:
    case IR_ARG:
    case IR_PARAM:
    case IR_READ:
    case IR_WRITE:
        code->u.nonop.result = result;
        break;
    case IR_ASSIGN:
    case IR_REF_ASSIGN:
    case IR_DEREF_ASSIGN:
    case IR_ASSIGN_TO_DEREF:
    case IR_DEC:
    case IR_CALL:
        code->u.sinop.result = result;
        code->u.sinop.op = op1;
        break;
    case IR_ADD:
    case IR_SUB:
    case IR_MUL:
    case IR_DIV:
        code->u.binop.result = result;
        code->u.binop.op1 = op1;
        code->u.binop.op2 = op2;
        break;
    case IR_IF:
        code->u.trinop.result = result;
        code->u.trinop.op1 = op1;
        code->u.trinop.op2 = op2;
        code->u.trinop.relop = relop;
        break;
    default:
        break;
    }
    if (ir_head == NULL)
        ir_head = code;
    return code;
}

Operand *relop_reverse(Operand *relop) {
    Operand *new_relop = malloc(sizeof(Operand));
    new_relop->kind = OP_RELOP;
    new_relop->next_list_op = relop->next_list_op;
    new_relop->next = relop->next;
    switch (relop->u.relop.kind)
    {
    case RELOP_G: new_relop->u.relop.kind = RELOP_LE; break;
    case RELOP_L: new_relop->u.relop.kind = RELOP_GE; break;
    case RELOP_GE: new_relop->u.relop.kind = RELOP_L; break;
    case RELOP_LE: new_relop->u.relop.kind = RELOP_G; break;
    case RELOP_E: new_relop->u.relop.kind = RELOP_NE; break;
    case RELOP_NE: new_relop->u.relop.kind = RELOP_E; break;
    default: assert(0); break;
    }
    return new_relop;
}

int size_of_array(AST_Node *node) {
    int size = 1;
    Type *type = ir_exp_type(node);
    Type *cur_type = type;
    while (cur_type && cur_type->kind == ARRAY) {
        size *= cur_type->u.array.size;
        cur_type = cur_type->u.array.elem;
    }
    assert(cur_type);
    if (cur_type->kind == BASIC) {
        size *= 4;
    }
    else {
        assert(cur_type->kind == STRUCTURE);
        size *= cur_type->u.structure.size;
    }
    return size;
}

int get_constant(AST_Node *node) {
    if (strcmp(node->first_child->name, "Exp") == 0 && node->first_child->sibling) {
        if (strcmp(node->first_child->sibling->name, "PLUS") == 0) {
            return get_constant(node->first_child) + get_constant(node->first_child->sibling->sibling);
        }
        if (strcmp(node->first_child->sibling->name, "MINUS") == 0) {
            return get_constant(node->first_child) - get_constant(node->first_child->sibling->sibling);
        }
        if (strcmp(node->first_child->sibling->name, "STAR") == 0) {
            return get_constant(node->first_child) * get_constant(node->first_child->sibling->sibling);
        }
        if (strcmp(node->first_child->sibling->name, "DIV") == 0) {
            int divider = get_constant(node->first_child->sibling->sibling);
            if (divider == 0) {
                printf("Error: divide-by-zero\n");
                assert(0);
            }
            return get_constant(node->first_child) / get_constant(node->first_child->sibling->sibling);
        }
        assert(0);
        return 0;
    }
    if (strcmp(node->first_child->name, "MINUS") == 0) {
        return -get_constant(node->first_child->sibling);
    }
    if (strcmp(node->first_child->name, "LP") == 0) {
        return get_constant(node->first_child->sibling);
    }
    if (strcmp(node->first_child->name, "INT") == 0) {
        return atoi(node->first_child->value);
    }
    assert(0);
    return 0;
}

int all_constant(AST_Node *node) {
    if (strcmp(node->first_child->name, "Exp") == 0 && node->first_child->sibling && 
        (strcmp(node->first_child->sibling->name, "PLUS") == 0 || strcmp(node->first_child->sibling->name, "MINUS") == 0
        || strcmp(node->first_child->sibling->name, "STAR") == 0 || strcmp(node->first_child->sibling->name, "DIV") == 0)) {
        if (!all_constant(node->first_child) || !all_constant(node->first_child->sibling->sibling)) {
            return 0;
        }
        return 1;
    }
    if (strcmp(node->first_child->name, "MINUS") == 0 || strcmp(node->first_child->name, "LP") == 0) {
        if (!all_constant(node->first_child->sibling)) {
            return 0;
        }
        return 1;
    }
    if (strcmp(node->first_child->name, "INT") == 0) {
        return 1;
    }
    return 0;
}

Operand *get_id(AST_Node *node) {
    Field_List *variable = ir_query_field_hash_table(hash_pjw(node->first_child->value), node->first_child->value, node->first_child, 0);
    assert(variable);
    return variable->op;
}

int is_id(AST_Node *node) {
    if (node->first_child && strcmp(node->first_child->name, "ID") == 0 && node->first_child->sibling == NULL) 
        return 1;
    return 0;
}

void to_file(FILE *fp) {
    InterCode *cur = ir_head;
    while (cur) {
        fprintf(fp, "%s", show_ir(cur));
        cur = cur->next;
    }
}

char *show_ir(InterCode *code) {
    assert(code);
    char *buffer = malloc(50 * sizeof(char));
    switch (code->kind) {
    case IR_LABEL: sprintf(buffer, "LABEL %s :\n", show_op(code->u.nonop.result)); break;
    case IR_FUNCTION: sprintf(buffer, "FUNCTION %s :\n", show_op(code->u.nonop.result)); break;
    case IR_ASSIGN: sprintf(buffer, "%s := %s\n", show_op(code->u.sinop.result), show_op(code->u.sinop.op)); break;
    case IR_ADD: sprintf(buffer, "%s := %s + %s\n", show_op(code->u.binop.result), show_op(code->u.binop.op1), show_op(code->u.binop.op2)); break;
    case IR_SUB: sprintf(buffer, "%s := %s - %s\n", show_op(code->u.binop.result), show_op(code->u.binop.op1), show_op(code->u.binop.op2)); break;
    case IR_MUL: sprintf(buffer, "%s := %s * %s\n", show_op(code->u.binop.result), show_op(code->u.binop.op1), show_op(code->u.binop.op2)); break;
    case IR_DIV: sprintf(buffer, "%s := %s / %s\n", show_op(code->u.binop.result), show_op(code->u.binop.op1), show_op(code->u.binop.op2)); break;
    case IR_REF_ASSIGN: sprintf(buffer, "%s := &%s\n", show_op(code->u.sinop.result), show_op(code->u.sinop.op)); break;
    case IR_DEREF_ASSIGN: sprintf(buffer, "%s := *%s\n", show_op(code->u.sinop.result), show_op(code->u.sinop.op)); break;
    case IR_ASSIGN_TO_DEREF: sprintf(buffer, "*%s := %s\n", show_op(code->u.sinop.result), show_op(code->u.sinop.op)); break;
    case IR_GOTO: sprintf(buffer, "GOTO %s\n", show_op(code->u.nonop.result)); break;
    case IR_IF: sprintf(buffer, "IF %s %s %s GOTO %s\n", show_op(code->u.trinop.op1), show_op(code->u.trinop.relop), show_op(code->u.trinop.op2), show_op(code->u.trinop.result)); break;
    case IR_RETURN: sprintf(buffer, "RETURN %s\n", show_op(code->u.nonop.result)); break;
    case IR_DEC: sprintf(buffer, "DEC v%d %d\n", code->u.sinop.result->u.address.no, code->u.sinop.op->u.constant.val); break;
    case IR_ARG: sprintf(buffer, "ARG %s\n", show_op(code->u.nonop.result)); break;
    case IR_CALL: sprintf(buffer, "%s := CALL %s\n", show_op(code->u.sinop.result), show_op(code->u.sinop.op)); break;
    case IR_PARAM: sprintf(buffer, "PARAM %s\n", show_op(code->u.nonop.result)); break;
    case IR_READ: sprintf(buffer, "READ %s\n", show_op(code->u.nonop.result)); break;
    case IR_WRITE: sprintf(buffer, "WRITE %s\n", show_op(code->u.nonop.result)); break;
    default:
        break;
    }
    return buffer;
}

char *show_op(Operand *op) {
    assert(op);
    char *buffer = malloc(50 * sizeof(char));
    switch (op->kind) {
    case OP_VARIABLE: sprintf(buffer, "v%d", op->u.var.no); break;
    case OP_CONSTANT: sprintf(buffer, "#%d", op->u.constant.val); break;
    case OP_TEMP: sprintf(buffer, "t%d", op->u.temp.no); break;
    case OP_LABEL: sprintf(buffer, "label%d", op->u.label.no); break;
    case OP_FUNC: sprintf(buffer, "%s", op->u.func.func->name); break;
    case OP_RELOP: 
        switch (op->u.relop.kind) {
        case RELOP_G: sprintf(buffer, ">"); break;
        case RELOP_L: sprintf(buffer, "<"); break;
        case RELOP_GE: sprintf(buffer, ">="); break;
        case RELOP_LE: sprintf(buffer, "<="); break;
        case RELOP_E: sprintf(buffer, "=="); break;
        case RELOP_NE: sprintf(buffer, "!="); break;
        default: assert(0); break;
        }
        break;
    case OP_ADDRESS: 
        if (op->u.address.ref_hidden == 0) {
            if (op->u.address.val_kind == OP_TEMP) sprintf(buffer, "&t%d", op->u.address.no);
            else sprintf(buffer, "&v%d", op->u.address.no);
        }
        else {
            if (op->u.address.val_kind == OP_TEMP) sprintf(buffer, "t%d", op->u.address.no);
            else sprintf(buffer, "v%d", op->u.address.no);
        }
        break;
    default: assert(0); break;
    }
    return buffer;
}

void replace_label(int new_label_no, int old_label_no) {
    Operand *op = op_head;
    while (op != NULL) {
        if (op->kind == OP_LABEL && op->u.label.no == old_label_no) {
            op->u.label.no = new_label_no;
        }
        op = op->next_list_op;
    }
}

void replace_temp(int new_temp_no, int old_temp_no) {
    Operand *op = op_head;
    while (op != NULL) {
        if (op->kind == OP_TEMP && op->u.temp.no == old_temp_no) {
            op->u.temp.no = new_temp_no;
        }
        op = op->next_list_op;
    }
}

void post_optimize() {
    if (ir_head == NULL)
        return;
    InterCode *ir = ir_head;
    while (ir != NULL) {
        if (ir->kind == IR_LABEL && ir->next != NULL && ir->next->kind == IR_LABEL) {
            replace_label(ir->u.nonop.result->u.label.no, ir->next->u.nonop.result->u.label.no);
            ir->next = ir->next->next;
            ir->next->prev = ir;
        } /* LABEL label1 | LABEL label2 ---> LABEL label1 */
        if (ir->kind == IR_ADD && ir->u.binop.result->kind == OP_TEMP && ir->u.binop.op1->kind == OP_TEMP && ir->u.binop.op2->kind == OP_CONSTANT && ir->u.binop.op2->u.constant.val == 0) {
            replace_temp(ir->u.binop.op1->u.temp.no, ir->u.binop.result->u.temp.no);
            if (ir->prev != NULL) {
                ir->prev->next = ir->next;
            }
            if (ir->next != NULL) {
                ir->next->prev = ir->prev;
            }
            ir = ir->prev;
        } /* t1 = t2 + #0 ---> replace t1 by t2 */
        if ((ir->kind == IR_ASSIGN || ir->kind == IR_ADD || ir->kind == IR_SUB || ir->kind == IR_MUL || ir->kind == IR_DIV || ir->kind == IR_CALL)) {
            if ((ir->kind == IR_ASSIGN || ir->kind == IR_CALL) && ir->u.sinop.result->kind == OP_TEMP && ir->next != NULL
                && ir->next->kind == IR_ASSIGN && ir->next->u.sinop.result->kind == OP_VARIABLE
                && ir->next->u.sinop.op->kind == OP_TEMP && ir->u.sinop.result->u.temp.no == ir->next->u.sinop.op->u.temp.no) {
                int kind = ir->kind;
                InterCode *new_assign = make_ir(kind, ir->next->u.sinop.result, ir->u.sinop.op, NULL, NULL);
                new_assign->next = ir->next->next;
                new_assign->prev = ir->prev;
                if (ir->prev != NULL)
                    ir->prev->next = new_assign;
                if (ir->next->next != NULL)
                    ir->next->next->prev = new_assign;
                ir = new_assign;
            }
            else if ((ir->kind == IR_ADD || ir->kind == IR_SUB || ir->kind == IR_MUL || ir->kind == IR_DIV) && ir->u.binop.result->kind == OP_TEMP && ir->next != NULL
                && ir->next->kind == IR_ASSIGN && ir->next->u.sinop.result->kind == OP_VARIABLE
                && ir->next->u.sinop.op->kind == OP_TEMP && ir->u.binop.result->u.temp.no == ir->next->u.sinop.op->u.temp.no) {
                int kind = ir->kind;
                InterCode *new_assign = make_ir(kind, ir->next->u.sinop.result, ir->u.binop.op1, ir->u.binop.op2, NULL);
                new_assign->next = ir->next->next;
                new_assign->prev = ir->prev;
                if (ir->prev != NULL)
                    ir->prev->next = new_assign;
                if (ir->next->next != NULL)
                    ir->next->next->prev = new_assign;
                ir = new_assign;
            }
        } /* t = exp | v = t ---> v = exp */
        ir = ir->next;
    }
}