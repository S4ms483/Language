#ifndef NODE_H_
#define NODE_H_

#include <stdio.h>

typedef enum {
    Semicolon, // ;
    IfEqual,   // == 
    
    Add,
    Sub,
    Mul,
    Div,
    
    Eql,
    
    While,
    If,
    Else, 
    
    Input,
    Output,
    
    Num, 
    Var
}Type_t;


typedef union 
{
    int num; 
    char* var;
} Value_t;


typedef struct Node{
    Value_t value;
    Type_t type;
    Node* left;
    Node* right;
    Node* parent;
}Node;


typedef struct {
    Node* root;

    char** nameTable;
    int freeName;

    int freeLabel;
}Ast;


Node* NodeInit (Type_t type, Value_t value, Node* left, Node* right, Node* parent);
Ast* AstInit();
void AstDestroy(Ast* tree);
void NodeDestroy(Node** node);

#endif //NODE_H_