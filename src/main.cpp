#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "calc/tokenize.h"
#include "calc/rpn.h"
#include "calc/ast.h"

int main() {

    int size_token = 100;

    SetConsoleOutputCP(CP_UTF8);

    printf("Equação: 𝒇(𝒙) = 𝒔𝒊𝒏(𝒙) + 2 × 5\n");

    printf("EQUATION: 𝒇(𝒙) = 𝒄𝒐𝒔(𝒙) * (𝒔𝒊𝒏(1) - 0.5) + 1 \n");

    Token tokens[size_token];
    int count = 0;
    const char* expressao = "cos(x) * (sin(1) - 0.5) + 1";

    tokenize(expressao, tokens, &count);

    printf("Tokens encontrados: %d\n", count);

    const char* token_type_names[] = {
        "TOK_NUM",
        "TOK_VAR",
        "TOK_FUNC",
        "TOK_OP",
        "TOK_LPAREN",
        "TOK_RPAREN",
    };

    parse_to_rpn(tokens, &count, size_token);

    for(int i = 0; i < count; i++) {
        printf("Token %d: tipo=%-12s", i, token_type_names[tokens[i].type]);

        switch(tokens[i].type) {
            case TOK_NUM:  printf(" value=%.4f", tokens[i].value);       break;
            case TOK_OP:   printf(" op='%c'",    tokens[i].op);          break;
            case TOK_FUNC: printf(" func=%p",    (void*)tokens[i].func); break;
            case TOK_VAR:                                                  break;
            default:                                                       break;
        }
        printf("\n");
    }
    fflush(stdout);

    Node* root = build_node_tree(tokens, count);
    printf("press any key to continue...\n");
    scanf("");

    for(double x = -1; x < 1; x+=0.01) {
        double result = eval_node(root, x);
        printf("with x = %.2f Result: %.4f\n", x, result);
    }
    node_free_mem(root);

    return 0;
}