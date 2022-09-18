//Nome: Nicolas Paris - Cartao: 302650
//Still lacking some features

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

void reverseTacList(Tac **list) {
    Tac *node;
    for (node = *list; node->prev; node = node->prev)
        node->prev->next = node;
    *list = node;
}

void tacPrintSingle(Tac *tac, FILE *output) {
    if (!tac || !output)
        return;

    if (tac->type == TAC_SYMBOL)
        return;
    
    fprintf(output, "Tac(");
    switch(tac->type) {
        case TAC_SYMBOL: fprintf(output, "TAC_SYMBOL"); break;
        case TAC_MOVE: fprintf(output, "TAC_MOVE"); break;
        case TAC_IFZ: fprintf(output, "TAC_IFZ"); break;
        case TAC_LABEL: fprintf(output, "TAC_LABEL"); break;
        case TAC_JUMP: fprintf(output, "TAC_JUMP"); break;
        case TAC_PRINT: fprintf(output, "TAC_PRINT"); break;
        case TAC_READ: fprintf(output, "TAC_READ"); break;
        case TAC_VEC_DEC: fprintf(output, "TAC_VEC_DEC"); break;
        case TAC_BEGIN_FUNC: fprintf(output, "TAC_BEGIN_FUNC"); break;
        case TAC_END_FUNC: fprintf(output, "TAC_END_FUNC"); break;
        case TAC_PARAM: fprintf(output, "TAC_PARAM"); break;
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
        case TAC_NOT: fprintf(output, "TAC_NOT"); break;
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

void tacPrintForwards(Tac *tac, FILE *output) {
    Tac *node;
    for (node = tac; node; node = node->next)
        tacPrintSingle(node, output);
}

void tacPrintBackwards(Tac *tac, FILE *output) {
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

Tac* makeIf(Tac *code0, Tac *code1, HashTable *table) {
    Tac *ifTac;
    Tac *labelTac;
    HashNode *label;

    label = makeLabel(table);
    ifTac = tacCreate(TAC_IFZ, label, code0 ? code0->res : 0, 0);
    labelTac = tacCreate(TAC_LABEL, label, 0, 0);
    return tacJoin(tacJoin(tacJoin(code0, ifTac), code1), labelTac);
}

Tac* makeIfElse(Tac *code0, Tac *code1, Tac* code2, HashTable* table) {
    HashNode *labelElse = makeLabel(table);
    HashNode *labelAfterElse = makeLabel(table);

    Tac *jumpzTac = tacCreate(TAC_IFZ, labelElse, code0 ? code0->res : 0, 0);
    tacJoin(code0, jumpzTac);

    Tac *jumpTac = tacCreate(TAC_JUMP, labelAfterElse, 0, 0);
    tacJoin(code1, jumpTac);

    Tac *labelElseTac = tacCreate(TAC_LABEL, labelElse, 0, 0);
    tacJoin(jumpTac, labelElseTac);

    Tac *labelAfterElseTac = tacCreate(TAC_LABEL, labelAfterElse, 0, 0);
    tacJoin(code2, labelAfterElseTac);

    tacJoin(jumpzTac, jumpTac);
    return tacJoin(labelElseTac, labelAfterElseTac);
}

Tac* makeWhile(Tac *code0, Tac *code1, HashTable *table) {

    HashNode *labelBefore = makeLabel(table);
    Tac *labelBeforeTac = tacCreate(TAC_LABEL, labelBefore, 0, 0);
    tacJoin(labelBeforeTac, code0);

    HashNode *labelAfter = makeLabel(table);
    Tac *labelAfterTac = tacCreate(TAC_LABEL, labelAfter, 0, 0);

    Tac *ifTac = tacCreate(TAC_IFZ, labelAfter, code0->res, 0);
    tacJoin(code0, ifTac);

    Tac *jump = tacCreate(TAC_JUMP, labelBefore, 0, 0);
    tacJoin(code1, jump);

    tacJoin(jump, labelAfterTac);

    tacJoin(ifTac, jump);
    return labelAfterTac;
}

Tac* makeFuncDec(Tac *code1, Tac *code2, Tac *code3) {
    
    return tacJoin(tacJoin(tacJoin(code2, tacCreate(TAC_BEGIN_FUNC, code1->res, 0, 0)), code3), tacCreate(TAC_END_FUNC, code1->res, 0, 0));
}

Tac* generateCode(Ast *node, HashTable *table, char *functionImIn) {
    if (!node)
        return 0;

    int i;
    Tac *result=0;
    Tac *code[MAX_SONS];

    for (i = 0; i < MAX_SONS; i++) {
        if (node->type == AST_FUNC_DEC)
            code[i] = generateCode(node->son[i], table, node->son[1]->symbol->value);
        else
            code[i] = generateCode(node->son[i], table, functionImIn);
            
    }

    switch(node->type) {
        case AST_IDENTIFIER: 
        case AST_LITERAL:
            result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0); break;
        case AST_ADD ... AST_NOT: 
            result = makeBinOp(node->type, code[0], code[1], table); break;
        case AST_ASSIGNMENT: 
            result = tacJoin(code[1], tacCreate(TAC_MOVE, code[0] ? code[0]->res : 0, code[1] ? code[1]->res : 0, 0)); break;
        case AST_IF: 
            result = makeIf(code[0], code[1], table); break;
        case AST_IF_ELSE: 
            result = makeIfElse(code[0], code[1], code[2], table); break;
        case AST_WHILE: 
            result = makeWhile(code[0], code[1], table); break;
        case AST_PRINT:
            result = tacJoin(code[0], tacCreate(TAC_PRINT, code[0] ? code[0]->res : 0, 0, 0)); break;
        case AST_READ:
            result = tacCreate(TAC_READ, code[0] ? code[0]->res : 0, 0, 0); break;
        case AST_VEC_READ:
            result = tacJoin(code[1], tacCreate(TAC_READ, code[0] ? code[0]->res : 0, code[1] ? code[1]->res : 0 ,0)); break;
        case AST_VAR_DEC:
            result = tacCreate(TAC_MOVE, code[1] ? code[1]->res : 0, code[2] ? code[2]->res : 0, 0); break;
        case AST_VEC_DEC:
            result = tacJoin(tacCreate(TAC_VEC_DEC, code[1] ? code[1]->res : 0, node->symbol, 0), code[2]); break;
        case AST_PARAM:
            result = tacCreate(TAC_PARAM, code[1] ? code[1]->res : 0, hashInsert(table, functionImIn ? functionImIn : 0, functionImIn ? functionImIn : 0, SYMBOL_LABEL), 0); break;
        case AST_FUNC_DEC:
            result = makeFuncDec(code[1], code[2], code[3]); break;
        default: 
            result = tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
            break;
    }

    return result;
}