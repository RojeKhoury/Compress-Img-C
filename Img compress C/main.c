#include "projectHeader.h"
void main()
{
	grayImage *imagePGM = NULL, *imageWithTheSameColor = NULL;
	imgPosList *segments = NULL;
	int choice = 0, checkTheChoice;
	int numOfSegments = -1;
	int treshold;
	char name[MAX];
	char nameNew[MAX];
	char nameF[MAX];
	printf("Hello user!\n");
	printf("enter the number section you would like to choose. \n");
	printf("1. Read an image file in PGM format.\n");
	printf("2. Find all segments.\n");
	printf("3. Color the segments. \n");
	printf("4. Save the colored image in a compressed format. \n");
	printf("5. Compress and save the original image in in a compressed.\n");
	printf("6. Convert a compressed image to PGM format.\n");
	printf("7. Press 7 for exit the programm.\n");
	printf("\n\n You don't need to write .pgm or.bin\n\n");
	checkTheChoice = 0;
	while (choice != 7)
	{
		printf("Please choose one of the options: \n");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
		{
			checkTheChoice = choice;
			printf("You chose \n1. Read an image file in PGM format. \n ");
			printf("Enter the name of the file -> ");
			scanf("%s", name);
			strcat(name, ".pgm");
			if (imagePGM != NULL)
			{
				charFree(imagePGM->pixels, imagePGM->rows);
				free(imagePGM);
			}
			imagePGM = readPGM(name);
			break;
		}
		case 2:
		{
			if (checkTheChoice + 1 != choice)
				printf("First you need to enter the file name .\n");
			else
			{
				checkTheChoice = choice;
				printf("You chose \n2. Find all segments. \n\n");
				if (segments != NULL)
				{
					freeListCell(segments->head);
					free(segments);
				}
				printf("Please enter the threshold for the segemnts:\n");
				scanf("%d", &treshold);
				numOfSegments = findAllSegements(imagePGM, (unsigned char)treshold, &segments);
			}
			break;
		}
		case 3:
		{
			if (checkTheChoice + 1 != choice)
				printf("First you need to chose 2 .\n");
			else
			{
				printf("You chose \n 3. Color the segments. \n");
				if (imageWithTheSameColor != NULL)
				{
					charFree(imageWithTheSameColor->pixels, imageWithTheSameColor->rows);
					free(imageWithTheSameColor);
				}

				imageWithTheSameColor = colorSegments(segments, numOfSegments);
				checkTheChoice = choice;
			}
			break;
		}
		case 4:
		{
			if (checkTheChoice + 1 != choice)
				printf("First you need to chose 3.\n");
			else
			{
				printf("You chose \n4. Save the colored image in a compressed format. \n");
				printf("Please enter a binary file name to save the compressed colored image:\n");
				scanf("%s", name);
				strcat(name, ".bin");
				printf("Please enter the max gray level (0-127):\n");
				scanf("%d", &treshold);
				saveCompressed(name, imageWithTheSameColor,(unsigned char)treshold);
				checkTheChoice = choice;
			}
			break;
		}
		case 5:
		{

			printf("You chose \n5. Compress and save the original image in in a compressed. \n");
			printf("Please enter a PGM file name to compress: ");
			scanf("%s", nameNew);
			strcat(nameNew, ".pgm");

			if (imagePGM != NULL)
			{
				charFree(imagePGM->pixels, imagePGM->rows);
				free(imagePGM);
			}

			imagePGM = readPGM(nameNew);
			if (imagePGM != NULL)
			{
				printf("Please enter the max gray level (0-127) ");
				scanf("%d", &treshold);
				printf("Please enter a binary file name to save the compressed image:\n");
				scanf("%s", nameF);
				strcat(nameF, ".bin");
				saveCompressed(nameF, imagePGM, treshold);
			}
		

		


			break;
		}
		case 6:
		{
			printf("You chose \n6.Convert a compressed image to PGM format. \n");
			printf("Please enter a binary file name that contains a compressed image:\n ");
			scanf("%s", nameF);
			strcat(nameF, ".bin");
			printf("Please enter a PGM file name to save the compressed image:\n ");
			scanf("%s", nameNew);
			strcat(nameNew, ".pgm");
			convertCompressedImageToPGM(nameF, nameNew);

			break;
		}
		case 7:
		{
			if (imagePGM != NULL)
			{
				charFree(imagePGM->pixels, imagePGM->rows);
				free(imagePGM);
			}
			if (imageWithTheSameColor != NULL)
			{
				charFree(imageWithTheSameColor->pixels, imageWithTheSameColor->rows);
				free(imageWithTheSameColor);
			}
			if (segments != NULL)
			{
				freeListCell(segments->head);
				free(segments);
			}
			printf("Bye Bye \n");
			exit(0);
			break;
		}
		default:
		{
			printf("Wrong number! \n please enter again ! \n");
		}
		break;
		}
	}
}
