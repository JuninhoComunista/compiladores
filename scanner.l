%{
    #include "tokens.h"
    int lineCount = 1;
    int running = 1;

    int isRunning(void);
    int getLineNumber(void);
%}

%x COMMENT

%%

"##".*              {}
"\\#"               {BEGIN COMMENT;}
<COMMENT>\n         {lineCount++;}
<COMMENT>"#\\"      {BEGIN INITIAL;}
<COMMENT>.          {}

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

(<-)                {return ASSIGNMENT;}
(<=)                {return OPERATOR_LE;}    
(>=)                {return OPERATOR_GE;}
(==)                {return OPERATOR_EQ;}
(!=)                {return OPERATOR_DIF;}

[a-z_A-Z]*          {return TK_IDENTIFIER;}

[0-9]+              {return LIT_INTEGER;}
[0-9]+,[0-9]+       {return LIT_FLOAT;}
\'.\'               {return LIT_CHAR;}
\"(\\.|[^"\\])*\"   {return LIT_STRING;}

[ \t]*              {}
\n                  {lineCount++;}

.                   {return TOKEN_ERROR;}

%%

#include "main.c"

int isRunning(void) {
    return running;
}

int getLineNumber(void) {
    return lineCount;
}

int yywrap(void) {
    running = 0;
    printf("Total number of lines: %d\n", getLineNumber());
    return 1;
}
