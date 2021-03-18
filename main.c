#include "main.h"
/* flex and bison both have prefix scr, for scripts */
extern FILE* scrin;

/* We need to initialize these variables before parsing anything
 * The parser assigns values to the variables, then creates a command
 * which is put onto the stack all_commands, which is an array.
 * Variables are global because they're needed in scripts.y */
extern command_stack *all_commands;

extern command *current_command;

extern string_stack *current_args;

extern argstack *current_argstack;

int verbosity = 0; /* Global so we don't have to pass it to every function */

/* Seems unlikely a path will be longer than 4095 characters */
char data_dir[4096]; /* Where we read the files */

char scripts_dir[4096]; /* When executing a script, we check here before searching path */

int main(int argc, char **argv, char **envp)
{
    int c;                  /* Option for getopt_long */
    int to_execute = 0;     /* Whether the provided commands are to be carried out */
    int show_commands = 0;  /* Show each command, togther with its arguments */
    int file_provided = 0;  /* Whether a file was provided on the command line */
    int do_dry_run = 0;     /* Show what commands would be executed without running them */
    int completions = 0;    /* Used for bash tab completion */
    int force_all = 0;      /* In case the user really wants to run all commands */

    while(1) {
        static struct option long_options[] = {
            {"file",    required_argument,      0,  'f'},
            {"complete",no_argument,            0,  'c'},
            {"dry-run", no_argument,            0,  'd'},
            {"exec",    no_argument,            0,  'e'},
            {"help",    no_argument,            0,  'h'},
            {"quiet",   no_argument,            0,  'q'},
            {"show",    no_argument,            0,  's'},
            {"verbose", no_argument,            0,  'v'},
            {"all",     no_argument,            0,  'a'},
            {0,         0,                      0,   0 },
        };

        int option_index = 0;
        c = getopt_long(argc, argv, "vehsf:dcqa", long_options, &option_index);
        if(c == -1) break; /* End of options */

        switch(c) {
            case 0:
                break;
            case 'a':
                force_all = 1;
                break;
            case 'v':
                ++verbosity;
                break;
            case 'e':
                to_execute = 1;
                break;
            case 'h':
                show_help();
                exit(EXIT_SUCCESS);
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
            case 'd':
                do_dry_run = 1;
                break;
            case 'c':
                completions = 1;
                break;
            case 'q':
                --verbosity;
                break;
            case '?':
                break;
            default:
                abort();
        }
    }
    all_commands = init_cmd_stack(6);
    current_args = init_str_stack(6);
    current_argstack = init_argstack(6);        /* Must be initialized before we parse the files */
    str_stack_push(current_args, "NOT USED");   /* This will be replaced with */
                                                /* the script name before execution in command_exec */

    /* If a file was not provided, check in XDG_CONFIG_HOME, in .config, and in the home directory.
     * If there is not a valid data directory anywhere, print a message and exit.
     */
    if(!file_provided) {
        if(set_data_dir(data_dir)) {            /* If return value not zero, no data directory was accessible */
            fprintf(stderr, "Unable to access data directory.\n");
            fprintf(stderr, "Please create one.\n");
            exit(EXIT_FAILURE);
        }
        nftw(data_dir, parse_file, FDS, FTW_NS); /* Travese whatever data directory was found first */
        sprintf(scripts_dir, "%s/%s", data_dir, "/scripts");
    } else {
        scrparse();         /* scrin is checked when the filename is passed, so no need to check here */
    }

    /* Print all ids for bash tab completion */
    if(completions) {       /* For completions to work, we need to read the files, but do nothing else */
        print_completions(all_commands);
        exit(EXIT_SUCCESS);
    }
    if(argc == 1) {             /* The user passed no arguments.  Show all commands and exit */
        for(int i = 0; i < all_commands->count; ++i) {
            all_commands->data[i]->to_run = 1;
        }
        show_commands = 1;
        to_execute = 0;     /* Ensure that the commands cannot be run to prevent running all by mistake */
    }
    if(force_all) {
        for(int i = 0; i < all_commands->count; ++i) {
            all_commands->data[i]->to_run = 1;
        }
        if(!to_execute && !show_commands && !do_dry_run)
            show_commands = 1;
    }
    if(optind < argc && !force_all) {/* All provided arguments will be marked for execution, */
        int i = optind;              /* or to be shown in a dry run, or in show mode */
        while(i < argc) {            /* Unless -a --all flags passed */
            mark_cmd(all_commands, argv[i++]);
        }
    } else if(optind < argc && force_all) {
        fprintf(stderr, "-a --all flags mark all for execution.  If this was intended, run again without ids.\n");
        exit(EXIT_FAILURE);
    }
    if(optind == argc) {         /* All arguments were ids.  Show them and exit */
        for(int i = 0; i < all_commands->count; ++i) {
            all_commands->data[i]->to_run = 1;
        }
        if(to_execute && !(force_all)) {
            to_execute = 0;         /* Don't accidentally run all commands */
            fprintf(stderr, "Not executing all commands.  Use -a to force all commands to run.\n");
        }
    }
    if(show_commands)
        print_commands(all_commands, 0);
    if(do_dry_run) {        /* Make sure that the program exits before it can execute anything in dry-run mode */
        dry_run(all_commands);
        exit(EXIT_SUCCESS);
    }
    if(to_execute)
        exec_all_cmds(all_commands);
    command_stack_free(all_commands);
    if(current_argstack) argstack_free(current_argstack);   /* Free much of the memory used in parsing */
    if(current_args) str_stack_free(current_args);          /* Valgrind still shows memory being allocated */
    if(current_command) command_free(current_command);      /* but not freed, but that is from flex or bison */
    return 0;
}

