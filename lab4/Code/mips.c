#include "semantics.h"
#include "utils.h"
#include "AST.h"
#include "debug.h"
#include "ir.h"
#include "math.h"
#include "mips.h"

extern InterCode *ir_head;
extern Operand *read_op;
extern Operand *write_op;
static Func *current_func = NULL;
static int reg[32];
static MipsOperand *fp_reg = NULL;
static MipsOperand *sp_reg = NULL;
static MipsOperand *v0_reg = NULL;
static MipsOperand *ra_reg = NULL;
static MipsOperand *a0_reg = NULL;
static MipsCode *mips_head = NULL;

// int cnt = 0;

void mips_init() {
    fp_reg = malloc(sizeof(MipsOperand));
    fp_reg->kind = M_OP_REG;
    fp_reg->u.reg.id = REG_FP;
    sp_reg = malloc(sizeof(MipsOperand));
    sp_reg->kind = M_OP_REG;
    sp_reg->u.reg.id = REG_SP;
    v0_reg = malloc(sizeof(MipsOperand));
    v0_reg->kind = M_OP_REG;
    v0_reg->u.reg.id = 2;
    ra_reg = malloc(sizeof(MipsOperand));
    ra_reg->kind = M_OP_REG;
    ra_reg->u.reg.id = REG_RA;
    a0_reg = malloc(sizeof(MipsOperand));
    a0_reg->kind = M_OP_REG;
    a0_reg->u.reg.id = 4;
}

void insert_mips_code(MipsCode *code) {
    if (code == NULL)
        return;
    if (mips_head == NULL)
        mips_head = code;
    else {
        MipsCode *cur_code = mips_head;
        while (cur_code != NULL && cur_code->next != NULL) {
            cur_code = cur_code->next;
        }
        cur_code->next = code;
    }
}

void mips_generate() {
    mips_init();
    InterCode *cur_ir = ir_head;
    while (cur_ir != NULL) {
        InterCode *old_ir = cur_ir;
        cur_ir = ir_to_mips(cur_ir);
        assert(cur_ir != old_ir);
        clear_reg(); // naive register allocation
    }
}

void clear_reg() { // free all the t_ regs
    int i;
    for (i = 8; i < 15; i ++) {
        reg[i] = 0;
    }
    for (i = 24; i < 26; i ++) {
        reg[i] = 0;
    }
}

static inline int reg_t(const int index) {
    if (index < 0 || index > 9)
        assert(0);
    if (index >= 0 && index <= 7)
        return reg[index + 8];
    return reg[index + 24];
}

static inline int reg_v(const int index) {
    if (index < 0 || index > 1)
        assert(0);
    return reg[index + 2];
}

static inline int reg_a(const int index) {
    if (index < 0 || index > 3)
        assert(0);
    return reg[index + 4];
}

static inline int reg_s(const int index) {
    if (index < 0 || index > 7)
        assert(0);
    return reg[index + 16];
}

