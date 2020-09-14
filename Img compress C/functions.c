#include "projectHeader.h"
	bool** creatBoolTable(short x, short y)
{
	bool**tb;
	int i, j;
	tb = (bool**)malloc((x + 2) * sizeof(bool*));//+ 2 for the limit of the table
	Memory(tb) Error
		for (i = 0; i < x + 2; i++)
		{
			*(tb + i) = (bool*)malloc((y + 2) * sizeof(bool));//+2 for the limite of the table
			Memory(tb + i) Error
				for (j = 0; j < y + 2; j++)
				{
					if (i == 0 || i == x + 1)//limite false
						tb[i][j] = 0;
					else if (j == 0 || j == y + 1)//limite false
						tb[i][j] = 0;
					else tb[i][j] = 1;//other true
				}
		}
	return tb;
}
grayImage *readPGM(char *fname)
{
	int i, j;//for for
	int pixelTemp, gray;//for temp pixel and for a level of gray
	char name[MAX];//for scan name of file
	FILE *fw;//for open the file
	grayImage *result;//for return a result
	result = (grayImage*)malloc(sizeof(grayImage));

	Memory result Error

	fw = fopen(fname, "r");
	if (fw == NULL)
	{
		printf("It's not a good name file\n");
		return NULL;
	}
	fgets(name, MAX, fw);//delete name of file
	fscanf(fw, "%hu %hu", &result->cols, &result->rows);//take col and row
	fscanf(fw, "%d", &gray);//scan level of gray

	result->pixels = (unsigned char**)malloc(result->rows * sizeof(unsigned char*));
	Memory result->pixels Error
	printf("PGM file successfully loaded into memory\n");
	printf("The PGM matrix we loaded is:\n");
		for (i = 0; i < result->rows; i++)
		{
			*(result->pixels + i) = (unsigned char*)malloc((result->cols) * sizeof(unsigned char));
			Memory *(result->pixels + i) Error

				for (j = 0; j < result->cols; j++)
				{
					fscanf(fw, "%d", &pixelTemp);//scan pixel after pixel
					printf("%*d",3, pixelTemp);//\t do line for your settings's computer(max 255 it's good)
					result->pixels[i][j] = pixelTemp;//insert in table
				}
			printf("\n");
		}
	fclose(fw);
	return result;//return image
}
Segment findSingleSegment(grayImage *img, imgPos start, unsigned char threshold)
{
	Segment result;//for return
	result.root = (treeNode*)malloc(sizeof(treeNode));
	Memory result.root Error

		InitialiseTReeNode(result.root);//initialise root
	bool **tb = creatBoolTable(img->rows, img->cols);//table true or false
	result.root = findMe(start, threshold, img, tb, img->pixels[start[0]][start[1]]);//recursive
	BoolFree(tb, img->rows);//free table
	return result;//return result
}
void BoolFree(bool **tb, int x)//free table true and false
{
	int i;
	for (i = 0; i < x + 2; i++)
		free(*(tb + i));
	free(tb);
}
treeNode *findMe(imgPos start, unsigned char threshold, grayImage *img, bool **tb, unsigned char important)
{
	int veri, howManyRecursive = 0;//verif for know if is a good difference 
	int i, j;//for for
	imgPos *posForRecursive;//for save the position for recursive
	treeNode *node;//for register
	treeNode *nodeTempForCell;//for register in cell

	posForRecursive = (imgPos*)malloc(sizeof(imgPos));
	Memory posForRecursive Error

		node = (treeNode*)malloc(sizeof(treeNode));
	Memory node Error


	InitialiseTReeNode(node);//initialise node
	node->position[0] = start[0];
	node->position[1] = start[1];



	tb[start[0] + 1][start[1] + 1] = 0;
	for (i = start[0]; i <= start[0] + 2; i++)
	{
		for (j = start[1]; j <= start[1] + 2; j++)
		{
			if (tb[i][j])//table true or false
			{
				veri = verify(important, img->pixels[i - 1][j - 1], threshold);//if is in dif
				if (veri)
				{
					howManyRecursive++;//one recusrive better
					if (howManyRecursive != 1)//realoc if how>1
					{
						posForRecursive = (imgPos*)realloc(posForRecursive, howManyRecursive * sizeof(imgPos));
						Memory posForRecursive Error
					}
					posForRecursive[howManyRecursive - 1][0] = i - 1;//register position
					posForRecursive[howManyRecursive - 1][1] = j - 1;//register position
					tb[i][j] = 0;//table position egal false
				}
			}
		}
	}
	for (i = 0; i < howManyRecursive; i++)//for do recursive
	{
		nodeTempForCell = findMe(posForRecursive[i], threshold, img, tb, important);//recursive
		node->next_possible_positions = insertListCell(nodeTempForCell, node->next_possible_positions);//insert in next
	}
	free(posForRecursive);//free imgpos
	return node;//return result
}
int verify(unsigned char ch, unsigned char compar, unsigned char threshort)
{
	if (ch - compar >= -(int)threshort && (ch - compar) <= (int)threshort)//if good interval
		return 1;
	return 0;
}
void InitialiseTReeNode(treeNode *node)//initialise
{
	node->next_possible_positions = NULL;
}
void Initialise_treeNodeListCell(treeNodeListCell*cell)//initialise
{
	cell->next = NULL;
	cell->node = NULL;
}
treeNodeListCell* insertListCell(treeNode *node, treeNodeListCell*cell)//insert
{
	if (cell == NULL)
	{
		cell = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
		Memory cell Error

			cell->node = node;//find last element
		cell->next = NULL;
	}
	else
		cell->next = insertListCell(node, cell->next);//recursive
	return cell;
}
int findAllSegements(grayImage *img, unsigned char threshold, imgPosList **segments)
{
	int HowManySegments = 0;
	int i, j;
	imgPos start;
	Segment TempForCopy;//temp for copy
	imgPosList *seg;//for return in segments
	bool **tb;//table true or false
	tb = creatBoolTable(img->rows, img->cols);
	seg = (imgPosList*)malloc(sizeof(imgPosList));//*seg=(x,y)-> malloc
	Memory seg Error

		for (i = 0; i < img->rows; i++)
		{
			for (j = 0; j < img->cols; j++)
			{
				if (tb[i + 1][j + 1])//if pixel not egal with segments others
				{
					HowManySegments++;//plus how
					start[0] = i;
					start[1] = j;
					TempForCopy.root = findMe(start, threshold, img, tb, img->pixels[start[0]][start[1]]);//use fonction finde me

					if (HowManySegments != 1)
					{
						seg = (imgPosList*)realloc(seg, HowManySegments * sizeof(imgPosList));//realloc if how >1
						Memory seg Error
					}
					makeEmptyList(&seg[HowManySegments - 1]);//make empty
					creatSegment(TempForCopy.root, &seg[HowManySegments - 1]);//translate temp in seg
					FreeTreeOfSegments(TempForCopy);//free temps
				}
			}
		}
	printf("All the segments in the picture were found\n");
	BoolFree(tb, img->rows);//free table true or false
	*segments = seg;
	return HowManySegments;
}
void CreatSegmentList(treeNodeListCell *NodeCell, imgPosList *list)//translate with recursive
{
	if (NodeCell != NULL)
	{
		insertDataToEndList(list, NodeCell->node->position);
		if (NodeCell->next != NULL)
			CreatSegmentList(NodeCell->next, list);
		if (NodeCell->node->next_possible_positions != NULL)
			CreatSegmentList(NodeCell->node->next_possible_positions, list);
	}
}
void creatSegment(treeNode *node, imgPosList *list)//translate
{
	if (node != NULL)
	{
		insertDataToEndList(list, node->position);
		if (node->next_possible_positions != NULL)
			CreatSegmentList(node->next_possible_positions, list);
	}
}
void makeEmptyList(imgPosList  *lst)
{
	lst->head = lst->tail = NULL;
}
void insertDataToEndList(imgPosList * lst, imgPos position)
{
	imgPosCell *node;
	node = creatNode(position);//node->next=null
	if (lst->head == NULL)
	{
		node->prev = NULL;//first prev egal null
		lst->head = lst->tail = node;
	}
	else
	{
		lst->tail->next = node;
		lst->tail->next->prev = lst->tail;//insert prev
		lst->tail = node;
	}
}
imgPosCell * creatNode(imgPos positions)
{
	imgPosCell * node;
	node = (imgPosCell *)malloc(sizeof(imgPosCell));
	Memory node Error

		node->position[0] = positions[0];
	node->position[1] = positions[1];
	node->next = NULL;
	return node;
}
grayImage *colorSegments(imgPosList *segments, unsigned int size)
{
	grayImage *res;
	int col, colMax = 0;
	int row, rowMax = 0;
	int i, j;
	unsigned char newCharact;
	res = (grayImage*)malloc(sizeof(grayImage));
	Memory res Error
		for (i = 0; i < (int)size; i++)//find the max col and row
		{
			col = colM(segments[i].head);
			row = rowM(segments[i].head);
			if (row > rowMax)
				rowMax = row;
			if (col > colMax)
				colMax = col;
		}
	res->cols = colMax + 1;//insert in res
	res->rows = rowMax + 1;//insert in res
	res->pixels = (unsigned char**)malloc(res->rows * sizeof(unsigned char*));
	Memory res->pixels Error
		for (i = 0; i < (int)(res->rows); i++)
		{
			*(res->pixels + i) = (unsigned char*)malloc((res->cols) * sizeof(unsigned char));
			Memory *(res->pixels + i) Error
				for (j = 0; j < (int)(res->cols); j++)
					res->pixels[i][j] = ' ';//for decoration
		}
	for (i = 0; i < (int)size; i++)
	{
		if (size != 1)
			newCharact = (i * 255) / (size - 1);//find the new color
		else
			newCharact = 0;
		res = imgCreate(res, segments[i].head, newCharact);//insert the new color
	}
	printf("Coloring the segment in the picture was successfully completed\n");

	return res;
}
grayImage*imgCreate(grayImage*imgTemp, imgPosCell *cell, unsigned char ch)
{
	imgTemp->pixels[cell->position[0]][cell->position[1]] = ch;
	if (cell->next != NULL)
		imgTemp = imgCreate(imgTemp, cell->next, ch);
	return imgTemp;
}
int colM(imgPosCell *cell)//find colM
{
	int colMax;
	if (cell->next == NULL)
		return cell->position[1];
	colMax = colM(cell->next);
	if (colMax >cell->position[1])
		return colMax;
	else
		return cell->position[1];
}
int rowM(imgPosCell *cell)//find RowM
{
	int row = 0, rowMax;
	rowMax = cell->position[0];
	if (cell->next != NULL)
		row = rowM(cell->next);
	if (row > rowMax)
		return row;
	return rowMax;
}
void saveCompressed(char *file_name, grayImage*image, unsigned char max_gray_level)//compress file
{
	FILE * fw;
	int i, j, index = 0;
	unsigned char*chForPrintInDoc;
	unsigned char tmp = 0x0;
	int res = logTwo(max_gray_level) + 1;

	fw = fopen(file_name, "wb");
	if (fw == NULL)
	{
		printf("It's nota good name file\n");
		return;
	}
	else {
		fwrite(&image->cols, sizeof(unsigned short int), 1, fw);
		fwrite(&image->rows, sizeof(unsigned short int), 1, fw);
		fwrite(&max_gray_level, sizeof(unsigned char), 1, fw);

		chForPrintInDoc = (unsigned char*)malloc(sizeof(unsigned char)*(image->cols*image->rows));
		Memory chForPrintInDoc Error

			for (i = 0; i < image->rows; i++)
			{
				for (j = 0; j < image->cols; j++)
				{
					chForPrintInDoc[index] = (image->pixels[i][j] * max_gray_level) / MAXGRAY;//translate
					index++;
				}
			}
		for (i = 0; i < index; i++)
		{
			tmp |= ((chForPrintInDoc[i] << (8 - res)) >> ((i*res) % 8));//all the time
			if (((i + 1)*res % 8) < res)//if character is complete
			{
				fwrite(&tmp, sizeof(unsigned char), 1, fw);//print the character
				tmp = 0x0;//reinitailise the character
				tmp |= chForPrintInDoc[i] << (8 - (((i + 1)*res) % 8));//done the rest
			}
			if ((i == index - 1) && (((index * res) % 8) != 0))//if the last character not print 
				fwrite(&tmp, sizeof(unsigned char), 1, fw);
		}
		printf("Compressing the picture into file was successfully completed\n");
		free(chForPrintInDoc);//free
		fclose(fw);
	}
}
int logTwo(int a)//log2
{
	int res = 0;
	while (a > 1)
	{
		res++;
		a /= 2;
	}
	return res;
}
void convertCompressedImageToPGM(char* compresed_file_name, char*pgm_file_name)
{
	unsigned short int col, row;//for scan col and row
	unsigned char ch;//level of gray
	unsigned char*PgmNumbers;//for print in file text
	unsigned char *tmpNumbers;//for temp
	unsigned char mask = 0x0;//for play with bit
	FILE*fr;//file read
	FILE *fw;//file write
	int MaxIndex = 0;
	int i;
	int power;//log2 level of gray
	int index = 0;

	fr = fopen(compresed_file_name, "rb");
	if (fr == NULL)
	{
		printf("It's nota good name file\n");
		return;
	}
	else {
		fw = fopen(pgm_file_name, "w");

		fread(&col, sizeof(unsigned short int), 1, fr);
		fread(&row, sizeof(unsigned short int), 1, fr);
		fread(&ch, sizeof(unsigned char), 1, fr);

		power = logTwo(ch) + 1;

		if ((col*row*power) % 8 != 0)//if i have charcater not complete
			MaxIndex++;
		MaxIndex += ((col*row* power) / 8);//how many character in file read

		PgmNumbers = (unsigned char*)calloc(row*col, sizeof(unsigned char));
		Memory PgmNumbers Error

			tmpNumbers = (unsigned char*)malloc(sizeof(unsigned char)*MaxIndex);
		Memory tmpNumbers Error

			for (i = 0; i < MaxIndex; i++)
				fread(&tmpNumbers[i], sizeof(unsigned char), 1, fr);//scan temp
		fclose(fr);//close read

		i = 0;
		fprintf(fw, "%s \n", "P2");
		fprintf(fw, "%d  ", col);
		fprintf(fw, "%d \n", row);
		fprintf(fw, "%d \n", ch);

		while ((index < MaxIndex) && (i < col*row))//not finish read and write
		{
			if ((((i + 1)*power) % 8) >= power)//not need to change the character read
			{
				mask = 0x0;
				mask |= (tmpNumbers[index] << ((i*power) % 8));
				PgmNumbers[i] |= (mask >> (8 - power));//save the character
				i++;//new character
			}
			else//need
			{
				mask = 0x0;//reinitialise
				mask |= (tmpNumbers[index] << ((i*power) % 8));
				PgmNumbers[i] |= (mask >> (8 - power));//save the character
				index++;//need

				mask = 0x0;//reinitialise
				mask |= ((tmpNumbers[index]) >> (8 - ((i + 1)*power) % 8));
				PgmNumbers[i] |= mask;//save the character
				i++;//new character
			}
		}
		for (i = 0; i < col*row; i++)
		{
			if ((i + 1) % col != 0)
				fprintf(fw, "%d \t", ((PgmNumbers[i] * MAXGRAY) / (int)ch));
			else
				fprintf(fw, "%d\n", ((PgmNumbers[i] * MAXGRAY) / (int)ch));
		}
		free(PgmNumbers);//free write
		free(tmpNumbers);//free read
		fclose(fw);
	}
}
void charFree(unsigned char **ch, unsigned int x)//free character
{
	int i;
	for (i = 0; i <(int)x; i++)
		free(*(ch + i));

	free(ch);
}
void FreeTreeOfSegmentsList(treeNode *node)//free seg
{
	treeNodeListCell *p, *deleted;
	if (node != NULL)
	{
		p = node->next_possible_positions;
		while (p != NULL)
		{
			deleted = p;
			p = p->next;
			FreeTreeOfSegmentsList(deleted->node);
			free(deleted);
		}
	}
	free(node);
}
void FreeTreeOfSegments(Segment tr)//free seg
{

	FreeTreeOfSegmentsList(tr.root);

}
void freeListCell(imgPosCell*cell)
{
	if (cell->next != NULL)
	{
		freeListCell(cell->next);
	}
	free(cell);
}

