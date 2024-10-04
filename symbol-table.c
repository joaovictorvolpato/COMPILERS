#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

list_t **hash_table;
 
void init_hash_table(){
    int i; 
    hash_table = malloc(SIZE * sizeof(list_t*));
    for(i = 0; i < SIZE; i++) hash_table[i] = NULL;
}
 
unsigned int hash(char *key){
    unsigned int hashval = 0;

    for(;*key!='\0';key++) hashval += *key;

    hashval += key[0] % 11 + (key[0] << 3) - key[0];

    return hashval % SIZE;
}
 
void insert_into_symbol_table(Token *token, int lineno){
    unsigned int hashval = hash(token->token_str);
    list_t *l = hash_table[hashval];
    
    while ((l != NULL) && (strcmp(token->token_str,l->lexeme) != 0)) l = l->next;
    
    /* variable not yet in table */
    if (l == NULL){
        l = (list_t*) malloc(sizeof(list_t));
        strncpy(l->lexeme, token->token_str, token_len(token));  
        /* add to hashtable */
        l->type = token->token_type;
        l->hash_val = hashval;
        l->scope = cur_scope;
        l->lines = (RefList*) malloc(sizeof(RefList));
        l->lines->line_number = lineno;
        l->lines->next = NULL;
        l->next = hash_table[hashval];
        hash_table[hashval] = l; 
        printf("Inserted %s for the first time with linenumber %d!\n", token->token_str, lineno); // error checking
    }
    /* found in table, so just add line number */
    else{
        l->scope = cur_scope;
        RefList *t = l->lines;
        while (t->next != NULL) t = t->next;
        /* add linenumber to reference list */
        t->next = (RefList*) malloc(sizeof(RefList));
        t->next->line_number = lineno;
        t->next->next = NULL;
        printf("Found %s again at line %d!\n", token->token_str, lineno);
    }
}
 
list_t *lookup(char *name){ /* return symbol if found or NULL if not found */
    unsigned int hashval = hash(name);
    list_t *l = hash_table[hashval];
    while ((l != NULL) && (strcmp(name,l->lexeme) != 0)) l = l->next;
    return l; // NULL is not found
}
 
list_t *lookup_scope(char *name, int scope){ /* return symbol if found or NULL if not found */
    unsigned int hashval = hash(name);
    list_t *l = hash_table[hashval];
    while ((l != NULL) && (strcmp(name,l->lexeme) != 0) && (scope != l->scope)) l = l->next;
    return l; // NULL is not found
}
 
void hide(){ /* hide the current scope */
    if(cur_scope > 0) cur_scope--;
}
 
void incr_scope(){ /* go to next scope */
    cur_scope++;
}
 
/* print to stdout by default */ 
void symtab_dump(FILE * of){  
  int i;
  fprintf(of,"------------ ------ ------------\n");
  fprintf(of,"Name         Type   Line Numbers\n");
  fprintf(of,"------------ ------ -------------\n");
  for (i=0; i < SIZE; ++i){ 
    if (hash_table[i] != NULL){ 
        list_t *l = hash_table[i];
        while (l != NULL){ 
            RefList *t = l->lines;
            fprintf(of,"%-12s ",l->lexeme);
            if (l->type == INT) fprintf(of,"%-7s","int");
            else if (l->type == IDENT) fprintf(of,"%-7s","ident");
            else if (l->type == FLOAT) fprintf(of, "%-7s", "float");
            else fprintf(of,"%-7s","outro");
            while (t != NULL){
                fprintf(of,"%4d ",t->line_number);
            t = t->next;
            }
            fprintf(of,"\n");
            l = l->next;
        }
    }
  }
}