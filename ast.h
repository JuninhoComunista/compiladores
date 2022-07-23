// Nome: Nicolas Paris - Cartao: 302650

#pragma once

#include "hash.h"

#define MAX_SONS 4

typedef struct Ast {
    struct Ast *node;
    int type;
    struct Ast *son[MAX_SONS];
    HashNode *symbol;
} Ast;

Ast *astCreate(int type, HashNode *symbol, Ast *son0, Ast *son1, Ast *son2, Ast *son3);

void astPrint(Ast *node, int level);