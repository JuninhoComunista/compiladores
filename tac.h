//Nome: Nicolas Paris - Cartao: 302650

#pragma once

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_COPY 2
#define TAC_IFZ 3
#define TAC_LABEL 4
#define TAC_JUMP 5
#define TAC_ADD     (AST_ADD)
#define TAC_SUB     (AST_SUB)
#define TAC_MUL     (AST_MUL)
#define TAC_DIV     (AST_DIV)
#define TAC_LES     (AST_LES)
#define TAC_GRE     (AST_GRE)
#define TAC_OR      (AST_OR )
#define TAC_AND     (AST_AND)
#define TAC_LE      (AST_LE )
#define TAC_GE      (AST_GE )
#define TAC_EQ      (AST_EQ )
#define TAC_DIF     (AST_DIF)   

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
void tacPrintBackwards(Tac *tac, FILE *output);
void tacPrintForwards(Tac *tac, FILE *output);
void reverseTacList(Tac **list);
Tac* tacJoin(Tac *l1, Tac *l2);
Tac* generateCode(Ast *node, HashTable *table);