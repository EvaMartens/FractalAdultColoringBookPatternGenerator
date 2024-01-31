#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "callStackList.h"

#define LINEREAD_MAX_SIZE 1000
#define G_ARRAY_BUFFER 50

//#define DEBUG_CALLSTACKLIST
//---------------------------------------------------------------------------------------------------------
//TYPEDEF DATASTRUCTURES
const char *instructions[3] = {"TRANSFORM", "FRACTAL", "GRAPHIC"};

//---------------------------------------------------------------------------------------------------------
//FUNCTION SIGNATURES
int intro();

//functions to make structures

transPtr makeTranslation(double x, double y);

//FUNCTION TO READ FILE
void nfsfFileReading(FILE *input, next_Transform_node *transform_head,
                     fractalPtr *fractal_head, graphicPtr *graphic_head);

char *changeFileExtension(char *inputString);

//FILE WRITING FUNCTIONS
void svgFileWriting(FILE *out, next_Call_node head);
void svgFileWriting2(FILE *out, next_Call_node head);
void svgFileWriting3(FILE *out, next_Call_node head);

//---------------------------------------------------------------------------------------------------------

//The main program reads in a user specified file.
//Then creates 4 Lists
//The first List is the call stack which helps keep track of what to do next
//The remaining Lists store the data of the various branches, graphics and transforms


int main() {
    int answer;
    answer = intro();
    //reading in the file
    FILE *nfsfFile;
    char userInuputBuffer[INPUT_MAX_SIZE] = "C:\\Users\\epm04\\CLionProjects\\NCBV2\\testFile1.txt";
//    do {
//        printf("Please name the NFSF file you would like to display as an SVG file:\n");
//        scanf(" %s", userInuputBuffer);
//        printf("Your file is called %s\n", userInuputBuffer);
//        nfsfFile = fopen(userInuputBuffer, "r");
//        if (nfsfFile == NULL) {
//            printf("Error: The file of the name %s could not be read in.\n", userInuputBuffer);
//        }
//    } while (nfsfFile == NULL);
    //while the file is read

    nfsfFile = fopen(userInuputBuffer, "r");
    //creating heads to the various Lists
    next_Call_node call_Stack_List_Head = NULL;
    next_Transform_node transform_List_Head = NULL;
    fractalPtr fractal_List_Head = NULL;
    graphicPtr graphic_List_Head = NULL;

    //read the file
    nfsfFileReading(nfsfFile, &transform_List_Head, &fractal_List_Head,
                    &graphic_List_Head);

    //print the info to the console to ensure everything was read right
    gDisplayInstructions(graphic_List_Head);
    trDisplayInstructions(transform_List_Head);
    frDisplayInstructions(fractal_List_Head);

    //connect branches to Graphic List, Fractal List and Transfrom List
    connectBranchestoFRogGR(graphic_List_Head, fractal_List_Head, transform_List_Head);


    fclose(nfsfFile);
    //--------FILE READ
    //--------NEXT: Output file
    FILE *output;
    char *outputfileName = changeFileExtension(userInuputBuffer);
    output = fopen(outputfileName,
                   "w"); //put data into svg file at same location as input file with same name but .svg file extension
    //svg file writing
    next_Call_node head = nextInstructionNode("FIN\n", -1);
    //initialising output list
    listStart(&call_Stack_List_Head, head);
    fractalPtr fNode = fractal_List_Head;

    while (fNode != NULL) { //for every fractal
        stack_node stackNode_List_Head = NULL;
        makeStackListStart(&stackNode_List_Head, fNode);
        //add all coordinates to string
        makeCallStackWithStackList(call_Stack_List_Head, stackNode_List_Head);
        fNode = fNode->next;
    }

#ifdef DEBUG_CALLSTACKLIST
    displayInstructions(call_Stack_List_Head);
#endif

    //print output:
    if(answer == 1) {
        svgFileWriting(output, call_Stack_List_Head);
    } else if (answer == 2) {
        svgFileWriting2(output, call_Stack_List_Head);
    } else {
        //https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
        srand(50);
        svgFileWriting3(output, call_Stack_List_Head);
    }
    fclose(output);


    //clear the memory
    clear_fractal_List(fractal_List_Head);
    clear_graphic_List(graphic_List_Head);
    clear_transform_List(transform_List_Head);
    clear_callStackList(call_Stack_List_Head);
    return 0;
}

