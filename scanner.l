%{
    //Nome: Nicolas Paris - Cartao: 302650
    #include "tokens.h"
    #include "hash.h"

    #define TABLESIZE 499
    int lineCount = 1;
    int running = 1;
    HashTable* identifiersTable;

    int isRunning(void);
    int getLineNumber(void);

    void initMe(void);

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

[a-z_A-Z]*          {hashInsert(identifiersTable, yytext, yytext); return TK_IDENTIFIER;}

[0-9]+              {hashInsert(identifiersTable, yytext, yytext); return LIT_INTEGER;}
[0-9]+,[0-9]+       {hashInsert(identifiersTable, yytext, yytext); return LIT_FLOAT;}
\'.\'               {hashInsert(identifiersTable, yytext, yytext); return LIT_CHAR;}
\"(\\.|[^"\\])*\"   {hashInsert(identifiersTable, yytext, yytext); return LIT_STRING;}

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

void initMe(void) { 
    identifiersTable = createTable(TABLESIZE);
}

int yywrap(void) {
    running = 0;
    printTable(identifiersTable);
    printf("Total number of lines: %d\n", getLineNumber());
    return 1;
}
