#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "node.h"
#include "read.h"
#include "visdump.h"
#include "asm.h"

int main(int argc, char** argv) {
    char* buffer = FileInput(argc, argv); 

    Ast* ast = CodeToAst(buffer);

    TreeToAsm(ast);

    AstDestroy(ast);
    free(buffer); 
}