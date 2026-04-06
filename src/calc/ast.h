#ifndef AST_H
#define AST_H

#ifdef __cplusplus
extern "C" {
#endif

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


Node* node_new_num(double val);

Node* node_new_var();

Node* node_new_op(char op, Node* left, Node* right);

Node* node_new_func(double (*func)(double), Node* left, Node* right);

Node* build_node_tree(Token* tokens, int count);

double eval_node(Node* node, double x_val);

void node_free_mem(Node* node);

#ifdef __cplusplus
}
#endif

#endif
