//Nome: Nicolas Paris - Cartao: 302650

#include "tac.h"

Tac* tacJoin(Tac *l1, Tac *l2) {
    if (!l1)
        return l2;

    if (!l2)
        return l1;

    Tac *tac;
    for (tac = l2; tac->prev; tac = tac->prev)
    ;
    tac->prev = l1;
    return l2;
}

void tacPrintSingle(Tac *tac, FILE *output) {
    if (!tac || !output)
        return;
    
    fprintf(output, "TAC(");
    switch(tac->type) {
        case TAC_SYMBOL: fprintf(output, "TAC_SYMBOL"); break;
        case TAC_ADD: fprintf(output, "TAC_ADD"); break;
        case TAC_COPY: fprintf(output, "TAC_COPY"); break;
        case TAC_IFZ: fprintf(output, "TAC_IFZ"); break;
        case TAC_LABEL: fprintf(output, "TAC_LABEL"); break;
    }
    if (tac->res)
        fprintf(output, ",%s", tac->res->value);
    else
        fprintf(output, ",0");

    if (tac->op1)
        fprintf(output, ",%s", tac->op1->value);
    else
        fprintf(output, ",0");

    if (tac->op2)
        fprintf(output, ",%s", tac->op2->value);
    else
        fprintf(output, ",0");

    fprintf(output, ")\n");
}

void tacPrintBack(TAC *tac, FILE *output) {
    Tac *node;
    for (node = tac; node; node = node->prev)
        tacPrintSingle(node, output);
}

Tac* makeBinOp(int type, Tac *code0, Tac *code1) {
    return tacJoin(
        tacJoin(code0, code1), 
        tacCreate(type, makeTemp(), code0 ? code0->res : 0, code1 ? code1->res : 0)
        );
}

Tac* makeIfThen(Tac *code0, Tac *code1) {
    Tac *ifTac;
    Tac *labelTac;
    HashNode *label;

    label = makeLabel();
    ifTac = tacCreate(TAC_IFZ, label, code0 ? code0->res : 0);
    labelTac = tacCreate(TAC_LABEL, label, 0, 0);
    return tacJoin(tacJoin(tacJoin(code0, ifTac), code1), labelTac);
}

Tac* generateCode(Ast *node) {
    if (!node)
        return;

    int i;
    Tac *result=0;
    Tac *code[MAX_SONS];

    for (i = 0; i < MAX_SONS; i++) {
        code[i] = generateCode(node->son[i]);
    }

    switch(node->type) {
        case AST_SYMBOL:
            break;
        case AST_ADD:
            break;
        case AST_ASSIGNMENT:
            break;
    }

    return result;
}