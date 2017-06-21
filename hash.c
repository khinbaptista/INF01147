/****************************************
	INF01147 - Compiladores
	Khin Baptista
	Lorenzo Dal'Aqua
****************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lang171.h"
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
			node->id_type = ID_SCALAR;
			node->datatype = HASH_TYPE_SHORT;
			break;
		case SYMBOL_LIT_REAL:
			node->id_type = ID_SCALAR;
			node->datatype = HASH_TYPE_FLOAT;
			break;
		case SYMBOL_LIT_CHAR:
			node->id_type = ID_SCALAR;
			node->datatype = HASH_TYPE_BYTE;
			break;
		case SYMBOL_LIT_STRING:
			node->id_type = ID_SCALAR;
			node->datatype = HASH_TYPE_STRING;
			break;
	}
	node->text = calloc(strlen(text) + 1, sizeof(char));
	strcpy(node->text, text);

	node->next = _table[address];
	_table[address] = node;

	return node;
}

HashNode* hash_make_temp(int datatype) {
	static int serial_number = 1;
	static char buffer[128];
	size_t length = 0;

	sprintf(buffer, "__temporary%d", serial_number++);
	length = strlen(buffer);

	HashNode *temp	= calloc(1, sizeof(HashNode));
	temp->text		= calloc(length + 1, sizeof(char));
	strcpy(temp->text, buffer);

	int address = hash_address(buffer);
	temp->next = _table[address];
	_table[address] = temp;

	temp->type = SYMBOL_TEMP;
	temp->id_type = ID_UNDEFINED;
	temp->datatype = datatype;

	return temp;
}

HashNode* hash_make_label() {
	static int serial_number = 1;
	static char buffer[128];
	size_t length = 0;

	sprintf(buffer, "__label%d", serial_number++);
	length = strlen(buffer);

	HashNode *label	= calloc(1, sizeof(HashNode));
	label->text		= calloc(length + 1, sizeof(char));
	strcpy(label->text, buffer);

	int address = hash_address(buffer);
	label->next = _table[address];
	_table[address] = label;

	label->type = SYMBOL_LABEL;
	label->id_type = ID_UNDEFINED;
	label->datatype = HASH_TYPE_UNDEFINED;

	return label;
}

void hash_print() {
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		if (_table[i]) {
			fprintf(stderr, "Hash[ %d ] =\t%s\n", i, _table[i]->text);
		}
	}
}

int numeric_hash_datatype(int type) {
	return type >= HASH_TYPE_BYTE;
}

int get_operation_hash_datatype(int left, int right) {
	if(numeric_hash_datatype(left) && numeric_hash_datatype(right)) {
		// Datatypes are an enum, larger values have more bits/precision
		if(left >= right) {
			return left;
		} else {
			return right;
		}
	} else {
		return HASH_TYPE_BYTE;
	}
}
