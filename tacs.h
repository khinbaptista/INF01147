#pragma once

#include "hash.h"
#include "astree.h"

enum tac_types {
	TAC_UNKNOWN = 0,
	TAC_LABEL,
	TAC_SYMBOL,

	/* Arrays */
	TAC_ARRAY_POS,		// helper for array attribution
	TAC_ARRAY_ACCESS,

	/* TAC only ops */
	TAC_INC,
	TAC_MOV,
	TAC_MOV_OFFSET,

	/* Binary Ops */
	TAC_ADD,
	TAC_SUB,
	TAC_MULT,
	TAC_DIV,
	TAC_LESSER,
	TAC_GREATER,
	TAC_LESSER_EQ,
	TAC_GREATER_EQ,
	TAC_EQUAL,
	TAC_NOT_EQUAL,
	TAC_OR,
	TAC_AND,

	/* Unary Ops */
	TAC_NOT,
	TAC_NEGATIVE,

	/* Commands */
	TAC_IFZ,
	TAC_JMP,
	TAC_FUNC_BEGIN,
	TAC_FUNC_END,
	TAC_CALL,
	TAC_ARG,
	TAC_RET,
	TAC_PRINT,
	TAC_READ,
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

TAC* tac_join(TAC*, TAC*);
TAC* tac_reverse(TAC*);

void tac_print_back(TAC* last);
void tac_print_forward(TAC* first);
