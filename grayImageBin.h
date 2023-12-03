#ifndef GRAYIMAGEBIN_H
#define GRAYIMAGEBIN_H
#define _CRT_SECURE_NO_WARNINGS

#include "grayImage.h"
#include "bitwiseFunc.h"
#include "misc.h"




/*SECTION 5 function*/
void saveCompressed(char* file_name, grayImage* image, unsigned char reduced_gray_level);

/*Get the number of bits representing a pixel.*/
int getNumOfBitsRep(size_t number);

/*Calculates devision fuctor used to reduce pixel size to new gray level.*/
ushort getDevisionFactor(uchar reduced_grLvl);



#endif