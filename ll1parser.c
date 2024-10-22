#include "ll1parser.h"


int parsing_table[NUM_NON_TERMINALS][NUM_TERMINALS] = {{-1}};
HashEntry* linear_table[TABLE_SIZE];

typedef struct {
    int value;
    struct frame * up;
    struct frame * bottom;
} frame;


typedef struct {
    frame * bottom;
    frame * top;

    int size;

    int (*pop_stack)(struct stack_type*);
    void (*push_stack)(struct stack_type*,int val);
    frame* (*lookup_stack)(struct stack_type*);

} stack_type;

int pop_stack(stack_type * self){

    if(self->top == NULL){
        printf("EMPTY STACK\n");
    }

    //printf("PREVIOUS TOP VALUE BEFORE POP %d \n",((frame*)(self->top)->value));
    frame * top = self->top;

    //printf("MEMORY ADDRESS OF SELF->TOP %ld \n",self->top);
    //printf("MEMORY ADDRESS OF TOP VARIABLE %ld \n",top);

    if(top->bottom == NULL){
        printf("stack size %d", self->size);
        printf("LAST ELEMENT IN THE STACK \n");
    }

    frame * one_under = top->bottom;
    one_under->up = NULL;
    //printf("ONE UNDER VALUE %d \n", one_under->value);
    self->top = one_under;
    //printf("NEW TOP VALUE ASSIGN %d \n",((frame*)(self->top)->value));
    int top_val = top->value;

    //printf("MEMORY ADDRESS OF SELF->TOP AFTER UPDATED %ld \n",self->top);

    free(top);
    //printf("NEW TOP VALUE AFTER FREE %d \n",((frame*)(self->top)->value));
    
    //printf("NEW TOP VALUE AFTER POP %d \n",((frame*)(self->top)->value));
    self->size--;
    return top_val;
};

void push_stack(stack_type * self, int val){
    frame * new_top = malloc(sizeof(frame));
    if (self->bottom == NULL && self->top == NULL){
        printf("BOTTOM WAS INITIALIZED \n");
        self->bottom = new_top;
    }
    new_top->bottom = self->top;
    self->top = new_top;
    new_top->value = val;
    new_top->up = NULL;
    self->size++;
    //printf("NEW TOP VALUE AFTER PUSH %d \n",((frame*)(self->top)->value));
}

frame * lookup_stack(stack_type * self){
    return self->top;
}

