%{
#include <string.h>
#include <stdio.h>
#include "command.h"
int yyerror(char *msg); /* Keep compiler from complaining. */
command_stack *all_commands;

command *current_command;

string_stack *current_args;

argstack *current_argstack;
%}

%union {
    char *string;
    int integer;
}

%define api.prefix {scr}

%token <string> STR
%token <integer> INT
%token <string> ID

%%

file:   /* Nothing */
    |   entries
    ;

entries:    entry               {/* Reset global variables */
                                 current_args = init_str_stack(6);
                                 str_stack_push(current_args, "NOT USED");
                                 current_argstack = init_argstack(6);
                                 }
       |    entries entry       {current_args = init_str_stack(6);
                                 str_stack_push(current_args, "NOT USED");
                                 current_argstack = init_argstack(6);}
       ;

strings:    STR                  {str_stack_push(current_args, $1);} /* Append string to current array of args */
       |    strings ',' STR      {str_stack_push(current_args, $3);} /* TODO: String concatenation and variables */
       ;

arglists:   strings ';'          {argstack_push(current_argstack, current_args); /* Append to array of arrays of strings */
                                  current_args = init_str_stack(6);
                                  str_stack_push(current_args, "NOT USED");}
        |   arglists strings ';'    {argstack_push(current_argstack, current_args);
                                    current_args = init_str_stack(6);
                                    str_stack_push(current_args, "NOT USED");}
        ;

entry:  ID '{' arglists '}' ID ';'      {current_command = init_command(current_argstack, $1, $5);
                                        command_stack_push(all_commands, current_command);
                                        current_command = NULL;}
    | STR ID '{' arglists '}' ID ';'    {current_command = init_command(current_argstack, $2, $6);
                                        current_command->docstring = strdup($1);
                                        command_stack_push(all_commands, current_command);
                                        current_command = NULL;
                                        }
     ;

%%

/*
 * No main function, we call scrparse() in main.c
 */

/*
 * Print the error.  "Error: Syntax error" is super helpful, afterall.
 */
int yyerror(char *msg)
{
    fprintf(stdout, "Error: %s\n", msg);
    return 0;
}
