#include "read.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "file.h"
#include "node.h"
#include "token.h"
#include "visdump.h"

static Node* GetGeneral(TArray* array, size_t* curr);

static Node* GetBody(TArray* array, size_t* curr); 

static Node* GetStatement(TArray* array, size_t* curr);

static Node* GetIf(TArray* array, size_t* curr);
static Node* GetWhile(TArray* array, size_t* curr); 
static Node* GetAssignment(TArray* array, size_t* curr);

static Node* GetInput(TArray* array, size_t* curr);
static Node* GetOutput(TArray* array, size_t* curr);

static Node* GetExpression(TArray* array, size_t* curr); 
static Node* GetTerm(TArray* array, size_t* curr);
static Node* GetParentheses(TArray* array, size_t* curr);

static Node* GetNumber(TArray* array, size_t* curr);
static Node* GetVariable(TArray* array, size_t* curr);
static Node* GetFunction(TArray* array, size_t* curr);

static Type_t DefineFunc(Token* token);
static bool IsFunction(Token* token);
static bool IsBraceSurr(Token* prev, Node* curr, Token* next, Type_t lB_type);
static bool CheckSmcln(TArray* array, size_t* curr);

static void SyntaxError(Node** node);

Node* GetTree(TArray* array) {
    assert(array != NULL);

    size_t curr = 0;

    Node* result = GetBody(array, &curr); 

    return result;
}


static void SyntaxError(Node** node) {
    if (!(*node)) { return; }
    Node** curr_node = node;
    
    while ((*node)->parent) {
        curr_node = &(*node)->parent;
    }

    NodeDestroy(curr_node);
}


static Node* GetBody(TArray* array, size_t* curr) {
    assert(array != NULL);
    bool error = false;

    Node* state_node = GetStatement(array, curr);
    if ((state_node == NULL) || (state_node->type == Error)) { 
        SyntaxError(&state_node); 
        error = true;
        return NULL;
    } 

    Node* first_smcln_node = NodeInit(Semicolon, noValue, state_node, NULL, NULL);
    if (*curr == array->size) { return first_smcln_node; }

    state_node = GetStatement(array, curr);
    if (state_node == NULL) { return first_smcln_node; }

    if (state_node->type == Error) {
        SyntaxError(&first_smcln_node); 
        error = true;
        return NULL;
    }

    Node* prev_smcln_node = first_smcln_node;
    Node* next_smcln_node = NULL;
    
    while (state_node != NULL) {
        next_smcln_node = NodeInit(Semicolon, noValue, state_node, NULL, prev_smcln_node);
        prev_smcln_node->right = next_smcln_node;

        if (*curr == array->size) { return first_smcln_node; }

        else if (state_node->type == Error) { 
            SyntaxError(&first_smcln_node); 
            error = true;
            return NULL;
        }
        
        state_node = GetStatement(array, curr); 
        prev_smcln_node = next_smcln_node;
    }
    
    if (error) { return NULL; }
    return first_smcln_node;
}


static Node* GetStatement(TArray* array, size_t* curr) {
    assert(array != NULL);

    Token* curr_token = array->tokens[*curr];
    assert(curr_token != NULL);

    Node* state_node = NULL;
    
    state_node = GetIf(array, curr);
    if (state_node != NULL) { return state_node; }
    
    state_node = GetWhile(array, curr);
    if (state_node != NULL) { return state_node; }

    state_node = GetInput(array, curr);

    if (!state_node) { state_node = GetAssignment(array, curr); }
    if (!state_node) { state_node = GetExpression(array, curr); }
    if (!state_node) { state_node = GetOutput(array, curr); }

    bool smcln = CheckSmcln(array, curr);

    if (state_node && (!smcln)) { return errorNode; }

    return state_node;
}


static bool CheckSmcln(TArray* array, size_t* curr) {
    assert(array != NULL);

    Token* next_token = array->tokens[*curr];

    if (next_token->type == Semicolon) { 
        (*curr)++; 
        return true;
    }

    return false;
}


static Node* GetIf(TArray* array, size_t* curr) {
    assert(array != NULL);

    Token* curr_token = array->tokens[*curr];
    assert(curr_token != NULL);

    if (curr_token->type != If) { return NULL; }

    (*curr) += 2;
    Token* lP1 = array->tokens[(*curr) - 1];
    Node* val1 = GetExpression(array, curr);
    Token* rP1 = array->tokens[*curr];

    if (!IsBraceSurr(lP1, val1, rP1, LParenth)) { return errorNode; }

    (*curr) += 2;
    Token* lP2 = array->tokens[(*curr) - 1];
    Node* val2 = GetBody(array, curr);
    Token* rP2 = array->tokens[*curr];

    if (!IsBraceSurr(lP2, val2, rP2, LParenth)) { return errorNode; }

    Node* if_node = NodeInit(If, noValue, val1, val2, NULL);
    (*curr)++;

    return if_node;
}


static Node* GetWhile(TArray* array, size_t* curr) {
    assert(array != NULL);

    Token* curr_token = array->tokens[*curr];
    assert(curr_token != NULL);

    if (curr_token->type != While) { return NULL; } 
    
    (*curr) += 2;
    Token* lP1 = array->tokens[(*curr) - 1];
    Node* val1 = GetExpression(array, curr);  
    Token* rP1 = array->tokens[*curr];

    if (!IsBraceSurr(lP1, val1, rP1, LParenth)) { return errorNode; }

    (*curr) += 2;

    Token* lP2 = array->tokens[(*curr) - 1];
    Node* val2 = GetBody(array, curr);
    Token* rP2 = array->tokens[*curr];

    if (!IsBraceSurr(lP2, val2, rP2, LParenth)) { return errorNode; }

    Node* while_node = NodeInit(While, noValue, val1, val2, NULL);
    (*curr)++;

    return while_node;
}


