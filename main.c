/****************************************
	INF01147 - Compiladores
	Khin Baptista
	Lorenzo Dal'Aqua
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "etapa1.h"

extern FILE* yyin;

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

	hash_print();
	fclose(input);
	exit(0);
}