InterCode *ir_to_mips(InterCode *code) {
    // Log("%d", code->kind);
    // Log("%d", cnt ++);
    switch (code->kind) {
        case IR_FUNCTION: {
            current_func = code->u.nonop.result->u.func.func;
            MipsOperand *func_op = make_m_op_func(code->u.nonop.result); 
            make_mips_code_func(func_op);
            if (is_main_func(func_op)) {
                make_mips_code_move(fp_reg, sp_reg);
                make_mips_code_sp_add(- current_func->stack_size);
            }
            return code->next;
        }
        case IR_LABEL: {
            MipsOperand *label_op = make_m_op_label(code->u.nonop.result); 
            make_mips_code_label(label_op);
            return code->next;
        }
        case IR_GOTO: {
            MipsOperand *label_op = make_m_op_label(code->u.nonop.result);
            make_mips_code_goto(label_op);
            return code->next;
        }
        case IR_ADD:
        case IR_SUB:
        case IR_MUL:
        case IR_DIV: { // a = b [+|-|*|/] c
            MipsOperand *op1 = make_m_op_m2r(code->u.binop.op1); // lw reg1 b
            MipsOperand *op2 = make_m_op_m2r(code->u.binop.op2); // lw reg2 c
            MipsOperand *dst_reg = make_m_op_new_temp(); // reg3
            make_mips_code_arith(dst_reg, op1, op2, code->kind); // reg3 = reg1 [+|-|*|/] reg2
            MipsOperand *dst_mem = make_m_op_get_m(code->u.binop.result, 4); // a
            make_mips_code_sw(dst_reg, dst_mem); // sw reg3 a
            return code->next;
        }
        case IR_DEC: { // DEC array [size]
            make_m_op_get_m(code->u.sinop.result, code->u.sinop.op->u.constant.val);
            return code->next;
        }
        case IR_IF: { // if a [relop] b goto label
            MipsOperand *op1 = make_m_op_m2r(code->u.trinop.op1); // lw reg1 a
            MipsOperand *op2 = make_m_op_m2r(code->u.trinop.op2); // lw reg2 b
            MipsOperand *label_op = make_m_op_label(code->u.trinop.result);
            make_mips_code_if(op1, code->u.trinop.relop->u.relop.kind, op2, label_op);
            return code->next;
        }
        case IR_ASSIGN: { // a = b
            MipsOperand *op = make_m_op_m2r(code->u.sinop.op); // lw reg1 b
            MipsOperand *dst_reg = make_m_op_new_temp(); // reg2
            make_mips_code_move(dst_reg, op);
            MipsOperand *dst_mem = make_m_op_get_m(code->u.sinop.result, 4); // a
            make_mips_code_sw(dst_reg, dst_mem);
            return code->next;
        }
        case IR_DEREF_ASSIGN: { // a = *b
            MipsOperand *dst_reg = make_m_op_m2r(code->u.sinop.op); // lw reg2 0(reg1(b))
            MipsOperand *dst_mem = make_m_op_get_m(code->u.sinop.result, 4); // a
            make_mips_code_sw(dst_reg, dst_mem); // sw reg2 a
            return code->next; 
        }
        case IR_ASSIGN_TO_DEREF: { // *a = b
            MipsOperand *src_reg = make_m_op_m2r(code->u.sinop.op);
            MipsOperand *dst_mem = make_m_op_get_m(code->u.sinop.result, 4); // 0(reg1(a))
            make_mips_code_sw(src_reg, dst_mem); // sw reg2 0(reg1(a))
            return code->next;
        }
        case IR_REF_ASSIGN: { // a = &b
            MipsOperand *src_mem = make_m_op_get_m(code->u.sinop.op, 4); // &b
            MipsOperand *dst_reg = make_m_op_new_temp(); // reg1
            make_mips_code_la(dst_reg, src_mem); // la reg1 &b
            MipsOperand *dst_mem = make_m_op_get_m(code->u.sinop.result, 4); // a
            make_mips_code_sw(dst_reg, dst_mem); // sw reg1 a
            return code->next;
        }
        case IR_RETURN: { // return x
            MipsOperand *src_reg = make_m_op_m2r(code->u.nonop.result); // lw|li reg1 x
            make_mips_code_move(v0_reg, src_reg); // move v0 reg1
            make_mips_code_jr(ra_reg); // jr ra
            return code->next;
        }
        case IR_CALL: { // x = call f
            MipsOperand *dst_label = make_m_op_func(code->u.sinop.op); // f
            make_mips_code_jal(dst_label); // jal f
            make_mips_code_lw(ra_reg, make_m_op_arg_mem(-4)); // lw ra -4(fp)
            make_mips_code_move(sp_reg, fp_reg); // sp = fp
            make_mips_code_fp_add(current_func->stack_size); // fp = sp + size
            MipsOperand *ret_reg = make_m_op_new_temp(); // reg1
            make_mips_code_move(ret_reg, v0_reg); // move reg1 v0
            MipsOperand *ret_mem = make_m_op_get_m(code->u.sinop.result, 4); // x
            make_mips_code_sw(ret_reg, ret_mem); // sw reg1 x
            return code->next;
        }
        case IR_READ: { // read x
            MipsOperand *dst_reg = make_m_op_new_temp(); // reg1
            MipsOperand *read_mips_op = make_m_op_func(read_op); // read
            make_mips_code_move(fp_reg, sp_reg); // fp = sp
            make_mips_code_sp_add(-4); // sp = sp - 4
            make_mips_code_sw(ra_reg, make_m_op_arg_mem(-4)); // sw ra -4(fp)
            make_mips_code_jal(read_mips_op); // jal read
            make_mips_code_lw(ra_reg, make_m_op_arg_mem(-4)); // lw ra -4(fp)
            make_mips_code_move(sp_reg, fp_reg); // sp = fp
            make_mips_code_fp_add(current_func->stack_size); // fp = fp + size
            make_mips_code_move(dst_reg, v0_reg); // move reg1 v0
            MipsOperand *ret_mem = make_m_op_get_m(code->u.nonop.result, 4); // x
            make_mips_code_sw(dst_reg, ret_mem); // sw reg1 x
            return code->next;
        }
        case IR_WRITE: { // write x
            MipsOperand *dst_reg = make_m_op_new_temp(); // reg1
            MipsOperand *write_mips_op = make_m_op_func(write_op); // write
            make_mips_code_move(fp_reg, sp_reg); // fp = sp
            make_mips_code_sp_add(-4); // sp = sp - 4
            MipsOperand *x_reg = make_m_op_m2r(code->u.nonop.result); // lw reg1 x
            make_mips_code_move(a0_reg, x_reg); // move a0 x
            make_mips_code_sw(ra_reg, make_m_op_arg_mem(-4)); // sw ra -4(fp)
            make_mips_code_jal(write_mips_op); // jal write
            make_mips_code_lw(ra_reg, make_m_op_arg_mem(-4)); // lw ra -4(fp)
            make_mips_code_move(sp_reg, fp_reg); // sp = fp
            make_mips_code_fp_add(current_func->stack_size); // fp = fp + size
            return code->next;
        }
        case IR_ARG: { //因为在实现中arg是相连的，可以统一处理 arg x
            InterCode *cur_arg_code = code;
            int cur_offset = 0;
            make_mips_code_move(fp_reg, sp_reg); // fp = sp
            make_mips_code_sp_add(- next_func(code)->stack_size); // sp = sp - size
            make_mips_code_sw(ra_reg, make_m_op_arg_mem(-4)); // sw ra -4(fp)
            while (cur_arg_code != NULL && cur_arg_code->kind == IR_ARG) {
                MipsOperand *src_reg = make_m_op_m2r(code->u.nonop.result); // lw reg1 x
                MipsOperand *dst_mem = make_m_op_arg_mem(cur_offset); // offset(fp)
                make_mips_code_sw(src_reg, dst_mem); // sw reg1 offset(fp)
                cur_arg_code = cur_arg_code->next;
                cur_offset += 4; //参数不可能是数组、结构体
            }
            assert(cur_arg_code != NULL && cur_arg_code->kind == IR_CALL);
            return cur_arg_code;
        }
        case IR_PARAM: { //依次读取fp上方offset=4开始的参数 param x
            InterCode *cur_param_code = code;
            int cur_offset = 4 * (current_func->param_size - 1);
            while (cur_param_code != NULL && cur_param_code->kind == IR_PARAM) {
                make_m_op_set_param(cur_param_code->u.nonop.result, cur_offset);
                cur_param_code = cur_param_code->next;
                cur_offset -= 4;
            }
            assert(cur_offset == 0);
            return cur_param_code;
        }
        default: break;
    }
    assert(0);
    return NULL;
}

