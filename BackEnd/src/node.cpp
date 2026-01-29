#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


Node* NodeInit (Type_t type, Value_t value, Node* left, Node* right, Node* parent) {
    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node != NULL);

    node->type = type;
    node->left = left;
    node->right = right;
    node->parent = parent;

    node->value = value;

    if (left != NULL) { left->parent = node; }
    if (right != NULL) { right->parent = node; }
    
    return node;
}


Ast* AstInit() {
    Ast* ast = (Ast*)calloc(sizeof(Ast), 1);
    assert(ast != NULL);

    ast->root = NULL;

    ast->nameTable = (char**)calloc(sizeof(char*), 1);
    ast->freeName = 0;

    ast->freeLabel = 1;

    return ast;
}


void NodeDestroy(Node** node) {
    assert(node != NULL);

    if ((*node)->type == Var) {free((*node)->value.var); }
    
    if ((*node)->left != NULL)
    {
        NodeDestroy(&((*node)->left));
    }
    
    if ((*node)->right != NULL)
    {
        NodeDestroy(&((*node)->right));
    }
    
    free(*node);
    *node = NULL;
}


void AstDestroy(Ast* ast) {
    NodeDestroy(&(ast->root));

    free(ast->nameTable);
    ast->freeName = 0;
    ast->freeLabel = 1;

    free(ast);
}