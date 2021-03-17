%{
#include "scripts.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%option prefix="scr"
%option noyywrap

%x COMMENT
%%

";"     {return ';';}

"{"     {return '{';}

"}"     {return '}';}

","     {return ',';}

\"(([^\"]|\\\")*[^\\])?\"  {scrlval.string = strndup(scrtext+1, strlen(scrtext)-2); return STR;}

[a-zA-Z_0-9.]+   {scrlval.string = strdup(scrtext); return ID;}

"/*"                    {BEGIN(COMMENT);} /* C style comments */
<COMMENT>[^*]*
<COMMENT>"*"+[^*/\n]*
<COMMENT>"*"+"/"        {BEGIN(INITIAL);}

#.*\n    { /* Ignore comments */ }

.|\n    { /* Ignore the rest */ }
