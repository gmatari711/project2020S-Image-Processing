#include "bitwiseFunc.h"
/*initializing BYTE to zero*/
void initializeByteArr(BYTE* arr, size_t size) {
	size_t i = 0;
	for (i = 0; i < size; i++)
		arr[i] = CLEAR_MASK;
}

/*Allocating a new set of bytes in a given size.*/
BYTE* newByte(size_t size) {
	BYTE* result = (BYTE*)malloc(BYTE_SIZE * size);
	allocTest(result);

	return result;
}

/*Splits a group of bits with a given size between two BYTES.devision is determined by the number of spots left in the first BYTE.*/
void SplitBitsBetweenBytes(BYTE* byte1, BYTE source, size_t spots_left, size_t bits_num) {
	size_t right_shift = spots_left * (-1),
		left_shift = (BITS_IN_BYTE - bits_num) + (spots_left + bits_num);
	byte1[CURRENT_BYTE] |= (source >> right_shift);
	byte1[NEXT_BYTE] |= (source << left_shift);

}