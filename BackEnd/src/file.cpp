#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

char* FileInput(const char* file) {
    assert(file != NULL);

    FILE* fp = fopen(file, "r+");
    assert(fp != NULL);

    int descriptor = fileno(fp);
    assert(descriptor != -1);

    struct stat statistics;
    fstat(descriptor, &statistics);
    assert(statistics.st_size != -1);

    long int size = statistics.st_size;
    char* buffer = (char*)calloc((size_t)size + 1, sizeof(char));
    assert(buffer != NULL);

    fread(buffer, sizeof(char), (size_t)size, fp);

    fclose(fp);

    return buffer;
}