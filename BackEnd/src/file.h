#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>

char* FileInput(int n_comm, char** line);
char* GetOutputFileName();

static const char* const defaultFile = "../FrontEnd/src/tree.txt";


#endif //FILE_H_