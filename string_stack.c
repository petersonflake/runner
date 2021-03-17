#include "string_stack.h"

string_stack* init_str_stack(int size)
{
    string_stack *tmp = malloc(sizeof(string_stack));
    tmp->data = calloc(sizeof(char*), size);
    tmp->alloc = size;
    tmp->count = 0;
    return tmp;
}

int str_stack_push(string_stack *stack, char *string)
{
    if(stack->count >= (stack->alloc - 2)) { /* Leave one extra spot for NULL terminator */
        stack->alloc += 10;                  /* of the array */
        void *tmp = reallocarray(stack->data, stack->alloc, sizeof(char*));
        if(tmp) stack->data = tmp; /* If we are unable to re-allocate the array, exit the program */
        else exit(1);
    }
    stack->data[stack->count++] = strdup(string);
    stack->data[stack->count] = NULL;
    return stack->count;
}

void str_stack_free(string_stack *stack)
{
    if(!stack) return; /* In case NULL is passed to the function */
    for(int i = 0; i < stack->alloc; ++i) {
        if(stack->data[i])
            free(stack->data[i]);
    }
    free(stack->data);
    free(stack);
}
