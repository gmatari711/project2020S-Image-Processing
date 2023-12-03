#ifndef PIXEL_H
#define PIXEL_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "grayImage.h"


#define SIZE_OF_PIXEL sizeof(Pixel)
#define SIZE_OF_PIXEL_PTR sizeof(Pixel*)

/*Struct made to team up pixels and positions. */
typedef struct _pixel {
	uchar pixel;
	imgPos position;
}Pixel;

/*array of pixels struct.*/
typedef struct _pixelsArray {
	Pixel* pixels_array;
	size_t size;
}PixelsArray;


void deletePixelArr(PixelsArray* arr);/*delete dynamicly allocated PixelArray.*/
void deleteStaticPixelArr(PixelsArray* arr);/*delete array inside static PixelArray.*/

PixelsArray getPixelArray(grayImage* img);/*get pixel array from grayImage.*/

void sortPixelsArray(PixelsArray* pxl_arr);/*sorts pixelArray using qaort.*/

void getPosFromPixel(Pixel pixel, imgPos dest);/*get position indexes from pixel and set it to dest.*/
void markPixelOnSegBoard(Pixel pixel, BOOL** seg_board);



#endif
