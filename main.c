/****************************************
	INF01147 - Compiladores
	Khin Baptista
	Lorenzo Dal'Aqua
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "astree.h"
#include "lang171.h"
#include "semantics.h"

extern FILE* yyin;
void yyparse(void);

int main(int argc, char* argv[]) {
	initMe();

	if (argc < 2) {
		fprintf(stderr, "Usage:\n\t%s <input_file> [<output_file>]\n\n", argv[0]);
		exit(1);
	}

	FILE *input = fopen(argv[1], "r");
	if (input == NULL) {
		fprintf(stderr, "Could not open input file '%s'\n", argv[1]);
		exit(2);
	}

	yyin = input;

	yyparse();
	fclose(input);
	printf("\n==== No syntax errors found. ====\n");
	semantics_check(astree_root);
	printf("\nFinished semantic check.\n");

	if(!semantic_errors_found()) {
		printf("\n==== Parsing and semantic checks sucessful. ====\n\n");
	}
	else {
		printf("\n==== Semantic errors. Exiting... ====\n\n");
		exit(4);
	}

	if (argc >= 3) {
		FILE *output = fopen(argv[2], "w");
		if (output == NULL) {
			fprintf(stderr, "Could not open output file '%s'", argv[2]);
			exit(5);
		}

		astree_write_code(output, astree_root);
		fclose(output);

		fprintf(stderr, "Output written to '%s'\n", argv[2]);
	}

	exit(0);
}
