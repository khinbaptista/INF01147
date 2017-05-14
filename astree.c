#include "astree.h"

ASTree* astree_create(
	int type,
	ASTree* child1,
	ASTree* child2,
	ASTree* child3,
	ASTree* child4,
	HashNode* symbol
) {
	ASTree* node = calloc(a, sizeof(ASTree));
	node->type = type;
	node->child1 = child1;
	node->child2 = child2;
	node->child3 = child3;
	node->child4 = child4;
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

	if (node->child1) astree_print(node->child1, level - 1);
	if (node->child2) astree_print(node->child2, level - 1);
	if (node->child3) astree_print(node->child3, level - 1);
	if (node->child4) astree_print(node->child4, level - 1);
}
