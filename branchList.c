//
// Created by epm04 on 30/11/2023.
//

#include "branchList.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

//--------------------------------------------------------------------------------------------------------
//Function to make a nodeList for a linked List for FRACTALS with BRANCHES

branchPtr makeBranchStruct(char *name, double t1, double t2, char *fractalName, char *graphicName) {
    branchPtr branch = malloc(sizeof(br));
    if (branch == NULL) {
        printf("WARNING: Unable to get memory for BRANCH %s\n", name);
        exit(1);
    }
    strcpy((char *) branch->callname, name);
    branch->coordinates[0] = t1;
    branch->coordinates[1] = t2;
    //one of the following two values will always be NULL
    strcpy(branch->fractalName, fractalName);
    strcpy(branch->graphicName, graphicName);
    return branch;
}

void setPtrsInBranch(branchPtr b, graphicPtr g, fractalPtr f) {
    b->graphic = g;
    b->fractalItemPtr = f;
}


fractalPtr makeFractal(char *name, branchPtr branches[], int numInArray) {
    size_t buff = sizeof(branchPtr);
    fractalPtr fractalpt = malloc(
            sizeof(fr) + (buff * numInArray)); //the size is the size for the struct plus the items in array

    if (fractalpt == NULL) {
        printf("WARNING: Unable to allocate memory for FRACTAL struct: %s\n", name);
        return NULL;
    }

    fractalpt->branches = numInArray;
    strcpy((char *) fractalpt->callname, name);
    for(int i = 0; i < numInArray; i++) {
        fractalpt->bParray[i] = branches[i];
    }

    return fractalpt;
}

//takes pointer to head and pointer to nodeList on top of list
//call by(&head, temporaryNodePointer) where head is a nodeList pointer
void frListStart(fractalPtr *pointerToHead, fractalPtr newNodePointer) {
    newNodePointer->next = *pointerToHead;
    *pointerToHead = newNodePointer;
}


void frInsertAtEnd(fractalPtr *pointerToHead, fractalPtr newPointer) {
    //temporary pointer to traverse list
    fractalPtr *temp;
    temp = pointerToHead;

    //walk until you find NULL for next nodeList
    while (*temp != NULL) {
        temp = &((*temp)->next);
    }

    //make the new nodeList point where the old link pointed
    //and then make old last link point to new
    newPointer->next = *temp;
    *temp = newPointer;

}

void frDisplayInstructions(fractalPtr listPointer) {
    while (listPointer != NULL) {
        int num = listPointer->branches;
        char *frName = listPointer->callname;
        printf("The FRACTAL %s at [%p] contains: %d branches:\n", frName, listPointer, num);
        for (int i = 0; i < listPointer->branches; i++) {
            printf("BRANCH %s", listPointer->bParray[i]->callname);
            printf(" graphic: %s", listPointer->bParray[i]->graphicName);
            printf(" fractal: %s", listPointer->bParray[i]->fractalName);
            printf(" iteration range: [%lf ", listPointer->bParray[i]->coordinates[0]);
            printf(" - %lf]\n", listPointer->bParray[i]->coordinates[1]);
        }
        listPointer = listPointer->next;
    }
    printf("--------------------------------------------\n");
}

//--------------------------------

//--------------------------------------------------------------------------------------------------------
//Function to make a nodeList for a linked List of Graphics
graphicPtr graphicListNode(char *name, transPtr grtr[], int items) {
    graphicPtr newPointer;
    newPointer = malloc(sizeof(gp) + (sizeof(newPointer) * items));

    if (newPointer == NULL) {
        //couldn't get memory
        return NULL;
    } else {
        //got memory, so allocate data
        strcpy(newPointer->graphic_name, name);
        for (int it = 0; it < items; it++) { //write poitner input array into struct
            newPointer->coodrinates[it] = grtr[it];
        }
        newPointer->itemsInArray = items;
        newPointer->next_g_node = NULL;
        return newPointer;
    }
}

//takes pointer to head and pointer to nodeList on top of list
//call by(&head, temporaryNodePointer) where head is a nodeList pointer
void gListStart(graphicPtr *pointerToHead, graphicPtr newNodePointer) {
    newNodePointer->next_g_node = *pointerToHead;
    *pointerToHead = newNodePointer;
}


