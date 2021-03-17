#ifndef COMMAND_H
#define COMMAND_H
#include "argstack.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
/*
 * Each command has an array of arrays of arrays of strings representing the arguments;
 * a text member representing the command or script to be executed;
 * an optional docstring, which should be a description of what the command does;
 * an id, which is compared with arguments provided on the command line to identify
 * commands to be run; and a flag that can be checked to determine if the command should be run.
 * When run, each array of strings in each array in the argstack member is passed to the command in exec.
 * Thus, if there is an argstack of size two, the command will be run twice, with each array of strings
 * passed as arguments on their respective runs.
 *
 * The command stack is an array of commands, and has functions to handle allocation and freeing
 * of memory.
 *
 * Iterate over the command stack to mark, show or execute commands.
 */

typedef struct {
    argstack *arguments;
    char *text;
    char *docstring;
    char *id;
    int to_run;
} command;

/*
 * This structure is to make memory management easier.  After a stack is initialized,
 * push commands to it, and they will be added to the end of the array.
 * Not really a stack since iteration starts at the beginning of the array when executing
 * commands, but it would be comparatively easy to add a pop function.
 * When the commands are being run, they run in the order defined in the file.
 */

typedef struct {
    int alloc;
    int count;
    command **data;
} command_stack;

/* Command functions */
command* init_command(argstack *args, char *command, char *id);

void command_free(command *cmd);

/* Command stack functions */
command_stack* init_cmd_stack(int size);

int command_stack_push(command_stack *cmd_stack, command *cmd);

void command_stack_free(command_stack *cmd_stack);

void command_exec(command *cmd);

extern int verbosity;

/* Set in main.  Global to avoid having to be passed around.
 * Holds the scripts directory, where we look for user defined scripts.
 * This is done so the user can write scripts for simle tasks without
 * cluttering up their path with extra directories, or their bin
 * directories with small scripts intended for simple tasks.
 */
extern char scripts_dir[4096];

#endif
