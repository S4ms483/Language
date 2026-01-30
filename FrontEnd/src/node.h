#ifndef NODE_H_
#define NODE_H_

#include "token.h"

typedef struct Node {
    Value_t value;
    Type_t type;
    Node* left;
    Node* right;
    Node* parent;
};


typedef struct {
    Node* root;
    FILE* tex;
    size_t nNames;
    char** names;
} Tree;


Node* NodeInit (Type_t type, Value_t value, Node* left, Node* right, Node* parent);
Tree* TreeInit(Node* root);
Node* NumNodeCreate(double num);
int NodeCalculate(Node* node);
Node* CopyNode(Node* node);
void NodeDestroy(Node** node);
void TreeDestroy(Tree* tree);

#endif //NODE_H_