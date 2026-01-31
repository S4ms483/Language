#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>

#include "node.h"

char* FileRead(const char* file_name);
char* CreateBuffer(const char* file);

void FileOutput(Node* root, char* file);
void NodeOutput(Node* node, FILE* file, size_t rank);

char* GetOutputFileName();

static const char* const fileStr = "( \"%s\" ";
static const char* const fileNum = "( \"%d\" ";

#endif //FILE_H_