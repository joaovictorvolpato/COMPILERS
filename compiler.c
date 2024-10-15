#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"

int cur_scope = 0;    
int line_number = 1;

const char* reserved_keywords[] = { "return", "def", "int", "float", "if","string","bool" };
const int num_keywords = sizeof(reserved_keywords) / sizeof(reserved_keywords[0]);

int e_letra(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int e_branco(char c) {
    if (c == '\n') { line_number++;}

    return c == ' ' || c == '\n' || c == '\t';
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
    } else{
        token->token_type = IDENT;
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
        token->token_type = FLOAT;
    } else {
        token->token_type = INT;
    }

    token->token_str[token_idx] = '\0';
    return token;
}

typedef Token* (*TokenParser)(const char**);

Token* parse_other(const char** current_ptr) {
    Token* token = (Token*)malloc(sizeof(Token));
    int token_idx = 0;
    token->token_str = (char*)malloc(128);
    token->token_str[0] = **current_ptr;
    while(**current_ptr == '=' || **current_ptr == '>' || **current_ptr == '<' || **current_ptr == '!'){
        token->token_str[token_idx++] = **current_ptr;
        (*current_ptr)++;
    }
    token->token_str[token_idx++] = '\0';
    token->token_type = OUTRO;
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
    Token* token;

    initialize_token_parsers();
    init_hash_table();

    // Get each token until reaching the end of the string
    while ((token = get_next_token(codigo, &current_ptr)) != NULL) {
        // Print the token information
        insert_into_symbol_table(token, line_number);
        //printf("Token Type: %s, Token String: %s\n", tokens_names[token->token_type], token->token_str);
        
        // Free the allocated memory for this token
        free_token(token);
    }

    symtab_dump(stdout);

    return 0;
}
