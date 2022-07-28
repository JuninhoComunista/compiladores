//Nome: Nicolas Paris - Cartao: 302650
#include "hash.h"
#include "ast.h"
#include "y.tab.h"
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

    if (argc < 3) {
        printf("call: ./etapa1 input.txt output.txt \n");
        exit(1);
    }

    if ((yyin = fopen(argv[1],"r")) == 0) {
        printf("Cannot open file %s... \n",argv[1]);
        exit(2);
    }

    initMe();

    yyparse();
    
    exit(0);
}