MipsOperand *make_m_op_new_temp() {
    // 返回一个暂用的可用寄存器
    MipsOperand *temp_op = malloc(sizeof(MipsOperand));
    temp_op->kind = M_OP_REG;
    temp_op->u.reg.id = get_temp_reg();
    return temp_op;
}

void make_m_op_set_param(Operand *op, int offset) {
    assert(op->kind == OP_VARIABLE);
    // Log("%d belongs to %s", op->u.var.no, op->u.var.parent_func->name);
    // assert(op->u.var.parent_func == NULL);
    op->u.var.parent_func = current_func;
    op->u.var.offset = offset;
}

MipsOperand *make_m_op_get_m(Operand *op, int size) {
    // 对于一个值，若它已经存储在栈中，则获取它的位置；若它尚未存储在栈中，则给它安置一个内存位置（置于所在函数的栈的top_offset）
    int offset = -1;
    assert(op != NULL);
    switch (op->kind) {
        case OP_TEMP: {
            if (op->u.temp.parent_func == NULL) {
                op->u.temp.parent_func = current_func;
                op->u.temp.offset = op->u.temp.parent_func->top_offset;
                op->u.temp.parent_func->top_offset -= size;
                offset = op->u.temp.offset;
            }
            else
                offset = op->u.temp.offset;
            break;
        }
        case OP_VARIABLE: {
            if (op->u.var.parent_func == NULL) {
                op->u.var.parent_func = current_func;
                op->u.var.offset = op->u.var.parent_func->top_offset;
                op->u.var.parent_func->top_offset -= size;
                offset = op->u.var.offset;
            }
            else
                offset = op->u.var.offset;
            break;
        }
        case OP_ADDRESS: {
            if (op->u.var.parent_func == NULL) {
                op->u.address.parent_func = current_func;
                op->u.address.offset = op->u.address.parent_func->top_offset;
                op->u.address.parent_func->top_offset -= size;
                offset = op->u.address.offset;
            }
            else
                offset = op->u.address.offset;
            break;
        }
        default: assert(0);
    }
    MipsOperand *new_op = malloc(sizeof(MipsOperand));
    new_op->kind = M_OP_STA;
    new_op->u.sta.offset = offset;
    new_op->u.sta.reg_id = REG_FP;
    return new_op;
}

