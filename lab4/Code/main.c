#include "AST.h"
#include "semantics.h"
#include "ir.h"
#include "mips.h"

extern FILE *in;
FILE *out;
FILE *out2;
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
	if (!(out = fopen(argv[2], "w"))) {
		perror(argv[2]);
		return 1;
	}
	if (!(out2 = fopen(argv[3], "w"))) {
		perror(argv[3]);
		return 1;
	}
    root = malloc(sizeof(AST_Node));
	yyparse();
    if (error_flag == 0){
        // print_AST(root, 0);
		// semantics_analysis(root);
		if (error_flag)
			print_error_list();
		else {
			ir_generate(root);
			ir_to_file(out);
			mips_generate();
			mips_to_file(out2);
		}
    }
	fclose(out);
	fclose(out2);
	return 0;
}