//Nome: Nicolas Paris - Cartao: 302650

#include "decompiler.h"

void mainSwitch(Ast *node, FILE *output) {
    
}

void decompile(Ast *program, FILE *output) {
    if (!program || !output)
        return;

    mainSwitch(program, output);
}