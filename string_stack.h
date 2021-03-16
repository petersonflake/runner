#ifndef STRING_STACK_H
#define STRING_STACK_H
#include <string.h>
#include <stdlib.h>

typedef struct {
    int alloc;
    int count;
    char **data;
} string_stack;

string_stack* init_str_stack(int size);

int str_stack_push(string_stack *stack, char *string);

void str_stack_free(string_stack *stack);

#endif
