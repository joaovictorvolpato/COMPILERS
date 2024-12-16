
#include "intermidiate-code-generator.h"


char* newTemp(CodeGenContext* ctx) {
    char* temp = malloc(30);
    sprintf(temp, "t%d", ctx->temp_count++);
    return temp;
}

char* newLabel(CodeGenContext* ctx) {
    char* label = malloc(10);
    sprintf(label, "L%d", ctx->label_count++);
    return label;
}

void emitCode(CodeGenContext* ctx, const char* code) {
    //printf("emitted code: %s \n",code);
    if (ctx->intermediate_code->line_count < MAX_CODE_LINES) {
        ctx->intermediate_code->code[ctx->intermediate_code->line_count++] = strdup(code);
    }
}

char* generateNumExpression(CodeGenContext* ctx) {
    char* place = generateTerm(ctx);
    //printf("CURRENT TOKEN NumExpression1 %s \n",ctx->tokens[ctx->current_token].token_str);
    
    while (strcmp(ctx->tokens[ctx->current_token].token_str, "+") == 0 ||
           strcmp(ctx->tokens[ctx->current_token].token_str, "-") == 0 ||
           strcmp(ctx->tokens[ctx->current_token].token_str, "*") == 0 ||
           strcmp(ctx->tokens[ctx->current_token].token_str, "/") == 0 ||
           strcmp(ctx->tokens[ctx->current_token].token_str, "%") == 0 ){
        char* op = ctx->tokens[ctx->current_token].token_str;
        //printf("CURRENT TOKEN NumExpression2 %s \n",ctx->tokens[ctx->current_token].token_str);
        ctx->current_token++;
        //printf("CURRENT TOKEN NumExpression3 %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
        
        char* place2 = generateTerm(ctx);
        //printf("CURRENT TOKEN NumExpression4 %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);

        char* temp = newTemp(ctx);
        //printf("CURRENT TOKEN NumExpression5 %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
        
        char code[100];
        sprintf(code, "%s = %s %s %s", temp, place, op, place2);
        emitCode(ctx, code);

        place = temp;
        //if(strcmp(ctx->tokens[ctx->current_token].token_str, ";") == 0 ){
        //    break;
        //}
    }

    //printf("CURRENT TOKEN NumExpression6 %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);

    //if(strcmp(ctx->tokens[ctx->current_token].token_str, ";") == 0 ){
    //    ctx->current_token++;
    //    printf("CURRENT TOKEN NumExpression %s \n",ctx->tokens[ctx->current_token].token_str);
    //}
    
    return place;
}

char* generateTerm(CodeGenContext* ctx) {
    // Implementação simplificada para demonstração
    //printf("CURRENT TOKEN generateTerm %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);

    if (ctx->tokens[ctx->current_token].token_type == IDENT ||
        ctx->tokens[ctx->current_token].token_type == INT_E ||
        ctx->tokens[ctx->current_token].token_type == FLOAT_E) {
        
        //printf("CURRENT TOKEN generateTerm %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);

        char* place = ctx->tokens[ctx->current_token].token_str;
        ctx->current_token++;

        //printf("CURRENT TOKEN generateTerm %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
        return place;
    }
    
    // Caso de expressão entre parênteses
    if (strcmp(ctx->tokens[ctx->current_token].token_str, "(") == 0) {

        //printf("CURRENT TOKEN generateTerm %s \n",ctx->tokens[ctx->current_token].token_str);
        ctx->current_token++;
        //printf("CURRENT TOKEN generateTerm %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
        char* place = generateExpression(ctx);
        
        // Consome o fecha parênteses
        if (strcmp(ctx->tokens[ctx->current_token].token_str, ")") == 0) {
            //printf("CURRENT TOKEN generateTerm %s \n",ctx->tokens[ctx->current_token].token_str);
            ctx->current_token++;
            //printf("CURRENT TOKEN generateTerm %s \n",ctx->tokens[ctx->current_token].token_str);
            //fflush(stdout);
        }
        
        return place;
    }
    
    return NULL;
}

