// Nicolas Paris - 00302650

#include "semantic.h"

int semanticErrors = 0;

void assignDeclaration(Ast *node) {
    if(!node)
        return;

    int i;

    switch(node->type) {
        case AST_VAR_DEC: {
            if (node->son[1]->type == AST_IDENTIFIER) {
                node->son[1]->type = ID_TYPE_ESC;
            } else {
                fprintf(stderr, "Error at line %d: Variable %s already declared", node->lineNumber, node->symbol->value);
                semanticErrors++;
            }
        }
        break;
    }
    
    for (i=0; i<MAX_SONS; i++) {
        assignDeclaration(node->son[i]);
    }
}

void runOnce(Ast *node, HashTable *table) {
    assignDeclaration(node);
}
