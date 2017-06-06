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

ASTree* semantics_check(ASTree* node) {
    if(!node) {
        return NULL;
    }
    switch(node->type) {
        case AST_IDENTIFIER:
            assert(node->symbol);
            if(node->symbol->datatype == AST_DATATYPE_UNDEFINED) {
                semantic_error_var(node->symbol->text, "undeclared.",node);
            }
            node->datatype = datatype_hash_to_ast(node->symbol->datatype);
            return node;
        case AST_EXPR_ARRAY_ACCESS:
            assert(node->children[0]);
            if( node->children[0]->symbol->id_type != ID_ARRAY &&
                node->children[0]->symbol->id_type != ID_UNDEFINED) {
                semantic_error_var(node->children[0]->symbol->text, "is not an array.",node);
            }
            node->datatype = datatype_hash_to_ast(node->children[0]->symbol->datatype);
            return node;
        case AST_EXPR_SCALAR_ACCESS:
            assert(node->children[0]);
            if( node->children[0]->symbol->id_type != ID_SCALAR &&
                node->children[0]->symbol->id_type != ID_UNDEFINED) {
                semantic_error_var(node->children[0]->symbol->text, "is not a scalar.", node);
            }
            node->datatype = datatype_hash_to_ast(node->children[0]->symbol->datatype);
            return node;
        default:
            semantics_check(node->children[0]);
            semantics_check(node->children[1]);
            semantics_check(node->children[2]);
            semantics_check(node->children[3]);
            return NULL;
    }
}