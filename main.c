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
extern Ast* tree;
extern HashTable* identifiersTable;
int isRunning(void);

int main(int argc, char** argv) {
    int ntoken;
    FILE *output;
    if (argc < 3) {
        printf("call: ./etapa1 input.txt output.txt \n");
        exit(1);
    }
    if (0==(yyin = fopen(argv[1],"r"))) {
        printf("Cannot open file %s... \n",argv[1]);
        exit(2);
    }
    if (0==(output = fopen(argv[2],"w"))) {
        printf("Cannot open file %s... \n",argv[2]);
        exit(4);
    }
    initMe();

    yyparse();

    printf("\nPrinting tree\n\n");
    astPrint(tree, 0, output);

    fclose(output);

    // printTable(identifiersTable);

    // printf("table numElements: %d\n", identifiersTable->numElements);
    
    exit(0);
}