//---------------------------------------------------------------------------------------------------------
//Intro to the program
//Determines the color of the output based on users choice
//Note that the input number needs to be typed twice..
//https://www.quora.com/In-C-language-how-could-I-check-if-the-input-entered-by-the-user-is-an-integer
int intro(){
    int choice;
    printf("----------------------------------------------------------------------\n");
    printf("Welcome to evm18's version of Neal's Fractal Coloring Book Assignment\n");
    printf("----------------------------------------------------------------------\n");
    printf("\n");
    bool notNum = true;
    do{
        printf("Please choose one of the following modes of printing:\n");
        printf("1) Black lines only\n");
        printf("2) Black and Blue (Best for identifying how the algorithm prints your fractal)\n");
        printf("3) Rainbow mode!\n");
        printf("\n");
        int input;
        scanf("%d\n", &input);
        choice = (int) input;
        if (choice == 1 || choice == 2 || choice == 3) {
                notNum = false;
                break;
        }
    } while(notNum);

    if(choice == 1) {
        printf("You chose the standard output in black\n");
    } else if (choice == 2){
        printf("You chose two black and blue :)\n");
    } else {
        printf("You chose to have a little fun!\n");
    }
    return choice;
}

//---------------------------------------------------------------------------------------------------------
//File reading function

//SOURCES, Specifically for reading in bracketed data
//https://www.educative.io/answers/how-to-read-data-using-sscanf-in-c
//https://stackoverflow.com/questions/9941962/how-can-i-use-sscanf-to-read-a-string-between-brackets
//https://stackoverflow.com/questions/13221844/fscanf-and-newline-character
//https://stackoverflow.com/questions/51556986/c-ignore-spaces-in-scanf


