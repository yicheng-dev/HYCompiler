#include "utils.h"

extern int yycolumn;

void update_column(char *text){
    int len = strlen(text);
    int chline_flag = 0;
    int i, j;
    for (i = len - 1; i >= 0; i --){
        if (text[i] == '\n'){
            chline_flag = 1;
            break;
        }
    }
    if (chline_flag == 1){
        yycolumn = 1;
        for (j = i; j < len; j++){
            yycolumn++;
        }
    }
}

unsigned hash_pjw(char *name){
    unsigned val = 0, i;
    while(*name){
        val = (val << 2) + *name;
        int i = val & ~0x3ff;
        if (i)
            val = (val ^ (i >> 12)) & 0x3fff;
        name ++;
    }
    return val;
}