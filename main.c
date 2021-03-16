#include <stdio.h>
#include "scripts.tab.h"
#include "argstack.h"

extern FILE* strin;

int main(int argc, char **argv)
{
    string_stack *test = init_str_stack(2);
    argstack* args = init_argstack(2);
    str_stack_push(test, "hello");
    str_stack_push(test, "hello");
    str_stack_push(test, "hello");
    argstack_push(args, test);
    for(int i = 0; i < test->count; ++i) {
        printf("%s\n", test->data[i]);
    }
    //scrparse();
    argstack_free(args);
    return 0;
}
