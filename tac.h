//Nome: Nicolas Paris - Cartao: 302650

#pragma once

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_COPY 3
#define TAC_IFZ 4
#define TAC_LABEL 5
#define TAC_ADD             14
#define TAC_SUB             15
#define TAC_MUL             16
#define TAC_DIV             17
#define TAC_LES             18
#define TAC_GRE             19
#define TAC_OR              20
#define TAC_AND             21
#define TAC_LE              22
#define TAC_GE              23
#define TAC_EQ              24
#define TAC_DIF             25    

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
Tac* generateCode(Ast *node, HashTable *table);