//Nome: Nicolas Paris - Cartao: 302650
#include "y.tab.h"
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
    exit(2);
    }
    initMe();

    yyparse();

    printTable(identifiersTable);

    // printf("table numElements: %d\n", identifiersTable->numElements);
    
    return 0;
}