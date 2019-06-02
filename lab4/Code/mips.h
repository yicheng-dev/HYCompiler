#ifndef MIPS_H
#define MIPS_H

#include "ir.h"
#include "AST.h"
#include "semantics.h"

#define REG_GP 28
#define REG_SP 29
#define REG_FP 30
#define REG_RA 31

struct MipsOperand {
    enum {
        M_OP_LABEL,
        M_OP_REG,
        M_OP_STA,
        M_OP_IMM
    } kind;
    union {
        struct {
            char name[MAX_NAME_LEN];
            int label_id;
            int is_func;
        } label;
        struct {
            int id;
        } reg;
        struct {
            int reg_id;
            int offset;
        } sta;
        struct {
            int val;
        } imm;
    } u; 
};

struct MipsCode {
    enum {
        MIPS_LABEL,
        MIPS_LI,
        MIPS_MOVE,
        MIPS_ADDI,
        MIPS_ADD,
        MIPS_SUB,
        MIPS_MUL,
        MIPS_DIV,
        MIPS_MFLO,
        MIPS_LA,
        MIPS_LW,
        MIPS_SW,
        MIPS_J,
        MIPS_JAL,
        MIPS_JR,
        MIPS_BEQ,
        MIPS_BNE,
        MIPS_BGT,
        MIPS_BLT,
        MIPS_BGE,
        MIPS_BLE
    } kind;
    union {
        struct {
            struct MipsOperand *op1;
        } nonop;
        struct {
            struct MipsOperand *op1, *op2;
        } sinop;
        struct {
            struct MipsOperand *op1, *op2, *op3;
        } binop;
    } u;
    struct MipsCode *next;
};

typedef struct MipsOperand MipsOperand;
typedef struct MipsCode MipsCode;

/* top callers */
void mips_init();
void mips_generate();
void insert_mips_code(MipsCode *code);
void clear_reg();
InterCode *ir_to_mips(InterCode *code);

/* make mips operands */
MipsOperand *make_m_op_new_temp();
void make_m_op_set_param(Operand *op, int offset);
MipsOperand *make_m_op_get_m(Operand *op, int size);
MipsOperand *make_m_op_m2r(Operand *op);
MipsOperand *make_m_op_arg_mem(int offset);
MipsOperand *make_m_op_func(Operand *func_op);
MipsOperand *make_m_op_label(Operand *label_op);

/* make mips codes */
void make_mips_code_la(MipsOperand *addr_reg, MipsOperand *addr_mem);
void make_mips_code_sp_add(int size);
void make_mips_code_fp_add(int size);
void make_mips_code_jr(MipsOperand *ra_op);
void make_mips_code_jal(MipsOperand *label_op);
void make_mips_code_move(MipsOperand *dst_op, MipsOperand *op);
void make_mips_code_arith(MipsOperand *dst_op, MipsOperand *op1, MipsOperand *op2, int type);
void make_mips_code_if(MipsOperand *op1, int relop_kind, MipsOperand *op2, MipsOperand *label_op);
void make_mips_code_sw(MipsOperand *var_reg, MipsOperand *var_mem);
void make_mips_code_lw(MipsOperand *var_reg, MipsOperand *var_mem);
void make_mips_code_li(MipsOperand *imm_reg, MipsOperand *imm);
void make_mips_code_func(MipsOperand *func_op);
void make_mips_code_label(MipsOperand *label_op);
void make_mips_code_goto(MipsOperand *label_op);

/* io functions */
void mips_to_file(FILE *fp);
char *show_mips(MipsCode *code);
char *show_m_op(MipsOperand *op);
char *show_m_op_reg(int reg_id);

/* helper functions */
void pre_load_head(FILE *fp);
int is_main_func(MipsOperand *func_op);
int get_temp_reg();
Func *next_func(InterCode *code);



#endif