#include <glib.h>

#define MAX_STACK_SIZE 100

typedef struct {
    int stack[MAX_STACK_SIZE];
    int top;
} ContextStack;

// Inicializar a pilha
void init_stack(ContextStack *stack);
int is_stack_empty(ContextStack *stack);
void push(ContextStack *stack, int context);
void pop(ContextStack *stack);
int top(ContextStack *stack);
void handle_loop_start(ContextStack *context_stack);
void handle_loop_end(ContextStack *context_stack);
void handle_break(ContextStack *context_stack, int line_number);
void loop_checker(GList *tokens);