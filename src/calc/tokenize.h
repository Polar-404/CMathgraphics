#ifndef TOKENIZE_H
#define TOKENIZE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file tokenize.h
 * @brief Lexical analysis module for mathematical expressions.
 * * provides tools to scan raw strings and categorize components into 
 * structured tokens (numbers, variables, operators, and functions). 
 * this is the first stage of the expression parsing pipeline.
 */
/**
 * @enum MathTokenType
 * @brief Identifiers for the grammatical nature of a token.
 */
typedef enum {
    TOK_NUM,    /* numeric constant (stored as double) */
    TOK_VAR,    /* symbolic variable (e.g., 'x') */
    TOK_TIME,   /* variable that will oscilate with time in a 1:1 relation (+1 sec = +1 val )*/
    TOK_FUNC,   /* mathematical function (e.g., sin, cos, sqrt) */
    TOK_OP,     /* arithmetic operator (+, -, *, /) */
    TOK_LPAREN, /* left parenthesis '(' */
    TOK_RPAREN, /* right parenthesis ')' */
} MathTokenType;

/**
 * @struct Token
 * @brief An atomic element of a mathematical expression.
 */
typedef struct {
    MathTokenType type;     /**< the category of the token */
    double value;           /**< numerical value if type is TOK_NUM */
    char op;                /**< character literal if type is TOK_OP */
    double (*func)(double); /**< function pointer if type is TOK_FUNC */
} Token;

/**
 * @brief Lexical analyzer that converts a string into an array of Tokens.
 * * scans the input string and populates the provided token array. 
 * It ignores whitespace and recognizes numbers, operators, and basic functions.
 * * @param input  The raw math expression string (e.g., "3 * sin(x)")
 * @param count  Pointer to an integer to store the resulting token count
 */
Token* tokenize(const char* input, int* count);

#ifdef __cplusplus
}
#endif

#endif