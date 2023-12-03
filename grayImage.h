#ifndef GRAYIMAGE_H
#define GRAYIMAGE_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "misc.h"



/**********************CONST VAR DEFINITIONS********************/
/*index definitions for type imgPos*/
#define IMGPOS_ROW 0
#define IMGPOS_COL 1

/*const sizes*/
#define ORIGINAL_GRAY_LEVELS 256
#define MAX_GRAY_VAL 255/*brightest shade of gray (white).*/
#define MIN_GRAY_VAL 0/*darkest shade of gray (black)*/

#define GRAYIMAGE_SIZE sizeof(grayImage)/*size of a grayImage variable.*/

/**********************GRAYIMAGE TYPE definitions*******************/
typedef unsigned short imgPos[2];



typedef struct _grayImage {

	unsigned short rows, cols;
	unsigned char** pixels;

}grayImage;
/********************************************************************/


/*grayImage 'constructor'.*/
grayImage* new_grayImage(ushort rows, ushort cols);

/*grayImage destructor. */
grayImage* delete_grayImage(grayImage* img);

/*allocates and initialize a pixel matrix.*/
uchar** new_pixels(ushort rows, ushort cols);

/*prints pixel matrix. */
void print_pixels_arr(uchar** pixels, ushort rows, ushort cols);/*NEW*/

/*if given indexes are out of bound func will return FALSE. otherwise returns TRUE.*/
BOOL isOutOfBound(grayImage* img, int row, int col);



/*TEST*/
grayImage* new_grayImage_test(ushort rows, ushort cols);
uchar** new_pixels_test(ushort rows, ushort cols);

uchar** new_pixels_test2(ushort rows, ushort cols);
/*TEST*/


#endif