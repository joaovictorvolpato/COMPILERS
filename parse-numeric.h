#include "compiler.h"
#include "ll1parser.h"

#ifndef PARSE_NUM
#define PARSE_NUM

typedef struct SytaxTree SyntaxTree; // Forward reference

typedef enum {
    NODE_OPERATOR,
    NODE_OPERAND
} NodeType;

// Operator types
typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_UNARY_PLUS,
    OP_UNARY_MINUS
} OperatorType;

// Value types for operands
typedef enum {
    VAL_INT,
    VAL_FLOAT,
    VAL_STRING,
    VAL_NULL,
    VAL_IDENT
} ValueType;


// Token types
typedef enum {
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_NULL,
    TOKEN_IDENT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MODULO,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END
} TokenType;

// AST Node structure
typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            OperatorType op;
            struct ASTNode* left;
            struct ASTNode* right;
        } operator;
        struct {
            ValueType val_type;
            union {
                int int_val;
                float float_val;
                char* string_val;
                char* ident_val;
            } value;
        } operand;
    } data;
} ASTNode;

#endif

// Function prototypes
ASTNode* createOperatorNode(OperatorType op, ASTNode* left, ASTNode* right);
ASTNode* createOperandNode(ValueType type, void* value);
ASTNode* parseNumExpression(GList* tokens, int* current);
ASTNode* parseNumExpressionTail(GList* tokens, int* current, ASTNode* left);
ASTNode* parseTerm(GList* tokens, int* current);
ASTNode* parseTermTail(GList* tokens, int* current, ASTNode* left);
ASTNode* parseUnaryExpr(GList* tokens, int* current);
ASTNode* parseFactor(GList* tokens, int* current);
int build_AST_for_numeric_expessions(GList * tokens_list);
