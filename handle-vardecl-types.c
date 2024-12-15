#include "handle-vardecl-types.h"


void update_symbol_type(char* name, Tokens new_type, int scope) {
    list_t* entry = lookup(name);

    //printf("UPDATING SYMBOL TABLE FOR %s, in scope %d \n",name,scope);
    
    if (entry) {

        RefList *t = entry->lines;
        while (t->next != NULL) {
            if (t->type != new_type){
                if(t->type != 0 && t->scope == scope){
                    fprintf(stderr, "Error: Symbol '%s' already declare in scope %d with other type\n", name, scope);
                } else if(t->type == 0 && t->scope == scope){
                    t->type = new_type;
                }
            }
            t = t->next;
        }

        if (entry->type != new_type) {
            entry->type = new_type;
        }

    } else {
        fprintf(stderr, "Error: Symbol '%s' not found in symbol table\n", name);
    }
}

// Apply Semantic Rules for Type Update
void add_types_to_symbol_table(GList* tokens, int token_count) {
    Tokens current_type = UNDEFINED;
    int peek;
    int scope = 0;

    printf("CALLED ADD TYPES TO SYMBOL TABLE \n");
    
    for (int i = 0; i < token_count; i++) {
        GList * first_token_l = g_list_nth(tokens,i);
        Token * token = (Token*)(first_token_l)->data;
        int value = token->token_num;
        //printf("TOKEN NUM %d\n",value);
        switch (value) {
            case INT:
                //printf("TOKEN INT \n");
                //printf("token string, %s \n", token->token_str);
                current_type = INT_E;
                peek = i;
                peek++;
                //printf("VALUE OF I, %d \n", i);
                //printf("VALUE OF PEEK, %d \n", peek);
                GList * first_token_l_int = g_list_nth(tokens,peek);
                Token * token_int = (Token*)(first_token_l_int)->data;
                value = token_int->token_num;

                //printf("TOKEN IDENT BEFORE IF \n");
                //printf("token string on IDENT BEFORE IF, %s \n", token_int->token_str);

                if(value == IDENT_){
                    //printf("TOKEN IDENT \n");
                    //printf("token string on IDENT, %s \n", token->token_str);
                    printf("CALLING FOR SCOPE %d \n",scope);
                    update_symbol_type(token_int->token_str, current_type,scope);
                }
                break;
            case FLOAT:
                current_type = FLOAT_E;
                peek = i;
                peek++;
                GList * first_token_l_float = g_list_nth(tokens,peek);
                Token * token_float = (Token*)(first_token_l_float)->data;
                value = token_float->token_num;
                if(value == IDENT_){
                    update_symbol_type(token_float->token_str, current_type,scope);
                }
                break;
            case STRING:
                current_type = STRING_E;
                peek = i;
                peek++;
                GList * first_token_l_string = g_list_nth(tokens,peek);
                Token * token_string = (Token*)(first_token_l_string)->data;
                value = token_string->token_num;
                if(value == IDENT_){
                    update_symbol_type(token_string->token_str, current_type,scope);
                }
                break;

            case LBRACE:
                printf("INCREMENTED SCOPE\n");
                scope++;
                break;
            case RBRACE:
                if(scope > 0){
                    scope--;
                }
                break;
            default:
                // Ignore other tokens
                break;
        }
    }
}