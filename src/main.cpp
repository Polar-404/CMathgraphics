#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <vector>
#include <iostream>
#include <string>
#include <atomic>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "calc/tokenize.h"
#include "calc/rpn.h"
#include "calc/ast.h"

//rendering:
#include "rendering/renderer.hpp"

std::string math_expr = "x";
std::atomic<bool> new_expr { false };

void user_interface() {
    std::cout << "Write an math expression to be plotted...\n" << std::endl;
    std::string temp;
    while(true) {
        std::getline(std::cin, temp);
        math_expr = temp;
        new_expr = true;
    }
}

struct ExprInfo {
    Token* tokens;
    Node* root;
    int count;
};

ExprInfo calc_expr_tree(const std::string& math_expr) {

    int count = 0;
    Token* tokens = tokenize(math_expr.c_str(), &count);

    printf("Found Tokens: %d\n", count);

    const char* token_type_names[] = {
        "TOK_NUM",
        "TOK_VAR",
        "TOK_FUNC",
        "TOK_OP",
        "TOK_LPAREN",
        "TOK_RPAREN",
    };

    parse_to_rpn(tokens, &count);

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

    std::vector<float> points;

    for(double x = -10; x <= 10; x+=0.01) {
        double result = eval_node(root, x);

        points.push_back(static_cast<float>(x * 0.1));      // X
        points.push_back(static_cast<float>(result * 0.1 /* multiplicar por meio pra achatar um pouco o grafico*/)); // Y
        points.push_back(0.0f);                       // Z (2D)
    }

    update_points(points.data(), points.size());
    
    return {tokens, root, count};
}
    

int main() {

    SetConsoleOutputCP(CP_UTF8);

    // C++ / OPENGL rendering:
    init_opengl();

    std::thread input_thread(user_interface);
    ExprInfo current_expr = { nullptr, nullptr, 0 };

    //EXECUTION LOOP
    while (true) {
        if (new_expr) {
            std::cout << "Updating graph for: ' " << math_expr << " '\n" << std::endl;
            new_expr = false;
            current_expr = calc_expr_tree(math_expr);

            std::cout << "To write a new expression just type it bellow...\n" << std::endl;
        }

        render_frame();
    }

    //CLEARING MEMORY
    cleanup_opengl();
    if (current_expr.root) {
        node_free_mem(current_expr.root);
        current_expr.root = nullptr;
    }
    if (current_expr.tokens) {
        free(current_expr.tokens);
        current_expr.tokens = nullptr;
    }

    return 0;
}