// Gera código para atribuição
void generateAtributionStatement(CodeGenContext* ctx) {
    char* lvalue = ctx->tokens[ctx->current_token].token_str;
    //printf("CURRENT TOKEN AtributionStatement%s \n",ctx->tokens[ctx->current_token].token_str);
    ctx->current_token++;
    //printf("CURRENT TOKEN AtributionStatement %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);
    
    // Consome o '='
    //printf("CURRENT TOKEN AtributionStatement %s \n",ctx->tokens[ctx->current_token].token_str);
    ctx->current_token++;
    //printf("CURRENT TOKEN AtributionStatement %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);
    
    char* place = generateExpression(ctx);
    //printf("CURRENT TOKEN AtributionStatement %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);
    
    char code[100];
    sprintf(code, "%s = %s", lvalue, place);
    emitCode(ctx, code);
}

// Geração de Expressões (expandido)
char* generateExpression(CodeGenContext* ctx) {
    char* place = generateNumExpression(ctx);
    //printf("CURRENT TOKEN Expression %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);
    
    // Suporte a operadores relacionais
    if (strcmp(ctx->tokens[ctx->current_token].token_str, "<") == 0 ||
        strcmp(ctx->tokens[ctx->current_token].token_str, ">") == 0 ||
        strcmp(ctx->tokens[ctx->current_token].token_str, "<=") == 0 ||
        strcmp(ctx->tokens[ctx->current_token].token_str, ">=") == 0 ||
        strcmp(ctx->tokens[ctx->current_token].token_str, "==") == 0 ||
        strcmp(ctx->tokens[ctx->current_token].token_str, "!=") == 0) {
        
        char* op = ctx->tokens[ctx->current_token].token_str;
        //printf("CURRENT TOKEN Expression %s \n",ctx->tokens[ctx->current_token].token_str);
        ctx->current_token++;
        //printf("CURRENT TOKEN Expression %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
        
        char* place2 = generateNumExpression(ctx);
        //printf("CURRENT TOKEN Expression %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
       
        char* temp = newTemp(ctx);
        
        char code[100];
        sprintf(code, "%s = %s %s %s", temp, place, op, place2);
        emitCode(ctx, code);

        //fflush(stdout);
        
        return temp;
    }
    
    return place;
}

