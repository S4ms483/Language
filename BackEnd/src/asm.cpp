#include "asm.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "file.h"
#include "visdump.h"

static void NodeWrite(FILE* file, Ast* ast, Node* curr_node);
static int FindVarInTable(Node* node, Ast* ast);
static void AddVarToTable(Node* node, Ast* ast);
static void KeyNodeWrite(Node* node, FILE* file);


void TreeToAsm(Ast* ast) {
    assert(ast != NULL);

    Node* root = ast->root;
VisualDump(root, 0);

    char* asm_file = GetOutputFileName();
    FILE* file = fopen(asm_file, "w");
    assert(file != NULL);

    NodeWrite(file, ast, root);
}


static void NodeWrite(FILE* file, Ast* ast, Node* node) {
    assert((file != NULL) && (ast != NULL) && (node != NULL));

    Type_t type = node->type;
    switch (type) {
        case Eql: {
            NodeWrite(file, ast, node->right);

            Node* var_node = node->left;
            int var_num = FindVarInTable(var_node, ast);

            if (var_num == 0) { 
                fprintf(file, "PUSHM [%cX]\n", (char)((int)'A' + ast->freeName)); 
                AddVarToTable(var_node, ast);
            }
            else { fprintf(file, "PUSHM [%cX]\n", (char)((int)'A' + var_num)); }
            return;
        }

        case While: { 
            fprintf(file, ":%d\n", ast->freeLabel);
            int first_label = ast->freeLabel;
            (ast->freeLabel)++;

            NodeWrite(file, ast, node->left);
            fprintf(file, "PUSH 0\n");
            fprintf(file, "JBE :%d\n", ast->freeLabel);
            int second_label = ast->freeLabel;
            (ast->freeLabel)++;

            NodeWrite(file, ast, node->right);
            fprintf(file, "JMP :%d\n", first_label);

            fprintf(file, ":%d\n", second_label);
            return;
        }

        case If: {
            NodeWrite(file, ast, node->left);
            fprintf(file, "PUSH 0\n");
            fprintf(file, "JBE :%d\n", ast->freeLabel);

            int label = ast->freeLabel;
            (ast->freeLabel)++;

            NodeWrite(file, ast, node->right);

            fprintf(file, ":%d\n", label);
            return;
        }
    }

    if (node->left) { NodeWrite(file, ast, node->left); }
    if (node->right) { NodeWrite(file, ast, node->right); }

    switch (type) {
        case Num: {
            fprintf(file, "PUSH %d\n", node->value.num);
            break;
        }

        case Var: {
            int node_num = FindVarInTable(node, ast);

            if (node_num == 0) { fprintf(stderr, "error with variable\n"); }
            fprintf(file, "POPM [%cX]\n", (char)((int)'A' + node_num));
            break;
        }

        case Add:
        case Sub:
        case Mul:
        case Div:
        case Input:
        case Output: {
            KeyNodeWrite(node, file);
        }

    }
}


static int FindVarInTable(Node* node, Ast* ast) {
    assert((node != NULL) && (ast != NULL));

    for (int i = 0; i < ast->freeName; i++) {
        if (strcmp(node->value.var, ast->nameTable[i]) == 0) { return i; }
    }

    return 0;
}


static void AddVarToTable(Node* node, Ast* ast) {
    assert((node != NULL) && (ast != NULL));

    ast->nameTable = (char**)realloc(ast->nameTable, (ast->freeName + 1) * sizeof(char*));
    ast->nameTable[ast->freeName] = node->value.var;

    (ast->freeName)++;
}


static void KeyNodeWrite(Node* node, FILE* file) {
    assert((node != NULL) && (file != NULL));

    for (int i = 0; i < sizeof(AsmKeys)/sizeof(AsmKeys[0]); i++) {
        if ((AsmKeys[i]).keyType == node->type) { fprintf(file, "%s\n", (AsmKeys[i]).keyName); }
    }
}
