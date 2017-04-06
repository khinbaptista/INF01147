#include <stdio.h>
#include "hash.h"

//int yylex();

int main() {
	hash_init();

	hash_insert(0, "abacate");
	hash_insert(2, "cavalo");

	hash_print();

	printf("\n");
	return 0;
}
