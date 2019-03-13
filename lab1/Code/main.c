#include "AST.h"

extern FILE *in;
extern AST_Node *root;
extern int error_flag;

int main(int argc, char **argv) {
    if (argc > 1){
	      if (!(in = fopen(argv[1], "r"))){
	          perror(argv[1]);
	          return 1;
	      }
	      yyrestart(in);
    }
    root = malloc(sizeof(AST_Node));
	  yyparse();
    if (error_flag == 0){
        print_AST(root, 0);
    }
	  return 0;
}