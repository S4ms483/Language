#ifndef ASM_H_
#define ASM_H_

#include <stdio.h>

#include "read.h"

FILE* TreeToAsm(Ast* ast);

const size_t codeSize = 50;
static const char* const asmFile = "src/asm.txt";

const Keys_t AsmKeys [] = {
    {"ADD",  Add},
    {"SUB",  Sub},
    {"MUL",  Mul},
    {"DIV",  Div},
    
    {"IN", Input},
    {"OUT", Output}
};

#endif //ASM_H_