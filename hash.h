#pragma once

/****************************************
	INF01147 - Compiladores
	Khin Baptista
	Lorenzo Dal'Aqua
****************************************/

#define HASH_SIZE 997

struct astree;

enum hash_symbol_type {
	SYMBOL_TK_IDENTIFIER,
	SYMBOL_LIT_INTEGER,
	SYMBOL_LIT_REAL,
	SYMBOL_LIT_CHAR,
	SYMBOL_LIT_STRING,
	SYMBOL_TEMP,
	SYMBOL_LABEL
};

enum hash_identifier_type {
	ID_UNDEFINED,
	ID_SCALAR,
	ID_ARRAY,
	ID_FUNCTION
};

enum hash_datatype {
	HASH_TYPE_UNDEFINED,
	HASH_TYPE_STRING,
	HASH_TYPE_BOOL,
	HASH_TYPE_BYTE,
	HASH_TYPE_SHORT,
	HASH_TYPE_LONG,
	HASH_TYPE_FLOAT,
	HASH_TYPE_DOUBLE
};

typedef struct hash_node {
	int type;
	int id_type;
	int datatype;
	char *text;

	struct hash_node* next;
	struct hash_node* scalar_init;

	struct astree* function_params;
	struct astree* array_init;
	int array_size;
	char *string_name;
} HashNode;

HashNode* _table[HASH_SIZE];

void hash_init();
int hash_address(char* text);
HashNode* hash_find(char* text);
HashNode* hash_insert(int type, char* text);
void hash_declare(int datatype, int id_type, HashNode* symbol);
void hash_declare_function(int datatype, HashNode* symbol, struct astree* params);

HashNode* hash_make_temp(int datatype);
HashNode* hash_make_label();

void hash_print();

int numeric_hash_datatype(int type);
int get_operation_hash_datatype(int left, int right);
