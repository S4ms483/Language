#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "node.h"
#include "read.h"
#include "visdump.h"
#include "asm.h"

int main() {
    char* buffer = FileInput("../FrontEnd/src/tree.txt"); 

    Ast* ast = CodeToAst(buffer);
    VisualDump(ast->root, 0);

    FILE* asm_file = TreeToAsm(ast);

    free(buffer); 
    AstDestroy(ast);
}