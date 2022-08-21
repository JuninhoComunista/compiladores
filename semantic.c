// Nicolas Paris - 00302650

#include "semantic.h"

int semanticErrors = 0;

int getExpressionDataType(Ast *node) {
    return 0;
}

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

    int arraySize = 0;
    
    if (node->type == AST_VEC_DEC) {
        Ast *list = node->son[2];
        switch(node->son[0]->type) {
            case AST_CHAR: {
                if (!list)
                    return DT_CHAR;
                while(list) {
                    arraySize++;
                    if (list->son[0]->symbol->dataType != DT_CHAR &&
                        list->son[0]->symbol->dataType != DT_INT)
                        return 0;
                    list = list->son[1];
                }
                if (arraySize != atoi(node->symbol->value))
                    return -1;

                return DT_CHAR;
            }
            case AST_INT: {
                if (!list)
                    return DT_INT;
                while(list) {
                    arraySize++;
                    if (list->son[0]->symbol->dataType != DT_CHAR &&
                        list->son[0]->symbol->dataType != DT_INT) 
                        return 0;
                    list = list->son[1];
                }
                if (arraySize != atoi(node->symbol->value))
                    return -1;

                return DT_INT;
            }
            case AST_FLOAT: {
                if (!list)
                    return DT_FLOAT;
                while(list) {
                    arraySize++;
                    if (list->son[0]->symbol->dataType != DT_FLOAT) 
                        return 0;
                    list = list->son[1];
                }
                if (arraySize != atoi(node->symbol->value))
                    return -1;

                return DT_FLOAT;
            }
        }
    }  
    
    if (node->type == AST_VAR_DEC) {
        switch(node->son[0]->type) {
            case AST_CHAR: {
                if (node->son[2]->symbol->dataType != DT_CHAR &&
                    node->son[2]->symbol->dataType != DT_INT) 
                    return 0;

                return DT_CHAR;
            }
            case AST_INT: {
                if (node->son[2]->symbol->dataType != DT_CHAR &&
                    node->son[2]->symbol->dataType != DT_INT)
                    return 0;

                return DT_INT;
            }
            case AST_FLOAT: {
                if (node->son[2]->symbol->dataType != DT_FLOAT)
                    return 0;

                return DT_FLOAT;
            }
        }
    }

    return 0;
}

void checkCorrectUsage(Ast *node) {
    if(!node)
        return;

    int i;

    for (i=0; i<MAX_SONS; i++) {
        checkCorrectUsage(node->son[i]);
    }

    switch(node->type) {
        case AST_ASSIGNMENT: {
            if (node->son[0]->symbol->type != ID_TYPE_ESC &&
                node->son[0]->symbol->type != SYMBOL_UNDECLARED) {  //Already informed error
                fprintf(stderr, "Error at line %d: Incorrect usage of array variable -> %s\n",node->son[0]->lineNumber,  node->son[0]->symbol->value);
                semanticErrors++;
            }
        }
        break;
        case AST_VEC_ASSIGNMENT: {
            if (node->son[0]->symbol->type != ID_TYPE_VEC &&
                node->son[0]->symbol->type != SYMBOL_UNDECLARED) {  //Already informed error
                fprintf(stderr, "Error at line %d: Incorrect usage of escalar variable -> %s\n",node->son[0]->lineNumber,  node->son[0]->symbol->value);
                semanticErrors++;
            }
        }
        break;
    }
}

void checkUndeclared(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        if (table->nodes[i]) {
            if (table->nodes[i]->type == SYMBOL_IDENTIFIER) {
                table->nodes[i]->type = SYMBOL_UNDECLARED;
                fprintf(stderr, "Error at line %d: Undeclared identifier -> %s\n",table->nodes[i]->lineNumber,  table->nodes[i]->value);
                semanticErrors++;
            }
            if (table->overflowBuckets[i]) {
                LinkedList *head = table->overflowBuckets[i];
                while (head) {
                    if (head->node->type == SYMBOL_IDENTIFIER) {
                        table->nodes[i]->type = SYMBOL_UNDECLARED;
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

            Ast *paramList = node->son[2];

            while(paramList) {
                paramList->son[0]->son[1]->symbol->dataType = getDataType(paramList->son[0]->son[0]);
                paramList->son[0]->son[1]->symbol->type = ID_TYPE_ESC;
                paramList = paramList->son[1];
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
    checkCorrectUsage(node);
    return semanticErrors;
}
