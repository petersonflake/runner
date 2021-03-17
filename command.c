#include "command.h"

command* init_command(argstack *args, char *cmd, char *id)
{
    command* tmp = malloc(sizeof(command));
    tmp->text = strdup(cmd);
    tmp->id = strdup(id);
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

void command_exec(command *cmd)
{
    /* Execute command if it has been marked on the command line. */
    if(!cmd->to_run) return;
    for(int i = 0; i < cmd->arguments->count; ++i) {
        pid_t pid = fork();
        /* Try executing the script from the scripts directory.  If that fails, use PATH */
        if(!pid) {
        /* Copy the name of the script into the first element of argv.  Manual says by convention,
         * the first element of char *argv[] should be the name of the executable.  Copying just in
         * case there are programs that rely on that.
         */
            char **argv = cmd->arguments->data[i]->data;
            free(argv[0]);
            /* Get full path to script, if it is in the scripts folder */
            argv[0] = strdup(cmd->text);
            char path[2048];
            sprintf(path, "%s/%s", scripts_dir, cmd->text);
            /* If verbosity less than 1, redirect stdout and stderr to dev/null */
            if(verbosity < -1) {
                int fd = open("/dev/null", O_WRONLY);
                dup2(fd, 1);
                dup2(fd, 2);
                close(fd);
            }

            /* Check if file can be executed */
            if(!access(path, X_OK))
                execv(path, cmd->arguments->data[i]->data);
            else /* Otherwise, see try getting it from path */
                if(execvp(cmd->text, cmd->arguments->data[i]->data))
                    if(verbosity)
                        fprintf(stdout, "Script or program \"%s\" not found.\n", cmd->text);
        } else {
            wait(NULL); /* Wait for fork to exit before moving on to next command. */
        }
    }
}
