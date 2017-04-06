#define HASH_SIZE 997

typedef struct hash_node {
	int type;
	char *text;
	struct hash_node* next;
} HashNode;

HashNode* _table[HASH_SIZE];

void hash_init();
int hash_address(char* text);
HashNode* hash_find(char* text);
HashNode* hash_insert(int type, char* text);

void hash_print();
