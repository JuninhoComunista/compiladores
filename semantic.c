// Nicolas Paris - 00302650

#include "semantic.h"

int semanticErrors = 0;
Ast *program;

int getExpressionDataType(Ast *node);

int isCompatibleDataType(int type1, int type2) {
    if(type1 == -1 || type2 == -1) 
        return -1;

    if(type1 == DT_FLOAT && type2 != DT_FLOAT)
        return 0;
    
    if(type1 == DT_BOOL && type2 != DT_BOOL)
        return 0;

    if((type1 == DT_INT || type1 == DT_CHAR) && 
        (type2 != DT_INT && type2 != DT_CHAR))
        return 0;

    return type1;
}

int checkFuncCall(Ast *node) {
    Ast *decList = program;
    Ast* dec;
    int declarationCount = 0, callCount = 0;
    while(decList) {
        dec = decList->son[0];
        decList = decList->son[1];
        if (dec->type == AST_FUNC_DEC) {
            if (strcmp(dec->son[1]->symbol->value, node->son[0]->symbol->value)) 
                continue;
            
            Ast *expressionList = node->son[1];
            Ast *paramList = dec->son[2];
            while(expressionList) {
                callCount++;
                if (!paramList) {
                    fprintf(stderr, "Error at line %d: More parameters then needed for function -> %s\n", node->lineNumber, node->son[0]->symbol->value);
                    semanticErrors++;
                    return -1;
                }
                
                if (!isCompatibleDataType(paramList->son[0]->dataType, getExpressionDataType(expressionList->son[0]))) {
                    fprintf(stderr, "Error at line %d: Incompatible function parameter data type at -> %s\n", node->lineNumber, node->son[0]->symbol->value);
                    semanticErrors++;
                    return -1;
                }
                    
                declarationCount++;
                paramList = paramList->son[1];
                expressionList = expressionList->son[1];
            }
            if (paramList) {
                fprintf(stderr, "Error at line %d: Less parameters then needed for function -> %s\n", node->lineNumber, node->son[0]->symbol->value);
                semanticErrors++;
                return -1;
            }
        }
    }
    return node->son[0]->symbol->dataType;
}

int getExpressionDataType(Ast *node) {
    if(!node) 
        return 0;
    
    switch(node->type) {
        case AST_IDENTIFIER:
        case AST_LITERAL: {
            return node->symbol->dataType;
        }
        case AST_VEC_ACESS: {
            return isCompatibleDataType(getExpressionDataType(node->son[1]), DT_INT);
        }
        case AST_PARENTHESIS: {
            return getExpressionDataType(node->son[0]);
        }
        case AST_FUNC_CALL: {
            return checkFuncCall(node);
        }
        case AST_ADD:
        case AST_SUB:
        case AST_MUL:
        case AST_DIV: {
            return isCompatibleDataType(getExpressionDataType(node->son[0]), getExpressionDataType(node->son[1]));
        }
        case AST_LES:
        case AST_GRE:
        case AST_OR:
        case AST_AND:
        case AST_LE:
        case AST_GE:
        case AST_EQ:
        case AST_DIF: {
            if (node->son[0]->son[0])
                return 0;

            if(!isCompatibleDataType(node->son[0]->symbol->dataType, node->son[1]->symbol->dataType))
                return 0;
            
            return DT_BOOL;
        }
        case AST_NOT: {
            return isCompatibleDataType(getExpressionDataType(node->son[0]), DT_BOOL);
        }
        
    }
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
                if(atoi(node->symbol->value) <= 0)
                    return -1;

                if (!list)
                    return DT_CHAR;

                while(list) {
                    arraySize++;
                    if (!isCompatibleDataType(list->son[0]->symbol->dataType, DT_CHAR))
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
                    if (!isCompatibleDataType(list->son[0]->symbol->dataType, DT_INT))
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
                    if (!isCompatibleDataType(list->son[0]->symbol->dataType, DT_FLOAT))
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
                if (!isCompatibleDataType(node->son[2]->symbol->dataType, DT_CHAR)) 
                    return 0;

                return DT_CHAR;
            }
            case AST_INT: {
                if (!isCompatibleDataType(node->son[2]->symbol->dataType, DT_INT)) 
                    return 0;

                return DT_INT;
            }
            case AST_FLOAT: {
                if (!isCompatibleDataType(node->son[2]->symbol->dataType, DT_FLOAT)) 
                    return 0;

                return DT_FLOAT;
            }
        }
    }

    return 0;
}

void checkReturnDataType(Ast *node) {
    if(!node)
        return;
    
    Ast *cmdList = node->son[3]->son[0];
    while(cmdList) {
        if(!cmdList->son[0])
            return;
        
        if(cmdList->son[0]->type == AST_RETURN &&
            !isCompatibleDataType(getExpressionDataType(cmdList->son[0]->son[0]), node->son[1]->symbol->dataType)) {
            fprintf(stderr, "Error at line %d: Incompatible return for function -> %s\n", node->son[1]->lineNumber, node->son[1]->symbol->value);
            semanticErrors++;
        }

        cmdList = cmdList->son[1];
    }
}

