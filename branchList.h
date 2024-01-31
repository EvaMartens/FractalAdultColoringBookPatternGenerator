//
// Created by epm04 on 30/11/2023.
//
#include "transformList.h"
#define ARRAY_BUFFER 30
#define G_ARRAY_BUFFER 50

typedef struct translation * transPtr;

typedef struct translation{
    double x;
    double y;
}tl;

typedef struct graphic * graphicPtr;

typedef struct graphic{
    int itemsInArray;
    graphicPtr next_g_node;
    transPtr coodrinates[G_ARRAY_BUFFER];
    char graphic_name[ARRAY_BUFFER];
}gp;

typedef struct fractal * fractalPtr;

typedef struct branch * branchPtr;

//The following contain a variable "array buffer" which is to simplify the code.

//fractals have multiple branches
typedef struct fractal{
    int branches;
    char callname[ARRAY_BUFFER];
    branchPtr bParray[ARRAY_BUFFER];
    fractalPtr next;
}fr;

//branches can have multiple graphics and a ptr to another fractal
typedef struct branch{
    char callname[ARRAY_BUFFER]; //this is actually the name of the translation
    double coordinates[2];
    char fractalName[ARRAY_BUFFER];
    char graphicName[ARRAY_BUFFER];
    fractalPtr fractalItemPtr;
    graphicPtr graphic;
    next_Transform_node transFormAdjustment;
}br;

//----------
//functions to create a LINKED LIST
//These functions are directly based on the asynchronous lectures of Week 5 by Neal Snook
branchPtr makeBranchStruct(char *name, double t1, double t2, char* fractalName, char* graphicName);

void setPtrsInBranch(branchPtr b, graphicPtr g, fractalPtr f);

fractalPtr makeFractal(char *name, branchPtr branches[], int numInArray);

void frInsertAtEnd(fractalPtr *pointerToHead, fractalPtr newPointer);

void frListStart(fractalPtr * pointerToHead, fractalPtr newNodePointer);

void frDisplayInstructions(fractalPtr fractalListPointer);
//----------

//functions to create a LINKED LIST for GRAPHICS
//These functions are directly based on the asynchronous lectures of Week 5 by Neal Snook
graphicPtr graphicListNode(char *name, transPtr grtr[], int items);

void gListStart(graphicPtr * pointerToHead, graphicPtr newNodePointer);

void gInsertAtEnd(graphicPtr * pointerToHeadG, graphicPtr newPointerG);

void gDisplayInstructions(graphicPtr listPointerG);
//----------
//Functions to connect Lists
graphicPtr searchGraphicList(char* gName, graphicPtr headOfGList);

fractalPtr searchFractalList(char * fName, fractalPtr headOfFrList);

next_Transform_node searchTransformNode(char * tName,next_Transform_node headOfTList);

void connectBranchestoFRogGR(graphicPtr gHead, fractalPtr fHead, next_Transform_node tHead);

//--------------
void clear_branch(branchPtr b);

void clear_fractal_List(fractalPtr f);

void clear_graphic_List(graphicPtr gHead);

#ifndef NCBV2_BRANCHLIST_H
#define NCBV2_BRANCHLIST_H

#endif //NCBV2_BRANCHLIST_H
