#ifndef READ_H_
#define READ_H_

#include "node.h"

typedef struct
{
    const char* keyName;
    Type_t keyType;
}Keys_t;

const Keys_t Keys [] =
{
    {";",  Semicolon},
    {"==", IfEqual},
    
    {"+",  Add},
    {"-",  Sub},
    {"*",  Mul},
    {"/",  Div},
    
    {"=",  Eql},
    
    {"while", While},
    {"if",    If},
    {"else",  Else},
    
    {"input", Input},
    {"output", Output}
};


Ast* CodeToAst(char* code);

const Value_t noValue = {.num = 0};
const size_t nilLen = 4;

#endif //READ_H_