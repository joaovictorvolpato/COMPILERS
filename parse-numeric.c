#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include "parse-numeric.h"

#define INGONE -2147483648


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
ASTNode* createOperandNode(Tokens type, void* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_OPERAND;
    node->data.operand.val_type = type;
    
    switch(type) {
        case INT_E:
            node->data.operand.value.int_val = *(int*)value;
            break;
        case FLOAT_E:
            node->data.operand.value.float_val = *(float*)value;
            break;
        case STRING_E:
            node->data.operand.value.string_val = strdup((char*)value);
            break;
        case IDENT:
            list_t* entry = lookup((char*)value);
            node->data.operand.val_type = entry->type;
            printf("VALUE BEEING INSERTED INTO AST, %s \n", (char*)value);
            node->data.operand.ident_string = strdup((char*)value);
            switch (node->data.operand.val_type)
            {
            case INT_E:
                node->data.operand.value.int_val = INGONE;
                break;

            case FLOAT_E:
                node->data.operand.value.float_val = FLT_MIN;
                break;

            case STRING_E:
                node->data.operand.value.string_val = NULL;
                break;

            default:
                break;
            }
            
            break;
        case UNDEFINED:
            break;
        default:
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
            node = createOperandNode(INT_E, &value);
            break;
        }
        case FLOAT_CONSTANT: {
            char* val = token->token_str;
            float value = (float) atoll(val);
            node = createOperandNode(FLOAT_E, &value);
            break;
        }
        case STRING_CONSTANT: {
            char* val = token->token_str;
            node = createOperandNode(STRING_E, val);
            break;
        }
        case NULL_LITERAL:
            node = createOperandNode(UNDEFINED, NULL);
            break;
        case IDENT_: {
            char* val = token->token_str;
            node = createOperandNode(IDENT, val);
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
            case INT_E:
                if(node->data.operand.value.int_val == INGONE){
                    printf("Integer: %s\n", node->data.operand.ident_string);
                } else{
                    printf("Integer: %d\n", node->data.operand.value.int_val);
                }
                break;
            case FLOAT_E:
                if(node->data.operand.value.float_val == FLT_MIN){
                    printf("Float: %s\n", node->data.operand.ident_string);
                } else{
                    printf("Float: %f\n", node->data.operand.value.float_val);
                }
                break;
            case STRING_E:
                if(node->data.operand.value.string_val == NULL){
                    printf("String: %s\n", node->data.operand.ident_string);
                } else{
                    printf("String: %s\n", node->data.operand.value.string_val);
                }
                break;
            case IDENT:
                printf("Identifier: %s\n", node->data.operand.value.ident_val);
                break;
            case UNDEFINED:
                printf("Null\n");
                break;
            default:
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
        if (node->data.operand.val_type == STRING_E || 
            node->data.operand.val_type == IDENT) {
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
    
    case IDENT_:
    //Disambiguate for funccals and attrbuite statement
        index++;
        GList * first_token_l = g_list_nth(tokens,index);
        Token * token = (Token*)(first_token_l)->data;
        int value = token->token_num;
        printf("MATCHED IDENT: NEXT VAL IS ->, %s\n", token->token_str);
        if (value == EQUAL || value == LPAREN || value == LESS_THAN || 
            value == GREATER_EQUAL || value == GREATER_THAN || value == LESS_EQUAL ||
            value == NOT_EQUAL || value == EQUAL_EQUAL || value == SEMICOLON || value == RPAREN){
            return false;
        }
        return true;

    case INT_CONSTANT:
            //Disambiguate for funccals and attrbuite statement
        index++;
        GList * first_token_l_INT_C = g_list_nth(tokens,index);
        Token * token_INTC = (Token*)(first_token_l_INT_C)->data;
        int valueI = token_INTC->token_num;
        if (valueI == EQUAL || valueI == LPAREN || valueI == LESS_THAN || 
            valueI == GREATER_EQUAL || valueI == GREATER_THAN || valueI == LESS_EQUAL ||
            valueI == NOT_EQUAL || valueI == EQUAL_EQUAL || valueI == RPAREN){
            return false;
        }
        return true;

    case FLOAT_CONSTANT:
        return true;

    case STRING_CONSTANT:
        return true;
    
    default:
        return false;
    }
}

// Function to perform DFS and get the first val_type encountered
Tokens getFirstValueTypeDFS(ASTNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: Tree is empty.\n");
        exit(EXIT_FAILURE); // No valid tree
    }

    if (node->type == NODE_OPERAND) {
        return node->data.operand.val_type;
    }

    if (node->type == NODE_OPERATOR) {
        // Search the left subtree first
        if (node->data.operator.left) {
            Tokens left_val_type = getFirstValueTypeDFS(node->data.operator.left);
            return left_val_type; // Return as soon as a value is found
        }

        // If no value was found in the left subtree, search the right subtree
        if (node->data.operator.right) {
            Tokens right_val_type = getFirstValueTypeDFS(node->data.operator.right);
            return right_val_type; // Return as soon as a value is found
        }
    }

    fprintf(stderr, "Error: No operand nodes found in the tree.\n");
    exit(EXIT_FAILURE); // No operand nodes found
}