MipsOperand *make_m_op_m2r(Operand *op) {
    switch (op->kind) {
        case OP_CONSTANT: {
            // 先把立即数存入一个寄存器，再返回那个寄存器
            MipsOperand *imm = malloc(sizeof(MipsOperand));
            imm->kind = M_OP_IMM;
            imm->u.imm.val = op->u.constant.val;
            MipsOperand *imm_reg = malloc(sizeof(MipsOperand));
            imm_reg->kind = M_OP_REG;
            imm_reg->u.reg.id = get_temp_reg();
            make_mips_code_li(imm_reg, imm);
            return imm_reg;
        }
        case OP_TEMP: {
            // 此处temp或var为右值，应当已存在于栈中。从栈lw到寄存器中
            assert(op->u.temp.parent_func != NULL && op->u.temp.parent_func == current_func);
            MipsOperand *tmp_mem = malloc(sizeof(MipsOperand));
            tmp_mem->kind = M_OP_STA;
            tmp_mem->u.sta.offset = op->u.temp.offset;
            tmp_mem->u.sta.reg_id = REG_FP;
            MipsOperand *tmp_reg = malloc(sizeof(MipsOperand));
            tmp_reg->kind = M_OP_REG;
            tmp_reg->u.reg.id = get_temp_reg();
            make_mips_code_lw(tmp_reg, tmp_mem);
            return tmp_reg;
        }
        case OP_VARIABLE: {
            assert(op->u.var.parent_func != NULL && op->u.var.parent_func == current_func);
            MipsOperand *var_mem = malloc(sizeof(MipsOperand));
            var_mem->kind = M_OP_STA;
            var_mem->u.sta.offset = op->u.var.offset;
            var_mem->u.sta.reg_id = REG_FP;
            MipsOperand *var_reg = malloc(sizeof(MipsOperand));
            var_reg->kind = M_OP_REG;
            var_reg->u.reg.id = get_temp_reg();
            make_mips_code_lw(var_reg, var_mem);
            return var_reg;
        }
        case OP_ADDRESS: {
            assert(op->u.address.parent_func != NULL && op->u.address.parent_func == current_func);
            MipsOperand *addr_mem = malloc(sizeof(MipsOperand));
            addr_mem->kind = M_OP_STA;
            addr_mem->u.sta.offset = op->u.address.offset;
            addr_mem->u.sta.reg_id = REG_FP;
            MipsOperand *addr_reg = malloc(sizeof(MipsOperand));
            addr_reg->kind = M_OP_REG;
            addr_reg->u.reg.id = get_temp_reg();
            make_mips_code_la(addr_reg, addr_mem);
            return addr_reg;
        }
        default: assert(0);
    }
    return NULL;
}

