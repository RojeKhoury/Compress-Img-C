#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include <stdlib.h>

#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

#define Memory if(

#define Error ==NULL){printf("memory error");exit(1);}

#define MAX 100

#define THRESHOLD 20

#define GrayImageBaseColor 127

#define MaxGrayLevel 1

#define MAXGRAY 255

typedef struct _grayImage {

	unsigned short int rows, cols;

	unsigned char **pixels;

}grayImage;

typedef unsigned short int imgPos[2];

typedef struct treeNode {

	imgPos position;

	struct treeNodeListCell * next_possible_positions;

}treeNode;

typedef struct treeNodeListCell {

	treeNode *node;

	struct treeNodeListCell *next;

}treeNodeListCell;

typedef struct segment {

	treeNode *root;

}Segment;

typedef struct imgPosCell {

	imgPos position;

	struct imgPosCell *next, *prev;

}imgPosCell;

typedef struct imtPosList {

	imgPosCell *head, *tail;

}imgPosList;

grayImage *readPGM(char *fname);//1


								//-----------------------------------------------------------------------------------------------------

bool** creatBoolTable(short x, short y);//2

treeNode *findMe(imgPos start, unsigned char threshold, grayImage *img, bool **tb, unsigned char important);//2

treeNodeListCell* insertListCell(treeNode *node, treeNodeListCell*cell);//2

void Initialise_treeNodeListCell(treeNodeListCell*cell);//2

void InitialiseTReeNode(treeNode *node);//2

int verify(unsigned char ch, unsigned char compar, unsigned char threshort);//2

Segment findSingleSegment(grayImage *img, imgPos start, unsigned char threshold);//2

																				 //-----------------------------------------------------------------------------------------------------

int findAllSegements(grayImage *img, unsigned char threshold, imgPosList **segments);//3

void makeEmptyList(imgPosList *lst);

void creatSegment(treeNode *rer, imgPosList *list);//3

void CreatSegmentList(treeNodeListCell *rer, imgPosList *list);//3

void insertDataToEndList(imgPosList * lst, imgPos position);//3

imgPosCell * creatNode(imgPos positions);//3

										 //-----------------------------------------------------------------------------------------------------

grayImage *colorSegments(imgPosList *segments, unsigned int size);//4

int colM(imgPosCell *cell);//4

int rowM(imgPosCell *cell);//4

grayImage*imgCreate(grayImage*imgTemp, imgPosCell *cell, unsigned char ch);//4

																		   //-----------------------------------------------------------------------------------------------------

void saveCompressed(char *file_name, grayImage*image, unsigned char max_gray_level);//5

void FreeTreeOfSegmentsList(treeNode *node);//free seg

void FreeTreeOfSegments(Segment tr);//free seg

void convertCompressedImageToPGM(char* compresed_file_name, char*pgm_file_name);



int logTwo(int a);

void BoolFree(bool **tb, int x);

void freeListCell(imgPosCell*cell);

void charFree(unsigned char **ch, unsigned int x);//free character
