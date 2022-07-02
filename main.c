//Nome: Nicolas Paris - Cartao: 302650
#include "tokens.h"
#include "hash.h"
#include <stdio.h>

int yylex();
void initMe();
extern char* yytext;
extern int lineCount;
extern FILE* yyin;
extern HashTable* identifiersTable;
int isRunning(void);

int main(int argc, char** argv) {
    int ntoken;
    if (argc < 2)
    {
    printf("call: ./etapa1 input.txt output.txt \n");
    exit(1);
    }
    if (0==(yyin = fopen(argv[1],"r")))
    {
    printf("Cannot open file %s... \n",argv[1]);
    exit(1);
    }
    initMe();
    
    while (isRunning())
    {
      ntoken = yylex();

      if(!isRunning())
        break;

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
              // printf("Integer \"%s\"\n", yytext);
              break;
          case LIT_FLOAT:
              // printf("Float \"%s\"\n", yytext);
              break;
          case LIT_CHAR:
              // printf("Char \"%c\"\n", yytext[1]);
              break;
          case LIT_STRING:
              // printf("String %s\n", yytext);
              break;
          case TK_IDENTIFIER:
              // printf("Identifier \"%s\"\n", yytext);
              break;
          case TOKEN_ERROR:
              printf("Invalid token \"%s\" in line %d\n", yytext, lineCount);
              break;
          default:
              printf("Operator %s\n", yytext);
      }
    }

    printf("table numElements: %d\n", identifiersTable->usedIndexes);
    
    return 0;
}