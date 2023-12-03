#include "misc.h"



void fopenInputTest(FILE* file) {
	if (!file) {
		fprintf(stderr, "Error opening file for input\n");
		exit(ERROR_OPENING_INPUT_FILE);
	}
}

void fopenOutputTest(FILE* file) {
	if (!file) {
		fprintf(stderr, "Error opening file for output\n");
		exit(ERROR_OPENING_OUTPUT_FILE);
	}
}
/*get 2 char ptrs. 1st is a name of a file and the 2nd is desired file ending. if compatible,TRUE is returned - FALSE otherwise.*/
BOOL checkFileEnding(char* file_name,char* format)
{
	int formatEndPos = strlen(file_name)-FORMAT_ENDING_SIZE;

	if (formatEndPos < 0)
		return FALSE;

	return (!strcmp(file_name+formatEndPos, format));
}

/*Generic function recive a ptr to an array and a ptr to its current size var and element size. it increments current size by 1 and then multiply by 2.
values passed by 'ref' and updated.*/
void* doubleSizeOfAnArray(void** arr, size_t* size, size_t sizeOfElem) {
	*size = (*size + 1) * 2;
	*arr = (void*)realloc(*arr,sizeOfElem*(*size));
}

/*CMP function for qsort. */
int listArrSortVarCmp(ListArrSortVar* var1, ListArrSortVar* var2) {
	return (var2->size) - (var1->size);/*var1 > var2 : result < 0
									     var1 = var2 : result = 0
										 var1 < var2 : result > 0*/
}