void initialize_parsing_table_and_linear_proble() {
    //fill the table with the corresponding productions 

    printf("Called initialize_parsing_table_and_linear_proble \n");
    fflush(stdout);

    for(int i = 0; i < NUM_NON_TERMINALS;i++){
        for (int j = 0; j < NUMEXPRESSION; j++){
            parsing_table[i][j] = -1;
        }
    }

    parsing_table[S][CIFER % 100] = S_PROGRAM_CIFER;
    parsing_table[S][DEF % 100] = S_PROGRAM_CIFER;
    parsing_table[S][IDENT_ % 100] = S_PROGRAM_CIFER;
    parsing_table[S][LBRACE % 100] = S_PROGRAM_CIFER;
    parsing_table[S][INT % 100] = S_PROGRAM_CIFER;
    parsing_table[S][FLOAT % 100] = S_PROGRAM_CIFER;
    parsing_table[S][STRING % 100] = S_PROGRAM_CIFER;
    parsing_table[S][TINT % 100] = S_PROGRAM_CIFER;
    parsing_table[S][TFLOAT % 100] = S_PROGRAM_CIFER;
    parsing_table[S][TSTRING % 100] = S_PROGRAM_CIFER;
    parsing_table[S][SEMICOLON % 100] = S_PROGRAM_CIFER;
    parsing_table[S][BREAK % 100] = S_PROGRAM_CIFER;
    parsing_table[S][PRINT % 100] = S_PROGRAM_CIFER;
    parsing_table[S][READ % 100] = S_PROGRAM_CIFER;
    parsing_table[S][RETURN % 100] = S_PROGRAM_CIFER;
    parsing_table[S][IF % 100] = S_PROGRAM_CIFER;
    parsing_table[S][ELSE % 100] = S_PROGRAM_CIFER;
    parsing_table[S][FOR % 100] = S_PROGRAM_CIFER;

    parsing_table[PROGRAM][CIFER % 100] = PROGRAM_EMPTY;
    parsing_table[PROGRAM][DEF % 100] = PROGRAM_FUNCLIST;
    parsing_table[PROGRAM][IDENT_ % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][PRINT % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][READ % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][RETURN % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][IF % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][FOR % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][SEMICOLON % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][BREAK % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][TSTRING % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][TFLOAT % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][TINT % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][STRING % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][FLOAT % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][INT % 100] = PROGRAM_STATEMENT;
    parsing_table[PROGRAM][LBRACE % 100] = PROGRAM_STATEMENT;

    parsing_table[FUNCLIST][DEF % 100] = FUNCLIST_FUNCDEF_FUNCLIST_PRIME;

    parsing_table[FUNCLIST_PRIME][CIFER % 100] = FUNCLIST_PRIME_EMPTY;

    parsing_table[FUNCDEF][DEF % 100] = FUNCDEF_DEF_IDENT_LPAREN_PARAMLIST_RPAREN_LBRACE_STATELIST_RBRACE;

    parsing_table[PARAMLIST][INT % 100] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][FLOAT % 100] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][STRING % 100] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][TINT % 100] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][TFLOAT % 100] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;
    parsing_table[PARAMLIST][TSTRING % 100] = PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL;

    parsing_table[PARAMLIST_TAIL][RPAREN % 100] = PARAMLIST_TAIL_EMPTY;
    parsing_table[PARAMLIST_TAIL][COMMA % 100] = PARAMLIST_TAIL_COMMA_PARAMLIST;

    parsing_table[PARAMLIST_TYPES][INT % 100] = PARAMLIST_TYPES_INT;
    parsing_table[PARAMLIST_TYPES][FLOAT % 100] = PARAMLIST_TYPES_FLOAT;
    parsing_table[PARAMLIST_TYPES][STRING % 100] = PARAMLIST_TYPES_STRING;
    parsing_table[PARAMLIST_TYPES][TINT % 100] = PARAMLIST_TYPES_TINT;
    parsing_table[PARAMLIST_TYPES][TFLOAT % 100] = PARAMLIST_TYPES_TFLOAT;
    parsing_table[PARAMLIST_TYPES][TSTRING % 100] = PARAMLIST_TYPES_TSTRING;

    parsing_table[STATEMENT][IDENT_ % 100] = STATEMENT_ATRIBSTAT_SEMICOLON;
    parsing_table[STATEMENT][LBRACE % 100] = STATEMENT_LBRACE_STATELIST_RBRACE_SEMICOLON;
    parsing_table[STATEMENT][INT % 100] = STATEMENT_VARDECL_SEMICOLON;
    parsing_table[STATEMENT][FLOAT % 100] = STATEMENT_VARDECL_SEMICOLON;
    parsing_table[STATEMENT][STRING % 100] = STATEMENT_VARDECL_SEMICOLON;
    parsing_table[STATEMENT][TINT % 100] = STATEMENT_TUPLEDECL_SEMICOLON;
    parsing_table[STATEMENT][TFLOAT % 100] = STATEMENT_TUPLEDECL_SEMICOLON;
    parsing_table[STATEMENT][TSTRING % 100] = STATEMENT_TUPLEDECL_SEMICOLON;
    parsing_table[STATEMENT][SEMICOLON % 100] = STATEMENT_SEMICOLON;
    parsing_table[STATEMENT][BREAK % 100] = STATEMENT_BREAK_SEMICOLON;
    parsing_table[STATEMENT][PRINT % 100] = STATEMENT_PRINTSTAT_SEMICOLON;
    parsing_table[STATEMENT][READ % 100] = STATEMENT_READSTAT_SEMICOLON;
    parsing_table[STATEMENT][RETURN % 100] = STATEMENT_RETURNSTAT_SEMICOLON;
    parsing_table[STATEMENT][IF % 100] = STATEMENT_IFSTAT_SEMICOLON;
    parsing_table[STATEMENT][FOR % 100] = STATEMENT_FORSTAT_SEMICOLON;


    parsing_table[TUPLEDECL][TINT % 100] = TUPLEDECL_TTYPE_IDENT_LBRACKET_INT_CONSTANT_RBRACKET;
    parsing_table[TUPLEDECL][TFLOAT % 100] = TUPLEDECL_TTYPE_IDENT_LBRACKET_INT_CONSTANT_RBRACKET;
    parsing_table[TUPLEDECL][TSTRING % 100] = TUPLEDECL_TTYPE_IDENT_LBRACKET_INT_CONSTANT_RBRACKET;

    parsing_table[TTYPE][TINT % 100] = TTYPE_TINT;
    parsing_table[TTYPE][TFLOAT % 100] = TTYPE_TFLOAT;
    parsing_table[TTYPE][TSTRING % 100] = TTYPE_TSTRING;

    parsing_table[VARDECL][INT % 100] = VARDECL_TYPE_IDENT_VARDECL_TAIL;
    parsing_table[VARDECL][FLOAT % 100] = VARDECL_TYPE_IDENT_VARDECL_TAIL;
    parsing_table[VARDECL][STRING % 100] = VARDECL_TYPE_IDENT_VARDECL_TAIL;

    parsing_table[TYPE][INT % 100] = TYPE_INT;
    parsing_table[TYPE][FLOAT % 100] = TYPE_FLOAT;
    parsing_table[TYPE][STRING % 100] = TYPE_STRING;

    parsing_table[VARDECL_TAIL][SEMICOLON % 100] = VARDECL_TAIL_EMPTY; 
    parsing_table[VARDECL_TAIL][LBRACKET % 100] = VARDECL_TAIL_LBRACKET_INT_CONSTANT_RBRACKET_VARDECL_TAIL;

    parsing_table[ATRIBSTAT][IDENT_ % 100] = ATRIBSTAT_LVALUE_EQUAL_ATRIBSTAT_TAIL;

    parsing_table[ATRIBSTAT_TAIL][IDENT_LPAREN % 100] = ATRIBSTAT_TAIL_FUNCCALL; 
    parsing_table[ATRIBSTAT_TAIL][LPAREN % 100] = ATRIBSTAT_TAIL_EXPRESSION; 
    parsing_table[ATRIBSTAT_TAIL][NEW % 100] = ATRIBSTAT_TAIL_ALLOCEXPRESSION;

    parsing_table[FUNCCALL][IDENT_LPAREN % 100] = VARDECL_TAIL_LBRACKET_INT_CONSTANT_RBRACKET_VARDECL_TAIL; 
    
    parsing_table[PARAMLISTCALL][IDENT_ % 100] = PARAMLISTCALL_PARAMLISTCALL_TAIL; 
    parsing_table[PARAMLISTCALL][RPAREN % 100] = PARAMLISTCALL_EMPTY; 

    parsing_table[PARAMLISTCALL_TAIL][IDENT_ % 100] = PARAMLISTCALL_TAIL_IDENT_PARAMLISTCALL_CONT;

    parsing_table[PARAMLISTCALL_CONT][RPAREN % 100] = PARAMLISTCALL_CONT_EMPTY; 
    parsing_table[PARAMLISTCALL_CONT][COMMA % 100] = PARAMLISTCALL_CONT_COMMA_PARAMLISTCALL;

    parsing_table[PRINTSTAT][PRINT % 100] = PRINTSTAT_PRINT_EXPRESSION;
    parsing_table[READSTAT][READ % 100] = READSTAT_READ_LVALUE;
    parsing_table[RETURNSTAT][RETURN % 100] = RETURNSTAT_RETURN_IDENTRET;

    parsing_table[IDENTRET][IDENT_ % 100] = IDENTRET_IDENT; 
    parsing_table[IDENTRET][SEMICOLON % 100] = IDENTRET_EMPTY;

    parsing_table[IFSTAT][IF % 100] = IFSTAT_IF_LPAREN_EXPRESSION_RPAREN_STATEMENT_IF_TAIL;

    parsing_table[IF_TAIL][SEMICOLON % 100] = IF_TAIL_EMPTY; 
    parsing_table[IF_TAIL][ELSE % 100] = IF_TAIL_ELSE_STATEMENT;

    parsing_table[FORSTAT][FOR % 100] = FORSTAT_FOR_ATRIBSTAT_EXPRESSION_ATRIBSTAT_STATEMENT;

    parsing_table[STATELIST][IDENT_ % 100] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][IF % 100] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][FOR % 100] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][PRINT % 100] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][READ % 100] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][RETURN % 100] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][LBRACE % 100] = STATELIST_STATEMENT_STATELIST_TAIL; 
    parsing_table[STATELIST][BREAK % 100] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][INT % 100] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][FLOAT % 100] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][STRING % 100] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][TINT % 100] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][TFLOAT % 100] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][TSTRING % 100] = STATELIST_STATEMENT_STATELIST_TAIL;
    parsing_table[STATELIST][SEMICOLON % 100] = STATELIST_STATEMENT_STATELIST_TAIL;

    parsing_table[STATELIST_TAIL][IDENT_ % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][IF % 100] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][FOR % 100] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][PRINT % 100] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][READ % 100] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][RETURN % 100] = STATELIST_TAIL_STATELIST; 
    parsing_table[STATELIST_TAIL][LBRACE % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][BREAK % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][INT % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][FLOAT % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][STRING % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][TINT % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][TFLOAT % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][TSTRING % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][SEMICOLON % 100] = STATELIST_TAIL_STATELIST;
    parsing_table[STATELIST_TAIL][RBRACE % 100] = STATELIST_TAIL_EMPTY;

    parsing_table[ALLOCEXPRESSION][NEW % 100] = ALLOCEXPRESSION_NEW_TYPE_NUMEXPRESSION_ALLOCEXPRESSION_TAIL_CONT;

    parsing_table[ALLOCEXPRESSION_TAIL_CONT][SEMICOLON % 100] = ALLOCEXPRESSION_TAIL_CONT_EMPTY;
    parsing_table[ALLOCEXPRESSION_TAIL_CONT][RPAREN % 100] = ALLOCEXPRESSION_TAIL_CONT_EMPTY;
    parsing_table[ALLOCEXPRESSION_TAIL_CONT][LBRACKET % 100] = ALLOCEXPRESSION_TAIL_CONT_NUMEXPRESSION_ALLOCEXPRESSION_TAIL_CONT;

    parsing_table[BOL_EXPRESSION][LPAREN % 100] = BOL_EXPRESSION_EXPRESSION_BOL_EXPRESSION_TAIL; 
    parsing_table[BOL_EXPRESSION][NOT % 100] = BOL_EXPRESSION_NOT_BOL_EXPRESSION; 

    parsing_table[BOL_EXPRESSION_TAIL][RPAREN % 100] = BOL_EXPRESSION_TAIL_EMPTY; 
    parsing_table[BOL_EXPRESSION_TAIL][SEMICOLON % 100] = BOL_EXPRESSION_TAIL_EMPTY; 

    parsing_table[BOL_EXPRESSION_TAIL][AND % 100] = BOL_EXPRESSION_TAIL_BOOL_OP_EXPRESSION_BOL_EXPRESSION_TAIL;
    parsing_table[BOL_EXPRESSION_TAIL][OR % 100] = BOL_EXPRESSION_TAIL_BOOL_OP_EXPRESSION_BOL_EXPRESSION_TAIL; 
    parsing_table[BOL_EXPRESSION_TAIL][NOT % 100] = BOL_EXPRESSION_TAIL_BOOL_OP_EXPRESSION_BOL_EXPRESSION_TAIL;

    parsing_table[BOOL_OP][NOT % 100] = BOOL_OP_NOT;
    parsing_table[BOOL_OP][AND % 100] = BOOL_OP_AND;
    parsing_table[BOOL_OP][OR % 100] = BOOL_OP_OR;

    parsing_table[EXPRESSION][LPAREN % 100] = EXPRESSION_NUMEXPRESSION_EXPRESSION_TAIL;

    parsing_table[EXPRESSION_TAIL][RPAREN % 100] = EXPRESSION_TAIL_EMPTY; 
    parsing_table[EXPRESSION_TAIL][SEMICOLON % 100] = EXPRESSION_TAIL_EMPTY; 
    parsing_table[EXPRESSION_TAIL][LESS_THAN % 100] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][GREATER_THAN % 100] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][LESS_EQUAL % 100] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][GREATER_EQUAL % 100] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][EQUAL_EQUAL % 100] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 
    parsing_table[EXPRESSION_TAIL][NOT_EQUAL % 100] = EXPRESSION_TAIL_REL_OP_NUMEXPRESSION; 

    parsing_table[REL_OP][LESS_THAN % 100] = REL_OP_LESS_THAN;
    parsing_table[REL_OP][GREATER_THAN % 100] = REL_OP_GREATER_THAN;
    parsing_table[REL_OP][LESS_EQUAL % 100] = REL_OP_LESS_EQUAL;
    parsing_table[REL_OP][GREATER_EQUAL % 100] = REL_OP_GREATER_EQUAL;
    parsing_table[REL_OP][EQUAL_EQUAL % 100] = REL_OP_EQUAL_EQUAL;
    parsing_table[REL_OP][NOT_EQUAL % 100] = REL_OP_NOT_EQUAL;


    parsing_table[NUMEXPRESSION][IDENT_ % 100] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION][INT_CONSTANT % 100] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION][FLOAT_CONSTANT % 100] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION][LPAREN % 100] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION][PLUS % 100] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL;
    parsing_table[NUMEXPRESSION][MINUS % 100] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL;
    parsing_table[NUMEXPRESSION][STRING_CONSTANT % 100] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL;
    parsing_table[NUMEXPRESSION][NULL_LITERAL % 100] = NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL;

    parsing_table[NUMEXPRESSION_TAIL][RPAREN % 100] = NUMEXPRESSION_TAIL_EMPTY; 
    parsing_table[NUMEXPRESSION_TAIL][LESS_THAN % 100] = NUMEXPRESSION_TAIL_EMPTY; 
    parsing_table[NUMEXPRESSION_TAIL][GREATER_THAN % 100] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][LESS_EQUAL % 100] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][GREATER_EQUAL % 100] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][EQUAL_EQUAL % 100] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][NOT_EQUAL % 100] = NUMEXPRESSION_TAIL_EMPTY;
    parsing_table[NUMEXPRESSION_TAIL][PLUS % 100] = NUMEXPRESSION_TAIL_ADD_OP_TERM_NUMEXPRESSION_TAIL; 
    parsing_table[NUMEXPRESSION_TAIL][MINUS % 100] = NUMEXPRESSION_TAIL_ADD_OP_TERM_NUMEXPRESSION_TAIL; 

    parsing_table[ADD_OP][PLUS % 100] = ADD_OP_PLUS;
    parsing_table[ADD_OP][MINUS % 100] = ADD_OP_MINUS;

    parsing_table[TERM][IDENT_ % 100] = TERM_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM][INT_CONSTANT % 100] = TERM_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM][FLOAT_CONSTANT % 100] = TERM_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM][STRING_CONSTANT % 100] = TERM_UNARYEXPR_TERM_TAIL;
    parsing_table[TERM][LPAREN % 100] = TERM_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM][PLUS % 100] = TERM_UNARYEXPR_TERM_TAIL;
    parsing_table[TERM][MINUS % 100] = TERM_UNARYEXPR_TERM_TAIL;
    parsing_table[TERM][NULL_LITERAL % 100] = TERM_UNARYEXPR_TERM_TAIL;

    parsing_table[TERM_TAIL][RPAREN % 100] = TERM_TAIL_EMPTY; 
    parsing_table[TERM_TAIL][SEMICOLON % 100] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][PLUS % 100] = TERM_TAIL_EMPTY; 
    parsing_table[TERM_TAIL][MINUS % 100] = TERM_TAIL_EMPTY; 
    parsing_table[TERM_TAIL][LBRACKET % 100] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][LESS_THAN % 100] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][GREATER_THAN % 100] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][LESS_EQUAL % 100] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][GREATER_EQUAL % 100] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][EQUAL_EQUAL % 100] = TERM_TAIL_EMPTY;
    parsing_table[TERM_TAIL][NOT_EQUAL % 100] = TERM_TAIL_EMPTY;

    parsing_table[TERM_TAIL][MULTIPLY % 100] = TERM_TAIL_MULT_OP_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM_TAIL][DIVIDE % 100] = TERM_TAIL_MULT_OP_UNARYEXPR_TERM_TAIL; 
    parsing_table[TERM_TAIL][MODULO % 100] = TERM_TAIL_MULT_OP_UNARYEXPR_TERM_TAIL; 

    parsing_table[MULT_OP][MULTIPLY % 100] = MULT_OP_MULTIPLY; 
    parsing_table[MULT_OP][DIVIDE % 100] = MULT_OP_DIVIDE; 
    parsing_table[MULT_OP][MODULO % 100] = MULT_OP_MODULO; 

    parsing_table[UNARYEXPR][IDENT_ % 100] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][LPAREN % 100] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][INT_CONSTANT % 100] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][FLOAT_CONSTANT % 100] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][STRING_CONSTANT % 100] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][NULL_LITERAL % 100] = UNARYEXPR_FACTOR;
    parsing_table[UNARYEXPR][PLUS % 100] = UNARYEXPR_UNARY_OP_FACTOR; 
    parsing_table[UNARYEXPR][MINUS % 100] = UNARYEXPR_UNARY_OP_FACTOR; 

    parsing_table[UNARY_OP][PLUS % 100] = UNARY_OP_PLUS; 
    parsing_table[UNARY_OP][MINUS % 100] = UNARY_OP_MINUS; 

    parsing_table[FACTOR][IDENT_ % 100] = FACTOR_LVALUE; 
    parsing_table[FACTOR][LPAREN % 100] = FACTOR_LPAREN_NUMEXPRESSION_RPAREN; 
    parsing_table[FACTOR][INT_CONSTANT % 100] = FACTOR_INT_CONSTANT; 
    parsing_table[FACTOR][FLOAT_CONSTANT % 100] = FACTOR_FLOAT_CONSTANT;
    parsing_table[FACTOR][STRING_CONSTANT % 100] = FACTOR_STRING_CONSTANT;
    parsing_table[FACTOR][NULL_LITERAL % 100] = FACTOR_NULL_LITERAL;

    parsing_table[LVALUE][IDENT_ % 100] = LVALUE_IDENT_LVALUE_TAIL;

    parsing_table[LVALUE_TAIL][RPAREN % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][SEMICOLON % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][LESS_THAN % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][GREATER_THAN % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][LESS_EQUAL % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][GREATER_EQUAL % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][EQUAL_EQUAL % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][NOT_EQUAL % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][PLUS % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][MINUS % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][MULTIPLY % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][DIVIDE % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][MODULO % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][LBRACKET % 100] = LVALUE_TAIL_LBRACKET_NUMEXPRESSION_RBRACKET_LVALUE_TAIL;
    parsing_table[LVALUE_TAIL][RBRACKET % 100] = LVALUE_TAIL_EMPTY;
    parsing_table[LVALUE_TAIL][EQUAL % 100] = LVALUE_TAIL_EMPTY;

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
    insert("int", INT);
    insert("float", FLOAT);
    insert("string", STRING);
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
    } else if (token->token_type == INT_E){
        token->token_num = INT_CONSTANT;
    } else if(token->token_type == FLOAT_E){
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

    printf("TOKEN NUM = 0 IS LEXEM %d", token->token_num);

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
    fflush(stdout);

    stack_type * stack = malloc(sizeof(stack_type));
    stack->bottom = NULL;
    stack->top = NULL;
    stack->pop_stack = &pop_stack;
    stack->push_stack = &push_stack;
    stack->lookup_stack = &lookup_stack;

    Token * cifer_token = malloc(sizeof(Token));
    cifer_token->token_num = CIFER;
    cifer_token->token_type = OUTRO;
    cifer_token->token_str = "$";
    token_list = g_list_append(token_list,cifer_token);

    int token_list_size = g_list_length(token_list);

    for (int i = 0; i < token_list_size; i++){
        //GList* list = g_list_nth(token_list,i);
        printf("TOKEN : %d \n", ((Token*)g_list_nth(token_list,i)->data)->token_num);
    }

    stack->push_stack(stack, CIFER);
    stack->push_stack(stack, S);

    printf(" Initialized stack \n ");
    fflush(stdout);
    


    while (1){

        frame * stack_top = stack->lookup_stack(stack);
        GList * first_token_l = g_list_first(token_list);
        Token * first_token = (Token*)(first_token_l)->data;

        printf("stack top value %d \n",stack_top->value);
        printf("First token value %d \n", first_token->token_num);    

        if (stack_top->value == CIFER && first_token->token_num == CIFER){
                printf(" 1 \n ");
                fflush(stdout);
            return 1;
        }

        if (stack_top->value == first_token->token_num){

                printf(" 2 \n ");
                fflush(stdout);

            stack->pop_stack(stack);

        
            token_list = g_list_remove(token_list,first_token);

            int value = ((Token*)g_list_first(token_list))->token_num;

            for (int i = 0; i < g_list_length(token_list); i++){
                //GList* list = g_list_nth(token_list,i);
                printf("TOKEN : %d \n", ((Token*)g_list_nth(token_list,i)->data)->token_num);
            }


        } else if (stack_top->value < 99 && parsing_table[stack_top->value % 100][first_token->token_num % 100] == -1){
            printf("RETURNED -1 here");
            return -1;

        } else if(parsing_table[stack_top->value % 100][first_token->token_num % 100] != -1){

                printf(" 5 \n ");
                fflush(stdout);
            int rule = parsing_table[stack_top->value % 100][first_token->token_num % 100];

            printf("NON-TERMINAL %d",stack_top->value % 100);
            printf("TERMINAL %d",first_token->token_num % 100);
            printf("RULE %d \n", rule);

            stack->pop_stack(stack);
            add_production_to_stack_in_reverse(rule,stack);

            //printf("stack_top at after push %d \n", ((frame*)stack->lookup_stack(stack))->value);

        } else{
            printf("RETURNED -1 else");
            return -1;
        }

    }
    return 1;
}


