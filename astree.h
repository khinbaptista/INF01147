#pragma once

#include "hash.h"

#define MAX_CHILDREN 4

enum {	// types
	LITERAL
}

typedef struct astree {
	int type;
	struct astree children[ MAX_CHILDREN ];
	HashNode* symbol;
} ASTree;

ASTree* astree_create(
	int type,
	ASTree* child1 = NULL,
	ASTree* child2 = NULL,
	ASTree* child3 = NULL,
	ASTree* child4 = NULL,
	HashNode* symbol = NULL
);

void astree_print(ASTree* root, int level);
