%{
    #include "tokens.h"
    int countLines = 1;
    int running = 1;

    int isRunning(void);
    int getLineNumber(void);
%}


%%

"char"              {return KW_CHAR;}
"int"               {return KW_INT;}
"float"             {return KW_FLOAT;}
"if"                {return KW_IF;}
"else"              {return KW_ELSE;}
"while"             {return KW_WHILE;}
"read"              {return KW_READ;}
"print"             {return KW_PRINT;}
"return"            {return KW_RETURN;}

[-;()[\]{}+./<>&|~] {return yytext[0];}

[1-9][0-9]*         {return LIT_INTEGER;}
[1-9][0-9]*,[0-9]+  {return LIT_FLOAT;}

[a-zA-Z]*           {return TK_IDENTIFIER;}

[ \t]*              {}
\n                  {countLines++;}

.*                  {return TOKEN_ERROR;}
%%

#include "main.c"

int isRunning(void) {
    return running;
}

int getLineNumber(void) {
    return countLines;
}

int yywrap(void) {
    running = 0;
    printf("Total number of lines: %d\n", getLineNumber());
    return 1;
}
