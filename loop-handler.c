#include "compiler.h"
#include "ll1parser.h"
#include "loop-handler.h"


void init_stack(ContextStack *stack) {
    stack->top = -1;
}

int is_stack_empty(ContextStack *stack) {
    return stack->top == -1;
}

void push(ContextStack *stack, int context) {
    if (stack->top < MAX_STACK_SIZE - 1) {
        stack->stack[++stack->top] = context;
    } else {
        printf("Stack overflow error!\n");
        exit(1);
    }
}

void pop(ContextStack *stack) {
    if (!is_stack_empty(stack)) {
        stack->top--;
    }
}


void loop_checker(GList *tokens) {
    ContextStack context_stack;
    init_stack(&context_stack);
    int scope = 0;
    GList *current = tokens;
    while (current != NULL) {
        Token *token = (Token *)current->data;
        int value = token->token_num;

        switch (value) {
            case FOR:
                push(&context_stack, token->token_type);
                break;

            case RBRACE:
                if (!is_stack_empty(&context_stack)) {
                    pop(&context_stack);
                }
                scope--;
                break;

            case LBRACE:
                scope++;
                break;

            case BREAK:
                if (is_stack_empty(&context_stack)) {
                    list_t* entry = lookup_scope(token->token_str,scope);
                    RefList* lines = entry->lines;
                    printf("Semantic error: 'break' used outside of a loop at line %d\n", lines->line_number);
                }
                break;

            default:
                // Other tokens, do nothing
                break;
        }

        current = current->next;
    }
}