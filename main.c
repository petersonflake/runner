#include <stdio.h>
#include "scripts.tab.h"
#include "command.h"

extern FILE* scrin;

extern command_stack *all_commands;

extern command *current_command;

extern string_stack *current_args;

extern argstack *current_argstack;

void print_commands(command_stack *stack);

void print_str_stack(string_stack *stack);

void print_arg_stack(argstack *args);

int main(int argc, char **argv)
{
    all_commands = init_cmd_stack(6);
    current_args = init_str_stack(6);
    current_argstack = init_argstack(6);
    //str_stack_push(current_args, "jack");
    //str_stack_push(current_args, "jane");
    //str_stack_push(current_args, "joe");
    //str_stack_push(current_args, "joseph");
    //argstack_push(current_argstack, current_args);
    //current_command = init_command(current_argstack, "echo");
    //command_stack_push(all_commands, current_command);
    //string_stack *test = init_str_stack(2);
    //argstack* args = init_argstack(2);
    //str_stack_push(test, "hello");
    //str_stack_push(test, "hello");
    //str_stack_push(test, "hello");
    //argstack_push(args, test);
    //for(int i = 0; i < test->count; ++i) {
    //    printf("%s\n", test->data[i]);
    //}
    scrin = fopen("test.txt", "r");
    scrparse();
    //argstack_free(args);
    print_commands(all_commands);
    return 0;
}

void print_str_stack(string_stack *stack)
{
    for(int i = 0; i < stack->count; ++i) {
        printf("%s ", stack->data[i]);
    }
    printf("\n");
}

void print_arg_stack(argstack *args)
{
    printf("Arg stack: \n");
    for(int i = 0; i < args->count; ++i) {
        print_str_stack(args->data[i]);
    }
}

void print_commands(command_stack *stack)
{
    for(int i = 0; i < stack->count; ++i) {
        printf("command: %s\n", stack->data[i]->text);
        print_arg_stack(stack->data[i]->arguments);
    }
}

