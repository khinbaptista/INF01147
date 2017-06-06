#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "semantics.h"
#include "lang171.h"

int semantic_error_flag = 0;

int semantic_errors_found() {
	return semantic_error_flag;
}

void semantic_error(char* message, ASTree* node) {
	fprintf(stderr, "\nError at line %d: %s\n", node->line, message);
	semantic_error_flag = 1;
}

void semantic_error_var(char* var, char* message, ASTree* node) {
	fprintf(stderr, "\nError at line %d: Identifier %s %s\n", node->line, var, message);
	semantic_error_flag = 1;
}

void declare_identifier(int datatype, int id_type, HashNode* symbol) {
	if(symbol->datatype != HASH_TYPE_UNDEFINED) {
		fprintf(stderr, "\nError at line %d: Identifier %s was already declared.\n", getLineNumber(), symbol->text);
		semantic_error_flag = 1;
	} else {
		fprintf(stderr, "\nDeclaring identifier %s", symbol->text);
		switch(id_type) {
			case ID_SCALAR: fprintf(stderr, " as scalar"); break;
			case ID_ARRAY: fprintf(stderr, " as array"); break;
			case ID_FUNCTION: fprintf(stderr, " as function"); break;
		}
	}
	symbol->datatype = datatype;
	symbol->id_type = id_type;
}

void declare_function(int datatype, HashNode* symbol, struct astree* params) {
	declare_identifier(datatype, ID_FUNCTION, symbol);
	symbol->function_params = params;
}

int numeric_datatype(int type) {
	return type >= AST_DATATYPE_BYTE;
}

int datatype_integer(int type) {
	return	type == AST_DATATYPE_BYTE	||
			type == AST_DATATYPE_SHORT	||
			type == AST_DATATYPE_LONG;
}

int get_operation_datatype(int left, int right) {
	if(numeric_datatype(left) && numeric_datatype(right)) {
		// Datatypes are an enum, larger values have more bits/precision
		if(left >= right) {
			return left;
		} else {
			return right;
		}
	} else {
		return AST_DATATYPE_BYTE;
	}
}

