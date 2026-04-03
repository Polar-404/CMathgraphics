#ifndef TOKENIZE_H
#define TOKENIZE_H

typedef enum {
    TOK_NUM,
    TOK_VAR, 
    TOK_FUNC,
    TOK_OP,
    TOK_LPAREN,
    TOK_RPAREN,
} MathTokenType;

typedef struct {
    MathTokenType type;
    double value;
    char op;
    double (*func)(double);
} Token;

void tokenize(const char* input, Token* tokens, int* count);

#endif