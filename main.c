#include "tokens.h"
#include <stdio.h>

extern int yylex();
extern int yylineno;
extern char* yytext;

int main(void) {
    
    int ntoken;
    ntoken = yylex();
    while (ntoken)
    {
        switch(ntoken) {
            case KW_CHAR:
            case KW_INT:
            case KW_FLOAT:
                printf("Keyword \"%s\"\n", yytext);
                break;
            case LIT_INTEGER:
                printf("Integer \"%s\"\n", yytext);
                break;
            case LIT_FLOAT:
                printf("Float \"%s\"\n", yytext);
                break;
            case TK_IDENTIFIER:
                printf("Identifier \"%s\"\n", yytext);
                break;
            case '[':
            case ']':
                printf("Special char \'%c\'\n", yytext[0]);
                break;
            case TOKEN_ERROR:
                printf("Invalid token \"%s\" in line %d\n", yytext, yylineno);
                break;
        }
        ntoken = yylex();
    }
    
    return 0;
}