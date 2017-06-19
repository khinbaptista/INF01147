#pragma once

#include <stdio.h>
#include "hash.h"
#include "tacs.h"

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

	AST_VAR_DECL,
	AST_ARRAY_DECL,
	AST_ARRAY_INIT,

	AST_FUNC_DECL,
	AST_FUNC_PARAMS_LIST,
	AST_FUNC_PARAM,
	AST_FUNC_CALL,
	AST_FUNC_ARGS_LIST,
	AST_FUNC_ARG,

	AST_CMD_LIST,
	AST_CMD_BLOCK,
	AST_CMD_VAR_ATTR,
	AST_CMD_ARRAY_ATTR,
	AST_CMD_READ,
	AST_CMD_PRINT,
	AST_CMD_RETURN,
	AST_PRINT_ARGS,
	AST_CMD_WHEN,
	AST_CMD_WHEN_ELSE,
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
	AST_EXPR_SCALAR_ACCESS,
	AST_EXPR_ARRAY_ACCESS

};

// ORDER OF ENUM IS IMPORTANT:
// Larger datatypes have more precision and also a higher enum value
enum ast_datatypes {
	AST_DATATYPE_UNDEFINED,
	AST_DATATYPE_BOOL,
	AST_DATATYPE_STRING,
	AST_DATATYPE_BYTE,
	AST_DATATYPE_SHORT,
	AST_DATATYPE_LONG,
	AST_DATATYPE_FLOAT,
	AST_DATATYPE_DOUBLE
};

typedef struct astree {
	int type;
	int datatype;
	int line;
	struct astree *children[ MAX_CHILDREN ];
	HashNode	*symbol;
	struct tac	*tac;
} ASTree;

ASTree* astree_create(
	int type,
	ASTree* child1,
	ASTree* child2,
	ASTree* child3,
	ASTree* child4,
	HashNode* symbol
);

ASTree* astree_root;

void astree_print(ASTree* root, int level);

void astree_write_code(FILE* file, ASTree* node);

int datatype_ast_to_hash(int ast_type);
int datatype_hash_to_ast(int hash_type);

int ast_param_list_count(ASTree *params_list);

ASTree* ast_literal(HashNode* symbol);
ASTree* ast_identifier(HashNode* symbol);
ASTree* ast_type_byte();
ASTree* ast_type_short();
ASTree* ast_type_long();
ASTree* ast_type_float();
ASTree* ast_type_double();
ASTree* ast_program(ASTree* decl, ASTree* decl_set);
ASTree* ast_var_decl(HashNode* name, ASTree*  type, ASTree* literal);
ASTree* ast_array_decl(HashNode* name, ASTree*  type, HashNode* array_size, ASTree*  array_init);
ASTree* ast_array_init(ASTree* literal, ASTree* array_init);
ASTree* ast_func_decl(ASTree* type, HashNode* name, ASTree* params_list, ASTree* command);
ASTree* ast_func_params_list(ASTree* next, ASTree* param);
ASTree* ast_func_param(ASTree* type, HashNode* name);
ASTree* ast_func_call(ASTree* name, ASTree* args_list);
ASTree* ast_func_args_list(ASTree* next, ASTree* arg);
ASTree* ast_func_arg(ASTree* expression);
ASTree* ast_cmd_block(ASTree* cmd_sequence);
ASTree* ast_cmd_list(ASTree* cmd_sequence, ASTree* command);
ASTree* ast_cmd_var_attr(ASTree* name, ASTree* value);
ASTree* ast_cmd_array_attr(ASTree* name, ASTree* index, ASTree* value);
ASTree* ast_cmd_read(ASTree* var);
ASTree* ast_cmd_print(ASTree* args);
ASTree* ast_cmd_return(ASTree* value);
ASTree* ast_print_args(ASTree* next, ASTree* arg);
ASTree* ast_cmd_when(ASTree* condition, ASTree* command);
ASTree* ast_cmd_when_else(ASTree* condition, ASTree* when_cmd, ASTree* else_cmd);
ASTree* ast_cmd_while(ASTree* condition, ASTree* command);
ASTree* ast_cmd_for(ASTree* var, ASTree* start, ASTree* end, ASTree* command);
ASTree* ast_expr_parens(ASTree* expression);
ASTree* ast_op(int type, ASTree* left, ASTree* right);
ASTree* ast_unary_op(int type, ASTree* operand);
ASTree* ast_expr_scalar_access(ASTree* var);
ASTree* ast_expr_array_access(ASTree* name, ASTree* index);
