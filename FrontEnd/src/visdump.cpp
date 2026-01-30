#include "visdump.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "node.h"
#include "token.h"

void VisualDump(Node* root, int n_dump) {
    assert(root != NULL);

    char graph[50] = {0};

    sprintf(graph, "log/graph%d.txt", n_dump); 

    FILE* graph_file = fopen(graph, "w");
    assert(graph_file != NULL);

    fprintf(graph_file, "digraph G\n \t{\n\t\trankdir = TB;\n\t\t node [shape = record\
                         colorscheme = pastel27 style = filled];\n");

    PrintNode(root, graph_file);

    fprintf(graph_file, "}");

    fclose(graph_file);

    char comm_svg [commLen] = {0};
    char comm_png [commLen] = {0};
    sprintf(comm_svg, textCommSvg, n_dump, n_dump);
    sprintf(comm_png, textCommPng, n_dump, n_dump);
    system(comm_svg);
    system(comm_png);
};


void PrintDbgNode(Node* node, FILE* file) {
    assert(node != NULL);
    assert(file != NULL);

    static int n_elem = 0;
    int parent = n_elem;

    switch (node->type) {
        case (Num): {
            fprintf(file, numStr, 
                    n_elem, node->parent, (node->value).num, node, node->left, node->right);
            break;
        }

        case (Var): {
            fprintf(file, varStr, 
                    n_elem, node->parent, (node->value).var, node, node->left, node->right);
            break;
        }

        default : {
            if (node->type == Semicolon) {
                fprintf(file, smcStr,
                    n_elem, node->parent, Keys[node->type].keyName, node, node->left, node->right);
                break;
            }
            fprintf(file, opStr,
                    n_elem, node->parent, Keys[node->type].keyName, node, node->left, node->right);
            break;
        }
    }


    if (node->left)
    {
        n_elem++;
        fprintf(file, "\t\t%d -> %d; \n", parent, n_elem);
        PrintNode(node->left, file);
    }

    if(node->right)
    {
        n_elem++;
        fprintf(file, "\t\t%d -> %d; \n", parent, n_elem);
        PrintNode(node->right, file);
    }
}


void PrintNoDbgNode(Node* node, FILE* file) {
    assert(node != NULL);
    assert(file != NULL);

    static int n_elem = 0;
    int parent = n_elem;

    switch (node->type) {
        case (Num): {
            fprintf(file, numStr, n_elem, (node->value).num);
            break;
        }

        case (Var): {
            fprintf(file, varStr, n_elem, (node->value).var);
            break;
        }

        default : {
            if (node->type == Semicolon) {
                fprintf(file, smcStr, n_elem, Keys[node->type].keyName);
                break;
            }
            fprintf(file, opStr, n_elem, Keys[node->type].keyName);
            break;
        }
    }


    if (node->left)
    {
        n_elem++;
        fprintf(file, "\t\t%d -> %d; \n", parent, n_elem);
        PrintNode(node->left, file);
    }

    if(node->right)
    {
        n_elem++;
        fprintf(file, "\t\t%d -> %d; \n", parent, n_elem);
        PrintNode(node->right, file);
    }
}