#ifndef ARGSTACK_H
#define ARGSTACK_H
#include "string_stack.h"

typedef struct {
    int alloc;
    int count;
    string_stack **data;
} argstack;

argstack* init_argstack(int size);

int argstack_push(argstack *stack, string_stack *strs);

void argstack_free(argstack *stack);

#endif
