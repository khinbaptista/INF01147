#pragma once

#include "astree.h"
#include "hash.h"

int semantic_errors_found();

void semantic_error(char* message, ASTree* node);

void semantic_error_var(char* var, char* message, ASTree* node);

ASTree* semantics_check(ASTree* node);

void declare_identifier(int datatype, int id_type, HashNode* symbol);

void declare_function(int datatype, HashNode* symbol, struct astree* params);