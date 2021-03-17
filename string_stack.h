#ifndef STRING_STACK_H
#define STRING_STACK_H
#include <string.h>
#include <stdlib.h>

/*
 * The string stack is an array of strings that manages its own memory.
 * It automatically resizes itself when a string is pushed to it, and always
 * has at least one extra element allocated and set to NULL so that the data
 * member can be passed directly to functions which take a NULL terminated
 * array of strings.  string_stack->data == char **argv
 *
 * NOTE: Each string passed is duplicated, so if a dynamically allocated
 * char * is passed to it, you should free that afterward.
 */

typedef struct {
    int alloc;
    int count;
    char **data;
} string_stack;

string_stack* init_str_stack(int size);

int str_stack_push(string_stack *stack, char *string);

void str_stack_free(string_stack *stack);

#endif
