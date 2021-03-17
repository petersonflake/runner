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

void exec_all_cmds(command_stack *stack);

int main(int argc, char **argv)
{
    all_commands = init_cmd_stack(6);
    current_args = init_str_stack(6);
    current_argstack = init_argstack(6);
    str_stack_push(current_args, "NOT USED");
    scrin = fopen("test.txt", "r");
    scrparse();
    //argstack_free(args);
    print_commands(all_commands);
    exec_all_cmds(all_commands);
    return 0;
}

void print_str_stack(string_stack *stack)
{
    printf("str stack:\n");
    for(int i = 0; stack->data[i]; ++i) {
        printf("arg %d: %s\n", i, stack->data[i]);
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
        printf("identifier: %s\n", stack->data[i]->id);
        print_arg_stack(stack->data[i]->arguments);
    }
}

void exec_all_cmds(command_stack *stack)
{
    for(int i = 0; i < stack->count; ++i) {
        command_exec(stack->data[i]);
    }
}
