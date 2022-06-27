#include "tokens.h"
#include <stdio.h>

extern int yylex();
extern void initMe();
extern char* yytext;
extern int lineCount;
extern HashTable* identifiersTable;

int main(void) {
    int ntoken;
    initMe();
    ntoken = yylex();
    while (ntoken)
    {
        // switch(ntoken) {
        //     case KW_CHAR:
        //     case KW_INT:
        //     case KW_FLOAT:
        //     case KW_IF:
        //     case KW_ELSE:
        //     case KW_WHILE:
        //     case KW_READ:
        //     case KW_PRINT:
        //     case KW_RETURN:
        //         printf("Keyword \"%s\"\n", yytext);
        //         break;
        //     case LIT_INTEGER:
        //         printf("Integer \"%s\"\n", yytext);
        //         break;
        //     case LIT_FLOAT:
        //         printf("Float \"%s\"\n", yytext);
        //         break;
        //     case LIT_CHAR:
        //         printf("Char \"%c\"\n", yytext[1]);
        //         break;
        //     case LIT_STRING:
        //         printf("String %s\n", yytext);
        //         break;
        //     case TK_IDENTIFIER:
        //         // printf("Identifier \"%s\"\n", yytext);
        //         break;
        //     case TOKEN_ERROR:
        //         printf("Invalid token \"%s\" in line %d\n", yytext, lineCount);
        //         break;
        //     default:
        //         printf("Operator %s\n", yytext);
        // }
        if (ntoken != TK_IDENTIFIER) {
            hashInsert(identifiersTable, yytext, yytext);
        }
        ntoken = yylex();
    }

    printf("table numElements: %d\n", identifiersTable->numElements);
    
    return 0;
}