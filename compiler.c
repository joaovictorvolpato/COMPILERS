#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <glib.h>
#include "compiler.h"
#include "ll1parser.h"
#include "parse-numeric.h"
#include "handle-vardecl-types.h"

int cur_scope = 0;    
int line_number = 1;

const char* reserved_keywords[] = { "return", "def", "int", "float", "if","string","bool","and","not","or","tint","tstring","tfloat","new","else",};
const int num_keywords = sizeof(reserved_keywords) / sizeof(reserved_keywords[0]);


gpointer token_copy(gconstpointer src, gpointer user_data) {
    const Token *original = (const Token *)src;
    Token *copy = g_new(Token, 1); // Allocate memory for the new Token

    // Copy data fields
    copy->token_type = original->token_type;
    copy->token_num = original->token_num;

    // Deep copy the string field
    copy->token_str = g_strdup(original->token_str);

    return copy;
}

int e_letra(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int e_branco(char c) {
    if (c == '\n' || c == '\r') { line_number++; return true;}

    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

int e_numero(char c) {
    return c >= '0' && c <= '9';
}

unsigned int token_len(Token* t) {
    return strlen(t->token_str);
}

// Function to check if a token string is a reserved keyword
int is_reserved_keyword(const char* token_str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(token_str, reserved_keywords[i]) == 0) {
            return 1;  // It's a reserved keyword
        }
    }
    return 0;  // Not a reserved keyword
}

Token* parse_identifier(const char** current_ptr) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->token_str = (char*)malloc(256);
    int token_idx = 0;

    while (e_letra(**current_ptr) || e_numero(**current_ptr)) {
        token->token_str[token_idx++] = **current_ptr;
        (*current_ptr)++;
    }

    token->token_str[token_idx] = '\0';

    if (is_reserved_keyword(token->token_str)) {
        token->token_type = RESERVED;
        find_token_num(token);
    } else{
        token->token_type = IDENT;
        find_token_num(token);
    }

    return token;
}

Token* parse_number(const char** current_ptr) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->token_str = (char*)malloc(256);
    int token_idx = 0;

    // Parse integer part
    while (e_numero(**current_ptr)) {
        token->token_str[token_idx++] = **current_ptr;
        (*current_ptr)++;
    }

    // Check for decimal point
    if (**current_ptr == '.') {
        token->token_str[token_idx++] = **current_ptr;
        (*current_ptr)++;

        // Parse fractional part
        while (e_numero(**current_ptr)) {
            token->token_str[token_idx++] = **current_ptr;
            (*current_ptr)++;
        }
        token->token_type = FLOAT_E;
    } else {
        token->token_type = INT_E;
    }

    token->token_str[token_idx] = '\0';
    find_token_num(token);
    return token;
}

typedef Token* (*TokenParser)(const char**);

Token* parse_other(const char** current_ptr) {
    Token* token = (Token*)malloc(sizeof(Token));
    int token_idx = 0;
    token->token_str = (char*)malloc(8);
    token->token_str[0] = **current_ptr;

    if(**current_ptr == '=' || **current_ptr == '>' || **current_ptr == '<' || **current_ptr == '!'){
        (*current_ptr)++;
        if (**current_ptr == '=' || **current_ptr == '>' || **current_ptr == '<'){
            token_idx++;
            token->token_str[token_idx] = **current_ptr;
        }
    } else{
        (*current_ptr)++;
    }
    token_idx++;
    token->token_str[token_idx] = '\0';

    token->token_type = OUTRO;
    if(strcmp(token->token_str,"{") == 0){
        incr_scope();
    }
    if(strcmp(token->token_str,"{") == 0){
        hide();
    }
    find_token_num(token);
    
    return token;
}

TokenParser token_parsers[128];


void initialize_token_parsers() {
    for (int i = 0; i < 128; i++) {
        token_parsers[i] = parse_other;
    }

    for (int i = 'a'; i <= 'z'; i++) {
        token_parsers[i] = parse_identifier;
    }
    for (int i = 'A'; i <= 'Z'; i++) {
        token_parsers[i] = parse_identifier;
    }

    for (int i = '0'; i <= '9'; i++) {
        token_parsers[i] = parse_number;
    }

    token_parsers['+'] = parse_other;
    token_parsers['='] = parse_other;
    token_parsers['{'] = parse_other;
    token_parsers['}'] = parse_other;
    token_parsers['('] = parse_other;
    token_parsers[')'] = parse_other;
    token_parsers[';'] = parse_other;
    token_parsers['-'] = parse_other;
    token_parsers['<'] = parse_other;
    token_parsers['>'] = parse_other;
    token_parsers['!'] = parse_other;
    token_parsers['*'] = parse_other;
    token_parsers['%'] = parse_other;
    token_parsers['/'] = parse_other;
}


void free_token(Token* token) {
    if (token != NULL) {
        free(token->token_str);
        free(token);
    }
}

char* read_from_file(const char *srcFilePath)
{
    FILE *pFile = fopen(srcFilePath, "r");

    if (!pFile) {
        perror(srcFilePath);
        return 0;
    }

    char *buffer = NULL;
    int buffer_size = 0;
    int c, i = 0;
    while ((c = fgetc(pFile)) != EOF) {
        if (i >= buffer_size) {
            buffer_size += 1000; // increase buffer size by 1000 bytes
            buffer = realloc(buffer, buffer_size); // resize buffer
            if (buffer == NULL) {
                printf("Error: Memory allocation failed.\n");
                fclose(pFile);
                return 0;
            }
        }
        buffer[i] = c;
        i++;
    }

    fclose(pFile);

    // Null-terminate the buffer
    buffer = realloc(buffer, buffer_size + 1); // resize to accommodate '\0'
    if (buffer == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 0;
    }
    buffer[i] = '\0';  // prevent out of bounds

    return buffer;
}

Token* get_next_token(const char* codigo, const char** current_ptr) {
    // Skip any leading whitespace
    while (e_branco(**current_ptr)) {
        (*current_ptr)++;
    }

    // If we reached the end of the string
    if (**current_ptr == '\0') {
        printf("Returned null \n");
        fflush(stdout);
        return NULL;
    }

    // Use the function pointer array to call the appropriate parsing function
    char first_char = **current_ptr;

    return token_parsers[(int)first_char](current_ptr);
}

int main() {
    const char inputfile[] = "input.txt";

    const char* codigo = read_from_file(inputfile);

    if(!codigo){
        printf("Erro ao ler o aquivo \n");
        return 0;
    }

    const char* current_ptr = codigo;
    GList* token_list = NULL;
    Token* token;

    initialize_token_parsers();
    printf("initialized token parsers");
    fflush(stdout);
    initialize_parsing_table_and_linear_proble();
    printf("parsing table");
    fflush(stdout);
    init_hash_table();


    // Get each token until reaching the end of the string
    while ((token = get_next_token(codigo, &current_ptr)) != NULL) {
        // Print the token information
        insert_into_symbol_table(token, line_number);

        token_list = g_list_append(token_list, token);

        GList* last = g_list_last(token_list);

        printf("data beeing added to the list %s \n", ((Token*)last->data)->token_str);
    }

    GList* token_l = g_slist_copy_deep(token_list,token_copy,NULL);

    do_ll1_parse(token_list);

    add_types_to_symbol_table(token_l,(int)g_list_length(token_l));

    build_AST_for_numeric_expessions(token_l);

    symtab_dump(stdout);

    free_table();

    return 0;
}
