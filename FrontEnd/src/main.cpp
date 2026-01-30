#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "read.h"
#include "node.h"
#include "visdump.h"
#include "file.h"

int main(int argc, char** argv) {
    TArray* array = Lexer(argc, argv);

    Node* root = GetTree(array);  
VisualDump(root, 1);

    FileOutput(root, "src/tree.txt");

    TokenArrayDestroy(&array);

    NodeDestroy(&root);
}