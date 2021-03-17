#ifndef COMMAND_H
#define COMMAND_H
#include "argstack.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct {
    argstack *arguments;
    char *text;
    char *docstring;
    char *id;
    int to_run;
} command;

typedef struct {
    int alloc;
    int count;
    command **data;
} command_stack;

command* init_command(argstack *args, char *command, char *id);

void command_free(command *cmd);

command_stack* init_cmd_stack(int size);

int command_stack_push(command_stack *cmd_stack, command *cmd);

void command_stack_free(command_stack *cmd_stack);

void command_exec(command *cmd);

extern command_stack *all_commands;

extern command *current_command;

extern string_stack *current_args;

extern int verbosity;

extern char scripts_dir[4096];

#endif
