// Nome: Nicolas Paris - Cartao: 302650

#include "ast.h"
#include "astTypes.h"

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

void astPrint(Ast *node, int level) {
    if (!node)
        return;
    int i;

    for (i=0; i<level; i++)
        fprintf(stderr, "--");

    switch(node->type){
        case AST_DEC: fprintf(stderr, "AST_DEC\n"); break;
        case AST_VAR_DEC: fprintf(stderr, "AST_VAR_DEC\n"); break;
        case AST_FUNC_DEC: fprintf(stderr, "AST_FUNC_DEC\n"); break;
        case AST_VEC_DEC: fprintf(stderr, "AST_VEC_DEC\n"); break;
        case AST_PARAM_LIST: fprintf(stderr, "AST_PARAM_LIST\n"); break;
        case AST_PARAM: fprintf(stderr, "AST_PARAM\n"); break;
        case AST_BLOCK: fprintf(stderr, "AST_BLOCK\n"); break;
        case AST_COMMAND: fprintf(stderr, "AST_COMMAND\n"); break;
        case AST_ASSIGNMENT: fprintf(stderr, "AST_ASSIGNMENT\n"); break;
        case AST_VEC_ASSIGNMENT: fprintf(stderr, "AST_VEC_ASSIGNMENT\n"); break;
        case AST_IDENTIER: fprintf(stderr, "AST_IDENTIER\n"); break;
        case AST_LITERAL: fprintf(stderr, "AST_LITERAL\n"); break;
        case AST_CHAR: fprintf(stderr, "AST_CHAR\n"); break;
        case AST_INT: fprintf(stderr, "AST_INT\n"); break;
        case AST_FLOAT: fprintf(stderr, "AST_FLOAT\n"); break;
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL\n"); break;
        case AST_LITERAL_LIST: fprintf(stderr, "AST_LITERAL_LIST\n"); break;
        default: fprintf(stderr, "AST_UNKNOWN\n"); break;
    }

    if (node->symbol) {
        for (i=0; i<level; i++)
            fprintf(stderr, "--");
        fprintf(stderr, "symbol= %s\n", node->symbol->value);
    }

    for (i=0; i<MAX_SONS; ++i){
        astPrint(node->son[i], level+1);
    }

}