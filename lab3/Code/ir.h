#ifndef IR_H
#define IR_H

#include "AST.h"

struct Operand {
    enum {
        VARIABLE,
        CONSTANT,
        ADDRESS
    } kind;
    union {
        int var_no;
        int value;
    } u;
};

struct InterCode {
    enum {
        LABEL,
        FUNCTION,
        ASSIGN,
        ADD,
        SUB,
        MUL,
        DIV,
        REF_ASSIGN,
        DEREF_ASSIGN,
        ASSIGN_TO_DEREF,
        GOTO,
        IF,
        RETURN,
        DEC,
        ARG,
        CALL,
        PARAM,
        READ,
        WRITE
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
    } u;
    struct InterCode *prev;
    struct InterCode *next;
};

typedef struct Operand Operand;
typedef struct InterCode InterCode;

/* intermediate representation's generation */
void ir_generate(AST_Node *);


/* helper functions */
InterCode *bind(InterCode *, InterCode *);

#endif