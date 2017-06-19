#include "tacs.h"

#include <stdio.h>
#include <stdlib.h>

TAC* tac_generate(ASTree *node) {
	TAC *code[MAX_CHILDREN];
	TAC *result = NULL;

	if (!node) return NULL;

	for (int i = 0; i < MAX_CHILDREN; i++) {
		code[i] = tac_generate(node->children[i]);
	}

	switch (node->type) {
		case AST_IDENTIFIER:
			result = tac_create(TAC_SYMBOL, node->symbol, NULL, NULL);
			break;
		case AST_EXPR_SUM:
			result = tac_create_op(TAC_ADD,
				node->children[0]->symbol,
				node->children[1]->symbol
			);
			break;
		default:
			result = tac_join(tac_join(tac_join(code[0], code[1]), code[2]), code[3]);
			break;
	}

	return result;
}

TAC* tac_create(int type, HashNode *res, HashNode *op1, HashNode *op2) {
	TAC *tac	= calloc(1, sizeof(TAC));
	tac->type	= type;
	tac->res	= res;
	tac->op1	= op1;
	tac->op2	= op2;
	tac->prev	= NULL;
	tac->next	= NULL;
	return tac;
}

TAC* tac_create_op(int type, HashNode *op1, HashNode *op2) {
	return tac_create(type, hash_make_temp(), op1, op2);
}

TAC* tac_join(TAC *tac1, TAC *tac2) {
	TAC *tac;

	if (!tac1) return tac2;
	if (!tac2) return tac1;

	tac = tac2;
	while (tac->prev) { tac = tac->prev; }
	tac->prev = tac1;

	return tac2;
}

TAC* tac_reverse(TAC* tac) {
	TAC *t = NULL;

	for (t = tac; t->prev; t = t->prev)
		t->prev->next = t;

	return t;
}

void _tac_print(TAC *tac) {
	fprintf(stderr, "TAC(");

	switch (tac->type) {
		case TAC_ADD:	fprintf(stderr, "ADD");		break;
		case TAC_SUB:	fprintf(stderr, "SUB");		break;
		case TAC_MULT:	fprintf(stderr, "MULT");	break;
		case TAC_DIV:	fprintf(stderr, "DIV");		break;
		default:		fprintf(stderr, "UNKNOWN");	break;
	}

	if (tac->res)	fprintf(stderr, ", %s", tac->res->text);
	else			fprintf(stderr, ", 0");
	if (tac->op1)	fprintf(stderr, ", %s", tac->op1->text);
	else			fprintf(stderr, ", 0");
	if (tac->op2)	fprintf(stderr, ", %s", tac->op2->text);
	else			fprintf(stderr, ", 0");

	fprintf(stderr, ")");
}

void tac_print_back(TAC *last) {
	TAC *tac = NULL;
	for (tac = last; tac; tac = tac->prev) {
		_tac_print(tac);
	}
}

void tac_print_forward(TAC *first) {
	TAC *tac = NULL;
	for (tac = first; tac; tac = tac->next) {
		_tac_print(tac);
	}
}
