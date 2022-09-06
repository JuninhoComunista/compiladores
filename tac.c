//Nome: Nicolas Paris - Cartao: 302650

#include "tac.h"

Tac* tacCreate(int type, HashNode *res, HashNode *op1, HashNode *op2) {
    Tac *newTac = (Tac*) calloc(1, sizeof(Tac));
    newTac->type = type;
    newTac->res = res;
    newTac->op1 = op1;
    newTac->op2 = op2;
    newTac->prev = 0;
    newTac->next = 0;
    
    return newTac;
}

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

    if (tac->type == TAC_SYMBOL)
        return;
    
    fprintf(output, "TAC(");
    switch(tac->type) {
        case TAC_COPY: fprintf(output, "TAC_COPY"); break;
        case TAC_IFZ: fprintf(output, "TAC_IFZ"); break;
        case TAC_LABEL: fprintf(output, "TAC_LABEL"); break;
        case TAC_ADD: fprintf(output, "TAC_ADD"); break;
        case TAC_SUB: fprintf(output, "TAC_SUB"); break;
        case TAC_MUL: fprintf(output, "TAC_MUL"); break;
        case TAC_DIV: fprintf(output, "TAC_DIV"); break;
        case TAC_LES: fprintf(output, "TAC_LES"); break;
        case TAC_GRE: fprintf(output, "TAC_GRE"); break;
        case TAC_OR: fprintf(output, "TAC_OR"); break;
        case TAC_AND: fprintf(output, "TAC_AND"); break;
        case TAC_LE: fprintf(output, "TAC_LE"); break;
        case TAC_GE: fprintf(output, "TAC_GE"); break;
        case TAC_DIF: fprintf(output, "TAC_DIF"); break;
    }
    if (tac->res)
        fprintf(output, ", %s", tac->res->value);
    else
        fprintf(output, ", 0");

    if (tac->op1)
        fprintf(output, ", %s", tac->op1->value);
    else
        fprintf(output, ", 0");

    if (tac->op2)
        fprintf(output, ", %s", tac->op2->value);
    else
        fprintf(output, ", 0");

    fprintf(output, ")\n");
}

void tacPrintBack(Tac *tac, FILE *output) {
    Tac *node;
    for (node = tac; node; node = node->prev)
        tacPrintSingle(node, output);
}

Tac* makeBinOp(int type, Tac *code0, Tac *code1, HashTable *table) {
    return tacJoin(
        tacJoin(code0, code1), 
        tacCreate(type, makeTemp(table), code0 ? code0->res : 0, code1 ? code1->res : 0)
        );
}

Tac* makeIfThen(Tac *code0, Tac *code1, HashTable *table) {
    Tac *ifTac;
    Tac *labelTac;
    HashNode *label;

    label = makeLabel(table);
    ifTac = tacCreate(TAC_IFZ, label, code0 ? code0->res : 0, 0);
    labelTac = tacCreate(TAC_LABEL, label, 0, 0);
    return tacJoin(tacJoin(tacJoin(code0, ifTac), code1), labelTac);
}

Tac* generateCode(Ast *node, HashTable *table) {
    if (!node)
        return 0;

    int i;
    Tac *result=0;
    Tac *code[MAX_SONS];

    for (i = 0; i < MAX_SONS; i++) {
        code[i] = generateCode(node->son[i], table);
    }

    switch(node->type) {
        case AST_IDENTIFIER: result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
            break;
        case AST_ADD ... AST_DIF: result = makeBinOp(node->type, code[0], code[1], table);
            break;
        case AST_ASSIGNMENT: result = tacJoin(code[1], tacCreate(TAC_COPY, code[0] ? code[0]->res : 0, code[1] ? code[1]->res : 0, 0));
            break;
        default: result = tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
            break;
    }

    return result;
}