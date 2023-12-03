#include "segment.h"

/**************************STATIC FUNCTIONS FOR LIMITED ACCESS.************************/

/*free all treeNodes from segment*/
static void delete_segment_helper(treeNode* node);

/*helper function to findSingleSegment. gets roots' neighbors and the neighbors' neighbors and so on...*/
static void findSingleSegmentHelper(Segment* seg, grayImage* img, treeNode* root, uchar threshold, BOOL** pixelsInSeg);

/*VERIFICATIONS AND INFORMATION PROCESS*/
/*Checks if current is a neighbor of center and also doesn't belong to any other segment.*/
static BOOL isNeighbor(grayImage* img, imgPos current, imgPos center, BOOL** seg_board);

/*Tests if a given pixel is within threshold value range.*/
static BOOL isPartOfSegment(imgPos position, grayImage* img, ushort threshold, Segment* seg);

/*Checks if current is a local kernel (kernel set by recursion).*/
static BOOL isKernel(imgPos current, imgPos kernel);

/*Finds and sets neighbor array to root.*/
static void getNeighbors(Segment* seg, treeNode* root, BOOL** segmentBoard, grayImage* img, uchar threshold);

/**************************************************************************************/




/*Extern matrix used in order to find several segment without duplications.*/
extern BOOL** EXTERN_helper_matrix = NULL;

/*::::::::::::::::::::::::::::::::::: SECTION 1 FUNCTION ::::::::::::::::::::::::::::::::::::::::::::*/
Segment* findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold) {
	BOOL** segment_board=NULL;/*used to track pixels which yet to be assignd to a Segment. */
	BOOL delFlag = FALSE;/*when 'TRUE' 'deleteHelperMatrix' is called at the end of the block.*/
	if (!EXTERN_helper_matrix) {
		segment_board = buildHelperMatrix(img->rows, img->cols);/*no extern segments board was sent to func so a new segBoard is allocated.*/
		delFlag = TRUE;
	}
	else
		segment_board = EXTERN_helper_matrix;/*extern sements board was passed by ref.*/
	
	if(isOutOfBound(img, kernel[IMGPOS_ROW],kernel[IMGPOS_COL]))/*verify validity of kernel.*/
		return NULL;
	
	Segment* result = new_segment(new_tree_node(kernel, NULL));
	findSingleSegmentHelper(result,img, result->root, threshold, segment_board);/*static func is called to build Segment.*/
	if (delFlag)/*means segments board was allocated localy so deletion is necessary.*/
		deleteHelperMatrix(segment_board, img->rows);

	return result;/*returnd Segment.*/
}

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/



/*::::::::::::::::::::::::::::::::::: SECTION 1 UTIL' FUNCTIONS ::::::::::::::::::::::::::::::::::::::::*/

static void findSingleSegmentHelper(Segment* seg, grayImage* img, treeNode* root, uchar threshold, char** pixelsInSeg) {
	size_t index = 0;
	if (!root)/*root is null ptr. return condition.*/
		return;

	getNeighbors(seg, root, pixelsInSeg, img, threshold);/*root isn't null. retrieve similar_neighbors.*/
	while (root->similar_neighbors[index]) {/*runs throuh all the neighbors.*/

		findSingleSegmentHelper(seg, img, root->similar_neighbors[index++], threshold, pixelsInSeg);/*steps in to recursion with each neighbor.*/
	}
}

