%{
#include <string.h>
#include <stdio.h>
int yyerror(char *msg);
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

arglist:    STR ';' {printf("read %s\n", $1);}
       |    arglist ',' STR
       ;

arglists:   arglist
        |   arglists arglist
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
