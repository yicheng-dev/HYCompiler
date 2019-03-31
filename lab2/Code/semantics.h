#ifndef SEMANTICS_H
#define SEMANTICS_H

#define MAX_NAME_LEN 128
#define MAX_HASH_TABLE_LEN 16384
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
    struct Type type;
    struct Field_List *next;
};

struct Var {
    char name[MAX_NAME_LEN];
    struct Type type;
    union{
        int int_val;
        float float_val;
    } val;
    struct Var *next_param; /* if the variable is a parameter of a function */
};

struct Func {
    char name[MAX_NAME_LEN];
    struct Type return_type;
    int defined;
    int param_size;
    struct Var *first_param;
};

typedef struct Var Var;
typedef struct Func Func;
typedef struct Type Type;
typedef struct Field_List Field_List;


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

#endif