static void getNeighbors(Segment* seg, treeNode* center, BOOL** segmentBoard, grayImage* img, uchar threshold) {
	/*variables declerations.*/
	int i = 0, j = 0, firstRow = center->position[IMGPOS_ROW] - 1, firstCol = center->position[IMGPOS_COL] - 1;
	imgPos current = { 0,0 }; size_t physSize = 0, logSize = 0;
	center->similar_neighbors = NULL;

	for (i = firstRow; i < firstRow + NEIGHBERHOOD_SIZE; i++)/*runs through centers' neighberhood as defined in SECTION 1.*/
		for (j = firstCol; j < firstCol + NEIGHBERHOOD_SIZE; j++) {
			if (!isOutOfBound(img, i, j)) {/*test if i and j are valid indexes.*/
				setPos(current, i, j);/*sets current position.*/
				if (isNeighbor(img, current, center->position, segmentBoard))/*is neighbor?*/
					if (isPartOfSegment(current, img, threshold, seg)) {/*does current pixel passes threshold? */
						markPixelInSegBoard(current, segmentBoard);/*adding pixel to segment tree and marking it on segments board.*/
						if (physSize == logSize) {
							(treeNode**)doubleSizeOfAnArray(&(center->similar_neighbors), &physSize,TREENODE_PTR_SIZE);
						}
						center->similar_neighbors[logSize++] = new_tree_node(current, NULL);/*adding new node*/
						center->similar_neighbors[logSize] = NULL;/*sets current end of array.*/
					}
			}

		}
	center->similar_neighbors = (treeNode**)realloc(center->similar_neighbors, TREENODE_PTR_SIZE * (logSize + 1));/*set size of array to logSize+1.*/
	center->similar_neighbors[logSize] = NULL;

	seg->size += logSize;/*updates current size of segment.*/
}


static BOOL isPartOfSegment(imgPos position, grayImage* img, ushort threshold, Segment* seg) {

	uchar original_kernel_shade, pixel_shade;

	original_kernel_shade = img->pixels[seg->root->position[IMGPOS_ROW]][seg->root->position[IMGPOS_COL]];/*get kernel shade of gray*/
	pixel_shade = img->pixels[position[IMGPOS_ROW]][position[IMGPOS_COL]];/*gets current shade of gray.*/

	if ((original_kernel_shade + threshold) >= pixel_shade && (original_kernel_shade - threshold) <= pixel_shade)/*checks if current passes threshold.*/
		return TRUE;

	return FALSE;


}

void markPixelInSegBoard(imgPos current, BOOL** segmentBoard) {
	segmentBoard[current[IMGPOS_ROW]][current[IMGPOS_COL]] = TRUE;
}

static BOOL isKernel(imgPos current, imgPos kernel) {
	return((current[IMGPOS_ROW] == kernel[IMGPOS_ROW]) && (current[IMGPOS_COL] == kernel[IMGPOS_COL]));
}


static BOOL isNeighbor(grayImage* img, imgPos current, imgPos center, BOOL** seg_board) {
	return(!isKernel(current, center) && !seg_board[current[IMGPOS_ROW]][current[IMGPOS_COL]]);

}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/



/*::::::::::::::::::::::::::::::::::::::::::::::: VARIABLE HANDLING :::::::::::::::::::::::::::::::::::::::::*/
void delete_segment(Segment* seg){
	delete_segment_helper(seg->root);
	free(seg->root);

	free(seg);
}


static void delete_segment_helper(treeNode* node){
	if (!node)
		return;
	size_t index = 0;
	treeNode** curr = node->similar_neighbors;
	while (curr[index]) {
		delete_segment_helper(curr[index]);
		free(curr[index++]);
	}
	free(node->similar_neighbors);
}




void setPos(imgPos pos, ushort row, ushort col) {
	pos[IMGPOS_ROW] = row;
	pos[IMGPOS_COL] = col;
}


Segment* new_segment(treeNode* root) {
	Segment* res;
	res = (Segment*)malloc(sizeof(Segment));
	allocTest(res);

	res->root = root;
	res->size = 0;

	return res;
}


treeNode* new_tree_node(imgPos pos, treeNode** childArr) {
	treeNode* res;
	res = (treeNode*)malloc(TREENODE_SIZE);
	allocTest(res);

	res->position[0] = pos[IMGPOS_ROW];
	res->position[1] = pos[IMGPOS_COL];
	res->similar_neighbors = childArr;

	return res;
}



char** buildHelperMatrix(ushort rows, ushort cols) {
	char** result = (char**)malloc(CHAR_PTR_SIZE * rows);
	int i = 0;

	for (i = 0; i < rows; i++) {
		result[i] = (char*)calloc(cols, CHAR_SIZE);
	}

	return result;
}

void deleteHelperMatrix(char** matrix, ushort rows) {
	int i = 0;
	for (i = 0; i < rows; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

void add_tree_node(treeNode** childArr, treeNode* tnode) {
	tnode->similar_neighbors = childArr;
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/