static Node* GetAssignment(TArray* array, size_t* curr) {
    assert(array != NULL);
    
    Node* val1 = GetVariable(array, curr);
    if (val1 == NULL) { return NULL; }
    
    Token* next_token = array->tokens[*curr];
    assert(next_token != NULL);
    
    if (next_token->type != Eql) { return NULL; } 

    (*curr)++;
    Node* val2 = GetExpression(array, curr);
    if (val2 == NULL) { return NULL; }
    
    Node* assign_node = NodeInit(Eql, noValue, val1, val2, NULL);

    return assign_node;
}


static Node* GetInput(TArray* array, size_t* curr) {
    assert(array != NULL);

    Node* val1 = GetVariable(array, curr);
    if (val1 == NULL) { return NULL; }

    Token* curr_token = array->tokens[*curr];
    assert(curr_token != NULL);
    
    Token* next_token = array->tokens[*curr + 1];
    assert(next_token != NULL);

    if ((curr_token->type != Eql) || 
        (next_token->type != Input)) { 
            NodeDestroy(&val1);
            (*curr)--;
            return NULL; 
        }

    (*curr) += 2;

    Node* input_node = NodeInit(Input, noValue, NULL, NULL, NULL);
    Node* eql_node = NodeInit(Eql, noValue, val1, input_node, NULL);
    input_node->parent = eql_node;

    return eql_node;
}


static Node* GetOutput(TArray* array, size_t* curr) {
    assert(array != NULL);

    Token* curr_token = array->tokens[*curr];
    assert(curr_token != NULL);

    if (curr_token->type != Output) { return NULL; }

    (*curr) += 2;
    Token* lP = array->tokens[(*curr) - 1];
    Node* val = GetVariable(array, curr);  
    Token* rP = array->tokens[*curr];

    if (!IsBraceSurr(lP, val, rP, LParenth)) { return errorNode; }

    Node* output_node = NodeInit(Output, noValue, NULL, val, NULL);
    (*curr)++;

    return output_node;
}


static Node* GetExpression(TArray* array, size_t* curr) {
    assert(array != NULL);
    
    Node* val2 = NULL;
    Node* tmp = NULL;
    
    Node* val1 = GetTerm(array, curr);
    if (val1 == NULL) { return NULL; }

    Token* next_token = array->tokens[*curr];
    Type_t next_type = next_token->type;

    while ((next_type == Add) || (next_type == Sub)) {
        (*curr)++;
        val2 = GetTerm(array, curr);
        if (val2 == NULL) { return errorNode; }
        
        tmp = val1;
        val1 = NodeInit(next_type, noValue, tmp, val2, NULL);

        next_token = array->tokens[*curr];
        next_type = next_token->type;
    }
    
    return val1;
}


static Node* GetTerm(TArray* array, size_t* curr) {
    assert(array != NULL);

    Node* val2 = NULL;
    Node* tmp = NULL;
    
    Node* val1 = GetParentheses(array, curr);
    if (val1 == NULL) { return NULL; }

    Token* next_token = array->tokens[*curr];
    Type_t next_type = next_token->type;

    while ((next_type == Mul) || (next_type == Div)) {
        (*curr)++;
        val2 = GetParentheses(array, curr);
        if (val2 == NULL) { return errorNode; }
        
        tmp = val1;
        val1 = NodeInit(next_type, noValue, tmp, val2, NULL);

        next_token = array->tokens[*curr];
        next_type = next_token->type;
    }
    
    return val1;
}



static Node* GetParentheses(TArray* array, size_t* curr) {
    assert(array != NULL);

    Token* curr_token = array->tokens[*curr];
    assert(curr_token != NULL);

    Node* new_node = NULL;
    
    if (curr_token->type == LParenth) {
        (*curr)++;
        new_node = GetExpression(array, curr);

        Token* next_token = array->tokens[*curr];
        if (next_token->type != RParenth) { return errorNode; }
        (*curr)++;

        return new_node;
    }

    new_node = GetNumber(array, curr);
    if (new_node != NULL) { return new_node; }

    new_node = GetVariable(array, curr);
    
    return new_node;
}


static Node* GetNumber(TArray* array, size_t* curr) {
    assert(array != NULL);

    Token* curr_token = array->tokens[*curr];
    assert(curr_token != NULL); 

    if (curr_token->type != Num) { return NULL; }

    (*curr)++;

    return NodeInit(Num, curr_token->value, NULL, NULL, NULL);
}


static Node* GetVariable(TArray* array, size_t* curr) {
    assert(array != NULL);

    Token* curr_token = array->tokens[*curr];
    assert(curr_token != NULL);

    if (curr_token->type != Var) { return NULL; }

    char* var_name = (curr_token->value).var;

    Value_t value;
    value.var = (char*)calloc(strlen(var_name) + 1, sizeof(char));
    assert(value.var != NULL);

    strcpy(value.var, var_name);
    (*curr)++;

    return NodeInit(Var, value, NULL, NULL, NULL);
}


static bool IsBraceSurr(Token* prev, Node* curr, Token* next, Type_t lB_type) {
    return ((curr != NULL) && 
            (prev->type == lB_type) && 
            (next->type == lB_type + 1)
           );
}

