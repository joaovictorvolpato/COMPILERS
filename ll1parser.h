#include "compiler.h"
#include <glib.h>
#include <stdbool.h>

#ifndef LL1_PARSER
#define LL1_PARSER

// Non-terminal symbols (starting from 0)
#define PROGRAM 0
#define FUNCLIST 1
#define FUNCLIST_PRIME 2
#define FUNCDEF 3
#define PARAMLIST 4
#define PARAMLIST_TAIL 5
#define PARAMLIST_TYPES 6
#define STATEMENT 7
#define TUPLEDECL 8
#define TTYPE 9
#define VARDECL 10
#define TYPE 11
#define VARDECL_TAIL 12
#define ATRIBSTAT 13
#define ATRIBSTAT_TAIL 14
#define FUNCCALL 15
#define PARAMLISTCALL 16
#define PARAMLISTCALL_TAIL 17
#define PARAMLISTCALL_CONT 18
#define PRINTSTAT 19
#define READSTAT 20
#define RETURNSTAT 21
#define IDENT_LPAREN 22
#define IFSTAT 23
#define IF_TAIL 24
#define FORSTAT 25
#define STATELIST 26
#define STATELIST_TAIL 27
#define ALLOCEXPRESSION 28
#define ALLOCEXPRESSION_TAIL_CONT 29
#define BOL_EXPRESSION 30
#define BOL_EXPRESSION_TAIL 31
#define BOOL_OP 32
#define EXPRESSION 33
#define EXPRESSION_TAIL 34
#define REL_OP 35
#define NUMEXPRESSION 36
#define NUMEXPRESSION_TAIL 37
#define ADD_OP 38
#define TERM 39
#define TERM_TAIL 40
#define MULT_OP 41
#define UNARYEXPR 42
#define UNARY_OP 43
#define FACTOR 44
#define LVALUE 45
#define LVALUE_TAIL 46
#define S 47
#define IDENTRET 48

// Terminal symbols (starting from 0)
#define IDENT_ 100
#define INT_CONSTANT 101
#define FLOAT_CONSTANT 102
#define STRING_CONSTANT 103
#define NULL_LITERAL 104
#define PRINT 105
#define READ 106
#define RETURN 107
#define IF 108
#define ELSE 109
#define FOR 110
#define BREAK 111
#define DEF 112
#define NEW 113
#define INT 114
#define FLOAT 115
#define STRING 116
#define TINT 117
#define TFLOAT 118
#define TSTRING 119
#define AND 120
#define OR 121
#define NOT 122
#define EQUAL 123
#define LESS_THAN 124
#define GREATER_THAN 125
#define LESS_EQUAL 126
#define GREATER_EQUAL 127
#define EQUAL_EQUAL 128
#define NOT_EQUAL 129
#define PLUS 130
#define MINUS 131
#define MULTIPLY 132
#define DIVIDE 133
#define MODULO 134
#define LPAREN 135
#define RPAREN 136
#define LBRACKET 137
#define RBRACKET 138
#define LBRACE 139
#define RBRACE 140
#define COMMA 141
#define SEMICOLON 142
#define CIFER 143

// Define the number of non-terminals and terminals
#define NUM_NON_TERMINALS 49  // Non-terminals are indexed from 0 to 48
#define NUM_TERMINALS 44      // Terminals are indexed from 0 to 43

