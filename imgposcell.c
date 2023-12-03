#include "imgposcell.h"

/*:::::::::::::::::::::::::::: STATIC FUNCTIONS FOR LIMITED ACCESS ::::::::::::::::::::::::::::*/
static unsigned char garyShadeFormula(int i, unsint n);
static void colorSingleSegment(unsigned char garyShade, imgPosCell* firstNode, grayImage* res);
static void linkNodes(imgPosCell* curr, imgPosCell* newNode);

/*helper function for getListFromSegment*/
static void getListFromSegmentHelper(treeNode* seg, BOOL** segments_board, imgPosCell** node);

imgPosCell** newImgPosCellArr(size_t size) {
	imgPosCell** result = (imgPosCell**)malloc(SIZE_OF_IMGPOSCELL_PTR * size);

	allocTest(result);
	
	return result;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/





/*::::::::::::::::::::::::::::::::::::::: SECTION 2 FUNCTION :::::::::::::::::::::::::::::::::::::*/
unsigned int findAllSegments(grayImage* img, unsigned char threshold, imgPosCell*** segments) {
	size_t i = 0, j = 0, logSize = 0, physSize = 0;
	EXTERN_helper_matrix = buildHelperMatrix(img->rows, img->cols);
	imgPosCell* currSegmentList = NULL; Segment* currentSegment = NULL; imgPos currPos={ 0,0 };
	imgPosCell** result = newImgPosCellArr(physSize); PixelsArray pixArr = getPixelArray(img);

	sortPixelsArray(&pixArr);
	for (i = 0; i < pixArr.size; i++) {
		getPosFromPixel(pixArr.pixels_array[i], currPos);
		if (!EXTERN_helper_matrix[currPos[IMGPOS_ROW]][currPos[IMGPOS_COL]]) {/*not in any segment*/
			EXTERN_helper_matrix[currPos[IMGPOS_ROW]][currPos[IMGPOS_COL]] = TRUE;/*marking current segment kernel*/
			currentSegment = findSingleSegment(img, currPos, threshold);/*get a single segment with position [i][j] as kernel.*/
			currSegmentList = getListFromSegment(currentSegment, EXTERN_helper_matrix);
			delete_segment(currentSegment); currentSegment = NULL;
			if (logSize == physSize)
				(imgPosCell**)doubleSizeOfAnArray(&result, &physSize, sizeof(SIZE_OF_IMGPOSCELL_PTR));
			result[logSize++] = currSegmentList;
		}
	}

	result = (imgPosCell**)realloc(result, SIZE_OF_IMGPOSCELL_PTR * logSize);
	allocTest(result);
	sort_imgPosCellArr(result, logSize);
	deleteStaticPixelArr(&pixArr);
	deleteHelperMatrix(EXTERN_helper_matrix,img->rows);
	*segments = result;
	return logSize;
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/



/*go through a given Segment. for each pixel in seg, if its position in sements_board contains the val of 0 it adds
its imgPos to imgPosCell list.*/

/*:::::::::::::::::::::::::::::: SECTION 2 UTIL FUNCTIONS :::::::::::::::::::::::::::::::::::::*/
imgPosCell* getListFromSegment(Segment* seg,BOOL** segments_board){
	imgPosCell* result = NULL;
	imgPosCell dummy = makeDummyHead(); imgPosCell* pDummy = &dummy;
	/*dummy head is used to keep to enable insersion of a node while sorting by position.*/

	insertImgPosCellNode(&pDummy, create_imgPosCell_node(seg->root->position,NULL,NULL));
	getListFromSegmentHelper(seg->root, segments_board, &pDummy);
	
	result = pDummy->next;
	result->prev = NULL;

	return result;
}

static void getListFromSegmentHelper(treeNode* seg, BOOL** segments_board, imgPosCell** node) {
	if (!seg)
		return;
	size_t index = 0; unsint row = 0, col = 0;
	treeNode* current_tnode = seg->similar_neighbors[index++];
	while (current_tnode) {
			insertImgPosCellNode(node, create_imgPosCell_node(current_tnode->position, NULL, NULL));
			segments_board[row][col] = TRUE;
			getListFromSegmentHelper(current_tnode, segments_board, node);/*recursion with current_tnode as root.*/
			current_tnode = seg->similar_neighbors[index++];/*moves to the next neighbor*/
	}
}

size_t getListSize(imgPosCell* list) {
	if (!list)
		return 0;

	return 1 + getListSize(list->next);
}


void sort_imgPosCellArr(imgPosCell** arr, size_t size) {
	ListArrSortVar* helper_arr = (ListArrSortVar*)malloc(sizeof(ListArrSortVar) * size);
	allocTest(helper_arr);

	int i = 0;
	for (i = 0; i < size; i++) {
		helper_arr[i].ptr = arr[i];
		helper_arr[i].size = getListSize(arr[i]);
	}
	qsort(helper_arr, size, sizeof(ListArrSortVar), listArrSortVarCmp);

	for (i = 0; i < size; i++)
		arr[i] = helper_arr[i].ptr;

	free(helper_arr);
}


/*set a node with minimal values in order to ensure it stays at the head of the list.*/
imgPosCell makeDummyHead() {
	imgPosCell result;
	result.position[IMGPOS_COL] = result.position[IMGPOS_ROW] = 0;
	result.next = result.prev = NULL;

	return result;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/





/*::::::::::::::::::::::::::::::::::::::::::::::: VARIABLE HANDLING :::::::::::::::::::::::::::::::::::::::::*/

/*sets row's and col's* values according to node's imgPos*/
void getRowAndCol(treeNode* node,unsint* row, unsint* col){
	*row = node->position[IMGPOS_ROW];/*sets *row value*/
	*col = node->position[IMGPOS_COL];/*sets *col value*/
}

/*Creates a new node and sets before and after as passed in arguments. to initialize pass NULL as before and after.*/
imgPosCell* create_imgPosCell_node(imgPos pos, imgPosCell* before, imgPosCell* after) {
	imgPosCell* new_imgPosCell_node=NULL;
	new_imgPosCell_node = (imgPosCell*)malloc(sizeof(imgPosCell));/*allocate memory*/
	allocTest(new_imgPosCell_node);
	new_imgPosCell_node->position[IMGPOS_ROW] = pos[IMGPOS_ROW]; new_imgPosCell_node->position[IMGPOS_COL] = pos[IMGPOS_COL];/*sets node positions according to pos.*/
	new_imgPosCell_node->prev = before;/*sets before value*/
	new_imgPosCell_node->next = after;/*sets after value*/

	return new_imgPosCell_node;
}

/*delete all linked nodes.*/
void free_imgPosCell_List(imgPosCell* firstNode) {
	imgPosCell* curr,*temp;
	curr = firstNode;/*sets curr at the beginning og list.*/
	while (curr) {/*when reaching NULL value means we're at the end of the list (or an error).*/
		temp = curr;/*value for deletion passed to temp.*/
		curr = curr->next;/*next value on list passed to curr.*/
		free(temp);/*node on temp is released.*/
	}
}
/*free all lists in imgPosCell* arr, and then release the array itself.*/
void free_imgPosCellArr(imgPosCell** imgPosCellArr, int arrSize) {
	int i;
	for (i = 0; i < arrSize; i++) /*runs through imgPosCellArr*/
		free_imgPosCell_List(imgPosCellArr[i]);/*sent for deletion.*/
	free(imgPosCellArr);/*delete array.*/
}

/*inesrts a new node to a list. node is positioned as requiered in sections' directions.*/
void insertImgPosCellNode(imgPosCell** firstNode, imgPosCell* newNode) {
	if (!(*firstNode)) {/*list is empty*/
		*firstNode = newNode;
		return;
	}
	imgPosCell* curr = *firstNode;
	BOOL qLoop=FALSE;/*quit loop flag*/

	while(curr&&(!qLoop)) {/*while not end of list and qflag is false.*/
		if (curr->position[IMGPOS_COL] > newNode->position[IMGPOS_COL]) {/*higher col is found.*/
			linkNodes(curr, newNode);/*linkNodes links between nodes.*/
			qLoop = TRUE;/*quit loop.*/
		}
		else if (curr->position[IMGPOS_COL] == newNode->position[IMGPOS_COL])
			if (curr->position[IMGPOS_ROW] > newNode->position[IMGPOS_ROW]) {/*column val are equal. switch to row test.*/
				linkNodes(curr, newNode);/*linkNodes links between nodes.*/
				qLoop = TRUE;/*quit loop.*/
			}
		if (!(curr->next)&&(!qLoop)) {/*last node case.*/
			newNode->prev = curr;
			curr->next = newNode;
			qLoop = TRUE;/*quit loop.*/
		}

		curr = curr->next;/*next node.*/
	}
}

/*links between nodes */
static void linkNodes(imgPosCell* curr, imgPosCell* newNode) {
	newNode->next = curr;
	newNode->prev = curr->prev;
	curr->prev = newNode;
	if (newNode->prev) /*curr is not the first node on the linked list.*/
		newNode->prev->next = newNode;
}


/*:::::::::::::::::::::::::::::::::::::: SECTION 3 FUNCTION ::::::::::::::::::::::::::::::::::::::::*/


grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size) {
	grayImage* res = NULL;
	int i=0;
	uchar garyShade = 0;

	res = new_grayImage(img->rows, img->cols);
	if (size == 1)/*zero case*/
		return res;
	for (i = 0; i < size; i++) {
		garyShade = garyShadeFormula(i, size);
		colorSingleSegment(garyShade, segments[i], res);
	}
	return res;
}
static uchar garyShadeFormula(int i, unsint n) {
	return i * (MAX_GRAY_VAL/ (n - 1));
}



/*::::::::::::::::::::::::::::::::::::::: SECTION 3 FUNCTION util' :::::::::::::::::::::::::::::::::::::*/

static void colorSingleSegment(unsigned char garyShade, imgPosCell* firstNode, grayImage* res) {
	int count = 0;
	imgPosCell* curr = firstNode;
	while (curr) {
		res->pixels[curr->position[IMGPOS_ROW]][curr->position[IMGPOS_COL]] = garyShade;
		curr = curr->next;
		count++;
	}

}

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/