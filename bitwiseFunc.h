#ifndef BITWISEFUNC_H
#define BITWISEFUNC_H
#define _CRT_SECURE_NO_WARNINGS

#include "misc.h"
#include <stdio.h>

/*BYTE VARIABLE DEFINITION*/
typedef unsigned char BYTE;

/**********************CONST VAR DEFINITIONS********************/

/*labels*/
#define CURRENT_BYTE 0
#define NEXT_BYTE 1
/*const sizes*/
#define BYTE_SIZE sizeof(BYTE)
#define BYTE_PTR_SIZE sizeof(BYTE*)
#define BITS_IN_BYTE 8
#define CLEAR_MASK 0

/*LSB Masks*/
#define LSB8_MASK(TYPE) ((TYPE)0xFF)	/*11111111*/
#define LSB7_MASK(TYPE) ((TYPE)0x7F)	/*01111111*/
#define LSB6_MASK(TYPE) ((TYPE)0x3F)	/*00111111*/
#define LSB5_MASK(TYPE) ((TYPE)0x1F)	/*00011111*/
#define LSB4_MASK(TYPE) ((TYPE)0xF)		/*00001111*/
#define LSB3_MASK(TYPE) ((TYPE)0x7)		/*00000111*/
#define LSB2_MASK(TYPE) ((TYPE)0x3)		/*00000011*/
#define LSB1_MASK(TYPE) ((TYPE)0x1)		/*00000001*/

/*MSB mask set by given I value*/
#define	BYTE_MSB_MASKE(I) ((0xFF)<<(I)) 

/*convert digit character to bits*/
#define CHAR_DIG_TO_BIT(c, TYPE) (TYPE)(c-'0')

/*Initialize a given BYTE as zero*/
void initializeByteArr(BYTE* arr, size_t size);

/*allocate a new byte/byte array depand on passed size.*/
BYTE* newByte(size_t size);

/*function will split a binary represented pixel between 2 bytes depened on the number of bits representing the pixel.*/
void SplitBitsBetweenBytes(BYTE* byte1, BYTE source, size_t spots_left, size_t bits_num);


#endif