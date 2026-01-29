#include "read.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "node.h"
#include "visdump.h"
#include "asm.h"

static Node* NodeRead(char** chr);
static char* NameRead(char** chr);
static Type_t DefineType(char* name);


Ast* CodeToAst(char* code) {
    assert(*code != NULL);
    char* chr = code;
    Node* root = NodeRead(&chr);

    Ast* ast = AstInit();
    ast->root = root;

    return ast;
}


static Node* NodeRead(char** chr) {
    assert((chr != NULL) && (*chr != NULL));

    char* next_parenth = strchr(*chr, '(');
    char* next_nil = strstr(*chr, "nil");

    if (next_parenth && (next_parenth < next_nil)) {
        *chr = next_parenth;
        char* node_name = NameRead(chr);

        Type_t node_type = DefineType(node_name);
        Value_t value;

        switch (node_type) {
            case Num: {
                int num = atoi(node_name);
                free(node_name);
                value.num = num;
                break;
            };

            case Var : { 
                value.var = node_name; 
                break;
            }

            default : { 
                value = noValue; 
                free(node_name);
            }
        }
        
        Node* left = NodeRead(chr);
        Node* right = NodeRead(chr); 

        Node* node = NodeInit(node_type, value, left, right, NULL);

        *chr = strchr(*chr, ')') + 1;
        return node;
    }

    else if (next_nil) { 
        *chr += nilLen;
        return NULL;
    }

    return NULL;
}


static char* NameRead(char** chr) {
    assert((chr != NULL) && (*chr != NULL));

    char* start = strchr(*chr, '\"');
    char* end = strchr(start + 1, '\"');
    size_t length = end - start;

    char* name = (char*)calloc(sizeof(char), length);
    assert(name != NULL);
    strncpy(name, start + 1, length - 1);

    size_t name_len = strlen(name);
    *chr = (end + 2);
    
    return name;
}
    

static Type_t DefineType(char* name) {
    assert(name != NULL);;

    for (int i = 0; i < sizeof(Keys)/sizeof(Keys[0]); i++) {
        if ((name - strstr(name, Keys[i].keyName)) == 0) { return Keys[i].keyType; }
    }

    if (isalpha(name[0])) { return Var; }

    return Num;
}