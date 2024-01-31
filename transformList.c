//
// Created by epm04 on 29/11/2023.
//

#include "transformList.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

//--------------------------------------------------------------------------------------------------------
//Function to make a nodeList for a linked List
next_Transform_node transformListNode(char *newName, double rotations, double translation1, double translation2, double scale) {
    next_Transform_node newPointer;
    newPointer = malloc(sizeof(transformNode));

    if (newPointer == NULL) {
        //couldn't get memory
        return NULL;
    } else {
        //got memory, so allocate data
        strcpy(newPointer->callname, newName);
        newPointer->rotation = rotations;
        newPointer->translation[0] = translation1;
        newPointer->translation[1] = translation2;
        newPointer->scale = scale;
        return newPointer;
    }
}

//takes pointer to head and pointer to nodeList on top of list
//call by(&head, temporaryNodePointer) where head is a nodeList pointer
void trListStart(next_Transform_node * pointerToHead, next_Transform_node newNodePointer) {
    newNodePointer->next_tranformNode = *pointerToHead;
    *pointerToHead = newNodePointer;
}


void trInsertAtEnd(next_Transform_node *pointerToHead, next_Transform_node newPointer) {
    //temporary pointer to traverse list
    next_Transform_node *temp;
    temp = pointerToHead;

    //walk until you find NULL for next nodeList
    while (*temp != NULL) {
        temp = &((*temp)->next_tranformNode);
    }

    //make the new nodeList point where the old link pointed
    //and then make old last link point to new
    newPointer->next_tranformNode = *temp;
    *temp = newPointer;

}

void trDisplayInstructions(next_Transform_node listPointer) {
    while (listPointer != NULL) {
        printf("The TRANSFORM data %s is as follows:\n", listPointer->callname);
        printf("(rotation: %f, ", listPointer->rotation);
        printf("translation: x = %f, y = %f, ", listPointer->translation[0], listPointer->translation[1]);
        printf("scale: %f)\n", listPointer->scale);
        listPointer = listPointer->next_tranformNode;
    }
    printf("--------------------------------------------\n");
}
//--------------------------------
//--------------------------------
//CLEAR MEMORY FUNCTIONS

void clear_transform_List(next_Transform_node tHead){
    while(tHead != NULL) {
        next_Transform_node next = tHead;
        tHead = tHead->next_tranformNode;
        free(next);
    }
}
