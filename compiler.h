
#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


// Enum for different token types
typedef enum {
    IDENT,
    INT,
    FLOAT,
    OUTRO,
    RESERVED
} Tokens;


static const char * const tokens_names[] = {
	[IDENT] = "IDENT",
	[INT] = "NI",
	[FLOAT] = "NPF",
	[OUTRO] = "OUTRO",
    [RESERVED] = "RESERVED"
};

// Struct to store the token information
typedef struct {
    Tokens token_type;
    char* token_str;
} Token;

unsigned int token_len(Token* t);


extern int cur_scope;
extern int line_number;

/* maximum size of hash table */
#define SIZE 211
 
/* maximum size of tokens-identifiers */
#define LEXEM_LEN 50

 
/* a linked list of references (lineno's) for each variable */
typedef struct RefList{ 
    int line_number;
    struct RefList *next;
    Tokens type;
}RefList;
 
// struct that represents a list node
typedef struct list_t{
    char lexeme[LEXEM_LEN];
    unsigned int hash_val;
    int st_ival; double st_fval; char *st_sval;
    int lexeme_size;
    int scope;
    Tokens type;
    RefList *lines;
    struct list_t *next;
}list_t;
 
/* the hash table */
extern list_t **hash_table;
 
// Function Declarations
void init_hash_table(); // initialize hash table
unsigned int hash(char *key); // hash function 
void insert_into_symbol_table(Token *token, int lineno); // insert entry
list_t *lookup(char *name); // search for entry
list_t *lookup_scope(char *name, int scope); // search for entry in scope
void hide(); // hide the current scope
void hide_scope(int scope); // hide a specific scope
void symtab_dump(FILE *of); // dump file

#endif