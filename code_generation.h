#pragma once

#include "hash.h"
#include "astree.h"

void generate_program(TAC *first, FILE* output);
void generate_instruction(TAC *tac, FILE* output);

void generate_variables_code(FILE* output);
void generate_var_code(HashNode* item, FILE* output);
void generate_array_init_code(ASTree* list, FILE* output);
void generate_string_declarations(FILE *output);