void checkCorrectUsage(Ast *node) {
    if(!node)
        return;

    int i;

    switch(node->type) {
        case AST_FUNC_DEC: {
            checkReturnDataType(node);
        }
        break;
        case AST_ASSIGNMENT: {
            if (node->son[0]->symbol->type == SYMBOL_UNDECLARED)
                break;

            if (isCompatibleDataType(getExpressionDataType(node->son[1]), node->son[0]->symbol->dataType) == -1)
                break;

            if (node->son[0]->symbol->type != ID_TYPE_ESC) { 
                fprintf(stderr, "Error at line %d: Incorrect usage of array variable -> %s\n",node->lineNumber,  node->son[0]->symbol->value);
                semanticErrors++;
                break;
            }
  
            if (!isCompatibleDataType(getExpressionDataType(node->son[1]), node->son[0]->symbol->dataType)) {
                fprintf(stderr, "Error at line %d: Incompatible data types at assignment of -> %s\n", node->lineNumber, node->son[0]->symbol->value);
                semanticErrors++;
                break;
            }

            switch(node->son[1]->type) {
                case AST_IDENTIFIER:
                    if (node->son[1]->symbol->type != ID_TYPE_ESC) {
                        fprintf(stderr, "Error at line %d: \"%s\" does not represent an escalar\n",node->lineNumber,  node->son[1]->symbol->value);
                        semanticErrors++;
                    }
                break;
                case AST_VEC_ACESS:
                    if (node->son[1]->son[0]->symbol->type != ID_TYPE_VEC) {
                        fprintf(stderr, "Error at line %d: \"%s\" does not represent an array\n",node->lineNumber,  node->son[1]->son[0]->symbol->value);
                        semanticErrors++;
                    }   
                break;
                case AST_FUNC_CALL:
                    if (node->son[1]->son[0]->symbol->type != ID_TYPE_FUNC) {
                        fprintf(stderr, "Error at line %d: \"%s\" does not represent a function\n",node->lineNumber,  node->son[1]->son[0]->symbol->value);
                        semanticErrors++;
                    }   
                break;
            }
            // fprintf(stderr, "Value %s\n", node->son[1]->symbol->value);
        }
        break;
        case AST_READ: {
            if (node->son[0]->symbol->type == SYMBOL_UNDECLARED)
                break;

            if (node->son[0]->symbol->type != ID_TYPE_ESC) {
                fprintf(stderr, "Error at line %d: Incorrect usage of array variable -> %s\n",node->lineNumber,  node->son[0]->symbol->value);
                semanticErrors++;
                break;
            }
        }
        break;
        case AST_VEC_ASSIGNMENT: {
            if (node->son[0]->symbol->type == SYMBOL_UNDECLARED)
                break;

            if (isCompatibleDataType(getExpressionDataType(node->son[1]), DT_INT) == -1)
                break;

            if (isCompatibleDataType(getExpressionDataType(node->son[2]), node->son[0]->symbol->dataType) == -1)
                break;

            if (node->son[0]->symbol->type != ID_TYPE_VEC) {  
                fprintf(stderr, "Error at line %d: Incorrect usage of escalar variable -> %s\n",node->lineNumber,  node->son[0]->symbol->value);
                semanticErrors++;
                break;
            }
            
            if (!isCompatibleDataType(getExpressionDataType(node->son[1]), DT_INT)) {
                fprintf(stderr, "Error at line %d: Incompatible data type at index of array -> %s\n", node->lineNumber, node->son[0]->symbol->value);
                semanticErrors++;
                break;
            }

            if (!isCompatibleDataType(getExpressionDataType(node->son[2]), node->son[0]->symbol->dataType)) {
                fprintf(stderr, "Error at line %d: Incompatible data types at assignment of -> %s\n", node->lineNumber, node->son[0]->symbol->value);
                semanticErrors++;
                break;
            }
        }
        break;
        case AST_VEC_READ: {
            if (node->son[0]->symbol->type == SYMBOL_UNDECLARED)
                break;

            if (isCompatibleDataType(getExpressionDataType(node->son[1]), DT_INT) == -1)
                break;

            if (node->son[0]->symbol->type != ID_TYPE_VEC) { 
                fprintf(stderr, "Error at line %d: Incorrect usage of escalar variable -> %s\n",node->lineNumber,  node->son[0]->symbol->value);
                semanticErrors++;
                break;
            }

            if (!isCompatibleDataType(getExpressionDataType(node->son[1]), DT_INT)) {
                fprintf(stderr, "Error at line %d: Incompatible data type at index of array -> %s\n", node->lineNumber, node->son[0]->symbol->value);
                semanticErrors++;
                break;
            }
        }
        break;
        case AST_WHILE: 
        case AST_IF: 
        case AST_IF_ELSE: {
            if (isCompatibleDataType(getExpressionDataType(node->son[0]), DT_BOOL) == -1)
                break;

            if (!isCompatibleDataType(getExpressionDataType(node->son[0]), DT_BOOL)) {
                fprintf(stderr, "Error at line %d: Incompatible data type at flow control expression\n", node->lineNumber);
                break;
            }
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
                break;
            }

            Ast *paramList = node->son[2];

            while(paramList) {
                paramList->son[0]->dataType = getDataType(paramList->son[0]->son[0]);
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
    program = node;
    assignDeclaration(node);
    checkUndeclared(table);
    checkCorrectUsage(node);
    return semanticErrors;
}
