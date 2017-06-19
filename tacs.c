#include "tacs.h"

#include <stdio.h>
#include <stdlib.h>

TAC* tac_generate(ASTree *node) {
	TAC *code[MAX_CHILDREN];
	TAC *result = NULL;

	if (!node) return NULL;

	for (int i = 0; i < MAX_CHILDREN; i++) {
		code[i] = tac_generate(node->children[i])
	}

	switch (node->type) {
		case ASTREE_IDENTIFIER:
			result = tac_create(TAC_SYMBOL, node->symbol, NULL, NULL);
			break;
		case: ASTREE_EXPR_SUM:
			result = tac_create_op(TAC_ADD, node->children[0], node->children[1]);
			break;
		default:
			result = tac_join(tac_join(tac_join(code[0], code[1]), code[2]), code[3]);
			break;
	}
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
