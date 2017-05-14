#include "astree.h"
#include <stdlib.h>
#include <stdio.h>

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
