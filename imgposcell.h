#ifndef LIST_H
#define LIST_H
#define _CRT_SECURE_NO_WARNINGS

#include "grayImage.h"
#include "segment.h"
#include "misc.h"
#include "Pixel.h"
#include <stdio.h>
#include <stdlib.h>

/**********************CONST VAR DEFINITIONS********************/
/*const size*/
#define SIZE_OF_IMGPOSCELL sizeof(imgPosCell)
#define SIZE_OF_IMGPOSCELL_PTR sizeof(imgPosCell*)
#define SIZE_OF_IMGPOSCELL_PTR_ARR sizeof(imgPosCell**)


/***************************************************************/

/*******************SECTION 2 TYPE DEFINITION*******************/
typedef struct _imgPosCell {
	imgPos position;
	struct _imgPosCell* next, * prev;
}imgPosCell;

/***************************************************************/




/*SECTION 2 FUNCTION*/
unsigned int findAllSegments(grayImage* img, unsigned char threshold, imgPosCell*** segments);

/*SECTION 3 FUNCTION*/
grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size);


/*Variable handling functions*/
imgPosCell** newImgPosCellArr(size_t size);/*allocates a new imgPosCell array with given size*/

imgPosCell* create_imgPosCell_node(imgPos position, imgPosCell* prev, imgPosCell* next);/*allocates a new imgPosCell node and set given
																						prev and next values.*/
void free_imgPosCell_List(imgPosCell* firstNode);/*Function will free all connected nodes.*/
void free_imgPosCellArr(imgPosCell** imgPosCellArr, int arrSize);/*free intire array of imgPosCell lists.*/


//imgPosCell** doubleImgPosCellArrSize(imgPosCell*** arr, size_t* phys);/*double size of an array.*/

/*UTIL' FUNCTIONS*/

/*Extract imgPosCell list from a givent segment.*/
imgPosCell* getListFromSegment(Segment* seg, BOOL** segments_board);


/*inesrts a new node to a list. node is positioned as requiered in sections' directions.*/
void insertImgPosCellNode(imgPosCell** firstNode, imgPosCell* newNode);

/*create a lowest case node to create a temp 'head' to the list.*/
imgPosCell makeDummyHead();

/*counts and returns the number of nodes linked to list.*/
size_t getListSize(imgPosCell* list);

/*sort arr by size of lists from the longest to the shortest.*/
void sort_imgPosCellArr(imgPosCell** arr, size_t size);


/*Passes position indexes from node to *row and *col.*/
void getRowAndCol(treeNode* node, unsint* row, unsint* col);



#endif