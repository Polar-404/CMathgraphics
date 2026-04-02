#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>

typedef enum {
    TYPE_VAR,
    TYPE_NUM, 
    TYPE_OP,
    TYPE_FUNC, 
} NodeType;

typedef struct {
    const char* name;
    double (*func_ptr)(double); //function that recives a double as parameter and returns a double
} MathFunction;

MathFunction func_map[] = {
    {"sin", sin},
    {"cos", cos},
    {"sqrt", sqrt},
};

struct Node {
    NodeType type;
    double value;
    char op;
    double (*func_ptr)(double); 
    struct Node *left;
    struct Node *right;
};

struct Node*
node_new_num(double val) {
    struct Node* node = malloc(sizeof(struct Node));

    if (node) {
        node -> type = TYPE_NUM;
        node -> value = val;
        node -> left = NULL;
        node -> right = NULL;
    }
    return node;
};

struct Node* 
node_new_op(char op, struct Node* left, struct Node* right) {
    struct Node* node = malloc(sizeof(struct Node));

    if (node) {
        node -> type = TYPE_OP;
        node -> op = op;
        node -> left = left;
        node -> right = right;
    }
    return node;
};

struct Node*
node_new_func(double (*func)(double), struct Node* left, struct Node* right) {
    struct Node* node = malloc(sizeof(struct Node));

    if (node) {
        node -> type = TYPE_FUNC;
        node -> func_ptr = func;
        node -> left = left;
        node -> right = right;
    }
    return node;
};

struct Node* 
node_new_var() {
    struct Node* node = malloc(sizeof(struct Node));
    if (node) {
        node->type = TYPE_VAR;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
};

double
eval_node(struct Node* node, double x_val) {
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
node_free_mem(struct Node* node) {
    if (!node) return;

    node_free_mem(node->left);
    node_free_mem(node->right);

    free(node);
}

int main() {

    struct Node* n2 = node_new_num(2.0);
    struct Node* n5 = node_new_num(5.0);
    struct Node* op_mul = node_new_op('*', n2, n5);

    struct Node* var_x = node_new_var();
    struct Node* f_sin = node_new_func(sin, var_x, NULL);

    struct Node* root = node_new_op('+', f_sin, op_mul);

    double x = 25.59;
    double result = eval_node(root, x);

    SetConsoleOutputCP(CP_UTF8);

    printf("Equação: 𝒇(𝒙) = 𝒔𝒊𝒏(𝒙) + 2 × 5\n");
    printf("with 𝒙 = %.2f, 𝒇(𝒙) = %.2f\n", x, result);

    node_free_mem(root);
    root = NULL;

    return 0;
}