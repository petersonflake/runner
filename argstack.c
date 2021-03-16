#include "argstack.h"

argstack* init_argstack(int size)
{
    argstack* tmp = malloc(sizeof(argstack));
    tmp->data = calloc(size, sizeof(string_stack*));
    tmp->alloc = size;
    tmp->count = 0;
    return tmp;
}

int argstack_push(argstack *stack, string_stack *strs)
{
    if(stack->count >= (stack->alloc-1)) {
        stack->alloc += 10;
        void *tmp = reallocarray(stack->data, sizeof(string_stack*), stack->alloc);
        if(tmp) stack->data = tmp;
        else exit(1);
    }
    stack->data[stack->count++] = strs;
    return stack->count;
}

void argstack_free(argstack *stack)
{
    if(!stack) return;
    for(int i = 0; i < stack->alloc; ++i) {
        if(stack->data[i]) {
            str_stack_free(stack->data[i]);
        }
    }
    free(stack->data);
    free(stack);
}