// Function to check the value_type of a single AST
bool checkASTValueType(const ASTNode* node, Tokens expected_type) {
    if (node == NULL) {
        return true; // Base case: null node is valid
    }

    if (node->type == NODE_OPERATOR) {
        // Recursively check left and right subtrees
        return checkASTValueType(node->data.operator.left, expected_type) &&
               checkASTValueType(node->data.operator.right, expected_type);
    } else if (node->type == NODE_OPERAND) {
        // Check if the operand's value_type matches the expected type
        return node->data.operand.val_type == expected_type;
    }

    return false; // Unexpected node type
}

// Function to check all ASTs in the array
bool assertASTArrayValueType(ASTNode** ast_array, int ast_count) {
    if (ast_array == NULL || ast_count == 0) {
        printf("No ASTs to check.\n");
        return true; // No ASTs, so we consider it valid
    }

    // Check each AST in the array
    for (int i = 0; i < ast_count; i++) {
        Tokens expected_type = getFirstValueTypeDFS(ast_array[i]);
        if (!checkASTValueType(ast_array[i], expected_type)) {
            printf("AST %d has mismatched value_type.\n", i + 1);
            return false;
        }
        printf("ASTs have consistent value_type: %d\n", expected_type);
    }

    return true;
}


// Example usage
int build_AST_for_numeric_expessions(GList * tokens_list) {
    int list_size = g_list_length(tokens_list);
    ASTNode** ast_array = malloc(list_size * sizeof(ASTNode*));
    if (!ast_array) {
        fprintf(stderr, "Memory allocation failed for AST array.\n");
        return -1;
    }
    int ast_count = 0;

    
    int index = 0;
;
    while (index < list_size){
        GList * first_token_l = g_list_nth(tokens_list,index);
        Token * token = (Token*)(first_token_l)->data;
        int value = token->token_num;
        bool is_token_in_numex_first = check_if_token_is_beggining_of_numex(tokens_list,index,value);
        if (is_token_in_numex_first == true){
            printf("BUILDING AST FOR EXPRESSION STARTING WITH, %s \n", token->token_str);
            ASTNode* ast = parseNumExpression(tokens_list, &index);
            ast_array[ast_count++] = ast; 
        }
        index++;
    }
    
    printf("Abstract Syntax Tree ast_count: %d\n",ast_count);
    assertASTArrayValueType(ast_array,ast_count);
    
    printf("Abstract Syntax Tree:\n");
    for (int i = 0; i < ast_count; i++) {
        printAST(ast_array[i], 0);
    }
    
    for (int i = 0; i < ast_count; i++) {
        freeAST(ast_array[i]);
    }
    
    return 0;
}