#ifndef ARGSTACK_H
#define ARGSTACK_H
#include "string_stack.h"

/*
 * This structure holds an array of arrays of strings.
 * Each array of strings holds the arguments passed to the
 * command on a single run, so the argstack holds all
 * the sets of arguments to be passed.
 */

typedef struct {
    int alloc;
    int count;
    string_stack **data;
} argstack;

argstack* init_argstack(int size);

int argstack_push(argstack *stack, string_stack *strs);

void argstack_free(argstack *stack);

#endif
