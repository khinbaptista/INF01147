#include "astree.h"
#include "lang171.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Semantic check functions prototypes
void declare_identifier(int datatype, int id_type, HashNode* symbol);
void declare_function(int datatype, HashNode* symbol, struct astree* params);

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
	node->line = getLineNumber();
	node->datatype = AST_DATATYPE_UNDEFINED;
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
			fprintf(file, "%s", node->symbol->text);
			break;

		case AST_IDENTIFIER:
			assert(node->symbol);
			fprintf(file, "%s", node->symbol->text);
			break;

		// Types
		case AST_TYPE_BYTE:		fprintf(file, "byte");		break;
		case AST_TYPE_SHORT:	fprintf(file, "short");		break;
		case AST_TYPE_LONG:		fprintf(file, "long");		break;
		case AST_TYPE_FLOAT:	fprintf(file, "float");		break;
		case AST_TYPE_DOUBLE:	fprintf(file, "double");	break;

		// Variables
		case AST_VAR_DECL:
			assert(node->children[0] && node->children[1] && node->children[2]);
			astree_write_code(file, node->children[0]); fprintf(file, " : ");
			astree_write_code(file, node->children[1]); fprintf(file, " ");
			astree_write_code(file, node->children[2]); fprintf(file, ";\n");
			break;
		case AST_ARRAY_DECL:
			assert(node->children[0] && node->children[1] && node->children[2]);
			astree_write_code(file, node->children[0]); fprintf(file, " : ");
			astree_write_code(file, node->children[1]); fprintf(file, "[");
			astree_write_code(file, node->children[2]); fprintf(file, "] ");
			if (node->children[3]) { astree_write_code(file, node->children[3]); }
			fprintf(file, ";\n");
			break;
		case AST_ARRAY_INIT:
			if (node->children[0]) { astree_write_code(file, node->children[0]); }
			if (node->children[1]) {
				fprintf(file, " "); astree_write_code(file, node->children[1]);
			}
			break;

		// Functions
		case AST_FUNC_DECL:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]); fprintf(file, " ");
			astree_write_code(file, node->children[1]); fprintf(file, "(");
			if (node->children[2]) { astree_write_code(file, node->children[2]); }
			fprintf(file, ") ");
			if (node->children[3]) { astree_write_code(file, node->children[3]); }
			fprintf(file, ";\n");
			break;
		case AST_FUNC_PARAMS_LIST:
		//case AST_FUNC_ARGS_LIST:	// rolaria fazer isso, mas se pá é feio
			assert(node->children[0]);
			astree_write_code(file, node->children[0]);
			if (node->children[1]) {
				fprintf(file, ", ");
				astree_write_code(file, node->children[1]);
			}
			break;
		case AST_FUNC_PARAM:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]); fprintf(file, " ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_FUNC_CALL:
			assert(node->children[0]);
			astree_write_code(file, node->children[0]); fprintf(file, "(");
			if (node->children[1]) { astree_write_code(file, node->children[1]); }
			fprintf(file, ")");
			break;
		case AST_FUNC_ARGS_LIST:
			assert(node->children[0]);
			astree_write_code(file, node->children[0]);
			if (node->children[1]) {
				fprintf(file, ", ");
				astree_write_code(file, node->children[1]);
			}
			break;

		// Commands

		//case AST_CMD: break;

		case AST_CMD_BLOCK:
			fprintf(file, "{");
			if (node->children[0]) {
				fprintf(file, "\n");
				astree_write_code(file, node->children[0]);
				fprintf(file, "\n");
			}
			fprintf(file, "}\n");
			break;
		case AST_CMD_LIST:
			if (node->children[0]) {
				astree_write_code(file, node->children[0]);
			}
			if (node->children[1]) {
				astree_write_code(file, node->children[1]);
			}
			fprintf(file, ";\n");
			break;
		case AST_CMD_VAR_ATTR:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]); fprintf(file, " = ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_CMD_ARRAY_ATTR:
			assert(node->children[0] && node->children[1] && node->children[2]);
			astree_write_code(file, node->children[0]); fprintf(file, "#");
			astree_write_code(file, node->children[1]); fprintf(file, " = ");
			astree_write_code(file, node->children[2]);
			break;
		case AST_CMD_READ:
			assert(node->children[0]);
			fprintf(file, "read ");
			astree_write_code(file, node->children[0]);
			break;
		case AST_CMD_PRINT:
			assert(node->children[0]);
			fprintf(file, "print ");
			astree_write_code(file, node->children[0]);
			break;
		case AST_CMD_RETURN:
			assert(node->children[0]);
			fprintf(file, "return ");
			astree_write_code(file, node->children[0]);
			break;
		case AST_PRINT_ARGS:
			assert(node->children[0]);
			astree_write_code(file, node->children[0]);
			if (node->children[1]) {
				fprintf(file, " "); astree_write_code(file, node->children[1]);
			}
			break;
		case AST_CMD_WHEN:
			assert(node->children[0]);
			fprintf(file, "when ("); astree_write_code(file, node->children[0]);
			fprintf(file, ")\nthen ");
			if (node->children[1]) { astree_write_code(file, node->children[1]); }
			break;
		case AST_CMD_WHEN_ELSE:
			assert(node->children[0]);
			fprintf(file, "when ("); astree_write_code(file, node->children[0]);
			fprintf(file, ")\nthen ");
			if (node->children[1]) { astree_write_code(file, node->children[1]); }
			fprintf(file, "\nelse ");
			if (node->children[2]) { astree_write_code(file, node->children[2]); }
			break;
		case AST_CMD_WHILE:
			assert(node->children[0]);
			fprintf(file, "while ("); astree_write_code(file, node->children[0]);
			fprintf(file, ")\n");
			if (node->children[1]) { astree_write_code(file, node->children[1]); }
			break;
		case AST_CMD_FOR:
			assert(node->children[0] && node->children[1] && node->children[2]);
			fprintf(file, "for (");	astree_write_code(file, node->children[0]);
			fprintf(file, " = ");	astree_write_code(file, node->children[1]);
			fprintf(file, " to ");	astree_write_code(file, node->children[2]);
			fprintf(file, ")\n");
			if (node->children[3]) { astree_write_code(file, node->children[3]); }
			break;

		// Reconstruction helper
		case AST_EXPR_PARENS:
			assert(node->children[0]);
			fprintf(file, "(");
			astree_write_code(file, node->children[0]);
			fprintf(file, ")");
			break;

		// Operators
		case AST_EXPR_SUM:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " + ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_SUB:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " - ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_MULT:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " * ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_DIV:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " / ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_LESSER:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " < ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_GREATER:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " > ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_LESSER_EQ:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " <= ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_GREATER_EQ:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " >= ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_EQUAL:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " == ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_NOT_EQUAL:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " != ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_OR:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " || ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_AND:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, " && ");
			astree_write_code(file, node->children[1]);
			break;
		case AST_EXPR_NOT:
			assert(node->children[0]);
			fprintf(file, "!");
			astree_write_code(file, node->children[0]);
			break;
		case AST_EXPR_NEGATIVE:
			assert(node->children[0]);
			fprintf(file, "-");
			astree_write_code(file, node->children[0]);
			break;
		case AST_EXPR_SCALAR_ACCESS:
			assert(node->children[0]->symbol);
			fprintf(file, "%s", node->children[0]->symbol->text);
			break;
		case AST_EXPR_ARRAY_ACCESS:
			assert(node->children[0] && node->children[1]);
			astree_write_code(file, node->children[0]);
			fprintf(file, "[");
			astree_write_code(file, node->children[1]);
			fprintf(file, "]");
			break;

		default:
			fprintf(stderr,"Unknown AST node!!");
			break;
	}
}