/*
 * Helper functions for showing command information.
 */
void print_str_stack(string_stack *stack)
{
    printf("Arguments:\n");
    for(int i = 1; stack->data[i]; ++i) {
        printf("\t%d: %s\n", i, stack->data[i]);
    }
    printf("\n");
}

void print_arg_stack(argstack *args)
{
    for(int i = 0; i < args->count; ++i) {
        printf("Run: %d\n", i+1);
        print_str_stack(args->data[i]);
    }
}

void print_commands(command_stack *stack, int allflag)
{
    for(int i = 0; i < stack->count; ++i) {
        command *tmp = stack->data[i];
        if(!allflag)
            if(!tmp->to_run) continue;
        printf("Command: %s\n", tmp->text);
        printf("Identifier: %s\n", tmp->id);
        if(tmp->docstring)
            printf("Doc: %s\n", tmp->docstring);
        print_arg_stack(stack->data[i]->arguments);
    }
}

/*
 * Execute every marked command.
 */
void exec_all_cmds(command_stack *stack)
{
    for(int i = 0; i < stack->count; ++i) {
        if(stack->data[i]->to_run)
            command_exec(stack->data[i]);
    }
}

/*
 * Mark every command with a matching id.
 */
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
    printf("runner [OPTIONS] tasks\n");
    printf("--help -h       Show this help.\n");
    printf("--all -a        Mark all commands for show or execution.\n");
    printf("--file -f       Provide file as argument.\n");
    printf("--exec -e       Execute all provided commands.\n");
    printf("--show -s       Show details about the command.\n");
    printf("--quiet -q      Reduce program output.\n");
    printf("--verbose -v    Increase verbosity.\n");
    printf("with -qq, output from commands is routed to /dev/null.\n");
    printf("Please see README or manual page for more information.\n");
}

/*
 * Show what commands would be run, in the form <command> <args>...
 */
void dry_run(command_stack *stack)
{
    int printed_something = 0;
    if(verbosity > 0)
        printf("Dry Run: showing commands to be executed.\n");
    for(int i = 0; i < stack->count; ++i) {
        command *cmd = stack->data[i];
        if(!cmd->to_run) continue;
        printed_something = 1;
        if(verbosity > 0)
            if(cmd->docstring) printf("%s\n", cmd->docstring);
        for(int j = 0; j < cmd->arguments->count; ++j) {
            argstack *arg_stack = cmd->arguments;
            if(verbosity > 0)
                printf("\t%s ", cmd->text);
            else printf("%s ", cmd->text);
            for(int k = 1; k < arg_stack->data[j]->count; ++k) {
                printf("%s ", arg_stack->data[j]->data[k]);
            }
        }
        printf("\n");
    }
    if(verbosity > 0)
        if(!printed_something) fprintf(stderr, "No commands with that id found.\n");
}

void print_completions(command_stack *stack)
{
    for(int i = 0; i < stack->count-1; ++i) {
        command *cmd = stack->data[i];
        printf("%s ", cmd->id);
    }
    printf("%s", stack->data[stack->count-1]->id);
}

int parse_file(const char *filepath, const struct stat *info,
        int typeflag, struct FTW *pathinfo)
{
    if(typeflag != FTW_F) return 0;
    if(pathinfo->level > 1) return 0;
    if(verbosity > 1)
        printf("Reading file: \"%s\"\n", filepath);
    FILE *fp;
    if((fp = fopen(filepath, "r"))) {
        scrin = fp;
        scrparse();
        fclose(fp);
    }
    return 0;
}

int set_data_dir(char *buffer)
{
    char *config_home = getenv("XDG_CONFIG_HOME");
    if(config_home) {
        strcpy(data_dir, config_home);
        strcat(data_dir, "/runner");
    } else {
        if(verbosity > 0) {
            fprintf(stderr, "Unable to read $XDG_CONFIG_HOME.\n");
        }
    }
    if(is_valid_datadir(buffer)) return 0;
    if(verbosity > 0)
        fprintf(stdout, "Unable to get XDG_CONFIG_HOME/runner.\n");
    char *home_dir = getenv("HOME");
    if(home_dir) {
        strcpy(buffer, home_dir);
        strcat(buffer, "/.config/runner");
        if(is_valid_datadir(buffer)) return 0;
        if(verbosity > 0)
            fprintf(stderr, "Unable to access ~/.config/runner.\n");
        strcpy(buffer, home_dir);
        strcat(buffer, "/.runner");
        if(is_valid_datadir(buffer)) return 0;
        if(verbosity > 0)
            fprintf(stderr, "Unable to get ~/.runner. Exiting\n");
        return -1;
    } else {
        if(verbosity > 0) {
            fprintf(stderr, "Unable to get $HOME directory.\n");
        }
        return -1;
    }
}

int is_valid_datadir(char *path)
{
    struct stat info;
    stat(path, &info);
    if(S_ISDIR(info.st_mode)) {
        if(!access(path, R_OK)) return 1;
    }
    return 0;
}