void nfsfFileReading(FILE *input, next_Transform_node *transform_head,
                     fractalPtr *fractal_head, graphicPtr *graphic_head) {
    //Data to track elements
    int graphic_Items = 0;
    int tranform_Items = 0;
    int frac_Items = 0;
    //booleans which help regulate while loop
    bool breakActive = false;
    bool endOfFile = false;

    while (1) {
        //is it end of file?
        endOfFile = feof(input);
        if (endOfFile) {
            break;
        }

        char nextInstruction[LINEREAD_MAX_SIZE];
        //the following if-block determines if nextInstruction needs to be read in or already contains relevant data

        if (!breakActive) {
            fscanf(input, "%s[A-Z]", nextInstruction); //Contains keyphrase at start of line in nfsf file
        } else {
            breakActive = false;
        }

        //check if it's a comment, then scan the whole line until finding a new keyword
        while(nextInstruction[0] == '/'){
            fscanf(input, " %s\n", nextInstruction);
            if(strcmp(nextInstruction, instructions[0])==0 ||strcmp(nextInstruction, instructions[1])==0 ||strcmp(nextInstruction, instructions[2])==0){
                continue;
            } else {
                fscanf(input, "%s[A-Z] ", nextInstruction);
            }
        }

        //compare against instruction array amd make appropriate struct
        //add to list
        //each if-section starts with variables to store the data in
        //and then scans the line accordingly
        if (strcmp(nextInstruction, instructions[0]) == 0) { //<----------------------------------------------------TRANSFORM
            //TRANSFORM DATA
            char tName[LINEREAD_MAX_SIZE], rName[10], sName[10];
            char buff[LINEREAD_MAX_SIZE];
            double rotation, translation1, translation2;
            float scale;
            //------

            //NAME
            fscanf(input, "%s[a-z]", tName);
            //ROTATION
            fscanf(input, " %s[A-Z]", rName);
            fscanf(input, " %s[1-9]", rName);
            sscanf(rName, "%lf", &rotation);
            //TRANSLATION :( it work but there has to be a better way
            //Please see sources above
            fscanf(input, " %s[A-Z]", buff);
            fscanf(input, " %s[^(),/]", buff);
            char *deliminator = "(";
            char *cl = strtok(buff, deliminator);
            char *stringSep = strchr(cl, ',');
            translation1 = strtod(&cl[0], &stringSep);
            deliminator = ",";
            cl = strtok(cl, deliminator);
            deliminator = ")";
            cl = strtok(NULL, deliminator);
            translation2 = strtof(cl, NULL);
            //SCALE
            fscanf(input, " %s[A-Z]", sName);
            fscanf(input, " %s[1-9]", sName);
            sscanf(sName, "%f", &scale);
            //------------
            //allocate generate TRANSFORM struct and add to List, or exit
            next_Transform_node newTNode = transformListNode(tName, rotation, translation1, translation2, scale);
            if (newTNode != NULL) {
                if (tranform_Items < 1) {
                    trListStart(transform_head, newTNode);
                    tranform_Items++;
                } else {
                    trInsertAtEnd(transform_head, newTNode);
                    tranform_Items++;
                }
            } else {
                printf("WARNING: Transform data for %s did not get allocated\n", tName);
                exit(1);
            }
        } else if (strcmp(nextInstruction, instructions[1]) == 0) { //<-------------------------------------------------------FRACTAL
            //FRACTAL DATA
            char nameOfFractal[LINEREAD_MAX_SIZE];
            branchPtr pointer[ARRAY_BUFFER]; //array buffer is 30
            int branches = 0;
            char branchCall[LINEREAD_MAX_SIZE];
            char coordiantesB[LINEREAD_MAX_SIZE];
            double coordBA;
            double coordBB;
            char nameOfBranch[LINEREAD_MAX_SIZE];
            char nameOfGraphic[LINEREAD_MAX_SIZE];
            //------------
            //NAME
            fscanf(input, " %s[a-z]", nameOfFractal);
            //printf("%s\n", nameOfFractal);
            //BRANCH
            while ((fscanf(input, " %s[A-Z]", branchCall) == 1) && (branches < 30)) {
                if (strcmp(branchCall, instructions[1]) == 0 || branchCall[0] == '/') { //IF NEXT KEYWORD IS FRACTAL or a COMMENT, break
                    strcpy(nextInstruction, branchCall);
                    breakActive = true;
                    break;
                }
                //NAME OF BRANCH
                fscanf(input, " %s[a-z]", nameOfBranch);
                //COORDIANTES
                //Please see sources above
                fscanf(input, " %s[^[]:]", coordiantesB);
                char *deliminator = "[";
                char *cg = strtok(coordiantesB, deliminator);
                char *stringSep = strchr(cg, ':');
                coordBA = strtod(&cg[0], &stringSep);
                deliminator = ":";
                cg = strtok(cg, deliminator);
                deliminator = "]";
                cg = strtok(NULL, deliminator);
                char *stringSep2 = strchr(cg, ']');
                coordBB = strtod(&cg[0], &stringSep2);
                fscanf(input, " %s[A-Z]", branchCall);
                char fracInBranch[ARRAY_BUFFER];
                //-----------
                //Allocate memory and save in fractal struct (refere to branchList.h)
                branchPtr newBranch;
                if (strcmp(branchCall, instructions[2]) == 0) { //equals GRAPHIC
                    fscanf(input, " %s[a-z]\n", nameOfGraphic);
                    strcpy(fracInBranch, "[none]");
                    newBranch = makeBranchStruct(nameOfBranch, coordBA, coordBB, fracInBranch, nameOfGraphic);
                } else { //equals FRACTAL
                    fscanf(input, " %s[a-z]\n", fracInBranch);
                    strcpy(nameOfGraphic, "[none]");
                    newBranch = makeBranchStruct(nameOfBranch, coordBA, coordBB, nameOfFractal, nameOfGraphic);
                }
                if (newBranch != NULL) {
                    pointer[branches] = newBranch;
                    branches++;
                } else {
                    printf("WARNING: could not create BRANCH %s in FRACTAL %s\n", nameOfBranch, nameOfFractal);
                }
            }//end of while inside FRACTAL
            //generate FRACTAL struct or exit
            fractalPtr fracPt = makeFractal(nameOfFractal, pointer, branches);
            if (fracPt != NULL) {
                if (frac_Items < 1) {
                    frListStart(fractal_head, fracPt);
                    frac_Items++;
                } else {
                    frInsertAtEnd(fractal_head, fracPt);
                    frac_Items++;
                }
            } else {
                printf("WARNING: could not create FRACTAL %s\n", nameOfFractal);
                exit(1);
            }
        } else if (strcmp(nextInstruction, instructions[2]) == 0) { //<-------------------------------------------------GRAPHIC
            //GRAPHIC DATA
            //------
            transPtr pointerArr[G_ARRAY_BUFFER];
            int items = 0;
            int warnings = 0;
            char nameOfGraphic[LINEREAD_MAX_SIZE];
            char coordinateG[10];
            double coordA;
            double coordB;
            //-------
            //NAME
            fscanf(input, " %s[a-z]", nameOfGraphic);
            //COORDINATES
            //Please see sources above
            while ((fscanf(input, " %s[^(),/]", coordinateG) == 1) && (items < G_ARRAY_BUFFER)) {
                //the following code was commented out bc of causing an endless loop in makeCallStackWithStackList() -> callStackList.c
                // ------------------------------------------
                    //                if(strcmp(nameOfGraphic, "none") == 0){
                    //                   transPtr coordDot0 = makeTranslation(0,0); pointerArr[0] = coordDot0;
                    //                   transPtr coordDot1 = makeTranslation(0.1,0.1); pointerArr[1] = coordDot1;
                    //                   transPtr coordDot2 = makeTranslation(0.2,0.2); pointerArr[2] = coordDot2;
                    //                   transPtr coordDot3 = makeTranslation(0,0); pointerArr[3] = coordDot3;
                    //                   items = 4;
                    //                   strcpy(nextInstruction, coordinateG);
                    //                   breakActive = true;
                    //                   break;
                    //                }
                //------------------------------------------
                if (strcmp(coordinateG, instructions[0]) == 0 || strcmp(coordinateG, instructions[1]) == 0 ||
                    strcmp(coordinateG, instructions[2]) == 0 || coordinateG[0] == '/'){
                    strcpy(nextInstruction, coordinateG);
                    breakActive = true;
                    break;
                }
                char *deliminator = "(";
                char *cg = strtok(coordinateG, deliminator);
                char *stringSep = strchr(cg, ',');
                coordA = strtod(&cg[0], &stringSep);
                deliminator = ",";
                cg = strtok(cg, deliminator);
                deliminator = ")";
                cg = strtok(NULL, deliminator);
                coordB = strtof(cg, NULL);
                transPtr graphicTranslation = makeTranslation(coordA, coordB);
                if (graphicTranslation == NULL) {
                    printf("WARNING: Graphic %s cannot save coordinates %d\n", nameOfGraphic, items);
                    warnings++;
                } else {
                    pointerArr[items] = graphicTranslation;
                    items++;
                }
            }
            //generate GRAPHIC struct or exit
            if (warnings == 0) {
                //data line read, so make struct and add to List
                graphicPtr newGraphic = graphicListNode(nameOfGraphic, pointerArr, items);
                if (graphic_Items < 1) {
                    gListStart(graphic_head, newGraphic);
                    graphic_Items++;
                } else {
                    gInsertAtEnd(graphic_head, newGraphic);
                    graphic_Items++;
                }
            } else {
                printf("Could not make graphic %s\n", nameOfGraphic);
                exit(1);
            }
        }//end of GRAPHIC
    }//end of WHILE
}

