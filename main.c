#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "grayImage.h"
#include "imgposcell.h"
#include "grayImageBin.h"
#include "PGM.h"


void main()
{
	imgPosCell** segments;
	grayImage* img1, * img2, * img3, * img4;
	uchar threshold = 5;
	unsigned int res;

	img1 = readPGM("feep.ascii.pgm");/*SECTION 4*/
	res = findAllSegments(img1, threshold, &segments);/*SECTIONS 1 & 2*/
	img2 = colorSegments(img1, segments, res);/*3*/
	print_pixels_arr(img2->pixels, img2->rows, img2->cols);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	delete_grayImage(img1);
	delete_grayImage(img2);

	img3 = readPGM("feep.ascii.pgm");/*SECTION 4*/
	print_pixels_arr(img3->pixels, img3->rows, img3->cols);
	saveCompressed("feep.bin", img3, 32);/*SECTION 5*/
	convertCompressedImageToPGM("feep.bin", "new_feep.pgm");/*SECTION 6*/
	delete_grayImage(img3);


	img4 = readPGM("new_feep.pgm");/*SECTION 4*/
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_pixels_arr(img4->pixels, img4->rows, img4->cols);
	delete_grayImage(img4);
	free_imgPosCellArr(segments,res);

	return 0;
}

/*:::::::::::::::::::::::::::::::::::::::::::: INITIAL TESTS USED :::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*TEST*/
grayImage* new_grayImage_test(ushort rows, ushort cols) {
	grayImage* newImage = (grayImage*)malloc(GRAYIMAGE_SIZE);

	allocTest(newImage);

	newImage->rows = rows;
	newImage->cols = cols;
	newImage->pixels = new_pixels_test2(rows, cols);/*there are 2 test*/

	return newImage;
}
uchar** new_pixels_test(ushort rows, ushort cols) {
	char** res;
	int Rows, Cols;

	res = (uchar**)malloc(UCHAR_PTR_SIZE * rows);
	allocTest(res);

	for (Rows = 0; Rows < rows; Rows++) {
		res[Rows] = (uchar*)malloc(UCHAR_SIZE * cols);
		allocTest(res[Rows]);

		for (Cols = 0; Cols < cols; Cols++)
			res[Rows][Cols] = 0;
	}
	res[0][4] = 101;
	res[0][5] = 100;
	res[0][6] = 103;

	res[1][3] = 97;
	res[1][4] = 87;
	res[1][5] = 100;
	res[1][6] = 105;
	res[1][7] = 110;

	res[2][4] = 85;
	res[2][5] = 95;
	res[2][6] = 40;

	res[3][3] = 80;
	res[3][4] = 100;
	res[3][5] = 94;
	res[3][6] = 50;

	res[4][4] = 70;
	res[4][5] = 60;
	return res;
}

uchar** new_pixels_test2(ushort rows, ushort cols) {
	char** res;
	int Rows = 0, Cols = 0;

	res = (uchar**)malloc(UCHAR_PTR_SIZE * rows);
	allocTest(res);

	for (Rows = 0; Rows < rows; Rows++) {
		res[Rows] = (uchar*)malloc(UCHAR_SIZE * cols);
		allocTest(res[Rows]);
	}
	res[0][0] = 20; res[0][1] = 22; res[0][2] = 25; res[0][3] = 80; res[0][4] = 85; res[0][5] = 85; res[0][6] = 82; res[0][7] = 100; res[0][8] = 95; res[0][9] = 47;
	res[1][0] = 23; res[1][1] = 10; res[1][2] = 24; res[1][3] = 81; res[1][4] = 84; res[1][5] = 82; res[1][6] = 82; res[1][7] = 98; res[1][8] = 98; res[1][9] = 100;
	res[2][0] = 23; res[2][1] = 21; res[2][2] = 24; res[2][3] = 83; res[2][4] = 48; res[2][5] = 50; res[2][6] = 51; res[2][7] = 53; res[2][8] = 103; res[2][9] = 100;
	res[3][0] = 4; res[3][1] = 1; res[3][2] = 4; res[3][3] = 3; res[3][4] = 2; res[3][5] = 200; res[3][6] = 204; res[3][7] = 201; res[3][8] = 205; res[3][9] = 100;
	res[4][0] = 1; res[4][1] = 0; res[4][2] = 1; res[4][3] = 8; res[4][4] = 6; res[4][5] = 10; res[4][6] = 7; res[4][7] = 7; res[4][8] = 199; res[4][9] = 102;
	res[5][0] = 2; res[5][1] = 1; res[5][2] = 2; res[5][3] = 67; res[5][4] = 72; res[5][5] = 5; res[5][6] = 112; res[5][7] = 222; res[5][8] = 198; res[5][9] = 52;
	res[6][0] = 3; res[6][1] = 4; res[6][2] = 5; res[6][3] = 69; res[6][4] = 72; res[6][5] = 8; res[6][6] = 116; res[6][7] = 221; res[6][8] = 220; res[6][9] = 55;
	res[7][0] = 85; res[7][1] = 86; res[7][2] = 7; res[7][3] = 71; res[7][4] = 68; res[7][5] = 122; res[7][6] = 115; res[7][7] = 150; res[7][8] = 54; res[7][9] = 56;
	res[8][0] = 3; res[8][1] = 5; res[8][2] = 6; res[8][3] = 159; res[8][4] = 163; res[8][5] = 122; res[8][6] = 149; res[8][7] = 148; res[8][8] = 147; res[8][9] = 57;
	res[9][0] = 2; res[9][1] = 4; res[9][2] = 160; res[9][3] = 162; res[9][4] = 161; res[9][5] = 120; res[9][6] = 147; res[9][7] = 150; res[9][8] = 146; res[9][9] = 57;

	return res;
}
/*TEST*/