%{
#include <string.h>
#include <stdio.h>
#include "command.h"
int yyerror(char *msg);
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

entries:    entry               {//command_stack_push(all_commands, current_command);
                                 current_args = init_str_stack(6);
                                 current_argstack = init_argstack(6);}
       |    entries entry
       ;

strings:    STR                  {str_stack_push(current_args, $1);}
       |    strings ',' STR      {str_stack_push(current_args, $3);}
       ;

arglists:   strings ';'          {argstack_push(current_argstack, current_args);}
        |   arglists strings ';'
        ;

entry:  ID '{' arglists '}' ID ';' {current_command = init_command(current_argstack, $1);
                                    command_stack_push(all_commands, current_command);}
     ;

%%

//int main(int argc, char **argv)
//{
//    scrparse();
//    return 0;
//}

int yyerror(char *msg)
{
    fprintf(stdout, "Error: %s\n", msg);
    return 0;
}
