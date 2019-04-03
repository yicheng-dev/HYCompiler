#ifndef SEMANTICS_H
#define SEMANTICS_H

#define MAX_NAME_LEN 128
#define MAX_HASH_TABLE_LEN 16384
#define BASIC_INT 1
#define BASIC_FLOAT 2
#define MAX_ERROR_INFO_LEN 64
#define MAX_FIELD_NUM 256

#include "AST.h"

struct Type {
    enum {
        BASIC, ARRAY, STRUCTURE
    } kind;
    union {
        int basic;
        struct {
            struct Type *elem;
            int size;
        } array;
        struct Field_List *structure;
    } u;
    struct Type *next; // multiple return type in a CompSt
    struct Type *next_flat; // struct type to non-struct flat type list
    int line_num; // multiple return type in a CompSt
};

struct Field_List {
    char name[MAX_NAME_LEN];
    struct Type *type;
    struct Field_List *next;
    int wrapped_layer;
    int is_structure;
    int line_num;
    struct Field_List *parent_structure;
};

struct Func {
    char name[MAX_NAME_LEN];
    struct Type *return_type;
    int defined;
    int param_size;
    struct Field_List *first_param;
    int line_num;
};

struct Sem_Error_List {
    int type;
    int line_num;
    char info[MAX_ERROR_INFO_LEN];
    struct Sem_Error_List *next;
};


typedef struct Func Func;
typedef struct Type Type;
typedef struct Field_List Field_List;
typedef struct Sem_Error_List Error_List;

/* The entry function of semantics analysis */
void semantics_analysis(AST_Node *root);

/* initialzation of hash table */
void init_hash_table();

/* semantics of High-level Definitions */
void sem_program(AST_Node *); 
void sem_ext_def_list(AST_Node *);
void sem_ext_def(AST_Node *);
void sem_ext_dec_list(AST_Node *, Type *);

/* semantics of Specifiers */
Type* sem_specifier(AST_Node *, int, int);
Type* sem_struct_specifier(AST_Node *, int, int);

/* semantics of Declarators */
Field_List *sem_var_dec(AST_Node *, Type *, int, int);
Func *sem_fun_dec(AST_Node *);
Field_List *sem_var_list(AST_Node *);
Field_List *sem_param_dec(AST_Node *);

/* semantics of Statements */
void sem_comp_st(AST_Node *, int);
void sem_stmt_list(AST_Node *, int);
void sem_stmt(AST_Node *, int);

/* semantics of Local Definitions */
Field_List *sem_def_list(AST_Node *, int, int);
Field_List *sem_def(AST_Node *, int, int);
Field_List *sem_dec_list(AST_Node *, Type *, int, int);
Field_List *sem_dec(AST_Node *, Type *, int, int);

/* semantics of expressions */
Type *sem_exp(AST_Node *);
Type *sem_args(AST_Node *);

/* helper functions */
Field_List *insert_field_hash_table(unsigned, Type *, AST_Node *, int, int);
Field_List *query_field_hash_table(unsigned, AST_Node *, int);
Func *insert_func_hash_table(unsigned, Type *, Func *);
Func *query_func_hash_table(unsigned);
Func *insert_func_dec_hash_table(unsigned, Type *, Func *);
int check_equal_type(Type *, Type *);
int check_struct_equal_type(Type *, Type *);
int check_duplicate_field(Type *);
int check_equal_params(Field_List *, Type *);
int check_twofunc_equal_params(Field_List *, Field_List *);
void check_undec_func();
void pop_local_var(int);
Type *struct_type_to_list(Type *, Field_List *);
//void insert_struct_list(Type *, int);

/* error report list */
void add_error_list(int, char *, int);
void print_error_list();

/* for debug */
void print_field_list(int);

#endif