void add_production_to_stack_in_reverse(int production, stack_type * stack){
    printf("production %d \n", production);
    switch (production)
    {
    case S_PROGRAM_CIFER:
        stack->push_stack(stack,PROGRAM);
        break;
    
    case PROGRAM_FUNCLIST:
        push_stack(stack,FUNCLIST);
        break;
    
    case PROGRAM_STATEMENT:
        push_stack(stack,STATEMENT);
        break;
    
    case PROGRAM_EMPTY:
        break;

    case FUNCLIST_FUNCDEF_FUNCLIST_PRIME:
        push_stack(stack,FUNCLIST_PRIME);
        push_stack(stack,FUNCDEF);
        break;

    case FUNCLIST_PRIME_FUNCDEF_FUNCLIST_PRIME:
        push_stack(stack,FUNCLIST_PRIME);
        push_stack(stack,FUNCDEF);

    case FUNCLIST_PRIME_EMPTY:
        break;

    case FUNCDEF_DEF_IDENT_LPAREN_PARAMLIST_RPAREN_LBRACE_STATELIST_RBRACE:
        push_stack(stack,RBRACE);
        push_stack(stack,STATELIST);
        push_stack(stack,LBRACE);
        push_stack(stack,RPAREN);
        push_stack(stack,PARAMLIST);
        push_stack(stack,LPAREN);
        push_stack(stack,IDENT_);
        push_stack(stack,DEF);
        break;

    case PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL:
        push_stack(stack, PARAMLIST_TAIL);
        push_stack(stack, IDENT_);
        push_stack(stack, PARAMLIST_TYPES);
        break;

    case PARAMLIST_TAIL_COMMA_PARAMLIST:
        push_stack(stack, PARAMLIST);
        push_stack(stack, COMMA);
        break;

    case PARAMLIST_TAIL_EMPTY:
        break;

    case STATEMENT_VARDECL_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, VARDECL);
        break;

    case STATEMENT_TUPLEDECL_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, TUPLEDECL);
        break;

    case STATEMENT_ATRIBSTAT_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, ATRIBSTAT);
        break;

    case STATEMENT_PRINTSTAT_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, PRINTSTAT);
        break;

    case STATEMENT_READSTAT_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, READSTAT);
        break;

    case STATEMENT_RETURNSTAT_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, RETURNSTAT);
        break;

    case STATEMENT_IFSTAT_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, IFSTAT);
        break;

    case STATEMENT_FORSTAT_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, FORSTAT);
        break;

    case STATEMENT_LBRACE_STATELIST_RBRACE_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, RBRACE);
        push_stack(stack, STATELIST);
        push_stack(stack, LBRACE);
        break;

    case STATEMENT_BREAK_SEMICOLON:
        push_stack(stack, SEMICOLON);
        push_stack(stack, BREAK);
        break;

    case STATEMENT_SEMICOLON:
        push_stack(stack, SEMICOLON);
        break;

    case TUPLEDECL_TTYPE_IDENT_LBRACKET_INT_CONSTANT_RBRACKET:
        push_stack(stack, RBRACKET);
        push_stack(stack, INT_CONSTANT);
        push_stack(stack, LBRACKET);
        push_stack(stack, IDENT_);
        push_stack(stack, TTYPE);
        break;

    case TTYPE_TINT:
        push_stack(stack, TINT);
        break;

    case TTYPE_TFLOAT:
        push_stack(stack, TFLOAT);
        break;

    case TTYPE_TSTRING:
        push_stack(stack, TSTRING);
        break;

    case VARDECL_TYPE_IDENT_VARDECL_TAIL:
        push_stack(stack, VARDECL_TAIL);
        push_stack(stack, IDENT_);
        push_stack(stack, TYPE);
        break;

    case VARDECL_TAIL_LBRACKET_INT_CONSTANT_RBRACKET_VARDECL_TAIL:
        push_stack(stack, VARDECL_TAIL);
        push_stack(stack, RBRACKET);
        push_stack(stack, INT_CONSTANT);
        push_stack(stack, LBRACKET);
        break;

    case VARDECL_TAIL_EMPTY:
        break;

    case TYPE_INT:
        push_stack(stack, INT);
        break;

    case TYPE_FLOAT:
        push_stack(stack, FLOAT);
        break;

    case TYPE_STRING:
        push_stack(stack, STRING);
        break;

    case ATRIBSTAT_LVALUE_EQUAL_ATRIBSTAT_TAIL:
        push_stack(stack, ATRIBSTAT_TAIL);
        push_stack(stack, EQUAL);
        push_stack(stack, LVALUE);
        break;

    case ATRIBSTAT_TAIL_EXPRESSION:
        push_stack(stack, EXPRESSION);
        break;

    case ATRIBSTAT_TAIL_ALLOCEXPRESSION:
        push_stack(stack, ALLOCEXPRESSION);
        break;

    case ATRIBSTAT_TAIL_FUNCCALL:
        push_stack(stack, FUNCCALL);
        break;

    case FUNCCALL_IDENTLPAREN_PARAMLISTCALL_RPAREN:
        push_stack(stack, RPAREN);
        push_stack(stack, PARAMLISTCALL);
        push_stack(stack, LPAREN);
        push_stack(stack, IDENT_);
        break;
    
    case PARAMLISTCALL_PARAMLISTCALL_TAIL:
        push_stack(stack, PARAMLISTCALL_TAIL);
        break;

    case PARAMLISTCALL_EMPTY:
        break;

    case PARAMLISTCALL_TAIL_IDENT_PARAMLISTCALL_CONT:
        push_stack(stack, PARAMLISTCALL_CONT);
        push_stack(stack, IDENT_);
        break;

    case PARAMLISTCALL_CONT_COMMA_PARAMLISTCALL:
        push_stack(stack, PARAMLISTCALL);
        push_stack(stack, COMMA);
        break;

    case PARAMLISTCALL_CONT_EMPTY:
        break;

    case PRINTSTAT_PRINT_EXPRESSION:
        push_stack(stack, EXPRESSION);
        push_stack(stack, PRINT);
        break;

    case READSTAT_READ_LVALUE:
        push_stack(stack, LVALUE);
        push_stack(stack, READ);
        break;

    case RETURNSTAT_RETURN_IDENTRET:
        push_stack(stack, IDENTRET);
        push_stack(stack, RETURN);
        break;

    case IDENTRET_IDENT:
        push_stack(stack, IDENT_);
        break;

    case IDENTRET_EMPTY:
        break;

    case IFSTAT_IF_LPAREN_EXPRESSION_RPAREN_STATEMENT_IF_TAIL:
        push_stack(stack, IF_TAIL);
        push_stack(stack, STATEMENT);
        push_stack(stack, RPAREN);
        push_stack(stack, EXPRESSION);
        push_stack(stack, LPAREN);
        push_stack(stack, IF);
        break;

    case IF_TAIL_ELSE_STATEMENT:
        push_stack(stack, STATEMENT);
        push_stack(stack, ELSE);
        break;

    case IF_TAIL_EMPTY:
        break;

    case FORSTAT_FOR_ATRIBSTAT_EXPRESSION_ATRIBSTAT_STATEMENT:
        push_stack(stack, STATEMENT);
        push_stack(stack, ATRIBSTAT);
        push_stack(stack, EXPRESSION);
        push_stack(stack, ATRIBSTAT);
        push_stack(stack, FOR);
        break;

    case STATELIST_STATEMENT_STATELIST_TAIL:
        push_stack(stack, STATELIST_TAIL);
        push_stack(stack, STATEMENT);
        break;

    case STATELIST_TAIL_STATELIST:
        push_stack(stack, STATELIST);
        break;

    case STATELIST_TAIL_EMPTY:
        break;

    case ALLOCEXPRESSION_NEW_TYPE_NUMEXPRESSION_ALLOCEXPRESSION_TAIL_CONT:
        push_stack(stack, ALLOCEXPRESSION_TAIL_CONT);
        push_stack(stack, NUMEXPRESSION);
        push_stack(stack, TYPE);
        push_stack(stack, NEW);
        break;

    case ALLOCEXPRESSION_TAIL_CONT_NUMEXPRESSION_ALLOCEXPRESSION_TAIL_CONT:
        push_stack(stack, ALLOCEXPRESSION_TAIL_CONT);
        push_stack(stack, NUMEXPRESSION);
        break;

    case ALLOCEXPRESSION_TAIL_CONT_EMPTY:
        break;

    case BOL_EXPRESSION_NOT_BOL_EXPRESSION:
        push_stack(stack, BOL_EXPRESSION);
        push_stack(stack, NOT);
        break;

    case BOL_EXPRESSION_EXPRESSION_BOL_EXPRESSION_TAIL:
        push_stack(stack, BOL_EXPRESSION_TAIL);
        push_stack(stack, EXPRESSION);
        break;

    case BOL_EXPRESSION_TAIL_BOOL_OP_EXPRESSION_BOL_EXPRESSION_TAIL:
        push_stack(stack, BOL_EXPRESSION_TAIL);
        push_stack(stack, EXPRESSION);
        push_stack(stack, BOOL_OP);
        break;

    case BOL_EXPRESSION_TAIL_EMPTY:
        break;

    case BOOL_OP_AND:
        push_stack(stack, AND);
        break;

    case BOOL_OP_OR:
        push_stack(stack, OR);
        break;

    case BOOL_OP_NOT:
        push_stack(stack, NOT);
        break;

    case EXPRESSION_NUMEXPRESSION_EXPRESSION_TAIL:
        push_stack(stack, EXPRESSION_TAIL);
        push_stack(stack, NUMEXPRESSION);
        break;

    case EXPRESSION_TAIL_REL_OP_NUMEXPRESSION:
        push_stack(stack, NUMEXPRESSION);
        push_stack(stack, REL_OP);
        break;

    case EXPRESSION_TAIL_EMPTY:
        break;

    case REL_OP_LESS_THAN:
        push_stack(stack, LESS_THAN);
        break;

    case REL_OP_GREATER_THAN:
        push_stack(stack, GREATER_THAN);
        break;

    case REL_OP_LESS_EQUAL:
        push_stack(stack, LESS_EQUAL);
        break;

    case REL_OP_GREATER_EQUAL:
        push_stack(stack, GREATER_EQUAL);
        break;

    case REL_OP_EQUAL_EQUAL:
        push_stack(stack, EQUAL_EQUAL);
        break;

    case REL_OP_NOT_EQUAL:
        push_stack(stack, NOT_EQUAL);
        break;

    case NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL:
        push_stack(stack, NUMEXPRESSION_TAIL);
        push_stack(stack, TERM);
        break;

    case NUMEXPRESSION_TAIL_ADD_OP_TERM_NUMEXPRESSION_TAIL:
        push_stack(stack, NUMEXPRESSION_TAIL);
        push_stack(stack, TERM);
        push_stack(stack, ADD_OP);
        break;

    case NUMEXPRESSION_TAIL_EMPTY:
        break;

    case ADD_OP_PLUS:
        push_stack(stack, PLUS);
        break;

    case ADD_OP_MINUS:
        push_stack(stack, MINUS);
        break;

    case TERM_UNARYEXPR_TERM_TAIL:
        push_stack(stack, TERM_TAIL);
        push_stack(stack, UNARYEXPR);
        break;

    case TERM_TAIL_MULT_OP_UNARYEXPR_TERM_TAIL:
        push_stack(stack, TERM_TAIL);
        push_stack(stack, UNARYEXPR);
        push_stack(stack, MULT_OP);
        break;

    case TERM_TAIL_EMPTY:
        break;

    case MULT_OP_MULTIPLY:
        push_stack(stack, MULTIPLY);
        break;

    case MULT_OP_DIVIDE:
        push_stack(stack, DIVIDE);
        break;

    case MULT_OP_MODULO:
        push_stack(stack, MODULO);
        break;

    case UNARYEXPR_UNARY_OP_FACTOR:
        push_stack(stack, FACTOR);
        push_stack(stack, UNARY_OP);
        break;

    case UNARYEXPR_FACTOR:
        push_stack(stack, FACTOR);
        break;

    case UNARY_OP_PLUS:
        push_stack(stack, PLUS);
        break;

    case UNARY_OP_MINUS:
        push_stack(stack, MINUS);
        break;

    case FACTOR_INT_CONSTANT:
        push_stack(stack, INT_CONSTANT);
        break;

    case FACTOR_FLOAT_CONSTANT:
        push_stack(stack, FLOAT_CONSTANT);
        break;

    case FACTOR_STRING_CONSTANT:
        push_stack(stack, STRING_CONSTANT);
        break;

    case FACTOR_NULL_LITERAL:
        push_stack(stack, NULL_LITERAL);
        break;

    case FACTOR_LVALUE:
        push_stack(stack, LVALUE);
        break;

    case FACTOR_LPAREN_NUMEXPRESSION_RPAREN:
        push_stack(stack, RPAREN);
        push_stack(stack, NUMEXPRESSION);
        push_stack(stack, LPAREN);
        break;

    case LVALUE_IDENT_LVALUE_TAIL:
        push_stack(stack, LVALUE_TAIL);
        push_stack(stack, IDENT_);
        break;

    case LVALUE_TAIL_LBRACKET_NUMEXPRESSION_RBRACKET_LVALUE_TAIL:
        push_stack(stack, LVALUE_TAIL);
        push_stack(stack, RBRACKET);
        push_stack(stack, NUMEXPRESSION);
        push_stack(stack, LBRACKET);
        break;

    case LVALUE_TAIL_EMPTY:
        break;

    case PARAMLIST_TYPES_INT:
        push_stack(stack, INT);
        break;

    case PARAMLIST_TYPES_FLOAT:
        push_stack(stack, FLOAT);
        break;

    case PARAMLIST_TYPES_STRING:
        push_stack(stack, STRING);
        break;

    case PARAMLIST_TYPES_TINT:
        push_stack(stack, TINT);
        break;

    case PARAMLIST_TYPES_TFLOAT:
        push_stack(stack, TFLOAT);
        break;

    case PARAMLIST_TYPES_TSTRING:
        push_stack(stack, TSTRING);
        break;
    
    default:
        break;
    }
    return;
}