#pragma once

#include "hash.h"

enum {	// types

	// Operators
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	LESS,
	GRATER,
	LESS_EQUAL,
	GREATER_EQUAL,
	EQUAL,
	NOT_EQUAL,
	OR,
	AND,
	NOT,
	MINUS,

	// ?

	// Code reconstruction helpers
	PARENTHESIS,
	CURLY_BRACES
};

typedef struct astree {
	int type;
	struct astree* child1;
	struct astree* child2;
	struct astree* child3;
	struct astree* child4;
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
