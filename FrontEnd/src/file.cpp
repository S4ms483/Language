#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#include "node.h"
#include "visdump.h"

char* FileRead(const char* file_name) {
    assert(file_name != NULL);
    char* buffer = CreateBuffer(file_name);

    return buffer;
}


char* CreateBuffer(const char* file) {
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

    fread(buffer, sizeof(char), (size_t)size, fp);

    fclose(fp);

    return buffer;
}


void FileOutput(Node* root, char* file) {
    assert(root != NULL);
    assert(file != NULL);

    FILE* wFile = fopen(file, "w");
    assert(wFile != NULL);

    size_t rank = 0;

    NodeOutput(root, wFile, rank);
    free(file);
}


void NodeOutput(Node* node, FILE* file, size_t rank) {
    assert(node != NULL);
    assert(file != NULL);
    switch (node->type) {
        case (Num): {
            fprintf(file, fileNum, (node->value).num);
            break;
        }

        case (Var): {
            fprintf(file, fileStr, (node->value).var);
            break;
        }

        default : {
            fprintf(file, fileStr, Keys[node->type].keyName);
            break;
        }
    }
    
    if (!node->left) {
        fprintf(file, "nil ");
    }
    
    else {
        rank++;
        NodeOutput(node->left, file, rank);
    }

    if (!node->right) {
        fprintf(file, "nil ");
    }
    
    else {
        fprintf(file, "\n");
        for (size_t i = 0; i < rank; i++) {
            fprintf(file, "\t");
        }
        NodeOutput(node->right, file, rank);
    }

    fprintf(file, ")");
}



char* GetOutputFileName(){
    printf("Enter name of the output file\n");
    char* file_name = NULL;
    size_t name_len = 0;

    getline(&file_name, &name_len, stdin);
    assert(file_name != NULL);

    return file_name;
}