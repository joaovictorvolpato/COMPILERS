#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "compiler.h"

#define MAX_TOKENS 10000
#define MAX_CODE_LINES 50000
#define MAX_TEMP 1000
#define MAX_LABEL 1000
#define MAX_FUNC 100
#define MAX_PARAMS 50

typedef struct {
    char* name;
    char* params[MAX_PARAMS];
    int param_count;
} FunctionInfo;

// Estrutura para código intermediário
typedef struct {
    char* code[MAX_CODE_LINES];
    int line_count;
} IntermediateCode;

// Contexto do gerador de código
typedef struct {
    Token* tokens;
    int token_count;
    int current_token;
    IntermediateCode* intermediate_code;
    int temp_count;
    int label_count;
    FunctionInfo functions[MAX_FUNC];
    int function_count;
} CodeGenContext;



// Protótipos de funções
char* newTemp(CodeGenContext* ctx);
char* newLabel(CodeGenContext* ctx);
void emitCode(CodeGenContext* ctx, const char* code);
char* generateExpression(CodeGenContext* ctx);
char* generateNumExpression(CodeGenContext* ctx);
char* generateTerm(CodeGenContext* ctx);
void generateStatement(CodeGenContext* ctx);
void generateProgram(CodeGenContext* ctx);
void generateControlFlow(CodeGenContext* ctx);
void generateFunctionCall(CodeGenContext* ctx);
void generateFunctionDefinition(CodeGenContext* ctx);
int generetaIntCode(GList * tokens);