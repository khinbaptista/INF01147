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

extern FILE* yyin;
void yyparse(void);

int main(int argc, char* argv[]) {
	initMe();

	if (argc < 2) {
		fprintf(stderr, "Usage:\n\t%s <input_file>\n\n", argv[0]);
		exit(1);
	}

	FILE *input = fopen(argv[1], "r");
	if (input == NULL) {
		fprintf(stderr, "Could not open file '%s'\n", argv[1]);
		exit(2);
	}

	yyin = input;

	yyparse();
	printf("\n==== No syntax errors found. ====\n");
	printf("Hash table contents: \n");
	hash_print();
	fclose(input);
	exit(0);
}
