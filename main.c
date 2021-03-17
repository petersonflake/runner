#include <stdio.h>
#include <getopt.h>
#include "scripts.tab.h"
#include "command.h"

extern FILE* scrin;

extern command_stack *all_commands;

extern command *current_command;

extern string_stack *current_args;

extern argstack *current_argstack;

void print_commands(command_stack *stack, int allflag);

void print_str_stack(string_stack *stack);

void print_arg_stack(argstack *args);

void exec_all_cmds(command_stack *stack);

void mark_cmd(command_stack *stack, char *id);

void show_help();

int main(int argc, char **argv)
{
    int c;
    int verbosity = 0;
    int to_execute = 0;
    int show_commands = 0;
    int file_provided = 0;

    while(1) {
        static struct option long_options[] = {
            {"verbose", no_argument,            0,  'v'},
            {"exec",    no_argument,            0,  'e'},
            {"help",    no_argument,            0,  'h'},
            {"show",    no_argument,            0,  's'},
            {"file",    required_argument,      0,  'f'},
            {0,         0,                      0,   0 },
        };

        int option_index = 0;
        c = getopt_long(argc, argv, "vehsf:", long_options, &option_index);
        if(c == -1) break;

        switch(c) {
            case 0:
                break;

            case 'v':
                ++verbosity;
                break;
            case 'e':
                to_execute = 1;
                break;
            case 'h':
                show_help();
                break;
            case 's':
                show_commands = 1;
                break;
            case 'f':
                if(!(scrin = fopen(optarg, "r"))) {
                    printf("Unable to open file: %s\n", optarg);
                    exit(1);
                }
                file_provided = 1;
                break;

            case '?':
                break;

            default:
                abort();
        }
    }
    if(!to_execute && !show_commands && argc > 1)
        show_help();
    all_commands = init_cmd_stack(6);
    current_args = init_str_stack(6);
    current_argstack = init_argstack(6);
    str_stack_push(current_args, "NOT USED");
    if(!file_provided)
        scrin = fopen("test.txt", "r");
    if(scrin)
        scrparse();
    if(optind >= argc) {
        print_commands(all_commands, 1);
        return 0;
    }
    if(optind < argc) {
        while(optind < argc) {
            mark_cmd(all_commands, argv[optind++]);
        }
    }
    //argstack_free(args);
    if(show_commands)
        print_commands(all_commands, 0);
    if(to_execute)
        exec_all_cmds(all_commands);
    command_stack_free(all_commands);
    if(current_argstack) argstack_free(current_argstack);
    if(current_args) str_stack_free(current_args);
    if(current_command) command_free(current_command);
    return 0;
}

void print_str_stack(string_stack *stack)
{
    printf("str stack:\n");
    for(int i = 1; stack->data[i]; ++i) {
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

void print_commands(command_stack *stack, int allflag)
{
    for(int i = 0; i < stack->count; ++i) {
        command *tmp = stack->data[i];
        if(!allflag)
            if(!tmp->to_run) continue;
        printf("script: %s\n", tmp->text);
        printf("id: %s\n", tmp->id);
        if(tmp->docstring)
            printf("Doc: %s\n", tmp->docstring);
        print_arg_stack(stack->data[i]->arguments);
    }
}

void exec_all_cmds(command_stack *stack)
{
    for(int i = 0; i < stack->count; ++i) {
        if(stack->data[i]->to_run)
            command_exec(stack->data[i]);
    }
}

void mark_cmd(command_stack *stack, char *id)
{
    for(int i = 0; i < stack->count; ++i) {
        if(!strcmp(id, stack->data[i]->id)) {
            stack->data[i]->to_run = 1;
        }
    }
}

void show_help()
{
    printf("Usage here\n");
}
