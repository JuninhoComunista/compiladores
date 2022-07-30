//Nome: Nicolas Paris - Cartao: 302650

#include "decompiler.h"

int level = 0;

void decompileBlock(Ast *node, FILE *output);

char *typeToString(int type) {
    switch (type)
    {
    case AST_CHAR: return "char";
    case AST_FLOAT: return "float";
    case AST_INT: return "int";
    default: fprintf(stderr, "Unknown symbol type!\n");    exit(5);
    }
    
}

void decompileVarDec(Ast *node, FILE *output) {
    fprintf(output, "%s ", typeToString(node->son[0]->type));   //Type
    fprintf(output, "%s ", node->son[1]->symbol->value);        //Identifier
    fprintf(output, "(%s);\n", node->son[2]->symbol->value);    //Literal
}

void decompileVecDec(Ast *node, FILE *output) {
    fprintf(output, "%s ", typeToString(node->son[0]->type));   //Type
    fprintf(output, "%s", node->son[1]->symbol->value);         //Identifier
    fprintf(output, "[%s]", node->symbol->value);               //Size
    if (node->son[2]) {     
        Ast *list = node->son[2];
        while(list) {
            fprintf(output, " %s", list->son[0]->symbol->value);
            list = list->son[1];
        }
    } 
    fprintf(output, ";\n");
}

void decompileExpression(Ast *node, FILE *output) {
    switch(node->type) {
        case AST_IDENTIFIER: 
        case AST_LITERAL: {
            fprintf(output, "%s", node->symbol->value); //Identifier
        }
        break;
        case AST_VEC_ACESS: {
            fprintf(output, "%s", node->son[0]->symbol->value); //Identifier
            fprintf(output, "[");
            decompileExpression(node->son[1], output);
            fprintf(output, "]");
        }
        break;
        case AST_FUNC_CALL: {
            fprintf(output, "%s", node->son[0]->symbol->value); //Identifier  
            fprintf(output, "(");
            if (node->son[1]) {     
                Ast *list = node->son[1];
                while(list->son[1]) {
                    decompileExpression(list->son[0], output);
                    fprintf(output, " ");
                    list = list->son[1];
                }
                decompileExpression(list->son[0], output);
            } 
            fprintf(output, ")");
        }
        break;
        case AST_PARENTHESIS: {
            fprintf(output, "(");
            decompileExpression(node->son[0], output);
            fprintf(output, ")");
        }
        break;
        case AST_ADD: {

        }
        break;
        case AST_SUB: {

        }
        break;
        case AST_MUL: {

        }
        break;
        case AST_DIV: {

        }
        break;
        case AST_LES: {

        }
        break;
        case AST_GRE: {

        }
        break;
        case AST_OR: {

        }
        break;
        case AST_AND: {

        }
        break;
        case AST_LE: {

        }
        break;
        case AST_GE: {

        }
        break;
        case AST_EQ: {

        }
        break;
        case AST_DIF: {

        }
        break;
        case AST_NOT: {

        }
        break;
        default:
            fprintf(stderr, "Unknown expression type"); exit(8);
    }
}

void decompileCmd(Ast *node, FILE *output) {
    if(!node)
        return;

    for(int i = 0; i < level; i++)
                fprintf(output, "\t");

    switch(node->type) {
        case AST_ASSIGNMENT: {
            fprintf(output, "%s <- ", node->son[0]->symbol->value);        //Identifier
            decompileExpression(node->son[1], output);
        }
        break;
        case AST_VEC_ASSIGNMENT:
        break;
        case AST_READ:
        break;
        case AST_VEC_READ:
        break;
        case AST_PRINT:
        break;
        case AST_RETURN:
        break;
        case AST_BLOCK: {
            decompileBlock(node, output);
        }
        break;
        case AST_WHILE:
        break;
        case AST_IF:
        break;
        case AST_IF_ELSE:
        break;
        default:
            fprintf(stderr, "Unknown command type"); exit(7);
    }
}

void decompileBlock(Ast *node, FILE *output) {
    if (node->son[0]) {
        fprintf(output, "{\n");
        level++;
        Ast *list = node->son[0];
        while(list->son[1]) {
            decompileCmd(list->son[0], output);
            fprintf(output, ";\n");
            list = list->son[1];
        }
        decompileCmd(list->son[0], output);
        fprintf(output, "\n");
        level--;
        for(int i = 0; i < level; i++)
            fprintf(output, "\t");
        fprintf(output, "}");
    } else {
        fprintf(output, "{}");
    }
}

void decompileFuncDec(Ast *node, FILE *output) {
    //son0 - type, son1 - identifier, son2 - parameterList, son3 - block
    fprintf(output, "%s ", typeToString(node->son[0]->type));   //Type
    fprintf(output, "%s", node->son[1]->symbol->value);         //Identifier
    fprintf(output, "(");
    if (node->son[2]) {
        Ast *list = node->son[2];
        while(list->son[1]) {
            fprintf(output, "%s ", typeToString(list->son[0]->son[0]->type));     //Type
            fprintf(output, "%s ", list->son[0]->son[1]->symbol->value);    //Identifier
            list = list->son[1];
        }
        fprintf(output, "%s ", typeToString(list->son[0]->son[0]->type));     //Type
        fprintf(output, "%s", list->son[0]->son[1]->symbol->value);    //Identifier
    }
    fprintf(output, ")");
    decompileBlock(node->son[3], output);
}

void decompileDecList(Ast *node, FILE *output) {
    if (!node || !output)
        return;

    Ast *dec = node->son[0];
    switch(dec->type) {
        case AST_VAR_DEC: {
            decompileVarDec(dec, output);
            decompileDecList(node->son[1], output);
        }
        break;
        case AST_VEC_DEC: {
            decompileVecDec(dec, output);
            decompileDecList(node->son[1], output);
        }
        break;
        case AST_FUNC_DEC: {
            decompileFuncDec(dec, output);
            decompileDecList(node->son[1], output);
        }
        break;
        default:
            fprintf(stderr, "Unknown declaration type\n"); exit(6);
    }
    
}

void decompile(Ast *program, FILE *output) {
    if (!program || !output)
        return;

    decompileDecList(program, output);
}