#ifndef IR_H
#define IR_H

#include "AST.h"
#include "semantics.h"

struct Operand {
    enum {
        OP_VARIABLE,
        OP_CONSTANT,
        OP_ADDRESS,
        OP_LABEL,
        OP_TEMP,
        OP_FUNC,
        OP_RELOP
    } kind;
    union {
        struct {
            int no;
            struct Field_List *field;
        } var;
        struct {
            int no;
        } label;
        struct {
            int no;
        } temp;
        struct {
            int val;
        } constant;
        struct {
            struct Func *func;
        } func;
        struct {
            char content[5];
        } relop;
        struct {
            int no;
            int val_kind;
            struct Field_List *field;
            int ref_hidden;
        } address;
    } u;
    struct Operand *next;
    struct Operand *next_list_op;
};

struct InterCode {
    enum {
        IR_LABEL,
        IR_FUNCTION,
        IR_ASSIGN,
        IR_ADD,
        IR_SUB,
        IR_MUL,
        IR_DIV,
        IR_REF_ASSIGN,
        IR_DEREF_ASSIGN,
        IR_ASSIGN_TO_DEREF,
        IR_GOTO,
        IR_IF,
        IR_RETURN,
        IR_DEC,
        IR_ARG,
        IR_CALL,
        IR_PARAM,
        IR_READ,
        IR_WRITE
    } kind;
    union {
        struct {
            struct Operand *result;
        } nonop;
        struct {
            struct Operand *result, *op;
        } sinop;
        struct {
            struct Operand *result, *op1, *op2;
        } binop;
        struct {
            struct Operand *result, *op1, *relop, *op2;
        } trinop;
    } u;
    struct InterCode *prev;
    struct InterCode *next;
};

typedef struct Operand Operand;
typedef struct InterCode InterCode;

/* intermediate representation's generation */
void ir_generate(AST_Node *);

/* ir functions */
void ir_init_hash_table();
void ir_insert_read_write_func(char *name);
void ir_program(AST_Node *node);
InterCode *ir_ext_def_list(AST_Node *node);
InterCode *ir_ext_def(AST_Node *node);
struct Type* ir_specifier(AST_Node *node, int wrapped_layer, int in_structure);
InterCode *ir_fun_dec(AST_Node *node, struct Type *ret_type);
struct Field_List *ir_var_list(AST_Node *node);
struct Field_List *ir_param_dec(AST_Node *node);
InterCode *ir_comp_st(AST_Node *node, int wrapped_layer);
InterCode *ir_def_list(AST_Node *node, int wrapped_layer);
InterCode *ir_def(AST_Node *node, int wrapped_layer);
InterCode *ir_dec_list(AST_Node *node, struct Type *type, int wrapped_layer);
InterCode *ir_dec(AST_Node *node, struct Type *type, int wrapped_layer);
InterCode *ir_stmt_list(AST_Node *node, int wrapped_layer);
InterCode *ir_stmt(AST_Node *node, int wrapped_layer);
InterCode *ir_exp(AST_Node *node, Operand *place);
struct Type *ir_exp_type(AST_Node *node);
InterCode *ir_cond(AST_Node *node, Operand *label_true, Operand *label_false);
InterCode *ir_args(AST_Node *node);
struct Type* ir_struct_specifier(AST_Node *node, int wrapped_layer, int in_structure);
struct Field_List *ir_def_list_structure(AST_Node *node, int wrapped_layer);
struct Field_List *ir_def_structure(AST_Node *node, int wrapped_layer);
struct Field_List *ir_dec_list_structure(AST_Node *node, struct Type *type, int wrapped_layer);
struct Field_List *ir_dec_structure(AST_Node *node, struct Type *type, int wrapped_layer);
struct Field_List *ir_var_dec(AST_Node *node, struct Type *type, int in_structure, int wrapped_layer);



/* helper functions */
int build_size_offset(struct Type *structure_type);
InterCode *bind(InterCode *, InterCode *);
struct Field_List *ir_insert_field_hash_table(unsigned hash_index, char *str, struct Type *type, AST_Node *node, int wrapped_layer, int is_structure);
struct Func *ir_insert_func_hash_table(unsigned hash_index, char *str, struct Type *return_type, struct Func *func);
struct Func *ir_query_func_hash_table(unsigned hash_index, char *str);
struct Field_List *ir_query_field_hash_table(unsigned hash_index, char *str, AST_Node *node, int look_for_structure);
Operand *make_temp();
Operand *make_var(struct Field_List *field);
Operand *make_addr(struct Operand *var, int ref_hidden);
Operand *make_func(struct Func *func);
Operand *make_label();
Operand *make_constant(int val);
Operand *make_relop(char *content);
InterCode *make_ir(int kind, Operand *result, Operand *op1, Operand *op2, Operand *relop);
int size_of_array(AST_Node *node);
int size_of_array_type(struct Type *type);
int get_constant(AST_Node *node);
int all_constant(AST_Node *node);
Operand *get_id(AST_Node *node);
int is_id(AST_Node *node);
void to_file(FILE *fp);
char *show_ir(InterCode *code);
char *show_op(Operand *op);
void replace_label(int new_label_no, int old_label_no);
void replace_temp(int new_temp_no, int old_temp_no);
void post_optimize();
#endif