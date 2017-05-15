#pragma once

#include "hash.h"

#define MAX_CHILDREN 4

enum tree_types {	// types
	AST_LITERAL,
	AST_IDENTIFIER,
	AST_TYPE_BYTE,
	AST_TYPE_SHORT,
	AST_TYPE_LONG,
	AST_TYPE_FLOAT,
	AST_TYPE_DOUBLE,
	AST_PROGRAM,
	AST_CMD,	// never used?
	AST_VAR_DECL,
	AST_ARRAY_DECL,
	AST_ARRAY_INIT,
	AST_FUNC_DECL,
	AST_FUNC_PARAMS_LIST,
	AST_FUNC_PARAM,
	AST_FUNC_CALL,
	AST_FUNC_ARGS_LIST,
	AST_CMD_LIST,
	AST_CMD_BLOCK,
	AST_CMD_VAR_ATTR,
	AST_CMD_ARRAY_ATTR,
	AST_CMD_READ,
	AST_CMD_PRINT,
	AST_CMD_RETURN,
	AST_PRINT_ARGS,
	AST_CMD_WHEN,
	AST_CMD_WHILE,
	AST_CMD_FOR,
	AST_EXPR_PARENS,
	AST_EXPR_SUM,
	AST_EXPR_SUB,
	AST_EXPR_MULT,
	AST_EXPR_DIV,
	AST_EXPR_LESSER,
	AST_EXPR_GREATER,
	AST_EXPR_LESSER_EQ,
	AST_EXPR_GREATER_EQ,
	AST_EXPR_EQUAL,
	AST_EXPR_NOT_EQUAL,
	AST_EXPR_OR,
	AST_EXPR_AND,
	AST_EXPR_NOT,
	AST_EXPR_NEGATIVE,
	AST_EXPR_ARRAY_ACCESS

};

typedef struct astree {
	int type;
	struct astree* children[ MAX_CHILDREN ];
	HashNode* symbol;
} ASTree;

ASTree* astree_create(
	int type,
	ASTree* child1,
	ASTree* child2,
	ASTree* child3,
	ASTree* child4,
	HashNode* symbol
);

void astree_print(ASTree* root, int level);

void astree_write_code(FILE*, ASTree*)
