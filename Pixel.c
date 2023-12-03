#include "Pixel.h"
static Pixel* newPixelArray(size_t size);
static pixelcmp_for_qsort(Pixel* pixel1, Pixel* pixel2);
/*Pixel compare function to use with qsort*/

static pixelcmp_for_qsort(Pixel* pixel1, Pixel* pixel2) {/*pixel comparison function suited for qsort. */
	int num1 = pixel1->pixel;/*assign pixel1 gray level to num1.*/
	int num2 = pixel2->pixel;/*assign pixel2 gray level to num2.*/
	return num1 - num2;/*qsort compatible comparison.*/

}

void getPosFromPixel(Pixel pixel, imgPos dest) {/*get position indexes from pixel and set it to dest.*/
	dest[IMGPOS_ROW] = pixel.position[IMGPOS_ROW];
	dest[IMGPOS_COL] = pixel.position[IMGPOS_COL];
}

void deletePixelArr(PixelsArray* arr) {/*free all memory allocated to arr.*/
	free(arr->pixels_array);
	free(arr);
}

void deleteStaticPixelArr(PixelsArray* arr) {/*delete version for static PixelArr*/
	free(arr->pixels_array);
}

PixelsArray getPixelArray(grayImage* img) {
	PixelsArray result;
	size_t i = 0, j = 0, result_index = 0,
		num_of_pixels=(size_t)(img->cols)*(img->rows);/*get size.*/

	Pixel* arr = newPixelArray(num_of_pixels);
	for(i=0;i<img->rows;i++)
		for (j = 0; j < img->cols; j++) {
			arr[result_index].pixel = img->pixels[i][j];/*gets pixel's gray level.*/
			arr[result_index].position[IMGPOS_ROW] = i;/*gets pixel's row index.*/
			arr[result_index++].position[IMGPOS_COL] = j;/*gets pixel's col index.*/
		}
	
	result.pixels_array = arr;/*assign copied array to result.*/
	result.size = num_of_pixels;/*set size at result.*/

	return result;
}

void sortPixelsArray(PixelsArray* pxl_arr) {/*calls qaort for pixelArray.*/
	qsort(pxl_arr->pixels_array, pxl_arr->size, SIZE_OF_PIXEL, pixelcmp_for_qsort);
}

static Pixel* newPixelArray(size_t size) {/*allocates new Pixel Array*/
	Pixel* result = (Pixel*)malloc(SIZE_OF_PIXEL * size);
	allocTest(result);

	return result;
}

void markPixelOnSegBoard(Pixel pixel, BOOL** seg_board) {
	uchar col = pixel.position[IMGPOS_COL];
	uchar row = pixel.position[IMGPOS_ROW];
	seg_board[row][col] = TRUE;
}