int datatype_ast_to_hash(int ast_type) {
	switch(ast_type) {
		case AST_TYPE_BYTE:
			return HASH_TYPE_BYTE;
		case AST_TYPE_SHORT:
			return HASH_TYPE_SHORT;
		case AST_TYPE_LONG:
			return HASH_TYPE_LONG;
		case AST_TYPE_FLOAT:
			return HASH_TYPE_FLOAT;
		case AST_TYPE_DOUBLE:
			return HASH_TYPE_DOUBLE;
		default:
			return HASH_TYPE_UNDEFINED;
	}
}

int datatype_hash_to_ast(int hash_type) {
	switch(hash_type) {
		case HASH_TYPE_BYTE:
			return AST_DATATYPE_BYTE;
		case HASH_TYPE_SHORT:
			return AST_DATATYPE_SHORT;
		case HASH_TYPE_LONG:
			return AST_DATATYPE_LONG;
		case HASH_TYPE_FLOAT:
			return AST_DATATYPE_FLOAT;
		case HASH_TYPE_DOUBLE:
			return AST_DATATYPE_DOUBLE;
		default:
			return AST_DATATYPE_UNDEFINED;
	}
}

ASTree* ast_literal(HashNode* symbol) {
	return astree_create(AST_LITERAL, NULL, NULL, NULL, NULL, symbol);
}

