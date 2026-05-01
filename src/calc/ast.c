#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "tokenize.h"

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

Node*
build_node_tree(Token* tokens, int count) {
    Node* stack[count];
    int stack_ptr = -1;

    for(int i = 0; i < count; i++) {
        switch(tokens[i].type) {

            case TOK_NUM: 
                stack[++stack_ptr] = node_new_num(tokens[i].value);
                break;

            case TOK_VAR: 
                stack[++stack_ptr] = node_new_var();
                break;

            case TOK_FUNC:
                Node* arg = stack[stack_ptr--];
                stack[++stack_ptr] = node_new_func(tokens[i].func, arg, NULL);
                break;

            case TOK_OP:
                Node* right = stack[stack_ptr--];
                Node* left = stack[stack_ptr--];

                stack[++stack_ptr] = node_new_op(tokens[i].op, left, right);
                break;
        }
    }
    return stack[0];
}

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
                    case '^': return pow(eval_node(node->left, x_val), eval_node(node->right, x_val));
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