// Geração de Estruturas de Controle
void generateControlFlow(CodeGenContext* ctx) {
    // Tratamento de IF
    //printf("CURRENT TOKEN ControlFlow 1 %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);

    if (strcmp(ctx->tokens[ctx->current_token].token_str, "if") == 0) {
        ctx->current_token++; // consome 'if'
        
        // Consome '('
        ctx->current_token++;
        
        // Gera código para condição
        char* condition = generateExpression(ctx);
        
        // Consome ')'
        ctx->current_token++;

        //printf("CURRENT TOKEN ControlFlow 2 %s \n",ctx->tokens[ctx->current_token].token_str);

        //Consome '{'
        ctx->current_token++;

        //printf("CURRENT TOKEN ControlFlow 3 %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
        
        // Gera labels
        char* lfalse = newLabel(ctx);
        char* lend = newLabel(ctx);
        
        // Código para desvio condicional
        char code1[100];
        sprintf(code1, "if %s == 0 goto %s", condition, lfalse);
        emitCode(ctx, code1);
        
        // Bloco then
        generateStatement(ctx);

        //printf("CURRENT TOKEN ControlFlow 4 %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
        
        // Goto para pular else
        char code2[100];
        sprintf(code2, "goto %s", lend);
        emitCode(ctx, code2);
        
        // Label false (else)
        char code3[100];
        sprintf(code3, "%s:", lfalse);
        emitCode(ctx, code3);
        
        // Opcional: bloco else
        if (strcmp(ctx->tokens[ctx->current_token].token_str, "else") == 0) {
            //printf("CURRENT TOKEN ControlFlow 5 %s \n",ctx->tokens[ctx->current_token].token_str);
            ctx->current_token++; // consome 'else'
            //generateStatement(ctx);
        }

        // Label de fim
        char code4[100];
        sprintf(code4, "%s:", lend);
        emitCode(ctx, code4);

        //Consome '{'
        //printf("CURRENT TOKEN ControlFlow 7 %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
        ctx->current_token++;
        //printf("CURRENT TOKEN ControlFlow 7 %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);

    }
    // Tratamento de FOR
    else if (strcmp(ctx->tokens[ctx->current_token].token_str, "for") == 0) {
        //printf("CURRENT TOKEN ControlFlow 6 %s \n",ctx->tokens[ctx->current_token].token_str);
        ctx->current_token++; // consome 'for'
        //printf("CURRENT TOKEN ControlFlow 7 %s \n",ctx->tokens[ctx->current_token].token_str);
        
        // Consome '('
        ctx->current_token++;
        
        // Inicialização
        generateStatement(ctx);

        //printf("CURRENT TOKEN ControlFlow 7 %s \n",ctx->tokens[ctx->current_token].token_str);
        
        // Label de início do loop
        char* lstart = newLabel(ctx);
        char* lend = newLabel(ctx);
        
        char code1[100];
        sprintf(code1, "%s:", lstart);
        emitCode(ctx, code1);
        
        // Condição
        char* condition = generateExpression(ctx);
        //printf("CURRENT TOKEN ControlFlow 8 %s \n",ctx->tokens[ctx->current_token].token_str);

        //Consume ';'
        ctx->current_token++;
        
        // Código para desvio condicional
        char code2[100];
        sprintf(code2, "if %s == 0 goto %s", condition, lend);
        emitCode(ctx, code2);

        // Incremento/atualização
        generateStatement(ctx);
        //printf("CURRENT TOKEN ControlFlow 9 %s \n",ctx->tokens[ctx->current_token].token_str);
        
        // Consome ')'
        ctx->current_token++;
        //printf("CURRENT TOKEN ControlFlow 9 %s \n",ctx->tokens[ctx->current_token].token_str);

        // Consome '{'
        ctx->current_token++;
        //printf("CURRENT TOKEN ControlFlow 9 %s \n",ctx->tokens[ctx->current_token].token_str);

        // Corpo do loop
        generateStatement(ctx);
        //printf("CURRENT TOKEN ControlFlow 10 %s \n",ctx->tokens[ctx->current_token].token_str);

        
        // Volta para o início
        char code3[100];
        sprintf(code3, "goto %s", lstart);
        emitCode(ctx, code3);
        
        // Label de fim
        char code4[100];
        sprintf(code4, "%s:", lend);
        emitCode(ctx, code4);

        // Consome '}'
        ctx->current_token++;
        //printf("CURRENT TOKEN ControlFlow 9 %s \n",ctx->tokens[ctx->current_token].token_str);
        
    }
}

// Geração de Chamadas de Função
void generateFunctionCall(CodeGenContext* ctx) {
    // Nome da função
    char* func_name = ctx->tokens[ctx->current_token].token_str;
    ctx->current_token++;
    
    // Consome '('
    ctx->current_token++;
    
    // Temporário para resultado
    char* result_temp = newTemp(ctx);
    
    // Parâmetros
    int param_count = 0;
    while (strcmp(ctx->tokens[ctx->current_token].token_str, ")") != 0) {
        //printf("CURRENT TOKEN FunctionCall %s \n",ctx->tokens[ctx->current_token].token_str);
        //fflush(stdout);
        char* param = generateExpression(ctx);
        
        // Emite código para parâmetro
        char param_code[100];
        sprintf(param_code, "param %s", param);
        emitCode(ctx, param_code);
        
        param_count++;
        
        // Verifica próximo token (vírgula ou fecha parênteses)
        if (strcmp(ctx->tokens[ctx->current_token].token_str, ",") == 0) {
            ctx->current_token++;
        }
    }
    
    // Consome ')'
    ctx->current_token++;
    
    // Emite chamada de função
    char func_call[100];
    sprintf(func_call, "%s = call %s, %d", result_temp, func_name, param_count);
    emitCode(ctx, func_call);
}

