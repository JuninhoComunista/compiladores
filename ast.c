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

void astPrint(Ast *node, int level, FILE *output) {
    if (!node)
        return;
    int i;

    for (i=0; i<level; i++)
        fprintf(output, "--");

    switch(node->type){
        case AST_DEC: fprintf(output, "AST_DEC\n"); break;
        case AST_VAR_DEC: fprintf(output, "AST_VAR_DEC\n"); break;
        case AST_FUNC_DEC: fprintf(output, "AST_FUNC_DEC\n"); break;
        case AST_VEC_DEC: fprintf(output, "AST_VEC_DEC\n"); break;
        case AST_PARAM_LIST: fprintf(output, "AST_PARAM_LIST\n"); break;
        case AST_PARAM: fprintf(output, "AST_PARAM\n"); break;
        case AST_BLOCK: fprintf(output, "AST_BLOCK\n"); break;
        case AST_COMMAND: fprintf(output, "AST_COMMAND\n"); break;
        case AST_ASSIGNMENT: fprintf(output, "AST_ASSIGNMENT\n"); break;
        case AST_VEC_ASSIGNMENT: fprintf(output, "AST_VEC_ASSIGNMENT\n"); break;
        case AST_IDENTIER: fprintf(output, "AST_IDENTIER\n"); break;
        case AST_LITERAL: fprintf(output, "AST_LITERAL\n"); break;
        case AST_EXPR: fprintf(output, "AST_EXPR\n"); break;
        case AST_ADD: fprintf(output, "AST_ADD\n"); break;
        case AST_SUB: fprintf(output, "AST_SUB\n"); break;
        case AST_MUL: fprintf(output, "AST_MUL\n"); break;
        case AST_DIV: fprintf(output, "AST_DIV\n"); break;
        case AST_LES: fprintf(output, "AST_LES\n"); break;
        case AST_GRE: fprintf(output, "AST_GRE\n"); break;
        case AST_OR: fprintf(output, "AST_OR\n"); break;
        case AST_AND: fprintf(output, "AST_AND\n"); break;
        case AST_LE: fprintf(output, "AST_LE\n"); break;
        case AST_GE: fprintf(output, "AST_GE\n"); break;
        case AST_EQ: fprintf(output, "AST_EQ\n"); break;
        case AST_DIF: fprintf(output, "AST_DIF\n"); break;
        case AST_NOT: fprintf(output, "AST_NOT\n"); break;
        case AST_VEC_ACESS: fprintf(output, "AST_VEC_ACESS\n"); break;
        case AST_FUNC_CALL: fprintf(output, "AST_FUNC_CALL\n"); break;
        case AST_READ: fprintf(output, "AST_READ\n"); break;
        case AST_PRINT: fprintf(output, "AST_PRINT\n"); break;
        case AST_STRING: fprintf(output, "AST_STRING\n"); break;
        case AST_PARENTHESIS: fprintf(output, "AST_PARENTHESIS\n"); break;
        case AST_VEC_READ: fprintf(output, "AST_VEC_READ\n"); break;
        case AST_RETURN: fprintf(output, "AST_RETURN\n"); break;
        case AST_WHILE: fprintf(output, "AST_WHILE\n"); break;
        case AST_IF: fprintf(output, "AST_IF\n"); break;
        case AST_IF_ELSE: fprintf(output, "AST_IF_ELSE\n"); break;
        case AST_CHAR: fprintf(output, "AST_CHAR\n"); break;
        case AST_INT: fprintf(output, "AST_INT\n"); break;
        case AST_FLOAT: fprintf(output, "AST_FLOAT\n"); break;
        case AST_SYMBOL: fprintf(output, "AST_SYMBOL\n"); break;
        case AST_LITERAL_LIST: fprintf(output, "AST_LITERAL_LIST\n"); break;
        default: fprintf(output, "AST_UNKNOWN\n"); break;
    }

    if (node->symbol) {
        for (i=0; i<level; i++)
            fprintf(output, "--");
        fprintf(output, "symbol= %s\n", node->symbol->value);
    }

    for (i=0; i<MAX_SONS; i++){
        astPrint(node->son[i], level+1, output);
    }

}