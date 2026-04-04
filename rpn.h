#include "tokenize.h"

/**
 * @brief Reorders tokens into [Reverse Polish Notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) (RPN).
 * * Using an implementation of the Dijkstra's [Shunting-yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm). This process reorders 
 * the expression to handle operator precedence and associativity, 
 * effectively removing both the need for parentheses and operator precedence ambiguity.
 * * @note This function performs an **in-place** modification of the `tokens` array. 
 * Parentheses are discarded, and the `count` is updated accordingly.
 * * @param tokens     The array of tokens to be converted
 * @param count      Pointer to the token count (updated after parentheses removal)
 * @param size_token Maximum capacity of the token buffer to prevent overflows
 */
void parse_to_rpn(Token *tokens, int *count, int size_token);