// Nicolas Paris - 00302650

#include "semantic.h"

int semanticErrors = 0;

int isCompatibleDeclaration(Ast *node) {
    if (!node) {
        fprintf(stderr, "Would segfault!\n");
        return -1;
    }
    
    if (node->symbol) { //is VecDec
        switch(node->son[0]->type) {
            case AST_CHAR:
                if (node->symbol->dataType != DT_CHAR) 
                    return 0;
                break;
            case AST_INT:
                if (node->symbol->dataType != DT_INT)
                    return 0;  
                break;
            case AST_FLOAT:
                if (node->symbol->dataType != DT_FLOAT)
                    return 0;  
                break;
        }
    } else {    //is VarDec
        switch(node->son[0]->type) {
            case AST_CHAR:
                if (node->son[2]->symbol->dataType != DT_CHAR) 
                    return 0;
                break;
            case AST_INT:
                if (node->son[2]->symbol->dataType != DT_INT)
                    return 0;  
                break;
            case AST_FLOAT:
                if (node->son[2]->symbol->dataType != DT_FLOAT)
                    return 0;  
                break;
        }
    }
    return 1;
}
void checkCompatibility(Ast *node) {
    return;
}

void checkUndeclared(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        if (table->nodes[i]) {
            if (table->nodes[i]->type == SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Error at line %d: Undeclared identifier %s\n",table->nodes[i]->lineNumber,  table->nodes[i]->value);
                semanticErrors++;
            }
            if (table->overflowBuckets[i]) {
                LinkedList *head = table->overflowBuckets[i];
                while (head) {
                    if (table->nodes[i]->type == SYMBOL_IDENTIFIER) {
                        fprintf(stderr, "Undeclared identifier %s\n", table->nodes[i]->value);
                        semanticErrors++;
                    }
                    head = head->next;
                }
            }
        }
    }
}

void assignDeclaration(Ast *node) {
    if(!node)
        return;

    int i;

    switch(node->type) {
        case AST_VAR_DEC: {
            if (!isCompatibleDeclaration(node)) {
                fprintf(stderr, "Error at line %d: Incompatible data types\n", node->lineNumber);
                semanticErrors++;
                break;
            } 

            if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Error at line %d: Variable %s already declared\n", node->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
                break;
            } 

            node->son[1]->symbol->type = ID_TYPE_ESC;
        }
        break;
        case AST_VEC_DEC: {
            if (!isCompatibleDeclaration(node)) {
                fprintf(stderr, "Error at line %d: Incompatible data types\n", node->lineNumber);
                semanticErrors++;
                break;
            } 

            if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Error at line %d: Array %s already declared\n", node->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
                break;
            } 

            node->son[1]->symbol->type = ID_TYPE_VEC;
        }
        break;
        case AST_FUNC_DEC: {
            if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Error at line %d: Function %s already declared\n", node->son[1]->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
            } 

            node->son[1]->symbol->type = ID_TYPE_FUNC;
        }
        break;
    }
    
    for (i=0; i<MAX_SONS; i++) {
        assignDeclaration(node->son[i]);
    }
}

int runOnce(Ast *node, HashTable *table) {
    assignDeclaration(node);
    checkUndeclared(table);
    return semanticErrors;
}
