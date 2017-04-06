#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

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
