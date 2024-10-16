#include "ll1parser.h"


int parsing_table[NUM_NON_TERMINALS][NUM_TERMINALS] = {{-1}};
HashEntry* linear_table[TABLE_SIZE];

void initialize_parsing_table_and_linear_proble() {
    //fill the table with the corresponding productions 
    parsing_table[S][CIFER] = S_PROGRAM_CIFER;
    parsing_table[S][DEF] = S_PROGRAM_CIFER;
    parsing_table[S][IDENT_] = S_PROGRAM_CIFER;
    parsing_table[S][LBRACE] = S_PROGRAM_CIFER;
    parsing_table[S][INT] = S_PROGRAM_CIFER;
    parsing_table[S][FLOAT] = S_PROGRAM_CIFER;
    parsing_table[S][STRING] = S_PROGRAM_CIFER;
    parsing_table[S][TINT] = S_PROGRAM_CIFER;
    parsing_table[S][TFLOAT] = S_PROGRAM_CIFER;
    parsing_table[S][TSTRING] = S_PROGRAM_CIFER;
    parsing_table[S][SEMICOLON] = S_PROGRAM_CIFER;
    parsing_table[S][BREAK] = S_PROGRAM_CIFER;
    parsing_table[S][PRINT] = S_PROGRAM_CIFER;
    parsing_table[S][READ] = S_PROGRAM_CIFER;
    parsing_table[S][RETURN] = S_PROGRAM_CIFER;
    parsing_table[S][IF] = S_PROGRAM_CIFER;
    parsing_table[S][ELSE] = S_PROGRAM_CIFER;
    parsing_table[S][FOR] = S_PROGRAM_CIFER;

    parsing_table[PROGRAM][CIFER] = PROGRAM_EMPTY;
    parsing_table[PROGRAM][DEF] = PROGRAM_FUNCLIST;
    parsing_table[PROGRAM][IDENT_] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][PRINT] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][READ] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][RETURN] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][IF] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][FOR] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][SEMICOLON] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][BREAK] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][TSTRING] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][TFLOAT] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][TINT] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][STRING] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][FLOAT] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][INT] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][LBRACE] = PROGRAM_STATEMENT;

    parsing_table[FUNCLIST][DEF] = FUNCLIST_FUNCDEF_FUNCLIST_PRIME;

    parsing_table[FUNCLIST_PRIME][CIFER] = FUNCLIST_PRIME_EMPTY;

    parsing_table[FUNCDEF][DEF] = FUNCDEF_DEF_IDENT_PARAMLIST_STATELIST;

    parsing_table[PARAMLIST][INT] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][FLOAT] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][STRING] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][TINT] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][TFLOAT] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][TSTRING] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;

    parsing_table[PARAMLIST_TAIL][RPAREN] = PARAMLIST_TAIL_EMPTY;
    parsing_table[PARAMLIST_TAIL][COMMA] = PARAMLIST_TAIL_COMMA_PARAMLIST;

    parsing_table[PARAMLIST_TYPES][INT] = PARAMLIST_TYPES_INT;
    parsing_table[PARAMLIST_TYPES][FLOAT] = PARAMLIST_TYPES_FLOAT;
    parsing_table[PARAMLIST_TYPES][STRING] = PARAMLIST_TYPES_STRING;
    parsing_table[PARAMLIST_TYPES][TINT] = PARAMLIST_TYPES_TINT;
    parsing_table[PARAMLIST_TYPES][TFLOAT] = PARAMLIST_TYPES_TFLOAT;
    parsing_table[PARAMLIST_TYPES][TSTRING] = PARAMLIST_TYPES_TSTRING;

    parsing_table[STATEMENT][IDENT_] = STATEMENT_ATRIBSTAT_SEMICOLON;
    parsing_table[STATEMENT][LBRACE] = STATEMENT_STATELIST_SEMICOLON;
    parsing_table[STATEMENT][INT] = STATEMENT_VARDECL_SEMICOLON;
    parsing_table[STATEMENT][FLOAT] = STATEMENT_VARDECL_SEMICOLON;
    parsing_table[STATEMENT][STRING] = STATEMENT_VARDECL_SEMICOLON;
    parsing_table[STATEMENT][TINT] = STATEMENT_TUPLEDECL_SEMICOLON;
    parsing_table[STATEMENT][TFLOAT] = STATEMENT_TUPLEDECL_SEMICOLON;
    parsing_table[STATEMENT][TSTRING] = STATEMENT_TUPLEDECL_SEMICOLON;
    parsing_table[STATEMENT][SEMICOLON] = STATEMENT_SEMICOLON;
    parsing_table[STATEMENT][BREAK] = STATEMENT_BREAK_SEMICOLON;
    parsing_table[STATEMENT][PRINT] = STATEMENT_PRINTSTAT_SEMICOLON;
    parsing_table[STATEMENT][READ] = STATEMENT_READSTAT_SEMICOLON;
    parsing_table[STATEMENT][RETURN] = STATEMENT_RETURNSTAT_SEMICOLON;
    parsing_table[STATEMENT][IF] = STATEMENT_IFSTAT_SEMICOLON;
    parsing_table[STATEMENT][FOR] = STATEMENT_FORSTAT_SEMICOLON;

    parsing_table[TUPLEDECL][TINT] = TUPLEDECL_TTYPE_IDENT_INT_CONSTANT;
    parsing_table[TUPLEDECL][TFLOAT] = TUPLEDECL_TTYPE_IDENT_INT_CONSTANT;
    parsing_table[TUPLEDECL][TSTRING] = TUPLEDECL_TTYPE_IDENT_INT_CONSTANT;

    parsing_table[TTYPE][TINT] = TTYPE_TINT;
    parsing_table[TTYPE][TFLOAT] = TTYPE_TFLOAT;
    parsing_table[TTYPE][TSTRING] = TTYPE_TSTRING;

    parsing_table[VARDECL][INT] = VARDECL_TYPE_IDENT_VARDECL_TAIL;
    parsing_table[VARDECL][FLOAT] = VARDECL_TYPE_IDENT_VARDECL_TAIL;
    parsing_table[VARDECL][STRING] = VARDECL_TYPE_IDENT_VARDECL_TAIL;

    parsing_table[TYPE][INT] = TYPE_INT;
    parsing_table[TYPE][FLOAT] = TYPE_FLOAT;
    parsing_table[TYPE][STRING] = TYPE_STRING;

    parsing_table[VARDECL_TAIL][SEMICOLON] = VARDECL_TAIL_EMPTY; 
    parsing_table[VARDECL_TAIL][LBRACKET] = VARDECL_TAIL_INT_CONSTANT_VARDECL_TAIL;

    parsing_table[ATRIBSTAT][IDENT_] = ATRIBSTAT_LVALUE_EQUAL_ATRIBSTAT_TAIL;

    parsing_table[ATRIBSTAT_TAIL][IDENT_LPAREN] = ATRIBSTAT_TAIL_FUNCCALL; 
    parsing_table[ATRIBSTAT_TAIL][LPAREN] = ATRIBSTAT_TAIL_EXPRESSION; 
    parsing_table[ATRIBSTAT_TAIL][NEW] = ATRIBSTAT_TAIL_ALLOCEXPRESSION;

    parsing_table[FUNCCALL][IDENT_LPAREN] = FUNCCALL_IDENT_LPAREN_PARAMLISTCALL_RPAREN; 
    
    parsing_table[PARAMLISTCALL][IDENT_] = PARAMLISTCALL_PARAMLISTCALL_TAIL; 
    parsing_table[PARAMLISTCALL][RPAREN] = PARAMLISTCALL_EMPTY; 

    parsing_table[PARAMLISTCALL_TAIL][IDENT_] = PARAMLISTCALL_TAIL_IDENT_PARAMLISTCALL_CONT;

    parsing_table[PARAMLISTCALL_CONT][RPAREN] = PARAMLISTCALL_CONT_EMPTY; 
    parsing_table[PARAMLISTCALL_CONT][COMMA] = PARAMLISTCALL_CONT_COMMA_PARAMLISTCALL;

    parsing_table[PRINTSTAT][PRINT] = PRINTSTAT_PRINT_EXPRESSION;
    parsing_table[READSTAT][READ] = READSTAT_READ_LVALUE;
    parsing_table[RETURNSTAT][RETURN] = RETURNSTAT_RETURN_IDENTRET;

    parsing_table[IDENTRET][IDENT_] = IDENTRET_IDENT; 
    parsing_table[IDENTRET][SEMICOLON] = IDENTRET_EMPTY;

    parsing_table[IFSTAT][IF] = IFSTAT_IF_EXPRESSION_STATEMENT_IF_TAIL;

    parsing_table[IF_TAIL][SEMICOLON] = IF_TAIL_EMPTY; 
    parsing_table[IF_TAIL][ELSE] = IF_TAIL_ELSE_STATEMENT;

    parsing_table[FORSTAT][FOR] = FORSTAT_FOR_ATRIBSTAT_EXPRESSION_ATRIBSTAT_STATEMENT;

    parsing_table[STATELIST][IDENT_] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][IF] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][FOR] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][PRINT] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][READ] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][RETURN] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][LBRACE] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][BREAK] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][INT] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][FLOAT] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][STRING] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][TINT] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][TFLOAT] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][TSTRING] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][SEMICOLON] = STATELIST_STATEMENT_STATELIST_TAIL;

    parsing_table[STATELIST_TAIL][IDENT_] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][IF] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][FOR] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][PRINT] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][READ] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][RETURN] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][LBRACE] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][BREAK] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][INT] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][FLOAT] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][STRING] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][TINT] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][TFLOAT] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][TSTRING] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][SEMICOLON] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][RBRACE] = STATELIST_TAIL_EMPTY;

    parsing_table[ALLOCEXPRESSION][NEW] = ALLOCEXPRESSION_NEW_TYPE_NUMEXPRESSION_ALLOCEXPRESSION_TAIL_CONT;

    parsing_table[ALLOCEXPRESSION_TAIL_CONT][SEMICOLON] = ALLOCEXPRESSION_TAIL_CONT_EMPTY;
    parsing_table[ALLOCEXPRESSION_TAIL_CONT][RPAREN] = ALLOCEXPRESSION_TAIL_CONT_EMPTY;
    parsing_table[ALLOCEXPRESSION_TAIL_CONT][LBRACKET] = ALLOCEXPRESSION_TAIL_CONT_NUMEXPRESSION_ALLOCEXPRESSION_TAIL_CONT;

    parsing_table[BOL_EXPRESSION][LPAREN] = BOL_EXPRESSION_EXPRESSION_BOL_EXPRESSION_TAIL; 
    parsing_table[BOL_EXPRESSION][NOT] = BOL_EXPRESSION_NOT_BOL_EXPRESSION; 

    parsing_table[BOL_EXPRESSION_TAIL][RPAREN] = BOL_EXPRESSION_TAIL_EMPTY; 
    parsing_table[BOL_EXPRESSION_TAIL][SEMICOLON] = BOL_EXPRESSION_TAIL_EMPTY; 

    parsing_table[BOL_EXPRESSION_TAIL][AND] = BOL_EXPRESSION_TAIL_BOOL_OP_EXPRESSION_BOL_EXPRESSION_TAIL;
    parsing_table[BOL_EXPRESSION_TAIL][OR] = BOL_EXPRESSION_TAIL_BOOL_OP_EXPRESSION_BOL_EXPRESSION_TAIL; 
    parsing_table[BOL_EXPRESSION_TAIL][NOT] = BOL_EXPRESSION_TAIL_BOOL_OP_EXPRESSION_BOL_EXPRESSION_TAIL;

    parsing_table[BOOL_OP][NOT] = BOOL_OP_NOT;
    parsing_table[BOOL_OP][AND] = BOOL_OP_AND;
    parsing_table[BOOL_OP][OR] = BOOL_OP_OR;

    parsing_table[EXPRESSION][LPAREN] = EXPRESSION_NUMEXPRESSION_EXPRESSION_TAIL;

    parsing_table[EXPRESSION_TAIL][RPAREN] = EXPRESSION_TAIL_EMPTY; 
    parsing_table[EXPRESSION_TAIL][SEMICOLON] = EXPRESSION_TAIL_EMPTY; 
    parsing_table[EXPRESSION_TAIL][LESS_THAN] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][GREATER_THAN] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][LESS_EQUAL] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][GREATER_EQUAL] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][EQUAL_EQUAL] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][NOT_EQUAL] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 

    parsing_table[REL_OP][LESS_THAN] = REL_OP_LESS_THAN;
    parsing_table[REL_OP][GREATER_THAN] = REL_OP_GREATER_THAN;
    parsing_table[REL_OP][LESS_EQUAL] = REL_OP_LESS_EQUAL;
    parsing_table[REL_OP][GREATER_EQUAL] = REL_OP_GREATER_EQUAL;
    parsing_table[REL_OP][EQUAL_EQUAL] = REL_OP_EQUAL_EQUAL;
    parsing_table[REL_OP][NOT_EQUAL] = REL_OP_NOT_EQUAL;


    parsing_table[NUMEXPRESSION][IDENT_] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION][INT_CONSTANT] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION][FLOAT_CONSTANT] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION][LPAREN] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION][PLUS] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL;
    parsing_table[NUMEXPRESSION][MINUS] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL;
    parsing_table[NUMEXPRESSION][STRING_CONSTANT] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL;
    parsing_table[NUMEXPRESSION][NULL_LITERAL] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL;

    parsing_table[NUMEXPRESSION_TAIL][RPAREN] = NUMEXPRESSION_TAIL_EMPTY; 
    parsing_table[NUMEXPRESSION_TAIL][LESS_THAN] = NUMEXPRESSION_TAIL_EMPTY; 
    parsing_table[NUMEXPRESSION_TAIL][GREATER_THAN] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][LESS_EQUAL] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][GREATER_EQUAL] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][EQUAL_EQUAL] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][NOT_EQUAL] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][PLUS] = NUMEXPRESSION_TAIL_ADD_OP_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION_TAIL][MINUS] = NUMEXPRESSION_TAIL_ADD_OP_TERM_NUMEXPRESSION_TAIL; 

    parsing_table[ADD_OP][PLUS] = ADD_OP_PLUS;
    parsing_table[ADD_OP][MINUS] = ADD_OP_MINUS;

    parsing_table[TERM][IDENT_] = TERM_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM][INT_CONSTANT] = TERM_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM][FLOAT_CONSTANT] = TERM_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM][STRING_CONSTANT] = TERM_UNARYEXPR_TERM_TAIL;
    parsing_table[TERM][LPAREN] = TERM_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM][PLUS] = TERM_UNARYEXPR_TERM_TAIL;
    parsing_table[TERM][MINUS] = TERM_UNARYEXPR_TERM_TAIL;
    parsing_table[TERM][NULL_LITERAL] = TERM_UNARYEXPR_TERM_TAIL;

    parsing_table[TERM_TAIL][RPAREN] = TERM_TAIL_EMPTY; 
    parsing_table[TERM_TAIL][SEMICOLON] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][PLUS] = TERM_TAIL_EMPTY; 
    parsing_table[TERM_TAIL][MINUS] = TERM_TAIL_EMPTY; 
    parsing_table[TERM_TAIL][LBRACKET] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][LESS_THAN] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][GREATER_THAN] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][LESS_EQUAL] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][GREATER_EQUAL] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][EQUAL_EQUAL] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][NOT_EQUAL] = TERM_TAIL_EMPTY;

    parsing_table[TERM_TAIL][MULTIPLY] = TERM_TAIL_MULT_OP_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM_TAIL][DIVIDE] = TERM_TAIL_MULT_OP_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM_TAIL][MODULO] = TERM_TAIL_MULT_OP_UNARYEXPR_TERM_TAIL; 

    parsing_table[MULT_OP][MULTIPLY] = MULT_OP_MULTIPLY; 
    parsing_table[MULT_OP][DIVIDE] = MULT_OP_DIVIDE; 
    parsing_table[MULT_OP][MODULO] = MULT_OP_MODULO; 

    parsing_table[UNARYEXPR][IDENT_] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][LPAREN] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][INT_CONSTANT] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][FLOAT_CONSTANT] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][STRING_CONSTANT] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][NULL_LITERAL] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][PLUS] = UNARYEXPR_UNARY_OP_FACTOR; 
    parsing_table[UNARYEXPR][MINUS] = UNARYEXPR_UNARY_OP_FACTOR; 

    parsing_table[UNARY_OP][PLUS] = UNARY_OP_PLUS; 
    parsing_table[UNARY_OP][MINUS] = UNARY_OP_MINUS; 

    parsing_table[FACTOR][IDENT_] = FACTOR_LVALUE; 
    parsing_table[FACTOR][LPAREN] = FACTOR_LPAREN_NUMEXPRESSION_RPAREN; 
    parsing_table[FACTOR][INT_CONSTANT] = FACTOR_INT_CONSTANT; 
    parsing_table[FACTOR][FLOAT_CONSTANT] = FACTOR_FLOAT_CONSTANT;
    parsing_table[FACTOR][STRING_CONSTANT] = FACTOR_STRING_CONSTANT;
    parsing_table[FACTOR][NULL_LITERAL] = FACTOR_NULL_LITERAL;

    parsing_table[LVALUE][IDENT_] = LVALUE_IDENT_LVALUE_TAIL;

    parsing_table[LVALUE_TAIL][RPAREN] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][SEMICOLON] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][LESS_THAN] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][GREATER_THAN] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][LESS_EQUAL] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][GREATER_EQUAL] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][EQUAL_EQUAL] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][NOT_EQUAL] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][PLUS] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][MINUS] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][MULTIPLY] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][DIVIDE] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][MODULO] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][LBRACKET] = LVALUE_TAIL_LBRACKET_NUMEXPRESSION_RBRACKET_LVALUE_TAIL;
    parsing_table[LVALUE_TAIL][RBRACKET] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][EQUAL] = LVALUE_TAIL_EMPTY;

    for (int i = 0; i < TABLE_SIZE; i++) {
        linear_table[i] = NULL;
    }

    // Insert lexemes and token values into the hash table
    insert("null", NULL_LITERAL);
    insert("print", PRINT);
    insert("read", READ);
    insert("return", RETURN);
    insert("if", IF);
    insert("else", ELSE);
    insert("for", FOR);
    insert("break", BREAK);
    insert("def", DEF);
    insert("new", NEW);
    insert("tint", TINT);
    insert("tfloat", TFLOAT);
    insert("tstring", TSTRING);
    insert("and", AND);
    insert("or", OR);
    insert("not", NOT);
    insert("=", EQUAL);
    insert("<", LESS_THAN);
    insert(">", GREATER_THAN);
    insert("<=", LESS_EQUAL);
    insert("=>", GREATER_EQUAL);
    insert("==", EQUAL_EQUAL);
    insert("!=", NOT_EQUAL);
    insert("+", PLUS);
    insert("-", MINUS);
    insert("*", MULTIPLY);
    insert("/", DIVIDE);
    insert("%", MODULO);
    insert("(", LPAREN);
    insert(")", RPAREN);
    insert("[", LBRACKET);
    insert("]", RBRACKET);
    insert("{", LBRACE);
    insert("}", RBRACE);
    insert(",", COMMA);
    insert(";", SEMICOLON);
    insert("$", CIFER);

}

