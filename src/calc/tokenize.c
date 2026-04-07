#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

typedef enum {
    TOK_NUM,
    TOK_VAR, 
    TOK_FUNC,
    TOK_OP,
    TOK_LPAREN,
    TOK_RPAREN,
} TokenType;

typedef struct {
    TokenType type;
    double value;
    char op;
    double (*func)(double);
} Token;

//math.h functions into somewhat of a hashmap format
typedef struct {
    const char* name;
    double (*func_ptr)(double); //function that recives a double as parameter and returns a double
} MathFunction;

MathFunction func_map[] = {
    {"sin", sin},
    {"cos", cos},
    {"tan", tan},

    {"sqrt", sqrt},
    {"exp", exp},
    
    {"log", log},
    {"abs", fabs},
};

typedef double (*MathFunc)(double);

 //recieves a given math function name as a string (e.g: "cos"), and returns the corresponding math.h function ptr)
MathFunc
find_function(const char* name) { 
    int number_of_funcs = sizeof(func_map) / sizeof(func_map[0]);

    for(int i = 0; i < number_of_funcs; i++) {
        if (strcmp(name, func_map[i].name) == 0) {
            return func_map[i].func_ptr;
        }
    }
    return NULL;
}

Token*
tokenize(const char* input, int* count) {
    int capacity = 32;
    Token* tokens = (Token*)malloc(sizeof(Token) * capacity);
    
    const char* p = input; //creating a copy of the pointer so the original addr doenst get lost
    *count = 0; //dereferencing and writing zero at the given memory address

    while(*p) {
        //alocating more memory if needed
        if (*count >= capacity) {
            capacity *= 2;
            tokens = (Token*)realloc(tokens, sizeof(Token) * capacity);
            
            if (tokens == NULL) {
                printf("Erro: Falta de memoria!\n");
                exit(1);
            }
        }
        //spaces (ignores)
        if(isspace(*p)) { p++; continue; }

        //numbers
        if(isdigit(*p)) {
            char* end;
            tokens[*count].value = strtod(p, &end);
            tokens[*count].type = TOK_NUM;

            //increment both the pointer and the index counter
            p = end;
            (*count)++;
        }
        //math function or variable
        else if(isalpha(*p)) {
            // ------------ getting the full string ---------------
            char buf[64];
            int i = 0;
            while (isalpha(*p)) {
                buf[i++] = *p++;
            }
            buf[i] = '\0';

            // ----------- comparing the string -------------------

            if (strcmp(buf, "x") == 0) {
                tokens[*count].type = TOK_VAR;
            }
            else {
                double (*f) (double) = find_function(buf);
                if(*f) {
                    tokens[*count].type = TOK_FUNC;
                    tokens[*count].func = f;
                }
                else {
                    printf("Error, Unknown function... '%s'\n", buf);
                    exit(1);
                }
            }
            (*count)++;
        }
        //anything that isn't a number nor a letter
        else {
            if (*p == '(') tokens[*count].type = TOK_LPAREN;
            else if (*p == ')') tokens[*count].type = TOK_RPAREN;
            else {
                tokens[*count].type = TOK_OP;
                tokens[*count].op = *p;
            }
            p++;
            (*count)++;
        }
    }
    return tokens;
}