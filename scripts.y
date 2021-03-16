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

entries:    entry
       |    entries entry
       ;

string:     STR
      ;

strings:    string
       |    strings ',' string
       ;

arglists:   strings ';'
        |   arglists strings ';'
        ;

entry:  ID '{' arglists '}' ID ';'
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