ASTree* ast_identifier(HashNode* symbol) {
	return astree_create(AST_IDENTIFIER, NULL, NULL, NULL, NULL, symbol);
}

ASTree* ast_type_byte() {
	return astree_create(AST_TYPE_BYTE, NULL, NULL, NULL, NULL, NULL);
}

ASTree* ast_type_short() {
	return astree_create(AST_TYPE_SHORT, NULL, NULL, NULL, NULL, NULL);
}

ASTree* ast_type_long() {
	return astree_create(AST_TYPE_LONG, NULL, NULL, NULL, NULL, NULL);
}

ASTree* ast_type_float() {
	return astree_create(AST_TYPE_FLOAT, NULL, NULL, NULL, NULL, NULL);
}

ASTree* ast_type_double() {
	return astree_create(AST_TYPE_DOUBLE, NULL, NULL, NULL, NULL, NULL);
}

ASTree* ast_program(ASTree* decl, ASTree* decl_set) {
	return astree_create(AST_PROGRAM, decl, decl_set, NULL, NULL, NULL);
}

ASTree* ast_var_decl(HashNode* name, ASTree* type, ASTree* literal) {
	int var_type = datatype_ast_to_hash(type->type);
	ASTree* identifier = astree_create(AST_IDENTIFIER, NULL, NULL, NULL, NULL, name);
	declare_identifier(var_type, ID_SCALAR, name);
	return astree_create(AST_VAR_DECL, identifier, type, literal, NULL, NULL);
}

ASTree* ast_array_decl(HashNode* name, ASTree*  type, HashNode* array_size, ASTree*  array_init) {
	int var_type = datatype_ast_to_hash(type->type);
	ASTree* identifier = astree_create(AST_IDENTIFIER, NULL, NULL, NULL, NULL, name);
	declare_identifier(var_type, ID_ARRAY, identifier->symbol);
	return astree_create(AST_ARRAY_DECL, identifier, type, ast_literal(array_size), array_init, NULL);
}

ASTree* ast_array_init(ASTree* literal, ASTree* array_init) {
	return astree_create(AST_ARRAY_INIT, literal, array_init, NULL, NULL, NULL);
}

ASTree* ast_func_decl(ASTree* type, HashNode* name, ASTree* params_list, ASTree* command) {
	int return_type = datatype_ast_to_hash(type->type);
	ASTree* identifier = astree_create(AST_IDENTIFIER, NULL, NULL, NULL, NULL, name);
	declare_function(return_type, name, params_list);
	return astree_create(AST_FUNC_DECL, type, identifier, params_list, command, NULL);
}

