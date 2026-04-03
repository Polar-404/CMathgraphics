#include <stdlib.h>
#include "tokenize.h"

int
operator_to_precedence(char operator){
    switch(operator) {
        case '-': return 1;
        case '+': return 1;
        case '/': return 2;
        case 'x': return 2;
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
        return precedence >= stack->op;
    }
}

void
flush(Token *temp, Token *stack, int temp_ptr, int stack_ptr, Token *token){

    // sets precedence based on op or paren
    int precedence;

    if(token->type == TOK_LPAREN || token->type == TOK_RPAREN){
        precedence = token->type;
    }
    else{
        precedence = operator_to_precedence(token->op);
    }

    while(is_smaller_or_equal(precedence, &stack[stack_ptr])){ // bigger or equal top of stack
        temp[temp_ptr++] = stack[stack_ptr--]; // flush it to temp
    }
    stack[stack_ptr++] = *token; // alredy did token_ptr++ on the function call

}

void
rpn(Token *tokens, int count, int size_token){ // malditos poloneses
    int token_ptr = 0;
    int temp_ptr = 0;
    int stack_ptr = 0;

    Token temp[size_token];
    Token stack[size_token];

    stack[0].op = ' '; // initializing first value of op in order to compare later

    while (token_ptr < count){
        switch (tokens[token_ptr].type){
            case TOK_NUM:
            case TOK_VAR:
                temp[temp_ptr++] = tokens[token_ptr++];
                continue;
            case TOK_FUNC: // TO DO
                if(!is_smaller_or_equal(4, &stack[stack_ptr])){ // is bigger tan top stack
                    stack[stack_ptr++] = tokens[token_ptr++];
                }
                else{
                    flush(temp, stack, temp_ptr, stack_ptr, &tokens[token_ptr++]);
                    stack[stack_ptr++] = tokens[token_ptr++];
                }
                continue;
            case TOK_OP:
                if(!is_smaller_or_equal(operator_to_precedence(tokens[token_ptr].op), &stack[stack_ptr])){ // is bigger tan top stack
                    stack[stack_ptr++] = tokens[token_ptr++];
                }
                else{
                    flush(temp, stack, temp_ptr, stack_ptr, &tokens[token_ptr++]);
                    stack[stack_ptr++] = tokens[token_ptr++];
                }
                continue;
            case TOK_LPAREN:
            case TOK_RPAREN:
                if(!is_smaller_or_equal(tokens[token_ptr].type+1, &stack[stack_ptr])){ // is bigger tan top stack
                    stack[stack_ptr++] = tokens[token_ptr++];
                }
                else{
                    flush(temp, stack, temp_ptr, stack_ptr, &tokens[token_ptr++]);
                    stack[stack_ptr++] = tokens[token_ptr++];
                }
                continue;
            default:
                token_ptr++;
                continue;
        }
    }
    
    // end
    // removes parentheses
    int offset = 0;

    for(int i = 0; i < count; i++) {
        if(temp[i].type == TOK_LPAREN || temp[i].type == TOK_RPAREN){
            offset++;
        }
        else{
            tokens[i-offset] = temp[i];
        }
    }

    count -= offset; // new count since parentheses have been removed

}