#ifndef MISC_H
#define MISC_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

/*Misc is short for miscellaneous. group of useful functions and definitions. this function is not tailor made for this project, as 
in real life not all folder used are tailor made for a specific problem thus i decided to keep it as it is.*/

/*const sizes*/
#define INT_SIZE sizeof(int)
#define CHAR_SIZE sizeof(char)
#define CHAR_PTR_SIZE sizeof(char*)
#define USHORT_SIZE sizeof(unsigned short)
#define UCHAR_SIZE sizeof(unsigned char)
#define UCHAR_PTR_SIZE sizeof(unsigned char*)

#define FORMAT_ENDING_SIZE 4 /*'.' followed by three letters.*/

/*Exit with error # list*/
#define MEMORY_ALLOCATION_ERROR -1
#define ERROR_OPENING_OUTPUT_FILE -2
#define ERROR_OPENING_INPUT_FILE -3
#define INPUT_FAULT -4

#define BINARY_OUTPUT "wb"
#define BINARY_INPUT "rb"
#define TEXT_INPUT "r"
#define TEXT_OUTPUT	"w"



/*Type definition*/
/*Used for easier writing and readability of code.*/
typedef unsigned char uchar;
typedef unsigned int unsint;
typedef unsigned short ushort;
typedef char BOOL;

/*BOOLEAN VALS*/
#define TRUE 1
#define FALSE 0


/*Memory allocation validation test. made for readability of code.*/
#define allocTest(X) if(!(X)) \
					exit(MEMORY_ALLOCATION_ERROR)



/*a generic struct used for sorting lists by size when size is not given.*/
typedef struct _listArrSortVar{
	void* ptr;
	size_t size;
}ListArrSortVar;


void* doubleSizeOfAnArray(void** arr, size_t* size,size_t sizeOfElem );


/*get 2 char ptrs. 1st is a name of a file and the 2nd is desired file ending. if compatible,TRUE is returned - FALSE otherwise.*/
BOOL checkFileEnding(char* ,char* );


/*Functions below verifys that file was opened properly*/
void fopenOutputTest(FILE* file);/*output file*/
void fopenInputTest(FILE* file);/*input file*/

/*qsort compatible comparison function for type ListArrSortVar. */
int listArrSortVarCmp(ListArrSortVar* var1, ListArrSortVar* var2);

#endif