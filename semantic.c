// Nicolas Paris - 00302650

#include "semantic.h"

int semanticErrors = 0;

int getDataType(Ast *node) {
    if (!node) {
        fprintf(stderr, "Would segfault!\n");
        return 0;
    }

    switch(node->type) {
        case AST_CHAR: {
            return DT_CHAR;
        }
        case AST_INT: {
            return DT_INT;
        }
        case AST_FLOAT: {
            return DT_FLOAT;
        }
    }
    return 0;
}

int isCompatibleDeclaration(Ast *node) {
    if (!node) {
        fprintf(stderr, "Would segfault!\n");
        return 0;
    }

    int dataType;
    int arraySize = 0;
    
    if (node->type == AST_VEC_DEC) {
        if (node->son[2]) {
            Ast *list = node->son[2];
            switch(node->son[0]->type) {
                case AST_CHAR: {
                    while(list) {
                        arraySize++;
                        if (list->son[0]->symbol->dataType != DT_CHAR &&
                            list->son[0]->symbol->dataType != DT_INT)
                            return 0;
                        list = list->son[1];
                    }
                    if (arraySize != atoi(node->symbol->value))
                        return -1;
                    dataType = DT_CHAR;
                }
                break;
                case AST_INT: {
                    while(list) {
                        arraySize++;
                        if (list->son[0]->symbol->dataType != DT_CHAR &&
                            list->son[0]->symbol->dataType != DT_INT) 
                            return 0;
                        list = list->son[1];
                    }
                    if (arraySize != atoi(node->symbol->value))
                        return -1;
                    dataType = DT_INT;
                }
                break;
                case AST_FLOAT: {
                    while(list) {
                        arraySize++;
                        if (list->son[0]->symbol->dataType != DT_FLOAT) 
                            return 0;
                        list = list->son[1];
                    }
                    if (arraySize != atoi(node->symbol->value))
                        return -1;
                    dataType = DT_FLOAT;
                }
                break;
            }
        } 
        return dataType;
    }  
    
    if (node->type == AST_VAR_DEC) {
        switch(node->son[0]->type) {
            case AST_CHAR: {
                if (node->son[2]->symbol->dataType != DT_CHAR &&
                    node->son[2]->symbol->dataType != DT_INT) 
                    return 0;
                dataType = DT_CHAR;
            }
                break;
            case AST_INT: {
                if (node->son[2]->symbol->dataType != DT_CHAR &&
                    node->son[2]->symbol->dataType != DT_INT)
                    return 0;
                dataType = DT_INT;
            }
                break;
            case AST_FLOAT: {
                if (node->son[2]->symbol->dataType != DT_FLOAT)
                    return 0;
                dataType = DT_FLOAT;
            }
                break;
        }
        return dataType;
    }

    return 0;
}

void checkCompatibility(Ast *node) {
    return;
}

void checkCorrectUsage(Ast *node) {
    if(!node)
        return;

    int i;

    switch(node->type) {
        case AST_VAR_DEC: {
            
        }
        break;
        case AST_VEC_DEC: {
            
        }
        break;
        case AST_FUNC_DEC: {
            
        }
        break;
    }
    
    for (i=0; i<MAX_SONS; i++) {
        checkCorrectUsage(node->son[i]);
    }
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
                    if (head->node->type == SYMBOL_IDENTIFIER) {
                        fprintf(stderr, "Error at line %d: Undeclared identifier %s\n", head->node->lineNumber, head->node->value);
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
            int dataType = isCompatibleDeclaration(node);
            if (!dataType) {
                fprintf(stderr, "Error at line %d: Incompatible data types at declaration of -> %s\n", node->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
                break;
            } 

            if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Error at line %d: Variable %s already declared\n", node->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
                break;
            } 

            node->son[1]->symbol->type = ID_TYPE_ESC;
            node->son[1]->symbol->dataType = dataType;
        }
        break;
        case AST_VEC_DEC: {
            int dataType = isCompatibleDeclaration(node);

            if (dataType == -1) {
                fprintf(stderr, "Error at line %d: Incompatible quantity of itens at initialization of -> %s\n", node->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
                break;
            }
            
            if (!dataType) {
                fprintf(stderr, "Error at line %d: Incompatible data types at declaration of -> %s\n", node->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
                break;
            } 

            if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Error at line %d: Array %s already declared\n", node->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
                break;
            } 

            node->son[1]->symbol->type = ID_TYPE_VEC;
            node->son[1]->symbol->dataType = dataType;
        }
        break;
        case AST_FUNC_DEC: {
            if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Error at line %d: Function %s already declared\n", node->son[1]->lineNumber, node->son[1]->symbol->value);
                semanticErrors++;
            }

            Ast *list = node->son[2];

            while(list) {
                list->son[0]->son[1]->symbol->dataType = getDataType(list->son[0]->son[0]);
                list->son[0]->son[1]->symbol->type = ID_TYPE_ESC;
                list = list->son[1];
            }

            node->son[1]->symbol->dataType = getDataType(node->son[0]);
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
