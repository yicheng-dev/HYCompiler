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