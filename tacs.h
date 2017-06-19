#pragma once

#include "hash.h"
#include "astree.h"

enum tac_types {
	TAC_UNKNOWN = 0,
	TAC_LABLE,
	TAC_SYMBOL,
	// ...

	TAC_NOT,
	TAC_AND,
	TAC_OR,
	// ...

	TAC_ADD,
	TAC_SUB,
	TAC_MULT,
	TAC_DIV,
	// ...

	TAC_IFZ,
	TAC_MOV
	// ...
};

typedef struct tac {
	int type;
	HashNode *res;
	HashNode *op1;
	HashNode *op2;

	struct tac *prev;
	struct tac *next;
} TAC;

TAC* tac_generate(struct astree*);

TAC* tac_create(int type, HashNode*, HashNode*, HashNode*);
TAC* tac_create_op(int type, HashNode *op1, HashNode *op2);

TAC* tac_join(TAC*, TAC*);
TAC* tac_reverse(TAC*);

void tac_print_back(TAC* last);
void tac_print_forward(TAC* first);
