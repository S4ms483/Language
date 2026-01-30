#include "node.h"
#include "visdump.h"

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


Tree* TreeInit(Node* root) {
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    assert(tree != NULL);

    tree->root = root;

    tree->tex = NULL;

    tree->nNames = 1;
    tree->names = (char**)calloc(tree->nNames, sizeof(char*));

    return tree;
}


Node* NumNodeCreate(double num) {
    Value_t value;
    value.num = num;

    Node* new_node = NodeInit(Num, value, NULL, NULL, NULL);

    return new_node;
}


int NodeCalculate(Node* node) {
    assert(node != NULL);

    int l_value = 0;

    if (node->left) {l_value = ((node->left)->value).num; }
    int r_value = ((node->right)->value).num;

    switch (node->type) {
        case (Add): { return l_value + r_value; }
        case (Sub): { return l_value - r_value; }
        case (Mul): { return l_value * r_value; } 
        case (Div): { return l_value / r_value; }
        default: { return 0; }
    }
}


Node* CopyNode(Node* node) {
    Node* new_node = NodeInit(node->type, node->value, NULL, NULL, NULL);

    if (node->left)
    {
        new_node->left = CopyNode(node->left);
        (new_node->left)->parent = new_node;
    }

    if (node->right)
    {
        new_node->right = CopyNode(node->right);
        (new_node->right)->parent = new_node;
    }

    return new_node;
}



void NodeDestroy(Node** node) {
    assert(node != NULL);
    
    if ((*node)->left != NULL)
    {
        NodeDestroy(&((*node)->left));
    }
    
    if ((*node)->right != NULL)
    {
        NodeDestroy(&((*node)->right));
    }
    
    if ((*node)->type == Var) { free(((*node)->value).var); }

    free(*node);
    *node = NULL;
}


void TreeDestroy(Tree* tree) {
    assert(tree != NULL);

    NodeDestroy(&(tree->root));

    free(tree);
}