MipsOperand *make_m_op_arg_mem(int offset) {
    MipsOperand *op = malloc(sizeof(MipsOperand));
    op->kind = M_OP_STA;
    op->u.sta.offset = offset;
    op->u.sta.reg_id = REG_FP;
    return op;
}

MipsOperand *make_m_op_func(Operand *func_op) {
    assert(func_op->kind == OP_FUNC);
    MipsOperand *op = malloc(sizeof(MipsOperand));
    op->kind = M_OP_LABEL;
    op->u.label.is_func = 1;
    strncpy(op->u.label.name, func_op->u.func.func->name, strlen(func_op->u.func.func->name));
    // op->u.label.name[strlen(op->u.label.name)] = '\0';
    return op;
}

MipsOperand *make_m_op_label(Operand *label_op) {
    assert(label_op->kind == OP_LABEL);
    MipsOperand *op = malloc(sizeof(MipsOperand));
    op->kind = M_OP_LABEL;
    op->u.label.is_func = 0;
    op->u.label.label_id = label_op->u.label.no;
    return op;
}

void make_mips_code_la(MipsOperand *addr_reg, MipsOperand *addr_mem) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_LA;
    code->u.sinop.op1 = addr_reg;
    code->u.sinop.op2 = addr_mem;
    insert_mips_code(code);
}

void make_mips_code_sp_add(int size) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_ADDI;
    code->u.binop.op1 = sp_reg;
    code->u.binop.op2 = sp_reg;
    MipsOperand *imm = malloc(sizeof(MipsOperand));
    imm->kind = M_OP_IMM;
    imm->u.imm.val = size;
    code->u.binop.op3 = imm;
    insert_mips_code(code);
}

void make_mips_code_fp_add(int size) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_ADDI;
    code->u.binop.op1 = fp_reg;
    code->u.binop.op2 = fp_reg;
    MipsOperand *imm = malloc(sizeof(MipsOperand));
    imm->kind = M_OP_IMM;
    imm->u.imm.val = size;
    code->u.binop.op3 = imm;
    insert_mips_code(code);
}

void make_mips_code_jr(MipsOperand *ra_op) {
    assert(ra_op != NULL && ra_op->u.reg.id == REG_RA);
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_JR;
    code->u.nonop.op1 = ra_op;
    insert_mips_code(code);
}

void make_mips_code_jal(MipsOperand *label_op) {
    assert(label_op != NULL && label_op->kind == M_OP_LABEL);
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_JAL;
    code->u.nonop.op1 = label_op;
    insert_mips_code(code);
}

void make_mips_code_move(MipsOperand *dst_op, MipsOperand *op) {
    assert(dst_op != NULL && dst_op->kind == M_OP_REG && op != NULL && op->kind == M_OP_REG);
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_MOVE;
    code->u.sinop.op1 = dst_op;
    code->u.sinop.op2 = op;
    insert_mips_code(code);
}

void make_mips_code_arith(MipsOperand *dst_op, MipsOperand *op1, MipsOperand *op2, int type) {
    assert(dst_op != NULL && dst_op->kind == M_OP_REG && op1 != NULL && op1->kind == M_OP_REG && op2 != NULL && op2->kind == M_OP_REG);
    MipsCode *code = malloc(sizeof(MipsCode));
    switch (type) {
        case IR_ADD: code->kind = MIPS_ADD; break;
        case IR_SUB: code->kind = MIPS_SUB; break;
        case IR_MUL: code->kind = MIPS_MUL; break;
        case IR_DIV: code->kind = MIPS_DIV; break;
        default: assert(0);
    }
    if (code->kind != MIPS_DIV) {
        code->u.binop.op1 = dst_op;
        code->u.binop.op2 = op1;
        code->u.binop.op3 = op2;
        reg[dst_op->u.reg.id] = 1;
        insert_mips_code(code);
    }
    else {
        code->u.sinop.op1 = op1;
        code->u.sinop.op2 = op2;
        insert_mips_code(code);
        MipsCode *code2 = malloc(sizeof(MipsCode));
        code2->kind = MIPS_MFLO;
        code2->u.nonop.op1 = dst_op;
        insert_mips_code(code2);
    }
}

