//
// Created by epm04 on 30/11/2023.
//
#include "branchList.h"
#ifndef NEALSCOLORINGBOOK_CALLSTACKLIST_H
#define NEALSCOLORINGBOOK_CALLSTACKLIST_H
#define INPUT_MAX_SIZE 5000

//Neals Code
typedef struct {
    double x;
    double y;
}Translation;
//--------------------------------

enum instructions{
    TRANSFORM,
    BRANCH,
    FRACTAL,
    GRAPHIC
};

//adapted from
//https://www.linkedin.com/pulse/mapping-enum-string-c-language-sathishkumar-duraisamy/
static const char * const instructionStrings[] = {
        [TRANSFORM] = "transform",
        [BRANCH] = "branch",
        [FRACTAL] = "fractal",
        [GRAPHIC] = "graphic"
};

typedef struct stack * stack_node;

typedef struct stack {
    int iteration;
    fractalPtr f;
    next_Transform_node t;
    stack_node next;
} nodeS;

typedef struct callStackItem * next_Call_node;

typedef struct callStackItem {
    int iteration;
    char* coordinates;
    next_Call_node next;
} nodeList;

//----------
//functions to create a LINKED LIST for the callstack
//each node contains a pointer to a fractal
//and int to find iteration
//and a pointer to adjust the global transformation
//These functions are directly based on the asynchronous lectures of Week 5 by Neal Snook
next_Call_node nextInstructionNode(char *adjustedData, int iteration);

void listStart(next_Call_node *pointerToHead, next_Call_node newNodePointer);

void insertAtEnd(next_Call_node *pointerToHead, next_Call_node newPointer);

void displayInstructions(next_Call_node listPointer);

void clear_callStackList(next_Call_node head);

//----------------------------------------------------
stack_node makeStackNode(fractalPtr f, next_Transform_node t, int iteration);

void sListStart(stack_node *pointerToHead, stack_node newNodePointer);

void sInsertAtEnd(stack_node *pointerToHead, stack_node newPointer);

void sDisplayInstructions(stack_node listPointer);

stack_node sLast_in_List(stack_node head);

stack_node sPop_callStack(stack_node head);

stack_node sGetFirst(stack_node head);

stack_node makeStackListStart(stack_node * head, fractalPtr fHead);

next_Call_node makeCallStackWithStackList(next_Call_node headCS, stack_node headStack);

//------------------------------------
//output functions
char * addDataToString(double x, double y);

next_Call_node last_in_List(next_Call_node head);
next_Call_node pop_callStack(next_Call_node head, next_Call_node last);

char * makeStringFromCallStack(next_Call_node head);
void printLast(next_Call_node head);

//------------------------------------------
//Neals code
Translation * createTLStruct(double x, double y);
Translation * rotate(double rotation_angle, Translation * tr);
Translation * apply_transform_to_translation(double rotation, double scale, Translation global, Translation branch);

//----------
#endif //NEALSCOLORINGBOOK_CALLSTACKLIST_H