//---------------------------------------------------------------------------------------------------------
//Translation Struct function
//1 - allocate memory and make ptr
//2 - write data into memory

transPtr makeTranslation(double x, double y) {
    transPtr translationPtr = malloc(sizeof(tl));
    if (translationPtr == NULL) {
        printf("WARNING: could not create translation\n");
        return NULL;
    }
    translationPtr->x = x;
    translationPtr->y = y;
    return translationPtr;
}


//----------------------------------------------------------------------------------------------------------------------
//Functions which help generate the right file output

char *changeFileExtension(char *inputString) {
    char *outputfileName = inputString;
    char *fileExtension = ".svg";
    char *deliminator = ".";
    char *outputFile = strtok(outputfileName, deliminator);
    strcat(outputFile, fileExtension);
    printf("SVG File output to: %s\n", outputFile);
    return outputFile;
}

//based of https://www.w3schools.com/graphics/svg_polyline.asp tutorials
//I first used the sandbox are in w3schools.com and externally wrote a few test svg file to ensure I understood the format
//Then wrote the following function

//STANDARD OUTPUT FUNCTION
void svgFileWriting(FILE *out, next_Call_node head) {
    if (out != NULL) {
        char *svgHeader = "<svg width=\"700\" height=\"700\" xmlns=\"http://www.w3.org/2000/svg\">\n";
        char *border = "<rect x=\"0\" y=\"0\" width=\"700\" height=\"700\" style=\"fill:white;stroke-width:3;stroke:black\"/>\n";
        char *polylineStart = "<polyline points=\"";
        char *polylineEnd = "\" style=\"fill:none;stroke:black;stroke-width:1.0\"/>";
        char *svgEnd = "</svg>\n";

        fprintf(out, "%s", svgHeader);
        fprintf(out, "%s", border);

        char *xy;
        while ((xy = makeStringFromCallStack(head)) != NULL) {
            fprintf(out, "%s", polylineStart);
            fprintf(out, "%s", xy);
            fprintf(out, "%s\n", polylineEnd);
        }

        fprintf(out, "%s", svgEnd);
        free(xy);
    }
}

