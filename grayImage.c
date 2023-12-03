#include "grayImage.h"


grayImage* new_grayImage(ushort rows, ushort cols) {
	grayImage* newImage = (grayImage*)malloc(GRAYIMAGE_SIZE);

	allocTest(newImage);

	newImage->rows = rows;
	newImage->cols = cols;
	newImage->pixels = new_pixels(rows, cols);

	return newImage;
}

uchar** new_pixels(ushort rows, ushort cols) {
	uchar** res;
	int Row, Col;

	res = (uchar**)malloc(UCHAR_PTR_SIZE*rows);
	allocTest(res);

	for (Row = 0; Row < rows; Row++) {
		res[Row] = (uchar*)calloc(cols, UCHAR_SIZE);
		allocTest(res[Row]);
	}
	return res;
}


void print_pixels_arr(uchar** pixels, ushort rows, ushort cols) {
	int Rows, Cols;

	for (Rows = 0; Rows < rows; Rows++) {
		for (Cols = 0; Cols < cols; Cols++)
			printf("%*d", 4, pixels[Rows][Cols]);
		putchar('\n');
	}
}


grayImage* delete_grayImage(grayImage* img) {
	int Rows;

	for (Rows = 0; Rows < img->rows; Rows++)
		free(img->pixels[Rows]);

	free(img->pixels);
	free(img);

	return NULL;
}


BOOL isOutOfBound(grayImage* img, int row, int col) {/*position[0] = rows, position[1] = cols*/
	if ((row<0)||(col<0)||((row+1)>img->rows)||((col+1)>img->cols))
		return TRUE;
	return FALSE;
}


