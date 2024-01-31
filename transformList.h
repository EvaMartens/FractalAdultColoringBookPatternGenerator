//
// Created by epm04 on 29/11/2023.
//
#define ARRAY_BUFFER 30

typedef struct transform * next_Transform_node;

typedef struct transform{
    char callname[ARRAY_BUFFER];
    double rotation;
    double translation[2];
    double scale;
    next_Transform_node next_tranformNode;
}transformNode;


//----------
//functions to create a LINKED LIST for TRANSFORM structures
//These functions are directly based on the asynchronous lectures of Week 5 by Neal Snook
next_Transform_node transformListNode(char *newName, double rotations, double translation1, double translation2, double scale);

void trListStart(next_Transform_node *pointerToHead, next_Transform_node newNodePointer);

void trInsertAtEnd(next_Transform_node *pointerToHead, next_Transform_node newPointer);

void trDisplayInstructions(next_Transform_node listPointer);

void clear_transform_List(next_Transform_node tHead);
//----------