unsigned int hash_lexeme(const char* lexeme) {
    unsigned long hash_value = 0;
    while (*lexeme) {
        hash_value = (hash_value * 31) + *lexeme; // A prime multiplier is often used
        lexeme++;
    }
    return hash_value % TABLE_SIZE;
}

// Insert function using linear probing
void insert(const char* lexeme, int token_value) {
    unsigned int index = hash_lexeme(lexeme);
    unsigned int original_index = index;

    while (linear_table[index] != NULL) {
        // If lexeme already exists, update the token_value
        if (strcmp(linear_table[index]->lexeme, lexeme) == 0) {
            linear_table[index]->token_value = token_value;
            return;
        }
        index = (index + 1) % TABLE_SIZE; // Linear probing
        if (index == original_index) {
            printf("Error: Hash table is full\n");
            return;
        }
    }

    HashEntry* new_node = (HashEntry*)malloc(sizeof(HashEntry));
    new_node->lexeme = lexeme;
    new_node->token_value = token_value;
    linear_table[index] = new_node;
}

void find_token_num(Token* token) {

    if(token->token_type == IDENT){
        token->token_num = IDENT_;
    } else if (token->token_type == INT){
        token->token_num = INT_CONSTANT;
    } else if(token->token_type == FLOAT){
        token->token_num =FLOAT_CONSTANT;
    }


    unsigned int index = hash_lexeme(token->token_str);
    unsigned int original_index = index;

    while (linear_table[index] != NULL) {
        if (strcmp(linear_table[index]->lexeme, token->token_str) == 0) {
            token->token_num = linear_table[index]->token_value;
        }
        index = (index + 1) % TABLE_SIZE; // Linear probing
        if (index == original_index) {
            break;
        }
    }

    // If lexeme is not found
    return;
}