//BLACK AND BLUE
void svgFileWriting2(FILE *out, next_Call_node head) {
    if (out != NULL) {
        char *svgHeader = "<svg width=\"700\" height=\"700\" xmlns=\"http://www.w3.org/2000/svg\">\n";
        char *border = "<rect x=\"0\" y=\"0\" width=\"700\" height=\"700\" style=\"fill:white;stroke-width:3;stroke:black\"/>\n";
        char *polylineStart = "<polyline points=\"";
        char *polylineEnd = "\" style=\"fill:none;stroke:black;stroke-width:1.0\"/>";
        char *polylineEnd2 = "\" style=\"fill:none;stroke:#39bad4;stroke-width:1.0\"/>";
        char *svgEnd = "</svg>\n";

        fprintf(out, "%s", svgHeader);
        fprintf(out, "%s", border);

        char *xy;
        int count = 0;
        while ((xy = makeStringFromCallStack(head)) != NULL) {
            fprintf(out, "%s", polylineStart);
            fprintf(out, "%s", xy);
            if (count % 2 == 0) {
                fprintf(out, "%s\n", polylineEnd);
            } else {
                fprintf(out, "%s\n", polylineEnd2);
            }
            count++;
            //printLast(head);
        }

        fprintf(out, "%s", svgEnd);
        free(xy);
    }
}

//RAINBOW MODE
void svgFileWriting3(FILE *out, next_Call_node head) {
    if (out != NULL) {
        char *svgHeader = "<svg width=\"700\" height=\"700\" xmlns=\"http://www.w3.org/2000/svg\">\n";
        char *border = "<rect x=\"0\" y=\"0\" width=\"700\" height=\"700\" style=\"fill:white;stroke-width:3;stroke:black\"/>\n";
        char *polylineStart = "<polyline points=\"";
        char *polylineEndStart = "\" style=\"fill:none;stroke:";
        //hex code chosen with Google hex colour picker
        char *colors[16] = {"#4d0f0b", "#4595d6","#108512","#bd35b6","#d0d615","#e89402","#02e4e8","#5e02e8", "#bd5bb5", "#05240a", "#b36290", "#0a0938","red","orange","green","blue"};
        char *color;
        char *polylineEndEnd = ";stroke-width:1.0\"/>";
        char *svgEnd = "</svg>\n";

        fprintf(out, "%s", svgHeader);
        fprintf(out, "%s", border);

        char *xy;
        while ((xy = makeStringFromCallStack(head)) != NULL) {
            fprintf(out, "%s", polylineStart);
            fprintf(out, "%s", xy);
            fprintf(out, "%s\n", polylineEndStart);

                int index = rand()%16;
                color = colors[index];

            fprintf(out, "%s\n", color);
            fprintf(out, "%s\n", polylineEndEnd);
            //printLast(head);
        }

        fprintf(out, "%s", svgEnd);
        free(xy);
    }
}