// Definição de Função
void generateFunctionDefinition(CodeGenContext* ctx) {
    // Consome 'def'
    ctx->current_token++;

    //printf("CURRENT TOKEN FunctionDefinition %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);
    
    // Nome da função
    char* func_name = ctx->tokens[ctx->current_token].token_str;
    ctx->current_token++;

    //printf("CURRENT TOKEN FunctionDefinition %s \n",ctx->tokens[ctx->current_token].token_str);
    
    // Cria registro de função
    FunctionInfo* func = &ctx->functions[ctx->function_count++];
    func->name = strdup(func_name);
    func->param_count = 0;
    
    // Consome '('
    ctx->current_token++;

    //printf("CURRENT TOKEN FunctionDefinition %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);
    
    // Parâmetros
    while (strcmp(ctx->tokens[ctx->current_token].token_str, ")") != 0) {
        // Tipo do parâmetro
        char* param_type = ctx->tokens[ctx->current_token].token_str;
        ctx->current_token++;

        //printf("CURRENT TOKEN FunctionDefinition %s \n",ctx->tokens[ctx->current_token].token_str);
        
        // Nome do parâmetro
        char* param_name = ctx->tokens[ctx->current_token].token_str;
        ctx->current_token++;

        //printf("CURRENT TOKEN FunctionDefinition %s \n",ctx->tokens[ctx->current_token].token_str);
        
        // Adiciona parâmetro
        func->params[func->param_count++] = strdup(param_name);

        //printf("CURRENT TOKEN FunctionDefinition %s \n",ctx->tokens[ctx->current_token].token_str);
        
        // Emite código de declaração de parâmetro
        char param_code[100];
        sprintf(param_code, "param %s %s", param_type, param_name);
        emitCode(ctx, param_code);

        //fflush(stdout);
        
        // Verifica próximo token (vírgula ou fecha parênteses)
        if (strcmp(ctx->tokens[ctx->current_token].token_str, ",") == 0) {
            ctx->current_token++;
        }
    }
    
    // Consome ')'
    ctx->current_token++;

    //printf("CURRENT TOKEN FunctionDefinition %s \n",ctx->tokens[ctx->current_token].token_str);
    
    // Emite label de início de função
    char func_label[100];
    sprintf(func_label, "function %s:", func_name);
    emitCode(ctx, func_label);
    
    // Consome "{"
    ctx->current_token++;

    // Corpo da função
    generateStatement(ctx);
    
    // Emite fim de função
    emitCode(ctx, "end_function");
}

// Geração de Declaração de Variáveis
void generateVarDeclaration(CodeGenContext* ctx) {
    // Tipo da variável
    //printf("CURRENT TOKEN IN VARDECL %s \n",ctx->tokens[ctx->current_token].token_str);
    char* type = ctx->tokens[ctx->current_token].token_str;
    ctx->current_token++;
    //printf("CURRENT TOKEN  IN VARDECL %s \n",ctx->tokens[ctx->current_token].token_str);
    
    // Nome da variável
    //printf("CURRENT TOKEN  IN VARDECL %s \n",ctx->tokens[ctx->current_token].token_str);
    char* var_name = ctx->tokens[ctx->current_token].token_str;
    ctx->current_token++;
    //printf("CURRENT TOKEN  IN VARDECL %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);
    
    // Verifica se é um array
    if (strcmp(ctx->tokens[ctx->current_token].token_str, "[") == 0) {
        ctx->current_token++; // consome '['
        
        // Tamanho do array
        char* array_size = ctx->tokens[ctx->current_token].token_str;
        ctx->current_token++;
        
        // Consome ']'
        ctx->current_token++;
        
        // Emite declaração de array
        char array_code[100];
        sprintf(array_code, "declare array %s %s[%s]", type, var_name, array_size);
        emitCode(ctx, array_code);
    } else {
        // Emite declaração de variável simples
        char var_code[100];
        sprintf(var_code, "declare %s %s", type, var_name);
        emitCode(ctx, var_code);
    }
}

