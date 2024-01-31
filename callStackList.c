//
// Created by epm04 on 30/11/2023.
//
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "callStackList.h"

//--------------------------------------------------------------------------------------------------------
//Function to make a nodeList for a linked List
next_Call_node nextInstructionNode(char *adjustedData, int iteration) {
    next_Call_node newPointer;
    newPointer = malloc(sizeof(nodeList));

    if (newPointer == NULL) {
        //couldn't get memory
        return NULL;
    } else {
        //got memory, so allocate data
        newPointer->coordinates = adjustedData;
        newPointer->iteration = iteration;
        return newPointer;
    }
}

//takes pointer to head and pointer to nodeList on top of list
//call by(&head, temporaryNodePointer) where head is a nodeList pointer
void listStart(next_Call_node *pointerToHead, next_Call_node newNodePointer) {
    newNodePointer->next = *pointerToHead;
    *pointerToHead = newNodePointer;
}


void insertAtEnd(next_Call_node *pointerToHead, next_Call_node newPointer) {
    //temporary pointer to traverse list
    next_Call_node *temp;
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

void displayInstructions(next_Call_node listPointer) {
    printf("The processed data in the list is as followd\n");
    char *data = listPointer->coordinates;
    printf("List start %s", data);
    listPointer = listPointer->next;
    while (listPointer != NULL) {
        data = listPointer->coordinates;
        printf("[%s] , iteration %d\n", data, listPointer->iteration);
        listPointer = listPointer->next;
    }
}

//------------------------------------------------------------------------------------------
//Functions for the stack List
stack_node makeStackNode(fractalPtr f, next_Transform_node t, int iteration){
    stack_node newPointer;
    newPointer = malloc(sizeof(nodeS));

    if (newPointer == NULL) {
        //couldn't get memory
        return NULL;
    } else {
        //got memory, so allocate data
        newPointer->f = f;
        newPointer->t = t;
        newPointer->iteration = iteration;
        newPointer->next = NULL;
        return newPointer;
    }
}

//takes pointer to head and pointer to nodeList on top of list
//call by(&head, temporaryNodePointer) where head is a nodeList pointer
void sListStart(stack_node *pointerToHead, stack_node newNodePointer) {
    newNodePointer->next = *pointerToHead;
    *pointerToHead = newNodePointer;
}


void sInsertAtEnd(stack_node * pointerToHead, stack_node newPointer) {
    //temporary pointer to traverse list
    stack_node * temp;
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

void sDisplayInstructions(stack_node listPointer) {
    while (listPointer != NULL) {
        char *data = listPointer->f->callname;
        printf("Fractal %s iteration %d\n", data, listPointer->iteration);
        listPointer = listPointer->next;
    }
}

stack_node sLast_in_List(stack_node head) {
    while (head->next != NULL) {
        head = head->next;
    }
    return head;
}
stack_node sGetFirst(stack_node head) {
    stack_node first = head;
    return first;
}

// based on algorithm of: //https://codeforwin.org/data-structures/c-program-to-delete-first-node-of-singly-linked-list
stack_node sPop_callStack(stack_node head) {
    if(head != NULL){
        stack_node delete = head;
        head = head->next;
        free(delete);
    }
    return head;
}

//---------------------------------------------------------------------------------------------------------------
//Functions to deal with Transform Items
Translation *createTLStruct(double x, double y) {
    Translation *result_tr = (Translation *) malloc(sizeof(Translation));
    if (result_tr != NULL) {
        result_tr->x = x;
        result_tr->y = y;
        return result_tr;
    }
    printf("WARNING: Translation could not be made \n");
    return NULL;
}

//Rotating the element
Translation *rotate(double rotation_angle, Translation *tr) {
    Translation *result_tr = (Translation *) malloc(sizeof(Translation));
    double radians = rotation_angle * (M_PI / 180);
    result_tr->x = tr->x * cos(radians) - tr->y * sin(radians);
    result_tr->y = tr->x * sin(radians) + tr->y * cos(radians);
    return result_tr;
}

//Rotating the translation
Translation *apply_transform_to_translation(double rotation, double scale, Translation global, Translation branch) {
    Translation *new_translation = rotate(rotation, &branch);
    new_translation->x = new_translation->x * scale;
    new_translation->y = new_translation->y * scale;
    new_translation->x += global.x;
    new_translation->y += (global.y);
    return new_translation;
}

//----------
//evm18/epm (both acronyms are me, Eva Martens)
char *addDataToString(double x, double y) {
    char data[INPUT_MAX_SIZE] = ("");
    char xS[ARRAY_BUFFER];
    char yS[ARRAY_BUFFER];
    sprintf(xS, "%lf", x);
    sprintf(yS, "%lf", y);
    strcat(data, xS);
    char *comma = ",";
    strcat(data, comma);
    strcat(data, yS);
    char *dataPt = malloc(sizeof(data)); //needs to be allocated to not be lost
    strcpy(dataPt, data);
    return dataPt;
}

//-------------------------------------------------------------------------
//Neals algorithm to create output
stack_node makeStackListStart(stack_node * head, fractalPtr fHead){
    //make stack list start with blank global transformation
    next_Transform_node init = transformListNode("init", 0, 0, 0, 1);
    stack_node start = makeStackNode(fHead,init,0);
    sListStart(head, start);
    return start;
}

//based on provided algorithm required by Neal Snooks assignment,
//using one fractal at a time inserted at the stack list start
//then reading the fractal information to create the appropriate coodrdinates as output
//in this case storing them in the "call stack list" which consists of nodes that hold the poly line
//coordinates gained by combining the graphic items with the transformations

next_Call_node makeCallStackWithStackList(next_Call_node headCS, stack_node headStack){
    int graphicCounter = 0;
    while(headStack != NULL){
        stack_node last = malloc(sizeof(*headStack));
        last->next = headStack->next;
        last->f= headStack->f;
        last->t = headStack->t;
        last->iteration = headStack->iteration;
        headStack = sPop_callStack(headStack);
        for(int i = 0; i < last->f->branches; i++){
            branchPtr b = last->f->bParray[i];
            if(last->iteration >= b->coordinates[0] && last->iteration <= b->coordinates[1]){
                //Calculating and applying the translation
                double new_tr[4];
                next_Transform_node branchTransform = b->transFormAdjustment;
                new_tr[0] = last->t->scale * branchTransform->scale;
                new_tr[1] = last->t->rotation + branchTransform->rotation;
                Translation * globalTS = createTLStruct(last->t->translation[0], last->t->translation[1]);
                Translation * branchTW = createTLStruct(b->transFormAdjustment->translation[0], b->transFormAdjustment->translation[1]);
                Translation * newTS = apply_transform_to_translation(last->t->rotation, last->t->scale, *globalTS, *branchTW);
                new_tr[2] = newTS->x;
                new_tr[3] = newTS->y;
                next_Transform_node newTransform = transformListNode("item",new_tr[1],new_tr[2],new_tr[3],new_tr[0]);
                stack_node nextInStack = makeStackNode(last->f, newTransform, (last->iteration+1));
                //Applying to graphic if graphic isn't "[none]"
                char * gname = b->graphicName;
                char * none = "[none]";
                if(strcmp(gname, none) != 0){
                    for(int j = 0; j < b->graphic->itemsInArray; j++) { //add all Graphic coordiantes to stack to be printed
                        double xCoord = b->graphic->coodrinates[j]->x;//take branch coordinates
                        double yCoord = b->graphic->coodrinates[j]->y;
                        xCoord = xCoord * last->t->scale; //scale them
                        yCoord = yCoord * last->t->scale;
                        Translation *newCoordinates = createTLStruct(xCoord, yCoord);
                        Translation *rotationApplied = rotate(last->t->rotation, newCoordinates);//rotate them
                        free(newCoordinates);
                        rotationApplied->x = rotationApplied->x + last->t->translation[0]; //add translation
                        rotationApplied->y = (rotationApplied->y + last->t->translation[1]);
                        xCoord = rotationApplied->x + 350.00; //offset
                        yCoord = -1 * (rotationApplied->y) + 350.00; //account for negative y axis values in svg files
                        char * data = addDataToString(xCoord, yCoord);
                        //------
                        //inserting into call list (nodes with strings consisting of coordinates for polyline elements)
                        next_Call_node nextGraphicPrinted = nextInstructionNode(data, graphicCounter);
                        insertAtEnd(&headCS, nextGraphicPrinted);
                        free(rotationApplied);
                    }
                    graphicCounter++;
                    nextInStack->f = last->f;
                } else {
                    nextInStack->f = b->fractalItemPtr;
                }
                //add new node to stack
                if(headStack == NULL){
                    sListStart(&headStack, nextInStack);
                } else {
                    sInsertAtEnd(&headStack,nextInStack);
                }
            }
        }
        //clearing memory of buffer node
        free(last);
    }
    //returning list
    return headCS;
}



//---------------------------------------------------

next_Call_node last_in_List(next_Call_node head) {
    while (head->next != NULL) {
        head = head->next;
    }
    return head;
}

next_Call_node pop_callStack(next_Call_node head, next_Call_node last) {
    if(head == NULL) {
        return NULL;
    }
    while (head->next != last) {
        head = head->next;
    }
    head->next = NULL;
    return last;
}

//debugging methode
void printLast(next_Call_node head) {
    next_Call_node last = last_in_List(head);
    printf("last item is of iteration %d\n", last->iteration);
}

//------------------------------------
//file output function reads call stack and adds all corrisponding graphic coordinates into appropriate strings

char *makeStringFromCallStack(next_Call_node head) { //time complexity of n^2, with doubly linked list could be significantly improved
    //look for the last node in list (i.E. stack)
    char out[INPUT_MAX_SIZE] = ("");
    next_Call_node last = last_in_List(head);
    int currentIteration = last->iteration;
    if (last->iteration == -1) {
        return NULL;
    } //if we are not on the final node
    while (last->iteration != -1 && (last->iteration == currentIteration)) {
        next_Call_node popLast = pop_callStack(head, last);
        //add data to string while we are still in the same iteration i.E. same polyLine/graphic Item
        strcat(out, popLast->coordinates);
        last = last_in_List(head);
        if (last->iteration == currentIteration) {
            strcat(out, " ");
        }
        free(popLast);
    }
    char *retOut = malloc(sizeof(out));
    strcpy(retOut, out);
    return retOut;
}

void clear_callStackList(next_Call_node head) {
    while (head != NULL) {
        next_Call_node next = head;
        head = head->next;
        free(next);
    }
}


