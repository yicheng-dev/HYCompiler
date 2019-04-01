#ifndef SEMANTICS_H
#define SEMANTICS_H

#define MAX_NAME_LEN 128
#define MAX_HASH_TABLE_LEN 16384
#define BASIC_INT 1
#define BASIC_FLOAT 2
#define MAX_ERROR_INFO_LEN 64

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
};

struct Field_List {
    char name[MAX_NAME_LEN];
    struct Type *type;
    struct Field_List *next;
    int wrapped_layer;
};

struct Func {
    char name[MAX_NAME_LEN];
    struct Type return_type;
    int defined;
    int param_size;
    struct Var *first_param;
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
Type* sem_specifier(AST_Node *);
Type* sem_struct_specifier(AST_Node *);

/* semantics of Declarators */
void sem_var_dec(AST_Node *, Type *);



/* helper functions */
void insert_field_hash_table(unsigned, Type *, AST_Node *, int);

/* error report list */
void add_error_list(int, char *, int);
void print_error_list();

#endif