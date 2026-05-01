#include "tokenize.h"
#include <stdlib.h>
#include <stdio.h>

int
operator_to_precedence(char operator){
    switch(operator) {
        case '-': return 1;
        case '+': return 1;
        case '/': return 2;
        case '*': return 2;
        default:  return 0;
    }
}

int 
is_smaller_or_equal(int precedence, Token *stack){
    if(stack->type == TOK_LPAREN || stack->type == TOK_RPAREN){
        return precedence >= stack->type+1;
    }
    else if(stack->type == TOK_FUNC) {
        return precedence >= 4;
    }
    else{
        return precedence >= operator_to_precedence(stack->op);
    }
}

void
flush(Token *temp, Token *stack, int *temp_ptr, int *stack_ptr, Token *token){

    // sets precedence based on op or paren
    int precedence;

    if(token->type == TOK_LPAREN || token->type == TOK_RPAREN){
        precedence = token->type;
    }
    else{
        precedence = operator_to_precedence(token->op);
    }

    while(is_smaller_or_equal(precedence, &stack[*stack_ptr])){ // bigger or equal to top of stack
        temp[(*temp_ptr)++] = stack[(*stack_ptr)--]; // flush it to temp
    }
    stack[(*stack_ptr)++] = *token; // token_ptr++ already done on the function call

}

// converts a array of tokens into the "Reverse Polish Notation" format
void
parse_to_rpn(Token *tokens, int *count){ //malditos poloneses
    int token_ptr = 0;
    int temp_ptr  = 0;
    int stack_ptr = -1; // -1 = empty stack

    Token *temp = (Token*)malloc(sizeof(Token) * (*count));
    Token *stack = (Token*)malloc(sizeof(Token) * (*count));

    if (temp == NULL || stack == NULL) {
        printf("[error] Failed to alocate memory for the RPN parser\n");
        exit(1);
    }

    while (token_ptr < *count){
        Token *tok = &tokens[token_ptr];

        switch (tok->type){
            case TOK_NUM:

            case TOK_VAR:
                temp[temp_ptr++] = *tok;
                break;
            case TOK_TIME:
                temp[temp_ptr++] = *tok;
                break;
            case TOK_FUNC:
                stack[++stack_ptr] = *tok;
                break;

            case TOK_OP: {
                int prec = operator_to_precedence(tok->op);
                // flush operators with greater or equal precedence
                while (stack_ptr >= 0 &&
                       stack[stack_ptr].type != TOK_LPAREN &&
                       (stack[stack_ptr].type == TOK_FUNC ||
                        operator_to_precedence(stack[stack_ptr].op) >= prec))
                {
                    temp[temp_ptr++] = stack[stack_ptr--];
                }
                stack[++stack_ptr] = *tok;
                break;
            }

            case TOK_LPAREN:
                stack[++stack_ptr] = *tok; // always push, never compare
                break;

            case TOK_RPAREN:
                // flush until '(' is found
                while (stack_ptr >= 0 && stack[stack_ptr].type != TOK_LPAREN){
                    temp[temp_ptr++] = stack[stack_ptr--];
                }
                if (stack_ptr >= 0) stack_ptr--; // discard the '('

                // if there was a function before '(' it goes to the output
                if (stack_ptr >= 0 && stack[stack_ptr].type == TOK_FUNC){
                    temp[temp_ptr++] = stack[stack_ptr--];
                }
                // ')' never goes to the output
                break;

            default:
                break;
        }
        token_ptr++;
    }

    // empty whatever is left on the stack
    while (stack_ptr >= 0){
        temp[temp_ptr++] = stack[stack_ptr--];
    }

    // copy back (parentheses were already discarded above)
    int new_count = 0;
    for(int i = 0; i < temp_ptr; i++){
        if(temp[i].type != TOK_LPAREN && temp[i].type != TOK_RPAREN){
            tokens[new_count++] = temp[i];
        }
    }
    
    *count = new_count;

    free(temp);
    free(stack);
}