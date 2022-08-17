// Nicolas Paris - 00302650

#pragma once

#include "hash.h"
#include "ast.h"

void assignDeclaration(Ast *node);
void runOnce(Ast *node, HashTable *table);