// Geração de Comandos
void generateStatement(CodeGenContext* ctx) {
    // Verifica tipo de declaração/comando

    //printf("CURRENT TOKEN Statement 1 %s \n",ctx->tokens[ctx->current_token].token_str);
    //fflush(stdout);

    if(strcmp(ctx->tokens[ctx->current_token].token_str , "}") == 0) {
        return;
    }

    if (strcmp(ctx->tokens[ctx->current_token].token_str, "int") == 0 ||
        strcmp(ctx->tokens[ctx->current_token].token_str, "float") == 0 ||
        strcmp(ctx->tokens[ctx->current_token].token_str, "string") == 0) {
        generateVarDeclaration(ctx);

        // Consome ';'
        //printf("CURRENT TOKEN Statement 2 %s \n",ctx->tokens[ctx->current_token].token_str);
        ctx->current_token++;
        //printf("CURRENT TOKEN NUM %d \n", ctx->current_token);
        //printf("CURRENT TOKEN Statement 3 %s \n",ctx->tokens[ctx->current_token].token_str);
    } 
    else if (strcmp(ctx->tokens[ctx->current_token].token_str, "if") == 0 ||
             strcmp(ctx->tokens[ctx->current_token].token_str, "for") == 0) {
        generateControlFlow(ctx);

    }
    else if (strcmp(ctx->tokens[ctx->current_token].token_str, "def") == 0) {
        //printf("MATCHED TOKEN TO DEF");
        //fflush(stdout);
        generateFunctionDefinition(ctx);
    }
    else if (ctx->tokens[ctx->current_token].token_type == IDENT) {
        // Verifica se é chamada de função ou atribuição
        if (strcmp(ctx->tokens[ctx->current_token+1].token_str, "(") == 0) {
            generateFunctionCall(ctx);
        } else {
            char* lvalue = ctx->tokens[ctx->current_token].token_str;
            //printf("CURRENT TOKEN Statement 4 %s \n",ctx->tokens[ctx->current_token].token_str);
            //fflush(stdout);
            ctx->current_token++;
            //printf("CURRENT TOKEN Statement 5 %s \n",ctx->tokens[ctx->current_token].token_str);
            //fflush(stdout);
            // Consome o '='
            ctx->current_token++;
            
            char* place = generateExpression(ctx);
            //printf("CURRENT TOKEN Statement 6 %s \n",ctx->tokens[ctx->current_token].token_str);
            //fflush(stdout);
            
            char code[100];
            sprintf(code, "%s = %s", lvalue, place);
            emitCode(ctx, code);
            
            // Consome ';'
            ctx->current_token++;
        }
    }
}



// Geração de Programa
void generateProgram(CodeGenContext* ctx) {
    //printf("TOKEN COUNT: %d \n",ctx->token_count);
    while (ctx->current_token < ctx->token_count - 1) {
        generateStatement(ctx);
        //if(strcmp(ctx->tokens[ctx->current_token].token_str , "}") == 0) {
        //    printf("current token: %d vs token count %d", ctx->current_token, ctx->token_count);
        //return;
        //}
    }
        
}

// Função para imprimir código intermediário
void printIntermediateCode(IntermediateCode* ic) {
    //printf("chamou print \n");
    //printf("IC->LINE_COUNT %d \n", ic->line_count);
    for (int i = 0; i < ic->line_count; i++) {
        //printf("PRINTING \n");
        printf("%s\n", ic->code[i]);
    }
}

// Function to deep copy a GList into an array of Token structs
Token* glist_to_array_deep_copy_tokens(GList* list) {


    // Get the size of the GList
    int size = g_list_length(list);

    // Allocate memory for the array of Token structs
    Token* array = (Token*)malloc(size * sizeof(Token));
    if (array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(1);
    }

    // Deep copy each Token from the GList into the array
    GList* current = list;
    for (int i = 0; i < size; i++) {
        Token* src = (Token*)current->data;
        array[i].token_type = src->token_type;
        array[i].token_num = src->token_num;

        // Allocate and copy the token_str
        if (src->token_str != NULL) {
            array[i].token_str = strdup(src->token_str);
            if (array[i].token_str == NULL) {
                fprintf(stderr, "Error: Memory allocation failed for token_str.\n");
                free(array);
                exit(1);
            }
        } else {
            array[i].token_str = NULL;
        }

        current = current->next;
    }

    return array;
}

// Free the array of Token structs
void free_token_array_tokens(Token* array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i].token_str); // Free the token_str
    }
    free(array); // Free the array itself
}

int generetaIntCode(GList * tokens){
    int token_count = g_list_length(tokens);
    Token* tokens_arr = glist_to_array_deep_copy_tokens(tokens);
    IntermediateCode ic = {0};
    CodeGenContext ctx = {
        .tokens = tokens_arr,
        .token_count = token_count,
        .current_token = 0,
        .intermediate_code = &ic,
        .temp_count = 1,
        .label_count = 1
    };
    
    generateProgram(&ctx);

    fflush(stdout);
    
    printf("Código Intermediário Gerado:\n");
    printIntermediateCode(&ic);
    
    return 0;
    
}