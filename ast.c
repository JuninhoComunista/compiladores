// Nome: Nicolas Paris - Cartao: 302650

#include "ast.h"

Ast *astCreate(int lineNumber, int type, HashNode *symbol, Ast *son0, Ast *son1, Ast *son2, Ast *son3) {
    Ast *newNode;
    newNode = (Ast*) calloc(1, sizeof(Ast));
    newNode->type = type;
    newNode->lineNumber = lineNumber;
    newNode->symbol = symbol;
    newNode->son[0] = son0;
    newNode->son[1] = son1;
    newNode->son[2] = son2;
    newNode->son[3] = son3;

    return newNode;
}

void printType(Ast *node, FILE *output) {
    if (!node || !output)
        return;

    switch(node->type){
        case AST_DEC_LIST: fprintf(output, "AST_DEC_LIST"); break;
        case AST_VAR_DEC: fprintf(output, "AST_VAR_DEC"); break;
        case AST_FUNC_DEC: fprintf(output, "AST_FUNC_DEC"); break;
        case AST_VEC_DEC: fprintf(output, "AST_VEC_DEC"); break;
        case AST_PARAM_LIST: fprintf(output, "AST_PARAM_LIST"); break;
        case AST_PARAM: fprintf(output, "AST_PARAM"); break;
        case AST_BLOCK: fprintf(output, "AST_BLOCK"); break;
        case AST_COMMAND_LIST: fprintf(output, "AST_COMMAND_LIST"); break;
        case AST_ASSIGNMENT: fprintf(output, "AST_ASSIGNMENT"); break;
        case AST_VEC_ASSIGNMENT: fprintf(output, "AST_VEC_ASSIGNMENT"); break;
        case AST_IDENTIFIER: fprintf(output, "AST_IDENTIFIER"); break;
        case AST_LITERAL: fprintf(output, "AST_LITERAL"); break;
        case AST_EXPR_LIST: fprintf(output, "AST_EXPR_LIST"); break;
        case AST_EXPR: fprintf(output, "AST_EXPR"); break;
        case AST_ADD: fprintf(output, "AST_ADD"); break;
        case AST_SUB: fprintf(output, "AST_SUB"); break;
        case AST_MUL: fprintf(output, "AST_MUL"); break;
        case AST_DIV: fprintf(output, "AST_DIV"); break;
        case AST_LES: fprintf(output, "AST_LES"); break;
        case AST_GRE: fprintf(output, "AST_GRE"); break;
        case AST_OR: fprintf(output, "AST_OR"); break;
        case AST_AND: fprintf(output, "AST_AND"); break;
        case AST_LE: fprintf(output, "AST_LE"); break;
        case AST_GE: fprintf(output, "AST_GE"); break;
        case AST_EQ: fprintf(output, "AST_EQ"); break;
        case AST_DIF: fprintf(output, "AST_DIF"); break;
        case AST_NOT: fprintf(output, "AST_NOT"); break;
        case AST_VEC_ACESS: fprintf(output, "AST_VEC_ACESS"); break;
        case AST_FUNC_CALL: fprintf(output, "AST_FUNC_CALL"); break;
        case AST_READ: fprintf(output, "AST_READ"); break;
        case AST_PRINT: fprintf(output, "AST_PRINT"); break;
        case AST_STRING: fprintf(output, "AST_STRING"); break;
        case AST_PARENTHESIS: fprintf(output, "AST_PARENTHESIS"); break;
        case AST_VEC_READ: fprintf(output, "AST_VEC_READ"); break;
        case AST_RETURN: fprintf(output, "AST_RETURN"); break;
        case AST_WHILE: fprintf(output, "AST_WHILE"); break;
        case AST_IF: fprintf(output, "AST_IF"); break;
        case AST_IF_ELSE: fprintf(output, "AST_IF_ELSE"); break;
        case AST_CHAR: fprintf(output, "AST_CHAR"); break;
        case AST_INT: fprintf(output, "AST_INT"); break;
        case AST_FLOAT: fprintf(output, "AST_FLOAT"); break;
        case AST_SYMBOL: fprintf(output, "AST_SYMBOL"); break;
        case AST_LITERAL_LIST: fprintf(output, "AST_LITERAL_LIST"); break;
        default: fprintf(output, "AST_UNKNOWN"); break;
    }
}

void astPrintNode(Ast *node, FILE *output) {
    if (!node || !output)
        return;

    printType(node, output);

    if (node->symbol) {
        fprintf(output, "---> Symbol= %s ", node->symbol->value);

        if (node->symbol->type) {
            fprintf(output, "| Type= %d ", node->symbol->type);
        }
        if (node->symbol->dataType) {
            fprintf(output, "| dataType= %d", node->symbol->dataType);
        }
    }

    if (node->dataType) {
        fprintf(output, " | dataType= %d", node->dataType);
    }

    fprintf(output, "\n");
}

void astPrint(Ast *node, int level, FILE *output) {
    if (!node || !output)
        return;
    int i;

    for (i=0; i<level; i++)
        fprintf(output, "--");

    astPrintNode(node, output);

    for (i=0; i<MAX_SONS; i++){
        astPrint(node->son[i], level+1, output);
    }

}