void make_mips_code_if(MipsOperand *op1, int relop_kind, MipsOperand *op2, MipsOperand *label_op) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->u.binop.op1 = op1;
    code->u.binop.op2 = op2;
    code->u.binop.op3 = label_op;
    switch (relop_kind) {
        case RELOP_E: code->kind = MIPS_BEQ; break;
        case RELOP_NE: code->kind = MIPS_BNE; break;
        case RELOP_G: code->kind = MIPS_BGT; break;
        case RELOP_GE: code->kind = MIPS_BGE; break;
        case RELOP_LE: code->kind = MIPS_BLE; break;
        case RELOP_L: code->kind = MIPS_BLT; break;
        default: assert(0);
    }
    insert_mips_code(code);
}

void make_mips_code_sw(MipsOperand *var_reg, MipsOperand *var_mem) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_SW;
    code->u.sinop.op1 = var_reg;
    code->u.sinop.op2 = var_mem;
    insert_mips_code(code);
}

void make_mips_code_lw(MipsOperand *var_reg, MipsOperand *var_mem) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_LW;
    code->u.sinop.op1 = var_reg;
    code->u.sinop.op2 = var_mem;
    reg[var_reg->u.reg.id] = 1;
    insert_mips_code(code);
}

void make_mips_code_li(MipsOperand *imm_reg, MipsOperand *imm) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_LI;
    code->u.sinop.op1 = imm_reg;
    code->u.sinop.op2 = imm;
    reg[imm_reg->u.reg.id] = 1;
    insert_mips_code(code);
}

void make_mips_code_func(MipsOperand *func_op) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_LABEL;
    code->u.nonop.op1 = func_op;
    insert_mips_code(code);
}

void make_mips_code_label(MipsOperand *label_op) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_LABEL;
    code->u.nonop.op1 = label_op;
    insert_mips_code(code);
}

void make_mips_code_goto(MipsOperand *label_op) {
    MipsCode *code = malloc(sizeof(MipsCode));
    code->kind = MIPS_J;
    code->u.nonop.op1 = label_op;
    insert_mips_code(code);
}

void mips_to_file(FILE *fp) {
    MipsCode *cur = mips_head;
    pre_load_head(fp);
    while (cur != NULL) {
        fprintf(fp, "%s\n", show_mips(cur));
        cur = cur->next;
    }
}

void pre_load_head(FILE *fp) {
    fprintf(fp, ".data\n_prompt: .asciiz \"Enter an integer:\"\n_ret: .asciiz \"\\n\"\n"
        ".globl main\n.text\nread:\n  li $v0, 4\n  la $a0, _prompt\n  syscall\n  li $v0, 5\n"
          "  syscall\n  jr $ra\n\nwrite:\n  li $v0, 1\n  syscall\n  li $v0, 4\n  la $a0, _ret\n"
            "  syscall\n  move $v0, $0\n  jr $ra\n");
}

