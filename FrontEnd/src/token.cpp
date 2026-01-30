#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include "file.h"

static void SkipSpaces(char** chr);
static void RemoveComments(char** chr);

static Token* TokenInit(Type_t types);
static Token* NumTokenInit(int value);
static Token* VarTokenInit(char* name);

static TArray* TokenArrayInit();

static void AddToken(TArray* array, char** chr, Type_t type);
static void AddNumToken(TArray* array, char** chr);
static void AddVarToken(TArray* array, char** chr);
static Type_t DefineKey(char* chr, int start, int end);

static void TokenArrayAppend(TArray* array,  Token* newToken);


TArray* Lexer(int n_comm, char** line) {
    assert(line != NULL);

    const char* file = NULL;

    if (n_comm == 1) { file = defaultFile; }
    else { file = line[1]; }
    
    char* buffer = FileRead(file);
    assert(buffer != NULL);

    char* chr = buffer;

    TArray* array = TokenArrayInit();

    while (*chr != '\0') {
        SkipSpaces(&chr);
        RemoveComments(&chr);

        if (isdigit(*chr)) {
            AddNumToken(array, &chr);
            continue;
        }

        if (isalpha(*chr)) {
            Type_t keyType = DefineKey(chr, wordsStart, wordsEnd);

            if (keyType != Undef) {
                AddToken(array, &chr, keyType);
                continue;
            }

            AddVarToken(array, &chr);
            continue;
        }

        Type_t keyType = DefineKey(chr, symbolsStart, symbolsEnd);
        if (keyType == Undef) {
            printf("Unknown symbol :/\n");
            break;
        }
        AddToken(array, &chr, keyType);
    }

    free(buffer);

    return array;
}


static void RemoveComments(char** chr) {
    assert((chr != NULL) && (*chr != NULL));

    char* start = strstr(*chr, commStart);
    char* end = NULL;

    while (start != NULL) {
        end = strstr(start, commEnd);
        if (end == NULL) { 
            end = strchr(*chr, '\0'); 
            return;
        }

        memset(start, ' ', end - start + 2);
        start = strstr(end, commStart);
    }
}


static void SkipSpaces(char** chr) {
    assert((chr != NULL) && (*chr != NULL));

    if (isspace(**chr)) {
        while(isspace(**chr)) { (*chr)++; }
    }
}


static Token* TokenInit(Type_t type) {
    Token* token = (Token*)calloc(1, sizeof(Token));
    assert(token != NULL);

    token->type = type; 

    return token;
}


static Token* NumTokenInit(int value) {
    Token* token = TokenInit(Num);
    (token->value).num = value;

    return token;
}


static Token* VarTokenInit(char* name) {
    assert(name != NULL);

    Token* token = TokenInit(Var);
    (token->value).var = name;

    return token;
}


static TArray* TokenArrayInit() {
    TArray* array = (TArray*)calloc(1, sizeof(TArray));
    assert(array != NULL);

    array->tokens = (Token**)calloc(1, sizeof(Token*));
    assert(array->tokens != NULL);

    array->size = 0;
    array->capacity = 1;
    
    return array;
}


static void TokenArrayAppend(TArray* array, Token* newToken) {
    assert((array != NULL) && (newToken != NULL));

    if(array->size >= array->capacity) { 
        array->tokens = (Token**)realloc(array->tokens, (array->capacity) * 2 * sizeof(Token*));
        array->capacity = array->capacity * 2;
    }
    
    array->tokens[(array->size)] = newToken;  
    (array->size)++;
}


static Type_t DefineKey(char* chr, int start, int end) {
    assert(chr != NULL);

    for (int i = start; i < end; i++) {
        if ((chr - strstr(chr, Keys[i].keyName)) == 0) { return Keys[i].keyType; }
    }

    return Undef;
}


static void AddToken(TArray* array, char** chr, Type_t type) {
    assert((chr != NULL) && (*chr != NULL));
    assert(array != NULL);

    TokenArrayAppend(array, TokenInit(type));

    int keyNum = 0;

    for (int i = 0; i < wordsEnd; i++) {
        if (Keys[i].keyType == type) { keyNum = i; } 
    }

    size_t length = strlen(Keys[keyNum].keyName);
    (*chr) += length;
}


static void AddNumToken(TArray* array, char** chr) {
    assert((chr != NULL) && (*chr != NULL));
    assert(array != NULL); 

    char* num_end = NULL;
    int val = strtol(*chr, &num_end, 10);
    
    *chr = num_end;

    TokenArrayAppend(array, NumTokenInit(val));
}


static void AddVarToken(TArray* array, char** chr) {
    assert((chr != NULL) && (*chr != NULL));
    assert(array != NULL);
    
    char* name = (char*)calloc(nameLen, sizeof(char));
    assert(name != NULL);

    size_t i = 0;
    size_t name_size = nameLen;

    int curr_letter = (int)**chr;

    while (isalnum(curr_letter) || (curr_letter == '_')) {
        (*chr)++;

        if (i > nameLen - 2) {
            name = (char*)realloc(name, name_size + nameLen);
            assert(name != NULL);
        }

        name[i] = (char)curr_letter;
        i++;
        curr_letter = (int)**chr;
    }

    TokenArrayAppend(array, VarTokenInit(name));
}


void TokenOutput(Token* token) {
    if (token->type == Num) {
        fprintf(stderr, "Type = num, value = %d\n", (token->value).num);
        return;
    }

    if (token->type == Var) {
        fprintf(stderr, "Type = var, value = %s\n", (token->value).var);
        return;
    }

    fprintf(stderr, "Type = %s\n", Keys[token->type].keyName);
}


void TokenDestroy(Token* token) {
    assert(token != NULL);

    if (token->type == Var) {
        free((token->value).var);
    }

    free(token);
}


void TokenArrayDestroy(TArray** array) {
    assert((array != NULL) && (*array != NULL));

    TArray* tmp = *array;

    for (size_t i = 0; i < ((*array)->size); i++) { 
        TokenDestroy(tmp->tokens[i]); 
    }

    tmp->size = 0;
    tmp->capacity = 0;
    free(tmp->tokens);
    free(*array);
}