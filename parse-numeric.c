#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse-numeric.h"



// Create an operator node
ASTNode* createOperatorNode(OperatorType op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_OPERATOR;
    node->data.operator.op = op;
    node->data.operator.left = left;
    node->data.operator.right = right;
    return node;
}

// Create an operand node
ASTNode* createOperandNode(ValueType type, void* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_OPERAND;
    node->data.operand.val_type = type;
    
    switch(type) {
        case VAL_INT:
            node->data.operand.value.int_val = *(int*)value;
            break;
        case VAL_FLOAT:
            node->data.operand.value.float_val = *(float*)value;
            break;
        case VAL_STRING:
            node->data.operand.value.string_val = strdup((char*)value);
            break;
        case VAL_IDENT:
            node->data.operand.value.ident_val = strdup((char*)value);
            break;
        case VAL_NULL:
            break;
    }
    return node;
}

// Parse numeric expression
ASTNode* parseNumExpression(GList * tokens, int* current) {
    ASTNode* term = parseTerm(tokens, current);
    return parseNumExpressionTail(tokens, current, term);
}

// Parse numeric expression tail
ASTNode* parseNumExpressionTail(GList* tokens, int* current, ASTNode* left) {
    GList * first_token_l = g_list_nth(tokens,*current);
    Token * token = (Token*)(first_token_l)->data;
    int value = token->token_num;
    if (value == PLUS || value == MINUS) {
        OperatorType op = (value == PLUS) ? OP_ADD : OP_SUB;
        (*current)++;
        
        ASTNode* term = parseTerm(tokens, current);
        ASTNode* curr = createOperatorNode(op, left, term);
        
        return parseNumExpressionTail(tokens, current, curr);
    }
    return left;
}

// Parse term
ASTNode* parseTerm(GList* tokens, int* current) {
    ASTNode* unaryExpr = parseUnaryExpr(tokens, current);
    return parseTermTail(tokens, current, unaryExpr);
}

// Parse term tail
ASTNode* parseTermTail(GList* tokens, int* current, ASTNode* left) {
    GList * first_token_l = g_list_nth(tokens,*current);
    Token * token = (Token*)(first_token_l)->data;
    int value = token->token_num;
    if (value == MULTIPLY || 
        value == DIVIDE || 
        value == MODULO) {
        
        OperatorType op;
        switch(value) {
            case MULTIPLY: op = OP_MUL; break;
            case DIVIDE: op = OP_DIV; break;
            case MODULO: op = OP_MOD; break;
            default: return left;
        }
        
        (*current)++;
        ASTNode* unaryExpr = parseUnaryExpr(tokens, current);
        ASTNode* curr = createOperatorNode(op, left, unaryExpr);
        
        return parseTermTail(tokens, current, curr);
    }
    return left;
}

// Parse unary expression
ASTNode* parseUnaryExpr(GList* tokens, int* current) {
    GList * first_token_l = g_list_nth(tokens,*current);
    Token * token = (Token*)(first_token_l)->data;
    int value = token->token_num;
    if (value == PLUS || value == MINUS) {
        OperatorType op = (value == PLUS) ? OP_UNARY_PLUS : OP_UNARY_MINUS;
        (*current)++;
        
        ASTNode* factor = parseFactor(tokens, current);
        return createOperatorNode(op, NULL, factor);
    }
    return parseFactor(tokens, current);
}

// Parse factor
ASTNode* parseFactor(GList* tokens, int* current) {
    ASTNode* node = NULL;
    GList * first_token_l = g_list_nth(tokens,*current);
    Token * token = (Token*)(first_token_l)->data;
    int value = token->token_num;

    switch(value) {
        case INT_CONSTANT: {
            char* val = token->token_str;
            int value = (int) atoll(val);
            printf("Value of value %d", value);
            fflush(stdout);
            node = createOperandNode(VAL_INT, &value);
            break;
        }
        case FLOAT_CONSTANT: {
            char* val = token->token_str;
            float value = (float) atoll(val);
            node = createOperandNode(VAL_FLOAT, &value);
            break;
        }
        case STRING_CONSTANT: {
            char* val = token->token_str;
            node = createOperandNode(VAL_STRING, val);
            break;
        }
        case NULL_LITERAL:
            node = createOperandNode(VAL_NULL, NULL);
            break;
        case IDENT_: {
            char* val = token->token_str;
            node = createOperandNode(VAL_IDENT, val);
            break;
        }
        case LPAREN: {
            (*current)++;
            node = parseNumExpression(tokens, current);
            if (token->token_num != RPAREN) {
                // Error handling: missing closing parenthesis
                return NULL;
            }
            break;
        }
        default:
            // Error handling: unexpected token
            return NULL;
    }
    
    (*current)++;
    return node;
}

// Print AST (for debugging)
void printAST(ASTNode* node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    
    if (node->type == NODE_OPERATOR) {
        const char* opNames[] = {"+", "-", "*", "/", "%", "U+", "U-"};
        printf("Operator: %s\n", opNames[node->data.operator.op]);
        printAST(node->data.operator.left, depth + 1);
        printAST(node->data.operator.right, depth + 1);
    } else {
        switch(node->data.operand.val_type) {
            case VAL_INT:
                printf("Integer: %d\n", node->data.operand.value.int_val);
                break;
            case VAL_FLOAT:
                printf("Float: %f\n", node->data.operand.value.float_val);
                break;
            case VAL_STRING:
                printf("String: %s\n", node->data.operand.value.string_val);
                break;
            case VAL_IDENT:
                printf("Identifier: %s\n", node->data.operand.value.ident_val);
                break;
            case VAL_NULL:
                printf("Null\n");
                break;
        }
    }
}

// Free AST memory
void freeAST(ASTNode* node) {
    if (!node) return;
    
    if (node->type == NODE_OPERATOR) {
        freeAST(node->data.operator.left);
        freeAST(node->data.operator.right);
    } else {
        if (node->data.operand.val_type == VAL_STRING || 
            node->data.operand.val_type == VAL_IDENT) {
            free(node->data.operand.value.string_val);
        }
    }
    
    free(node);
}

bool check_if_token_is_beggining_of_numex(GList* tokens, int index, int token_value){
    switch (token_value)
    {
    case NULL_LITERAL:
        return true;

    case PLUS:
        return true;

    case MINUS:
        return true;
    
    case IDENT_:
    //Disambiguate for funccals and attrbuite statement
        index++;
        GList * first_token_l = g_list_nth(tokens,index);
        Token * token = (Token*)(first_token_l)->data;
        int value = token->token_num;
        if (value == EQUAL || value == LPAREN){
            return false;
        }
        return true;

    case INT_CONSTANT:
        return true;

    case FLOAT_CONSTANT:
        return true;

    case STRING_CONSTANT:
        return true;

    case LPAREN:
        return true;
    
    default:
        return false;
    }
}

// Example usage
int build_AST_for_numeric_expessions(GList * tokens_list) {
    ASTNode* ast;
    int list_size = g_list_length(tokens_list);
    int index = 0;
    printf("List size %d", list_size);
    printf("Called build AST\n");
    fflush(stdout);
    while (index < list_size){
        GList * first_token_l = g_list_nth(tokens_list,index);
        Token * token = (Token*)(first_token_l)->data;
        int value = token->token_num;
        bool is_token_in_numex_first = check_if_token_is_beggining_of_numex(tokens_list,index,value);
        if (is_token_in_numex_first == true){
            ast = parseNumExpression(tokens_list, &index);
        }
        index++;
    }
    
    
    printf("Abstract Syntax Tree:\n");
    printAST(ast, 0);
    
    freeAST(ast);
    
    return 0;
}