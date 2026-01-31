#ifndef READ_H_
#define READ_H_

#include "token.h"
#include "node.h"

const Value_t noValue = {.num = 0};
static Node* errorNode = NodeInit(Error, noValue, NULL, NULL, NULL);

Node* GetTree(TArray* array);

#endif //READ_H_