#include "PGM.h"
#include "bitwiseFunc.h"
#include <math.h>

static FILE* open_file(char* fname, char* op);
static void p2_chk(FILE* Tfile);
static void skip_hash(FILE* Tfile);
static void PGM_to_grayImage(FILE* Tfile, grayImage* img);
static long int print_pormat_cols_rows(FILE* Tfile, ushort cols, ushort rows);
static void Compressed_Bfile_to_PGM(FILE* Bfile, FILE* Tfile, int arrSize, ushort rows, ushort cols, int *Pmax, char reduced_gray_levels);
static void print_num_to_Tfile(char* arr, int arrSize, FILE* Tfile, int *Pmax, char reduced_gray_levels);
static int bin2int(char* arr, int arrSize);


BOOL chk_PGM_ending(char* pgm_file_name)
{
	int len = strlen(pgm_file_name);

	if (len <= 4)
		return FALSE;

	if (pgm_file_name[len - 4] == '.' && pgm_file_name[len - 3] == 'p' && pgm_file_name[len - 2] == 'g' && pgm_file_name[len - 1] == 'm')
		return TRUE;

	return FALSE;
}

grayImage* readPGM(char *fname) {
	if (!checkFileEnding(fname,PGM_FOEMAT_ENDING))
		exit(INPUT_FAULT);

	grayImage* res = NULL;
	FILE* Tfile;
	ushort rows = 0, cols = 0;
	int max = 0;

	Tfile = open_file(fname,"r");/*open the test file*/
	p2_chk(Tfile);/*chk if the PGM file in P2 pormat*/
	skip_hash(Tfile);/*skip all the lines that start with hash*/
	fscanf(Tfile,"%hu", &cols); fscanf(Tfile, "%hu", &rows); fgetc(Tfile);/*for the '\n' */
	fscanf(Tfile, "%d", &max); fgetc(Tfile);/*for the '\n' */

	res = new_grayImage(rows,cols);
	PGM_to_grayImage(Tfile, res);

	fclose(Tfile);
	return res;
}

static FILE* open_file(char* fname, char* op) {
	FILE* res;

	res = fopen(fname, op);
	if (!res)
		exit(FILE_FAILED_TO_OPEN);
	return res;
}

static void p2_chk(FILE* Tfile) {
	char ch1,ch2;

	ch1 = fgetc(Tfile);
	ch2 = fgetc(Tfile);

	if (ch1 != 'P' || ch2 != '2')
		exit(NOT_P2_PORMAT);
	 fgetc(Tfile);/*for the '\n' */
}

/*skip all the lines that start with hash*/
static void skip_hash(FILE* Tfile) {
	unsigned char ch;
	long int pos = 0;

	while (pos++, (ch = fgetc(Tfile)) == '#') {
		while (pos++, (ch = fgetc(Tfile)) != '\n') {

		}
	}
	fseek(Tfile, 3+pos-1, SEEK_SET);
}

static void PGM_to_grayImage(FILE* Tfile, grayImage* img) {
	int i = 0, j = 0;
	unsint shade = 0;

	for (i = 0; i < img->rows; i++) {
		for (j = 0; j < img->cols; j++){
			fscanf(Tfile, "%d", &shade);
			img->pixels[i][j] = shade;
	}
		fgetc(Tfile);/*for the '\n' in the end of the line*/
	}
}
/*:::::::::::::::::::::::::::::::::::::::::::: SECTION 6 FUNCTION ::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void convertCompressedImageToPGM(char *compressed_file_name, char *pgm_file_name) {
	if (!checkFileEnding(pgm_file_name, PGM_FOEMAT_ENDING)) {
		exit(INPUT_FAULT);
	}

	FILE* Tfile, *Bfile;
	ushort rows, cols;
	char reduced_gray_levels;
	int max = 0, arrSize = 0;
	long int location_for_max = 0;

	Bfile = open_file(compressed_file_name, "rb");
	Tfile = open_file(pgm_file_name, "w+");/*add .pgm*/
	fread(&rows,sizeof(ushort),1, Bfile); fread(&cols, sizeof(ushort), 1, Bfile);
	fread(&reduced_gray_levels, sizeof(char), 1, Bfile);
	location_for_max = print_pormat_cols_rows(Tfile,cols, rows);
	arrSize = getNumOfBitsRep(reduced_gray_levels);
	Compressed_Bfile_to_PGM(Bfile, Tfile, arrSize, rows, cols,&max, reduced_gray_levels);
	fseek(Tfile, location_for_max, SEEK_SET);
	fprintf(Tfile, "%d", max);
	fclose(Bfile);
	fclose(Tfile);
}


static long int print_pormat_cols_rows(FILE* Tfile, ushort cols, ushort rows) {
	long int res = 0;

	fprintf(Tfile,"P2\n");
	fprintf(Tfile, "%hu %hu\n", cols, rows);
	res = ftell(Tfile);
	fprintf(Tfile, "   \n");/*print space so when i will have the real max i will return to this line and over print him*/
	return res;
}

static void Compressed_Bfile_to_PGM(FILE* Bfile, FILE* Tfile, int arrSize, ushort rows, ushort cols, int *Pmax, char reduced_gray_levels) {
	char* arr;
	BYTE tempByte;
	int i, colsCounter = 0, how_much_in_arr = 0, pixelCounter = 0;

	arr = newByte(arrSize);
	

	fread(&tempByte, UCHAR_SIZE, 1, Bfile);
	while (!feof(Bfile) && (pixelCounter < (rows*cols)))
	{
		for (i = 0; i < BITS_IN_BYTE; i++)/*not n, only 8*/
		{
			arr[how_much_in_arr] = (BYTE)(tempByte << i) >> 7;
			how_much_in_arr++;
			if (how_much_in_arr == arrSize) {
				print_num_to_Tfile(arr, arrSize, Tfile,Pmax, reduced_gray_levels);
				how_much_in_arr = 0;
				pixelCounter++;
				colsCounter++;
			}
			if (colsCounter == cols) {
				fputc('\n', Tfile);
				colsCounter = 0;
			}
		}
		fread(&tempByte, BYTE_SIZE, 1, Bfile);
	}
	free(arr);
}

/*prints pixels to file one row at a time.*/
static void print_num_to_Tfile(char* arr, int arrSize, FILE* Tfile, int *Pmax, char reduced_gray_levels) {
	double temp = 0;
	double multFactor = MAX_GRAY_VAL / (reduced_gray_levels - 1);/*determine multiplication fuctor for conversion from reduced to regular level. */
	int Dcompreesed = 0;
	temp = (double)bin2int(arr, arrSize);/*converts from bin representation to int casted to double. */
	if(temp)
		temp = (temp + HIGEST_VAL_FACTOR) * multFactor;
	Dcompreesed = temp;/*decompressed pixel value.*/
	if (Dcompreesed > *Pmax)
		*Pmax = Dcompreesed;/*find highest graylevel in picture.*/
	fprintf(Tfile,"%*d", 4,Dcompreesed);/*write pixel to file.*/
}

/*convert binary representation to int. char* arr contains 1 or 0 in each cell which together represent a binary digit made with
arrSize bits.*/
static int bin2int(char* arr, int arrSize) {
	int i, j = arrSize-1, res = 0;

	for (i = 0; i < arrSize ;i++)
	{
		res +=(arr[i] * pow(2,j));
		j--;
	}
	return res;
}

