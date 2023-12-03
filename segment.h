#ifndef SEGMENT_H
#define SEGMENT_H
#define _CRT_SECURE_NO_WARNINGS
#include "grayImage.h"

/***********************CONST VAR DEFINITIONS*********************/
/*const sizes*/
#define NEIGHBERHOOD_SIZE 3
#define SEGMENT_SIZE sizeof(Segment)
#define TREENODE_SIZE sizeof(treeNode)
#define TREENODE_PTR_SIZE sizeof(treeNode*)

/*Extern matrix used in order to find several segment without duplications.*/
extern BOOL** EXTERN_helper_matrix;/*to be removed if needed*/

/*************SECTION 1 TYPE DEFINITIONS***********/
typedef struct _treeNode {
	imgPos position;
	struct _treeNode** similar_neighbors;
}treeNode;

typedef struct _segment {
	treeNode* root;
	unsigned int size;
}Segment;
/*************************************************/

/*SECTION 1 FUNCTION*/
Segment* findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold);

/*'CONSTRUCTORS' AND 'DESTRUCTORS'*/
/*Allocates a new segment and set given argument as root.*/
Segment* new_segment(treeNode* root);

/*Allocates a new treeNode ptr and assign childArr to similar_neighbors. */
treeNode* new_tree_node(imgPos pos, treeNode** childArr);

/*Builds a boolean segments board.*/
BOOL** buildHelperMatrix(ushort rows, ushort cols);
/*Delete a given segments board*/
void deleteHelperMatrix(BOOL** matrix, ushort rows);
/*Delet a given segment.*/
void delete_segment(Segment* seg);

/*VAR ACCESS AND MODIFICATION FUNCTIONS*/
/*Doubles the size and value of a treeNode ptr array and size_t size respectively, both passed by ref in order to return modified variables.*/
void  doubleTreeArr(treeNode*** arr, size_t* size);

/*Sets given imgPos with given row and col indexes.*/
void setPos(imgPos pos, ushort row, ushort col);

/*Marks a given position in segments board with the value of 'TRUE'.*/
void markPixelInSegBoard(imgPos current, BOOL** segmentBoard);

/*adds a treeNode to a given treeNode array*/
void add_tree_node(treeNode** childArr, treeNode* tnode);


















#endif


