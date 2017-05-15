#include "astree.h"
#include <stdlib.h>
#include <stdio.h>

ASTree* astree_create(
	int type,
	ASTree* child1,
	ASTree* child2,
	ASTree* child3,
	ASTree* child4,
	HashNode* symbol
) {
	ASTree* node = calloc(1, sizeof(ASTree));
	node->type = type;
	node->children[0] = child1;
	node->children[1] = child2;
	node->children[2] = child3;
	node->children[3] = child4;
	node->symbol = symbol;

	return node;
}

void astree_print(ASTree* node, int level) {
	int _level = level;
	if (_level > 1) {
		fprintf(stderr, "|");
		_level--;
	}

	for (; _level > 0; _level--) {
		fprintf(stderr, "--");
	}

	fprintf(stderr, "ASTree - Type %d\n", node->type);

	if (node->children[0]) astree_print(node->children[0], level - 1);
	if (node->children[1]) astree_print(node->children[1], level - 1);
	if (node->children[2]) astree_print(node->children[2], level - 1);
	if (node->children[3]) astree_print(node->children[3], level - 1);
}

// Assumes 'file' is a valid handle to an open file
void astree_write_code(FILE* file, ASTree* node) {
	switch (node->type) {
		case AST_PROGRAM:
			assert(node->children[0]);
			astree_write_code(file, node->children[0]);
			if (node->children[1]) { astree_write_code(file, node->children[1]); }
			break;

		case AST_LITERAL:
			assert(node->symbol);
			fprintf(file, "%s", file->symbol);
			break;

		case AST_IDENTIFIER:
			assert(node->symbol);
			fprintf(file, "%s", file->symbol);
			break;

		// Types
		case AST_TYPE_BYTE:
			fprintf(file, "byte"); break;
		case AST_TYPE_SHORT:
			fprintf(file, "short"); break;
		case AST_TYPE_LONG:
			fprintf(file, "long"); break;
		case AST_TYPE_FLOAT:
			fprintf(file, "float"); break;
		case AST_TYPE_DOUBLE:
			fprintf(file, "double"); break;

		// Variables
		case AST_VAR_DECL:
			assert(node->children[0] && node->children[1] && node->children[2]);
			astree_write_code(file, node->children[0]); fprintf(file, " : ");
			astree_write_code(file, node->children[1]); fprintf(file, " ");
			astree_write_code(file, node->children[2]); fprintf(file, ";\n");
			break;
		case AST_ARRAY_DECL:
			assert(
				node->children[0] && node->children[1] &&
				node->children[2] && node->children[3]
			);
			astree_write_code(file, node->children[0]); fprintf(file, " : ");
			astree_write_code(file, node->children[1]); fprintf(file, "[");
			astree_write_code(file, node->children[2]); fprintf(file, "] ");
			astree_write_code(file, node->children[3]); fprintf(file, ";\n");
			break;
		case AST_ARRAY_INIT:
			if (node->children[0]) { astree_write_code(file, node->children[0]); }
			if (node->children[1]) {
				fprintf(file, " "); astree_write_code(file, node->children[1]);
			}
			break;

		// Functions
		case AST_FUNC_DECL:
			
			break;
		case AST_FUNC_PARAMS_LIST:
			break;
		case AST_FUNC_PARAM:
			break;
		case AST_FUNC_CALL:
			break;
		case AST_FUNC_ARGS_LIST:
			break;

		// Commands

		//case AST_CMD: break;

		case AST_CMD_LIST:
			break;
		case AST_CMD_BLOCK:
			break;
		case AST_CMD_VAR_ATTR:
			break;
		case AST_CMD_ARRAY_ATTR:
			break;
		case AST_CMD_READ:
			break;
		case AST_CMD_PRINT:
			break;
		case AST_CMD_RETURN:
			break;
		case AST_PRINT_ARGS:
			break;
		case AST_CMD_WHEN:
			break;
		case AST_CMD_WHILE:
			break;
		case AST_CMD_FOR:
			break;

		// Reconstruction helpers
		case AST_EXPR_PARENS:
			break;

		// Operators
		case AST_EXPR_SUM:
			break;
		case AST_EXPR_SUB:
			break;
		case AST_EXPR_MULT:
			break;
		case AST_EXPR_DIV:
			break;
		case AST_EXPR_LESSER:
			break;
		case AST_EXPR_GREATER:
			break;
		case AST_EXPR_LESSER_EQ:
			break;
		case AST_EXPR_GREATER_EQ:
			break;
		case AST_EXPR_EQUAL:
			break;
		case AST_EXPR_NOT_EQUAL:
			break;
		case AST_EXPR_OR:
			break;
		case AST_EXPR_AND:
			break;
		case AST_EXPR_NOT:
			break;
		case AST_EXPR_NEGATIVE:
			break;
		case AST_EXPR_ARRAY_ACCES:
			break;

		default: break;
	}
}
