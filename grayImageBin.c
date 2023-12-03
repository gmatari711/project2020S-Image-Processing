#include "grayImageBin.h"

/**************************STATIC FUNCTIONS FOR LIMITED ACCESS.************************/

/*ALL WRITING FUNCTIONS WRITES THE DATA TO THE GIVEN FILE IN BINARY FORMAT.*/
/*Function recieves a desired gray level for compression an image ptr and a file ptr. the function will write all pixels in image 
to the file in the same manner specified in SECTION 5 */
static void writePixlesToFile(FILE* file, grayImage* img, uchar reduced_grLvl);

/*Function will write given graylevel value to file as specifeid in SECTION 5.*/
static void writeGrayLvlToFile(FILE* file, uchar glvl);
/*/*Function will write given image size value to file as specifeid in SECTION 5.*/
static void writePicSizeToFile(FILE* file, ushort rows, ushort cols);

/*Allocates new BYTE array for the compressed image.*/
static size_t newCompressedImg(BYTE** dest, size_t numOfPixels, ushort numOfBits);


/**************************************************************************************/

/*::::::::::::::::::::::::::::::::::: SECTION 5 FUNCTION ::::::::::::::::::::::::::::::::::::::*/
void saveCompressed(char* file_name, grayImage* image, unsigned char reduced_gray_level) {
	FILE* compressedImg = fopen(file_name, "wb");/*open new bin file for writing.*/
	fopenOutputTest(compressedImg);

	writePicSizeToFile(compressedImg, image->rows, image->cols);/*Writes size*/
	writeGrayLvlToFile(compressedImg, reduced_gray_level);/*Writes gray level*/
	writePixlesToFile(compressedImg, image, reduced_gray_level);/*Writes all pixels row after row.*/

	fclose(compressedImg);/*Closing file*/

}
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

/*:::::::::::::::::::::::::::::::::::: FILE OUTPUT FUNCTION ::::::::::::::::::::::::::::::::::::*/


/*Writes the number of rows and column as unsigned short to the file*/
static void writePicSizeToFile(FILE* file, ushort rows, ushort cols) {
	fwrite(&rows, USHORT_SIZE, 1, file);/*Write number of rows*/
	fwrite(&cols, USHORT_SIZE, 1, file);/*Write number of columns*/
}

/*Writes how many gray levels are in the compressed image as unsigned char.*/
static void writeGrayLvlToFile(FILE* file, uchar glvl) {
	fwrite(&glvl, UCHAR_SIZE, 1, file);/*Writing given gray level to file.*/
}

/*Writes adjusted pixels to the file one row at a time.*/
static void writePixlesToFile(FILE* file, grayImage* img, uchar reduced_grLvl) {
	uchar devFactor = (getDevisionFactor(reduced_grLvl));
	size_t i = 0, j = 0, bIndex = 0, numOfPixels = ((img->cols) * (img->rows)), compressedImgSize = 0, pixelSize = getNumOfBitsRep(reduced_grLvl);
	int freeBits = BITS_IN_BYTE;
	BYTE* compressedImg = NULL;	BYTE current = CLEAR_MASK;/*CompressedImg will contain compressed pixels*/

	compressedImgSize = newCompressedImg(&compressedImg, numOfPixels, pixelSize);

	for (i = 0; i < img->rows; i++)
		for (j = 0; j < img->cols; j++) {
			current = img->pixels[i][j] / devFactor;/*Gets current reduced level pixel.*/
			/*Following procedures positions and sets pixel in the right place*/
			freeBits -= pixelSize;
			if (freeBits < 0) {
				SplitBitsBetweenBytes(compressedImg + bIndex, current, freeBits, pixelSize);
				freeBits += BITS_IN_BYTE;
				bIndex++;
			}
			else {
				compressedImg[bIndex] |= (current << freeBits);
				if (!freeBits) {
					bIndex++;
					freeBits += BITS_IN_BYTE;
				}
			}

		}
	fwrite(compressedImg, BYTE_SIZE, compressedImgSize, file);/*Writes entire BYTE array to file.*/
	free(compressedImg);
}



/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

/*::::::::::::::::::::::::::::::::::: UTILITY FUNCTIONS ::::::::::::::::::::::::::::::::::::::::*/


/*get the size of each pixel representaion*/
int getNumOfBitsRep(size_t number) {
	if (number == 1)
		return 0;
	return 1 + getNumOfBitsRep(number / 2);
}

/*Returns the ratio between the OG gray level and the reduced gray level.*/
ushort getDevisionFactor(uchar reduced_grLvl) {
	return ORIGINAL_GRAY_LEVELS / reduced_grLvl;
}

/*Allocates a new BYTE arr to given pointer with the minimal size to hold image pixels and return its size.*/
static size_t newCompressedImg(BYTE** dest, size_t numOfPixels, ushort numOfBits) {
	BYTE* result;
	size_t size = (numOfPixels * numOfBits) / BITS_IN_BYTE,
		remainder = (numOfPixels * numOfBits) % BITS_IN_BYTE;/*Get remainder.*/

	if (remainder)/*Number of bits used to rep all the pixel is not devided by size of a byte without a remainder*/
		size++;/*Rounding up the number of bytes.*/
	result = newByte(size);


	initializeByteArr(result, size);/*Sets byte in array to 0*/

	*dest = result;/*Result return through reference*/

	return size;/*Function returns the size of array.*/

}

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/








