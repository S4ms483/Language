#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

char* FileInput(int n_comm, char** line) {
    assert(*line != NULL);
    FILE* fp = NULL;

    if (n_comm == 1) { fp = fopen(defaultFile, "r+"); }
    else { fp = fopen(line[1], "r+"); }

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


char* GetOutputFileName(){
    printf("Enter name of the output file\n");
    char* file_name = NULL;
    size_t name_len = 0;

    getline(&file_name, &name_len, stdin);
    assert(file_name != NULL);

    return file_name;
}