void semantics_check(ASTree* node) {
	if(!node) {
		return;
	}
	// Check children recursively before checking node.
	semantics_check(node->children[0]);
	semantics_check(node->children[1]);
	semantics_check(node->children[2]);
	semantics_check(node->children[3]);

	switch(node->type) {
		case AST_LITERAL:
			assert(node->symbol);
			node->datatype = datatype_hash_to_ast(node->symbol->datatype);
			break;

		case AST_IDENTIFIER:
			assert(node->symbol);
			if (node->symbol->datatype == AST_DATATYPE_UNDEFINED) {
				semantic_error_var(node->symbol->text, "undeclared.",node);
			}
			node->datatype = datatype_hash_to_ast(node->symbol->datatype);
			break;

		case AST_EXPR_ARRAY_ACCESS:
			assert(node->children[0]);
			if (node->children[0]->symbol->id_type != ID_ARRAY &&
				node->children[0]->symbol->id_type != ID_UNDEFINED) {
				semantic_error_var(node->children[0]->symbol->text, "is not an array.", node);
			}
			if (!datatype_integer(node->children[1]->datatype)) {
				semantic_error_var(node->children[0]->symbol->text, "index is not an integer.", node);
			}
			node->datatype = datatype_hash_to_ast(node->children[0]->symbol->datatype);
			break;

		case AST_EXPR_SCALAR_ACCESS:
			assert(node->children[0]);
			if( node->children[0]->symbol->id_type != ID_SCALAR &&
				node->children[0]->symbol->id_type != ID_UNDEFINED) {
				semantic_error_var(node->children[0]->symbol->text, "is not a scalar.", node);
			}
			node->datatype = datatype_hash_to_ast(node->children[0]->symbol->datatype);
			break;

		case AST_EXPR_PARENS:
			assert(node->children[0]);
			node->datatype = node->children[0]->datatype;
			break;

		case AST_EXPR_SUM:
		case AST_EXPR_SUB:
		case AST_EXPR_MULT:
		case AST_EXPR_DIV:
			assert(node->children[0] && node->children[1]);
			if(! numeric_datatype(node->children[0]->datatype)) {
				semantic_error("Left hand of math operation is not numeric.", node);
			}
			if(! numeric_datatype(node->children[1]->datatype)) {
				semantic_error("Right hand of math operation is not numeric.", node);
			}
			node->datatype = get_operation_datatype(node->children[0]->datatype, node->children[1]->datatype);
			break;

		case AST_EXPR_NEGATIVE:
			assert(node->children[0]);
			if(! numeric_datatype(node->children[0]->datatype)) {
				semantic_error("Unary negative applied to non numeric value.", node);
			}
			node->datatype = node->children[0]->datatype;
			break;

		case AST_EXPR_LESSER:
		case AST_EXPR_GREATER:
		case AST_EXPR_LESSER_EQ:
		case AST_EXPR_GREATER_EQ:
		case AST_EXPR_EQUAL:
		case AST_EXPR_NOT_EQUAL:
			assert(node->children[0] && node->children[1]);
			if(! numeric_datatype(node->children[0]->datatype)) {
				semantic_error("Left hand of comparison is not numeric.", node);
			}
			if(! numeric_datatype(node->children[1]->datatype)) {
				semantic_error("Right hand of comparison is not numeric.", node);
			}
			node->datatype = AST_DATATYPE_BOOL;
			break;

		case AST_EXPR_OR:
		case AST_EXPR_AND:
			assert(node->children[0] && node->children[1]);
			if(numeric_datatype(node->children[0]->datatype)) {
				semantic_error("Left hand of logic operator is numeric.", node);
			}
			if(numeric_datatype(node->children[1]->datatype)) {
				semantic_error("Right hand of logic operator is numeric.", node);
			}
			node->datatype = AST_DATATYPE_BOOL;
			break;

		case AST_EXPR_NOT:
			assert(node->children[0]);
			if(node->children[0]->datatype != AST_DATATYPE_BOOL) {
				semantic_error("Logical NOT applied to non boolean value.", node);
			}
			node->datatype = AST_DATATYPE_BOOL;
			break;

		case AST_FUNC_CALL:
			if (node->children[0]->symbol->id_type != ID_FUNCTION) {
				semantic_error("Identifier is not a function.", node);
			}
			int param_count	= ast_param_list_count(node->children[0]->symbol->function_params);
			int arg_count	= ast_param_list_count(node->children[1]);
			if (arg_count != param_count) {
				// about their types: if this is a bool it is always wrong, so check on node creation.
				semantic_error("Wrong number of parameters.", node);
			}
			break;

		case AST_CMD_READ:
			// Check if scalar or array
			if (node->children[0]->symbol->id_type != ID_SCALAR &&
				node->children[0]->symbol->id_type != ID_ARRAY)
			{
				semantic_error("Command 'read' expects a scalar or array", node);
			}
			break;

		case AST_CMD_RETURN:
			// Check if datatype is numeric
			if (!numeric_datatype(node->children[0]->datatype)) {
				semantic_error("Command 'return' expects a numeric value.", node);
			}
			break;

		case AST_CMD_WHEN:
		case AST_CMD_WHEN_ELSE:
		case AST_CMD_WHILE:
			// Check if condition is boolean?
		case AST_CMD_FOR:
			// Check if variable is scalar

		case AST_TYPE_BYTE:
		case AST_TYPE_SHORT:
		case AST_TYPE_LONG:
		case AST_TYPE_FLOAT:
		case AST_TYPE_DOUBLE:
		case AST_PROGRAM:
		case AST_VAR_DECL:
		case AST_ARRAY_DECL:
		case AST_ARRAY_INIT:
		case AST_FUNC_DECL:
		case AST_FUNC_PARAMS_LIST:
		case AST_FUNC_PARAM:
		case AST_FUNC_ARGS_LIST:
		case AST_CMD_LIST:
		case AST_CMD_BLOCK:
		case AST_CMD_VAR_ATTR:
		case AST_CMD_ARRAY_ATTR:
		case AST_CMD_PRINT:
		case AST_PRINT_ARGS:
		default:
			break;
	}
}
