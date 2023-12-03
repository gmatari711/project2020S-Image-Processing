#ifndef ___PGM_H___
#define ___PGM_H___
#define _CRT_SECURE_NO_WARNINGS

#include "grayImage.h"

#define PGM_FOEMAT_ENDING ".pgm"
#define FILE_FAILED_TO_OPEN -2
#define NOT_P2_PORMAT -2
#define HIGEST_VAL_FACTOR 0.99

grayImage *readPGM(char *fname);
BOOL chk_PGM_ending(char* pgm_file_name);
void convertCompressedImageToPGM(char *compressed_file_name, char *pgm_file_name);

#endif