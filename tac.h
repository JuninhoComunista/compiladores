//Nome: Nicolas Paris - Cartao: 302650

#pragma once

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_COPY 3
#define TAC_IFZ 4
#define TAC_LABEL 5

typedef struct tac_node {
    int type;
    HashNode* res;
    HashNode* op1;
    HashNode* op2;
    struct tac_node* prev;
    struct tac_node* next;
} Tac;

Tac* tacCreate(int type, HashNode *res, HashNode *op1, HashNode *op2);
void tacPrintSingle(Tac *tac, FILE *output);
void tacPrintBack(Tac *tac, FILE *output);
Tac* tacJoin(Tac *l1, Tac *l2);
Tac* generateCode(Ast *node);