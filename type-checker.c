#include "compiler.h"
#include "ll1parser.h"


bool check_no_type_conflicts_in_scope(RefList *head,char* token_string) {
    RefList *current = head;
    while (current != NULL) {
        //printf("ENTERED FIRST WHILE WITH, %d \n", current->type);
        RefList *other = current->next;
        while (other != NULL) {
            //printf("ENTERED SECOND WHILE WITH, %d \n", current->type);
            // Check if another element has the same scope but a different type
            if (current->scope == other->scope && current->type != other->type) {
                printf("---------------------");
                printf("Conflict: Variable %s in scope %d has multiple types (%d and %d).\n" ,token_string,current->scope, current->type, other->type);
                printf("---------------------");
                return false; // Conflict found
            }
            other = other->next;
        }
        current = current->next;
    }
    return true; // No conflicts found
}

void check_identifier_type_consistency(GList* token_list) {
    GList* current;
    
    // Iterate through each token in the list
    for (current = token_list; current != NULL; current = current->next) {
        // Assuming the token has a structure with name, type, and scope
        Token * token = (Token*)(current)->data;
        
        // Only process if the current token is an identifier
        if ( token->token_type != IDENT) 
            continue;

        //printf("LOOKING UP VAR, %s \n", token->token_str);
        
        list_t* entry = lookup(token->token_str);
        if(entry){
            //printf("has entry to hash table, %s \n", token->token_str);
            RefList* occurs = entry->lines;
            check_no_type_conflicts_in_scope(occurs,token->token_str);
        }
    }
}