void free_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (linear_table[i] != NULL) {
            free(linear_table[i]);
        }
    }
}

/*
will return 1 if the ll1 has finished suceffuly, -1 otherwise
*/

int do_ll1_parse(GList * token_list){

    printf("Called do_ll1_parse \n ");

    int size =  g_list_length(token_list);
    for( int i = 0; i < size; i++){
        GList* token = g_list_nth(token_list,i);
        
    }

    return 1;
}


typedef struct ll1_stack
{
    frame * bottom;
    frame * top;

    int (*pop_stack)(struct stack*);
    void (*push_stack)(struct stack*,int val);
    frame* (*lookup_stack)(struct stack*);

} stack;

int pop_stack(stack * self){
    frame * top = self->top;
    self->top = top->bottom;
    int top_val = top->value;
    free(top);
    return top_val;
};

void push_stack(stack * self, int val){
    frame * new_top = malloc(sizeof(frame));
    if (self->bottom == NULL && self->top == NULL){
        self->bottom = new_top;
    }
    self->top = new_top;
    new_top->value = val;
    new_top->bottom = self->top;
    new_top->up = NULL;
}

frame * lookup_stack(stack * self){
    return self->top;
}

typedef struct stack_frame{
    int value;
    frame * up;
    frame * bottom;
} frame;
