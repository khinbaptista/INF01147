#include "tacs.h"

#include <stdio.h>
#include <stdlib.h>

TAC* tac_create_op(int type, HashNode *op1, HashNode *op2);
TAC* tac_create_when(TAC* condition, TAC* cmd);
TAC* tac_create_when_else(TAC* condition, TAC* when_cmd, TAC* else_cmd);
TAC* tac_create_while(TAC* condition, TAC* cmd);
TAC* tac_create_for(TAC* var, TAC* start, TAC* end, TAC* command);

TAC* tac_generate(ASTree *node) {
	if (!node)  {
		return NULL;
	}

	TAC *children[MAX_CHILDREN];
	for (int i = 0; i < MAX_CHILDREN; i++) {
		children[i] = tac_generate(node->children[i]);
	}

	switch (node->type) {
		case AST_LITERAL:
			return tac_create(TAC_SYMBOL, node->symbol, NULL, NULL);
		case AST_IDENTIFIER:
			return tac_create(TAC_SYMBOL, node->symbol, NULL, NULL);
		/* Assignments */
		case AST_CMD_VAR_ATTR:
			return tac_create(TAC_MOV, children[0]->res, children[1]->res, NULL);
		case AST_CMD_ARRAY_ATTR:
			// !!!INCOMPLETE!!!!
			// Children: var, index, value, how to use index?
			return tac_create(TAC_MOV, children[0]->res, children[1]->res, NULL);

		/* Control flow */
		case AST_CMD_WHEN:
			return tac_create_when(children[0], children[1]);
		case AST_CMD_WHEN_ELSE:
			return tac_create_when_else(children[0], children[1], children[2]);
		case AST_CMD_WHILE:
			return tac_create_while(children[0], children[1]);
		case AST_CMD_FOR:
			return tac_create_for(children[0], children[1], children[2], children[3]);

		/* Binary operations */
		case AST_EXPR_SUM:
			return tac_create_op(TAC_ADD, children[0]->res, children[1]->res);
		case AST_EXPR_SUB:
			return tac_create_op(TAC_SUB, children[0]->res, children[1]->res);
		case AST_EXPR_MULT:
			return tac_create_op(TAC_MULT, children[0]->res, children[1]->res);
		case AST_EXPR_DIV:
			return tac_create_op(TAC_DIV, children[0]->res, children[1]->res);
		case AST_EXPR_LESSER:
			return tac_create_op(TAC_LESSER,children[0]->res, children[1]->res);
		case AST_EXPR_GREATER:
			return tac_create_op(TAC_GREATER,children[0]->res, children[1]->res);
		case AST_EXPR_LESSER_EQ:
			return tac_create_op(TAC_LESSER_EQ,children[0]->res, children[1]->res);
		case AST_EXPR_GREATER_EQ:
			return tac_create_op(TAC_GREATER_EQ,children[0]->res, children[1]->res);
		case AST_EXPR_EQUAL:
			return tac_create_op(TAC_EQUAL,children[0]->res, children[1]->res);
		case AST_EXPR_NOT_EQUAL:
			return tac_create_op(TAC_NOT_EQUAL,children[0]->res, children[1]->res);
		case AST_EXPR_OR:
			return tac_create_op(TAC_OR,children[0]->res, children[1]->res);
		case AST_EXPR_AND:
			return tac_create_op(TAC_AND,children[0]->res, children[1]->res);

		/* Unary operations */
		case AST_EXPR_NOT:
			return tac_create(TAC_NOT, hash_make_temp(), children[0]->res, NULL);
		case AST_EXPR_NEGATIVE:
			return tac_create(TAC_NEGATIVE, hash_make_temp(), children[0]->res, NULL);

		default:
			return tac_join(tac_join(tac_join(
				children[0], children[1]), children[2]), children[3]);
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

TAC* tac_create_when(TAC* condition, TAC* cmd) {
	HashNode* hash_label = hash_make_label();
	TAC* jump = tac_create(TAC_IFZ, hash_label, condition->res, NULL);
	TAC* label = tac_create(TAC_LABEL, hash_label, NULL, NULL);
	return tac_join(tac_join(tac_join(
		condition,
		jump),
		cmd),
		label);
}
TAC* tac_create_when_else(TAC* condition, TAC* when_cmd, TAC* else_cmd) {
	HashNode* hash_label_else = hash_make_label();
	HashNode* hash_label_end = hash_make_label();
	TAC* jump_to_else = tac_create(TAC_IFZ, hash_label_else, condition->res, NULL);
	TAC* label_else = tac_create(TAC_LABEL, hash_label_else, NULL, NULL);
	TAC* jump_to_end = tac_create(TAC_JMP, hash_label_end, NULL, NULL);
	TAC* label_end = tac_create(TAC_LABEL, hash_label_end, NULL, NULL);
	return tac_join(tac_join(tac_join(tac_join(tac_join(tac_join(
		condition,
		jump_to_else),
		when_cmd),
		jump_to_end),
		label_else),
		else_cmd),
		label_end);
}
TAC* tac_create_while(TAC* condition, TAC* cmd) {
	HashNode* hash_label_start = hash_make_label();
	HashNode* hash_label_skip = hash_make_label();
	TAC* jump_start = tac_create(TAC_JMP, hash_label_start, NULL, NULL);
	TAC* label_start = tac_create(TAC_LABEL, hash_label_start, NULL, NULL);
	TAC* jump_skip = tac_create(TAC_IFZ, hash_label_skip, condition->res, NULL);
	TAC* label_skip = tac_create(TAC_LABEL, hash_label_skip, NULL, NULL);
	return tac_join(tac_join(tac_join(tac_join(tac_join(
		label_start,
		condition),
		jump_skip),
		cmd),
		jump_start),
		label_skip);
}
TAC* tac_create_for(TAC* var, TAC* start, TAC* end, TAC* command) {
	TAC* init_var = tac_create(TAC_MOV, var->res, start->res, NULL);
	HashNode* hash_label_start = hash_make_label();
	TAC* label_start = tac_create(TAC_LABEL, hash_label_start, NULL, NULL);
	HashNode* hash_label_end = hash_make_label();
	TAC* label_end = tac_create(TAC_LABEL, hash_label_end, NULL, NULL);
	TAC* test_var = tac_create_op(TAC_LESSER_EQ, var->res, end->res);
	TAC* jump_to_end = tac_create(TAC_IFZ, hash_label_end, test_var->res, NULL);
	TAC* increment_var = tac_create(TAC_INC, var->res, NULL, NULL);
	TAC* jump_to_test = tac_create(TAC_JMP, hash_label_start, NULL, NULL);
	return tac_join(tac_join(tac_join(tac_join(tac_join(tac_join(tac_join(
		init_var,
		label_start),
		test_var),
		jump_to_end),
		command),
		increment_var),
		jump_to_test),
		label_end);
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
		case TAC_SYMBOL:		fprintf(stderr, "SYMBOL");		break;
		case TAC_LABEL:			fprintf(stderr, "LABEL");		break;
		case TAC_MOV:			fprintf(stderr, "MOV");			break;
		case TAC_INC:			fprintf(stderr, "INC");			break;
		case TAC_ADD:			fprintf(stderr, "ADD");			break;
		case TAC_SUB:			fprintf(stderr, "SUB");			break;
		case TAC_MULT:			fprintf(stderr, "MULT");		break;
		case TAC_DIV:			fprintf(stderr, "DIV");			break;
		case TAC_LESSER:		fprintf(stderr,"LESSER"); 		break;
		case TAC_GREATER:		fprintf(stderr,"GREATER"); 		break;
		case TAC_LESSER_EQ:		fprintf(stderr,"LESSER_EQ"); 	break;
		case TAC_GREATER_EQ:	fprintf(stderr,"GREATER_EQ"); 	break;
		case TAC_EQUAL:			fprintf(stderr,"EQUAL"); 		break;
		case TAC_NOT_EQUAL:		fprintf(stderr,"NOT_EQUAL"); 	break;
		case TAC_OR:			fprintf(stderr,"OR"); 			break;
		case TAC_AND:			fprintf(stderr,"AND"); 			break;
		case TAC_NOT:			fprintf(stderr,"NOT"); 			break;
		case TAC_NEGATIVE:		fprintf(stderr,"NEGATIVE"); 	break;
		case TAC_IFZ:			fprintf(stderr,"IFZ");			break;
		case TAC_JMP:			fprintf(stderr,"JMP");			break;
		default:				fprintf(stderr, "UNKNOWN");		break;
	}

	if (tac->res)	fprintf(stderr, ", %s", tac->res->text);
	else			fprintf(stderr, ", NULL");
	if (tac->op1)	fprintf(stderr, ", %s", tac->op1->text);
	else			fprintf(stderr, ", NULL");
	if (tac->op2)	fprintf(stderr, ", %s", tac->op2->text);
	else			fprintf(stderr, ", NULL");

	fprintf(stderr, ")\n");
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
