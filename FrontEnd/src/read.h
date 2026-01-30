#ifndef READ_H_
#define READ_H_

#include "token.h"
#include "node.h"

#define SyntaxError longjmp(Error, 1)

const Value_t noValue = {.num = 0};

Node* GetTree(TArray* array);

#endif //READ_H_