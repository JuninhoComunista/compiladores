//Nome: Nicolas Paris - Cartao: 302650

#include "decompiler.h"

int level = 0;
int flowControl = 0;

void decompileBlock(Ast *node, FILE *output);

void decompileExpression(Ast *node, FILE *output);

char *typeToString(int type) {
    switch (type)
    {
    case AST_CHAR: return "char";
    case AST_FLOAT: return "float";
    case AST_INT: return "int";
    default: fprintf(stderr, "Unknown symbol type!\n");    exit(11);
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

void decompileTuple(Ast *node, FILE *output, char *operator) {
    decompileExpression(node->son[0], output);
    fprintf(output, "%s", operator);
    decompileExpression(node->son[1], output);
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
            decompileTuple(node, output, "+");
        }
        break;
        case AST_SUB: {
            decompileTuple(node, output, "-");
        }
        break;
        case AST_MUL: {
            decompileTuple(node, output, ".");
        }
        break;
        case AST_DIV: {
            decompileTuple(node, output, "/");
        }
        break;
        case AST_LES: {
            decompileTuple(node, output, "<");
        }
        break;
        case AST_GRE: {
            decompileTuple(node, output, ">");
        }
        break;
        case AST_OR: {
            decompileTuple(node, output, "|");
        }
        break;
        case AST_AND: {
            decompileTuple(node, output, "&");
        }
        break;
        case AST_LE: {
            decompileTuple(node, output, "<=");
        }
        break;
        case AST_GE: {
            decompileTuple(node, output, ">=");
        }
        break;
        case AST_EQ: {
            decompileTuple(node, output, "==");
        }
        break;
        case AST_DIF: {
            decompileTuple(node, output, "!=");
        }
        break;
        case AST_NOT: {
            fprintf(output, "~");
            decompileExpression(node->son[0], output);
        }
        break;
        default:
            fprintf(stderr, "Unknown expression type"); exit(14);
    }
}

void decompilePrint(Ast *node, FILE *output) {
    if (node->type == AST_EXPR) {
        fprintf(output, " ");
        decompileExpression(node->son[0], output);
        if (node->son[1]) {
            decompilePrint(node->son[1], output);
        }
    } else {
        fprintf(output, " %s", node->symbol->value);
        if (node->son[0]) {
            decompilePrint(node->son[0], output);
        }
    }
}

void decompileCmd(Ast *node, FILE *output) {
    if(!node)
        return;

    if(!flowControl) {
        for(int i = 0; i < level; i++)
                fprintf(output, "\t");
    }

    switch(node->type) {
        case AST_ASSIGNMENT: {
            flowControl = 0;
            fprintf(output, "%s <- ", node->son[0]->symbol->value);        //Identifier
            decompileExpression(node->son[1], output);
        }
        break;
        case AST_VEC_ASSIGNMENT: {
            flowControl = 0;
            fprintf(output, "%s", node->son[0]->symbol->value); //Identifier
            fprintf(output, "[");
            decompileExpression(node->son[1], output);
            fprintf(output, "] <- ");
            decompileExpression(node->son[2], output);
        }
        break;
        case AST_READ: {
            flowControl = 0;
            fprintf(output,"read %s", node->son[0]->symbol->value);
        }
        break;
        case AST_VEC_READ: {
            flowControl = 0;
            fprintf(output, "read %s[", node->son[0]->symbol->value);
            decompileExpression(node->son[1], output);
            fprintf(output, "]");
        }
        break;
        case AST_PRINT:{
            flowControl = 0;
            fprintf(output, "print");
            decompilePrint(node->son[0], output);
        }
        break;
        case AST_RETURN: {
            flowControl = 0;
            fprintf(output, "return ");
            decompileExpression(node->son[0], output);
        }
        break;
        case AST_BLOCK: {
            flowControl = 0;
            decompileBlock(node, output);
        }
        break;
        case AST_WHILE: {
            flowControl = 1;
            fprintf(output, "while (");
            decompileExpression(node->son[0], output);
            fprintf(output,") ");
            decompileCmd(node->son[1], output);
        }
        break;
        case AST_IF: {
            flowControl = 1;
            fprintf(output, "if (");
            decompileExpression(node->son[0], output);
            fprintf(output,") ");
            decompileCmd(node->son[1], output);
        }
        break;
        case AST_IF_ELSE: {
            flowControl = 1;
            fprintf(output, "if (");
            decompileExpression(node->son[0], output);
            fprintf(output,") ");
            decompileCmd(node->son[1], output);
            fprintf(output, " else ");
            decompileCmd(node->son[2], output);
        }
        break;
        default:
            fprintf(stderr, "Unknown command type"); exit(13);
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
        fprintf(output, "{\n");
        for(int i = 0; i < level; i++)
            fprintf(output, "\t");
        fprintf(output, "}");
    }
}

void decompileFuncDec(Ast *node, FILE *output) {
    //son0 - type, son1 - identifier, son2 - parameterList, son3 - block
    fprintf(output, "\n%s ", typeToString(node->son[0]->type));   //Type
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
            fprintf(stderr, "Unknown declaration type\n"); exit(12);
    }
    
}

void decompile(Ast *program, FILE *output) {
    if (!program || !output)
        return;

    decompileDecList(program, output);
}