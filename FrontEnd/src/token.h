#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdio.h>

typedef enum
{
    LBrace,    // {
    RBrace,    // }
    LParenth,  // (
    RParenth,  // )
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
    Var,
    Undef,
    Error
}Type_t;


typedef union 
{
    int num; //TODO - add float
    char* var;
} Value_t;


typedef struct
{
    Type_t type;
    Value_t value;
}Token;


typedef struct 
{
    Token** tokens;
    size_t size;
    size_t capacity;
}TArray;


typedef struct
{
    const char* keyName;
    Type_t keyType;
}Keys_t;


const Keys_t Keys [] =
{
    {"{",  LBrace},
    {"}",  RBrace},
    {"(",  LParenth},
    {")",  RParenth},
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

const int symbolsStart = 0;
const int symbolsEnd = 11;
const int wordsStart = 11;
const int wordsEnd = 16;

static const char* const commStart = "/*";
static const char* const commEnd = "*/";

const size_t nameLen = 50;

static const char* const defaultFile = "src/code.txt";

TArray* Lexer(int n_comm, char** line);

void TokenDestroy(Token* token);
void TokenOutput(Token* token);
void TokenArrayDestroy(TArray** array);

#endif //TOKEN_H_