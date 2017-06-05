/****************************************
	INF01147 - Compiladores
	Khin Baptista
	Lorenzo Dal'Aqua
****************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

extern int semantic_error_flag;

void hash_init() {
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		_table[i] = NULL;
	}
}

int hash_address(char* text) {
	int i, address = 1;
	int n = strlen(text);

	for (i = 0; i < n; i++) {
		address = (address + text[i]) % HASH_SIZE + 1;
	}

	return address - 1;
}

HashNode* hash_find(char* text) {
	HashNode* node = _table[ hash_address(text) ];

	while (node != NULL) {
		if (strcmp(node->text, text) == 0) {
			return node;
		} else {
			node = node->next;
		}
	}

	return NULL;
}

HashNode* hash_insert(int type, char* text) {
	HashNode *node;
	node = hash_find(text);

	if (node != NULL) { return node; }

	int address = hash_address(text);

	node = calloc(1, sizeof(HashNode));
	node->type = type;
	node->function_params = NULL;
	switch(type) {
		case SYMBOL_TK_IDENTIFIER:
			node->id_type = ID_UNDEFINED;
			node->datatype = HASH_TYPE_UNDEFINED;
			break;
		case SYMBOL_LIT_INTEGER:
			node->id_type = ID_UNDEFINED;
			node->datatype = HASH_TYPE_SHORT;
			break;
		case SYMBOL_LIT_REAL:
			node->id_type = ID_UNDEFINED;
			node->datatype = HASH_TYPE_FLOAT;
			break;
		case SYMBOL_LIT_CHAR:
			node->id_type = ID_UNDEFINED;
			node->datatype = HASH_TYPE_BYTE;
			break;
		case SYMBOL_LIT_STRING:
			node->id_type = ID_UNDEFINED;
			node->datatype = HASH_TYPE_UNDEFINED;
			break;
	}
	node->text = calloc(strlen(text) + 1, sizeof(char));
	strcpy(node->text, text);

	node->next = _table[address];
	_table[address] = node;

	return node;
}

void hash_print() {
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		if (_table[i]) {
			fprintf(stderr, "Hash[ %d ] =\t%s\n", i, _table[i]->text);
		}
	}
}

void hash_declare(int datatype, int id_type, HashNode* symbol) {
	if(symbol->datatype != HASH_TYPE_UNDEFINED) {
		fprintf(stderr, "\nERROR: Redeclaration of identifier %s.\n", symbol->text);
		semantic_error_flag = 1;
	} else {
		// fprintf(stderr, "\nDeclaring identifier %s", symbol->text);
		// switch(id_type) {
		// 	case ID_SCALAR: fprintf(stderr, " as scalar"); break;
		// 	case ID_ARRAY: fprintf(stderr, " as array"); break;
		// 	case ID_FUNCTION: fprintf(stderr, " as function"); break;
		// }
	}
	symbol->datatype = datatype;
	symbol->id_type = id_type;
}

void hash_declare_function(int datatype, HashNode* symbol, struct astree* params) {
	hash_declare(datatype, ID_FUNCTION, symbol);
	symbol->function_params = params;
}
