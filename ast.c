// Nome: Nicolas Paris - Cartao: 302650

#include "ast.h"

Ast *astCreate(int type, HashNode *symbol, Ast *son0, Ast *son1, Ast *son2, Ast *son3) {
    Ast *newNode;
    newNode = (Ast*) calloc(1, sizeof(Ast));
    newNode->type = type;
    newNode->symbol = symbol;
    newNode->son[0] = son0;
    newNode->son[1] = son1;
    newNode->son[2] = son2;
    newNode->son[3] = son3;

    return newNode;
}

void astPrint(AST *node, int level){
    if (!node)
        return;
    
    int i;

    for (i=0; i<level; i++)
        fprintf(stderr, "--");

    fprintf(stderr, "\nTYPE: ");
    switch(node->type){

        default: fprintf(stderr, "AST_UNKNOWN"); break;
    }

    if (node->symbol)
        fprintf(stderr, "\nsymbol= %s\n", node->symbol->text);
    else
        fprintf(stderr, ",0\n");

    for (i=0; i<MAX_SONS; ++i){
        printf("\nson[%d] = ",i);
        astPrint(node->son[i], level+1);
    }

}