ASTree* ast_func_params_list(ASTree* list, ASTree* param) {
	return astree_create(AST_FUNC_PARAMS_LIST, list, param, NULL, NULL, NULL);
}

ASTree* ast_func_param(ASTree* type, ASTree* name) {
	return astree_create(AST_FUNC_PARAM, type, name, NULL, NULL, NULL);
}

ASTree* ast_func_call(ASTree* name, ASTree* args_list) {
	return astree_create(AST_FUNC_CALL, name, args_list, NULL, NULL, NULL);
}

ASTree* ast_func_args_list(ASTree* list, ASTree* arg) {
	return astree_create(AST_FUNC_ARGS_LIST, list, arg, NULL, NULL, NULL);
}

ASTree* ast_cmd_block(ASTree* cmd_sequence) {
	return astree_create(AST_CMD_BLOCK, cmd_sequence, NULL, NULL, NULL, NULL);
}

ASTree* ast_cmd_list(ASTree* cmd_sequence, ASTree* command) {
	return astree_create(AST_CMD_LIST, cmd_sequence, command, NULL, NULL, NULL);
}

ASTree* ast_cmd_var_attr(ASTree* name, ASTree* value) {
	return astree_create(AST_CMD_VAR_ATTR, name, value, NULL, NULL, NULL);
}

ASTree* ast_cmd_array_attr(ASTree* name, ASTree* index, ASTree* value) {
	return astree_create(AST_CMD_ARRAY_ATTR, name, index, value, NULL, NULL);
}

ASTree* ast_cmd_read(ASTree* var) {
	return astree_create(AST_CMD_READ, var, NULL, NULL, NULL, NULL);
}

ASTree* ast_cmd_print(ASTree* args) {
	return astree_create(AST_CMD_PRINT, args, NULL, NULL, NULL, NULL);
}

ASTree* ast_cmd_return(ASTree* value) {
	return astree_create(AST_CMD_RETURN, value, NULL, NULL, NULL, NULL);

}

ASTree* ast_print_args(ASTree* list, ASTree* arg)  {
	return astree_create(AST_PRINT_ARGS, list, arg, NULL, NULL, NULL);
}

ASTree* ast_cmd_when(ASTree* condition, ASTree* command) {
	return astree_create(AST_CMD_WHEN, condition, command, NULL, NULL, NULL);
}

ASTree* ast_cmd_when_else(ASTree* condition, ASTree* when_cmd, ASTree* else_cmd) {
	return astree_create(AST_CMD_WHEN_ELSE, condition, when_cmd, else_cmd, NULL, NULL);
}

ASTree* ast_cmd_while(ASTree* condition, ASTree* command) {
	return astree_create(AST_CMD_WHILE, condition, command, NULL, NULL, NULL);
}

ASTree* ast_cmd_for(ASTree* var, ASTree* start, ASTree* end, ASTree* command) {
	return astree_create(AST_CMD_FOR, var, start, end, command, NULL);
}

ASTree* ast_expr_parens(ASTree* expression) {
	return astree_create(AST_EXPR_PARENS, expression, NULL, NULL, NULL, NULL);
}

ASTree* ast_op(int type, ASTree* left, ASTree* right) {
	return astree_create(type, left, right, NULL, NULL, NULL);
}

ASTree* ast_unary_op(int type, ASTree* operand) {
	return astree_create(type, operand, NULL, NULL, NULL, NULL);
}

ASTree* ast_expr_scalar_access(ASTree* var) {
	return astree_create(AST_EXPR_SCALAR_ACCESS, var, NULL, NULL, NULL, NULL);
}

ASTree* ast_expr_array_access(ASTree* name, ASTree* index) {
	return astree_create(AST_EXPR_ARRAY_ACCESS, name, index, NULL, NULL, NULL);
}