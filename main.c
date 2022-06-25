#include "tokens.h"
#include "hash.h"
#include <stdio.h>

extern int yylex();
extern char* yytext;
extern int lineCount;

int main(void) {
    int ntoken;
    ntoken = yylex();
    while (ntoken)
    {
        switch(ntoken) {
            case KW_CHAR:
            case KW_INT:
            case KW_FLOAT:
            case KW_IF:
            case KW_ELSE:
            case KW_WHILE:
            case KW_READ:
            case KW_PRINT:
            case KW_RETURN:
                printf("Keyword \"%s\"\n", yytext);
                break;
            case LIT_INTEGER:
                printf("Integer \"%s\"\n", yytext);
                break;
            case LIT_FLOAT:
                printf("Float \"%s\"\n", yytext);
                break;
            case LIT_CHAR:
                printf("Char \"%c\"\n", yytext[1]);
                break;
            case LIT_STRING:
                printf("String %s\n", yytext);
                break;
            case TK_IDENTIFIER:
                // printf("Identifier \"%s\"\n", yytext);
                printf("Hash value of \"%s\": %lu\n", yytext, hash_function(yytext));
                break;
            case TOKEN_ERROR:
                printf("Invalid token \"%s\" in line %d\n", yytext, lineCount);
                break;
            default:
                printf("Operator %s\n", yytext);
        }
        ntoken = yylex();
    }
    
    return 0;
}