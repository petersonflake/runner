#include "command.h"

command* init_command(argstack *args, char *cmd)
{
    command* tmp = malloc(sizeof(command));
    tmp->text = strdup(cmd);
    tmp->to_run = 0;
    tmp->arguments = args;
    return tmp;
}
void command_free(command *cmd)
{
    if(!cmd) return;
    argstack_free(cmd->arguments);
    free(cmd->text);
    free(cmd);
}

command_stack* init_cmd_stack(int size)
{
    command_stack *tmp = malloc(sizeof(command_stack));
    tmp->data = calloc(size, sizeof(command*));
    tmp->alloc = size;
    tmp->count = 0;
    return tmp;
}

int command_stack_push(command_stack *cmd_stack, command *cmd)
{
    if(cmd_stack->alloc >= (cmd_stack->count-1)) {
        cmd_stack->alloc += 10;
        void *tmp = reallocarray(cmd_stack->data, cmd_stack->alloc, sizeof(command*));
        if(tmp) cmd_stack->data = tmp;
        else exit(1);
    }
    cmd_stack->data[cmd_stack->count++] = cmd;
    return cmd_stack->count;
}

void command_stack_free(command_stack *cmd_stack)
{
    if(!cmd_stack) return;
    for(int i = 0; i < cmd_stack->alloc; ++i) {
        if(cmd_stack->data[i]) {
            command_free(cmd_stack->data[i]);
        }
    }
    free(cmd_stack);
}

