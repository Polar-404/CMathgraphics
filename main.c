#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tokenize.h"
#include "rpn.h"

typedef enum {
    TYPE_VAR,
    TYPE_NUM, 
    TYPE_OP,
    TYPE_FUNC, 
} NodeType;

typedef struct Node{
    NodeType type;
    double value;
    char op;
    double (*func_ptr)(double); 
    struct Node *left;
    struct Node *right;
}Node;

Node*
node_new_num(double val) {
    Node* node = malloc(sizeof(Node));

    if (node) {
        node -> type = TYPE_NUM;
        node -> value = val;
        node -> left = NULL;
        node -> right = NULL;
    }
    return node;
};

Node* 
node_new_op(char op, Node* left, Node* right) {
    Node* node = malloc(sizeof(Node));

    if (node) {
        node -> type = TYPE_OP;
        node -> op = op;
        node -> left = left;
        node -> right = right;
    }
    return node;
};

Node*
node_new_func(double (*func)(double), Node* left, Node* right) {
    Node* node = malloc(sizeof(Node));

    if (node) {
        node -> type = TYPE_FUNC;
        node -> func_ptr = func;
        node -> left = left;
        node -> right = right;
    }
    return node;
};

Node* 
node_new_var() {
    Node* node = malloc(sizeof(Node));
    if (node) {
        node->type = TYPE_VAR;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
};

double
eval_node(Node* node, double x_val) {
    if (!node) return 0;

    switch (node->type) {
        case TYPE_NUM:  return node->value;
        case TYPE_VAR:  return x_val;
        case TYPE_OP: 
            switch (node->op) {
                    case '+': return eval_node(node->left, x_val) + eval_node(node->right, x_val);
                    case '-': return eval_node(node->left, x_val) - eval_node(node->right, x_val);
                    case '*': return eval_node(node->left, x_val) * eval_node(node->right, x_val);
                    case '/': return eval_node(node->left, x_val) / eval_node(node->right, x_val);
                    default: return 0;
                }
        case TYPE_FUNC:
            if (node->func_ptr != NULL) {
                return node->func_ptr(eval_node(node->left, x_val));
            }
            return 0;
    }
    return 0;
};

void // recursive function to clean the memory of a node and all it's children
node_free_mem(Node* node) {
    if (!node) return;

    node_free_mem(node->left);
    node_free_mem(node->right);

    free(node);
}

int main() {

    int size_token = 100;

    Node* n2 = node_new_num(2.0);
    Node* n5 = node_new_num(5.0);
    Node* op_mul = node_new_op('*', n2, n5);

    Node* var_x = node_new_var();
    Node* f_sin = node_new_func(sin, var_x, NULL);

    Node* root = node_new_op('+', f_sin, op_mul);

    double x = 0.99;
    double result = eval_node(root, x);

    SetConsoleOutputCP(CP_UTF8);

    printf("Equação: 𝒇(𝒙) = 𝒔𝒊𝒏(𝒙) + 2 × 5\n");
    printf("with 𝒙 = %.2f, 𝒇(𝒙) = %.2f\n", x, result);

    node_free_mem(root);
    root = NULL;


    printf("EQUATION TOKENS: 𝒇(𝒙) = 𝒔𝒊𝒏(𝒙) * (10 - 5) \n");

    Token tokens[size_token];
    int count = 0;
    const char* expressao = "x * 10 - 5";

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

    rpn(tokens, count, size_token);

    for(int i = 0; i < count; i++) {
        printf("Token %d: Tipo %s\n", i, token_type_names[tokens[i].type]);
    }

    return 0;
}