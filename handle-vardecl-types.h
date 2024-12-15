#ifndef VARDECL_H
#define VARDECL_H

#include "compiler.h"
#include "ll1parser.h"

#endif

void update_symbol_type(char* name, Tokens new_type,int scope);
void add_types_to_symbol_table(GList* tokens, int token_count);
