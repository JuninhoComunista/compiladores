// Nicolas Paris - 00302650

#include "semantic.h"

int semanticErrors = 0;

int isCompatible(Ast *node) {
    return 1;
}

void checkUndeclared(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        if (table->nodes[i]) {
            if (table->nodes[i]->type == SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Undeclared identifier %s\n", table->nodes[i]->value);
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
            if (node->son[1]->symbol->type == SYMBOL_IDENTIFIER) {
                node->son[1]->symbol->type = ID_TYPE_ESC;
            } else {
                fprintf(stderr, "Error at line %d: Variable %s already declared\n", node->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
            }
        }
        break;
        case AST_VEC_DEC: {
            if (!isCompatible(node)) {
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
            if (node->son[1]->symbol->type == SYMBOL_IDENTIFIER) {
                node->son[1]->symbol->type = ID_TYPE_FUNC;
            } else {
                fprintf(stderr, "Error at line %d: Function %s already declared\n", node->son[1]->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
            }
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
