#ifndef VISDUMP_H_
#define VISDUMP_H_

#define DEBUG

#include <stdio.h>

#include "node.h"

static const char* const LogF = "log/DiffLogfile.html";
static const char* const textCommSvg = "dot log/graph%d.txt -T png -o log/dump%d.svg";
static const char* const textCommPng = "dot log/graph%d.txt -T png -o log/dump%d.png";
static const size_t commLen = 50;

static const char* const dbgNumStr =  "\t\t%d [color = 3 label = \" { parent = %p | %d | %p | { left = %p | right = %p } }\"]; \n";
static const char* const dbgOpStr =   "\t\t%d [color = 1 label = \" { parent = %p | %s  | %p | { left = %p | right = %p } }\"]; \n";
static const char* const dbgSmcStr =  "\t\t%d [color = 5 label = \" { parent = %p | %s  | %p | { left = %p | right = %p } }\"]; \n";
static const char* const dbgVarStr =  "\t\t%d [color = 2 label = \" { parent = %p | %s  | %p | { left = %p | right = %p } }\"]; \n";

static const char* const noDbgNumStr = "\t\t%d [color = 3 label = \" { %d }\"]; \n";
static const char* const noDbgOpStr =  "\t\t%d [color = 1 label = \" { %s }\"]; \n";
static const char* const noDbgSmcStr = "\t\t%d [color = 5 label = \" { %s }\"]; \n";
static const char* const noDbgVarStr = "\t\t%d [color = 2 label = \" { %s }\"]; \n";

void VisualDump(Node* root, int n_dump);
void PrintDbgNode(Node* node, FILE* file);
void PrintNoDbgNode(Node* node, FILE* file);

#ifdef DEBUG 
    #define numStr dbgNumStr
    #define opStr dbgOpStr
    #define smcStr dbgSmcStr
    #define varStr dbgVarStr
    #define PrintNode(node, file) PrintDbgNode(node, file)
#else 
    #define numStr noDbgNumStr
    #define opStr noDbgOpStr
    #define smcStr noDbgSmcStr
    #define varStr noDbgVarStr
    #define PrintNode(node, file) PrintNoDbgNode(node, file)
#endif 

#endif // VISDUMP_H_