// Productions
#define PROGRAM_STATEMENT 200
#define PROGRAM_FUNCLIST 201
#define PROGRAM_EMPTY 202
#define FUNCLIST_FUNCDEF_FUNCLIST_PRIME 203
#define FUNCLIST_PRIME_FUNCDEF_FUNCLIST_PRIME 204
#define FUNCLIST_PRIME_EMPTY 205
#define FUNCDEF_DEF_IDENT_LPAREN_PARAMLIST_RPAREN_LBRACE_STATELIST_RBRACE 206
#define PARAMLIST_PARAMLIST_TYPES_IDENT_PARAMLIST_TAIL 207
#define PARAMLIST_TAIL_COMMA_PARAMLIST 208
#define PARAMLIST_TAIL_EMPTY 209
#define STATEMENT_VARDECL_SEMICOLON 210
#define STATEMENT_TUPLEDECL_SEMICOLON 211
#define STATEMENT_ATRIBSTAT_SEMICOLON 212
#define STATEMENT_PRINTSTAT_SEMICOLON 213
#define STATEMENT_READSTAT_SEMICOLON 214
#define STATEMENT_RETURNSTAT_SEMICOLON 215
#define STATEMENT_IFSTAT_SEMICOLON 216
#define STATEMENT_FORSTAT_SEMICOLON 217
#define STATEMENT_LBRACE_STATELIST_RBRACE_SEMICOLON 218
#define STATEMENT_BREAK_SEMICOLON 219
#define STATEMENT_SEMICOLON 220
#define TUPLEDECL_TTYPE_IDENT_LBRACKET_INT_CONSTANT_RBRACKET 221
#define TTYPE_TINT 222
#define TTYPE_TFLOAT 223
#define TTYPE_TSTRING 224
#define VARDECL_TYPE_IDENT_VARDECL_TAIL 225
#define VARDECL_TAIL_LBRACKET_INT_CONSTANT_RBRACKET_VARDECL_TAIL 226
#define VARDECL_TAIL_EMPTY 227
#define TYPE_INT 228
#define TYPE_FLOAT 229
#define TYPE_STRING 230
#define ATRIBSTAT_LVALUE_EQUAL_ATRIBSTAT_TAIL 231
#define ATRIBSTAT_TAIL_EXPRESSION 232
#define ATRIBSTAT_TAIL_ALLOCEXPRESSION 233
#define ATRIBSTAT_TAIL_FUNCCALL 234
#define FUNCCALL_IDENTLPAREN_PARAMLISTCALL_RPAREN 235
#define PARAMLISTCALL_PARAMLISTCALL_TAIL 236
#define PARAMLISTCALL_EMPTY 237
#define PARAMLISTCALL_TAIL_IDENT_PARAMLISTCALL_CONT 238
#define PARAMLISTCALL_CONT_COMMA_PARAMLISTCALL 239
#define PARAMLISTCALL_CONT_EMPTY 240
#define PRINTSTAT_PRINT_EXPRESSION 241
#define READSTAT_READ_LVALUE 242
#define RETURNSTAT_RETURN_IDENTRET 243
#define IDENTRET_IDENT 244
#define IDENTRET_EMPTY 245
#define IFSTAT_IF_LPAREN_EXPRESSION_RPAREN_STATEMENT_IF_TAIL 246
#define IF_TAIL_ELSE_STATEMENT 247
#define IF_TAIL_EMPTY 248
#define FORSTAT_FOR_ATRIBSTAT_EXPRESSION_ATRIBSTAT_STATEMENT 249
#define STATELIST_STATEMENT_STATELIST_TAIL 250
#define STATELIST_TAIL_STATELIST 251
#define STATELIST_TAIL_EMPTY 252
#define ALLOCEXPRESSION_NEW_TYPE_NUMEXPRESSION_ALLOCEXPRESSION_TAIL_CONT 253
#define ALLOCEXPRESSION_TAIL_CONT_NUMEXPRESSION_ALLOCEXPRESSION_TAIL_CONT 254
#define ALLOCEXPRESSION_TAIL_CONT_EMPTY 255
#define BOL_EXPRESSION_NOT_BOL_EXPRESSION 256
#define BOL_EXPRESSION_EXPRESSION_BOL_EXPRESSION_TAIL 257
#define BOL_EXPRESSION_TAIL_BOOL_OP_EXPRESSION_BOL_EXPRESSION_TAIL 258
#define BOL_EXPRESSION_TAIL_EMPTY 259
#define BOOL_OP_AND 260
#define BOOL_OP_OR 261
#define BOOL_OP_NOT 262
#define EXPRESSION_NUMEXPRESSION_EXPRESSION_TAIL 263
#define EXPRESSION_TAIL_REL_OP_NUMEXPRESSION 264
#define EXPRESSION_TAIL_EMPTY 265
#define REL_OP_LESS_THAN 266
#define REL_OP_GREATER_THAN 267
#define REL_OP_LESS_EQUAL 268
#define REL_OP_GREATER_EQUAL 269
#define REL_OP_EQUAL_EQUAL 270
#define REL_OP_NOT_EQUAL 271
#define NUMEXPRESSION_TERM_NUMEXPRESSION_TAIL 272
#define NUMEXPRESSION_TAIL_ADD_OP_TERM_NUMEXPRESSION_TAIL 273
#define NUMEXPRESSION_TAIL_EMPTY 274
#define ADD_OP_PLUS 275
#define ADD_OP_MINUS 276
#define TERM_UNARYEXPR_TERM_TAIL 277
#define TERM_TAIL_MULT_OP_UNARYEXPR_TERM_TAIL 278
#define TERM_TAIL_EMPTY 279
#define MULT_OP_MULTIPLY 280
#define MULT_OP_DIVIDE 281
#define MULT_OP_MODULO 282
#define UNARYEXPR_UNARY_OP_FACTOR 283
#define UNARYEXPR_FACTOR 284
#define UNARY_OP_PLUS 285
#define UNARY_OP_MINUS 286
#define FACTOR_INT_CONSTANT 287
#define FACTOR_FLOAT_CONSTANT 288
#define FACTOR_STRING_CONSTANT 289
#define FACTOR_NULL_LITERAL 290
#define FACTOR_LVALUE 291
#define FACTOR_LPAREN_NUMEXPRESSION_RPAREN 292
#define LVALUE_IDENT_LVALUE_TAIL 293
#define LVALUE_TAIL_LBRACKET_NUMEXPRESSION_RBRACKET_LVALUE_TAIL 294
#define LVALUE_TAIL_EMPTY 295
#define S_PROGRAM_CIFER 296
#define PARAMLIST_TYPES_INT 297
#define PARAMLIST_TYPES_FLOAT 298
#define PARAMLIST_TYPES_STRING 299
#define PARAMLIST_TYPES_TINT 300
#define PARAMLIST_TYPES_TFLOAT 301
#define PARAMLIST_TYPES_TSTRING 302

#define TABLE_SIZE 256
// Hash table

typedef struct {
    const char* lexeme;
    int token_value;
} HashEntry;

#endif
char* pase_next_token(Token * token);
void initialize_parsing_table_and_linear_proble();

int do_ll1_parse(GList * token_list);
unsigned int hash_lexeme(const char * lexem);
void find_token_num(Token* token);
void insert(const char* token_lexeme, int token_value);
void free_table();