void gInsertAtEnd(graphicPtr *pointerToHead, graphicPtr newPointer) {
    //temporary pointer to traverse list
    graphicPtr *temp;
    temp = pointerToHead;

    //walk until you find NULL for next nodeList
    while (*temp != NULL) {
        temp = &((*temp)->next_g_node);
    }

    //make the new nodeList point where the old link pointed
    //and then make old last link point to new
    newPointer->next_g_node = *temp;
    *temp = newPointer;

}

void gDisplayInstructions(graphicPtr listPointer) {
    while (listPointer != NULL) {
        char * gName = listPointer->graphic_name;
        printf("The Graphic %s data is as follows:\n", gName);
        for (int i = 0; i < listPointer->itemsInArray; i++) {
            printf("%d: (%f,%f)\n", (i + 1), listPointer->coodrinates[i]->x, listPointer->coodrinates[i]->y);
        }
        listPointer = listPointer->next_g_node;
    }
    printf("--------------------------------------------\n");
}
//--------------------------------------
//List connection functions
void connectBranchestoFRogGR(graphicPtr gHead, fractalPtr fHead, next_Transform_node tHead){
    while(fHead != NULL) {
        //go over branchlist in fractals and assign the valid pointers or exit(1)
        int branches = fHead->branches;
        for (int i = 0; i < branches; i++) {
            branchPtr b = fHead->bParray[i];
            if (strcmp(b->fractalName, "[none]")==0) { //branch ends in graphic
                char *searchG = b->graphicName;
                graphicPtr g = searchGraphicList(searchG, gHead);
                setPtrsInBranch(b,g,NULL);
            } else { //branch ends in fractal
                char *searchF = b->fractalName;
                fractalPtr fOfB = searchFractalList(searchF, fHead);
                setPtrsInBranch(b,NULL,fOfB);
            }
            char * searchT = (char *) b->callname;
            next_Transform_node tOfb = searchTransformNode(searchT,tHead);
            b->transFormAdjustment = tOfb;
        }
        fHead = fHead->next;
    }
}

next_Transform_node searchTransformNode(char * tName,next_Transform_node headOfTList){
    //traverse the list, comparing the graphic name and returning the node that matches names
    while(headOfTList != NULL){
        char * currentTname = headOfTList->callname;
        if(strcmp(tName,currentTname) == 0){
            return headOfTList;
        }
        headOfTList = headOfTList->next_tranformNode;
    }
    printf("WARNING: A branch calls a Transform dataset by name %s, not found in Transform List", tName);
    exit(1);
}

graphicPtr searchGraphicList(char * gName, graphicPtr headOfGList){
    //traverse the list, comparing the graphic name and returning the node that matches names
    while(headOfGList != NULL){
        char * currentGname = headOfGList->graphic_name;
        if(strcmp(gName,currentGname) == 0){
            return headOfGList;
        }
        headOfGList = headOfGList->next_g_node;
    }
    printf("WARNING: A branch calls a graphic by name %s, not found in Graphic List", gName);
    exit(1);
}

fractalPtr searchFractalList(char * fName, fractalPtr headOfFrList){
    //traverse the list, comparing the graphic name and returning the node that matches names
    while(headOfFrList != 0){
        char* currentFname = headOfFrList->callname;
        if(strcmp(fName, currentFname)==0){
            return headOfFrList;
        }
        headOfFrList = headOfFrList->next;
    }
    printf("WARNING: A branch calls a fractal by name %s, not found in Fractal List", fName);
    exit(1);
}

//--------------------------------
//CLEAR MEMORY FUNCTIONS

void clear_branch(branchPtr b) {
    free(b);
}

void clear_fractal_List(fractalPtr f) {
    while(f != NULL) {
    fractalPtr next = f;
        for (int i = 0; i < f->branches; i++) {
            if(f->bParray[i]!=NULL) {
                clear_branch(f->bParray[i]);
            }
        }
        f = f->next;
        free(next);

    }
}

void clear_graphic_List(graphicPtr gHead) {
    while (gHead != NULL) {
        graphicPtr next = gHead;
        for (int i = 0; i < G_ARRAY_BUFFER; i++) {
            gHead->coodrinates[i] = NULL;
        }
        gHead = gHead->next_g_node;
        free(next);
    }
}