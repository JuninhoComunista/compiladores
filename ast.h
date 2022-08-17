// Nome: Nicolas Paris - Cartao: 302650

#pragma once

#include "hash.h"
#include "astTypes.h"

#define MAX_SONS 4

typedef struct AstNode {
    int type;
    int dataType;
    int lineNumber;
    struct AstNode *son[MAX_SONS];
    HashNode *symbol;
} Ast;

Ast *astCreate(int lineNumber, int type, HashNode *symbol, Ast *son0, Ast *son1, Ast *son2, Ast *son3);

void astPrintNode(Ast *node, FILE *output);

void astPrint(Ast *node, int level, FILE *output);