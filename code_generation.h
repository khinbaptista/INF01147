#pragma once

#include "hash.h"
#include "astree.h"

void generate_program(TAC *first, FILE* output);
void generate_instruction(TAC *tac, FILE* output);


void generate_hash_table_code(FILE* output);
void generate_hash_item_code(HashNode* item, FILE* output);
void generate_hash_array_init_code(ASTree* list, FILE* output);