char *show_mips(MipsCode *code) {
    assert(code != NULL);
    char *buffer = malloc(50 * sizeof(char));
    switch (code->kind) {
        case MIPS_ADD: sprintf(buffer, "  add %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_ADDI: sprintf(buffer, "  addi %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_SUB: sprintf(buffer, "  sub %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_MUL: sprintf(buffer, "  mul %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_DIV: sprintf(buffer, "  div %s, %s", show_m_op(code->u.sinop.op1), show_m_op(code->u.sinop.op2)); break;
        case MIPS_MFLO: sprintf(buffer, "  mflo %s", show_m_op(code->u.nonop.op1)); break;
        case MIPS_BEQ: sprintf(buffer, "  beq %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_BGE: sprintf(buffer, "  bge %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_BGT: sprintf(buffer, "  bgt %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_BLE: sprintf(buffer, "  ble %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_BLT: sprintf(buffer, "  blt %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_BNE: sprintf(buffer, "  bne %s, %s, %s", show_m_op(code->u.binop.op1), show_m_op(code->u.binop.op2), show_m_op(code->u.binop.op3)); break;
        case MIPS_J: sprintf(buffer, "  j %s", show_m_op(code->u.nonop.op1)); break;
        case MIPS_JAL: sprintf(buffer, "  jal %s", show_m_op(code->u.nonop.op1)); break;
        case MIPS_JR: sprintf(buffer, "  jr %s", show_m_op(code->u.nonop.op1)); break;
        case MIPS_LA: sprintf(buffer, "  la %s, %s", show_m_op(code->u.sinop.op1), show_m_op(code->u.sinop.op2)); break;
        case MIPS_LI: sprintf(buffer, "  li %s, %s", show_m_op(code->u.sinop.op1), show_m_op(code->u.sinop.op2)); break;
        case MIPS_LW: sprintf(buffer, "  lw %s, %s", show_m_op(code->u.sinop.op1), show_m_op(code->u.sinop.op2)); break;
        case MIPS_MOVE: sprintf(buffer, "  move %s, %s", show_m_op(code->u.sinop.op1), show_m_op(code->u.sinop.op2)); break;
        case MIPS_SW: sprintf(buffer, "  sw %s, %s", show_m_op(code->u.sinop.op1), show_m_op(code->u.sinop.op2)); break;
        case MIPS_LABEL: 
            if (code->u.nonop.op1->u.label.is_func == 1)
                sprintf(buffer, "\n%s:", show_m_op(code->u.nonop.op1));
            else
                sprintf(buffer, "%s:", show_m_op(code->u.nonop.op1));
            break;
        default: assert(0);
    }
    return buffer;
}

char *show_m_op(MipsOperand *op) {
    assert(op != NULL);
    char *buffer = malloc(50 * sizeof(char));
    switch (op->kind) {
        case M_OP_IMM: sprintf(buffer, "%d", op->u.imm.val); break;
        case M_OP_LABEL:
            if (op->u.label.is_func == 1) {
                sprintf(buffer, "%s", op->u.label.name);
            }
            else {
                sprintf(buffer, "label%d", op->u.label.label_id);
            }
            break;
        case M_OP_REG: sprintf(buffer, "%s", show_m_op_reg(op->u.reg.id)); break;
        case M_OP_STA: sprintf(buffer, "%d(%s)", op->u.sta.offset, show_m_op_reg(op->u.sta.reg_id)); break;
        default: assert(0);
    }
    return buffer;
}

char *show_m_op_reg(int reg_id) {
    char *buffer = malloc(10 * sizeof(char));
    if (reg_id == REG_FP) sprintf(buffer, "$s8");
    else if (reg_id == REG_SP) sprintf(buffer, "$sp");
    else if (reg_id == REG_RA) sprintf(buffer, "$ra");
    else if (reg_id == REG_GP) sprintf(buffer, "$gp");
    else if (reg_id >= 2 && reg_id <= 3) sprintf(buffer, "$v%d", reg_id - 2);
    else if (reg_id >= 4 && reg_id <= 7) sprintf(buffer, "$a%d", reg_id - 4);
    else if (reg_id >= 8 && reg_id <= 15) sprintf(buffer, "$t%d", reg_id - 8);
    else if (reg_id >= 16 && reg_id <= 23) sprintf(buffer, "$s%d", reg_id - 16);
    else if (reg_id >= 24 && reg_id <= 25) sprintf(buffer, "$t%d", reg_id - 16);
    else assert(0);
    return buffer;
}

int get_temp_reg() {
    int i;
    for (i = 0; i <= 9; i ++) {
        if (reg_t(i) == 0) {
            if (i <= 7) return i + 8;
            else return i + 16;
        }
    }
    assert(0);
    return -1;
}

int is_main_func(MipsOperand *func_op) {
    assert(func_op != NULL && func_op->kind == M_OP_LABEL && func_op->u.label.is_func == 1);
    if (strcmp(func_op->u.label.name, "main") == 0)
        return 1;
    return 0;
}

Func *next_func(InterCode *code) {
    InterCode *cur = code;
    while (cur != NULL && cur->kind == IR_ARG) {
        cur = cur->next;
    }
    assert(cur != NULL && cur->kind == IR_CALL);
    assert(cur->u.sinop.op->kind == OP_FUNC);
    return cur->u.sinop.op->u.func.func;
}