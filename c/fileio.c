/* This file contains the functions for writing and reading files */
#include "fileio.h"

int readFile(int columns, int rows, int **data, char *filename)
{
	FILE *inputfile;
	if((inputfile = fopen(filename, "r")) == NULL)
	{
		perror("fileio.h read");
		fprintf(stderr, "File does not exist");
		exit(1);
	}

	int i, j, hold;

	for(j = 0; j < rows; j++)
	for(i = 0; i < columns; i++)
	{
		hold = getc(inputfile);
		if(hold == '\n' || hold == ',')
		{
			hold = getc(inputfile);
			if(hold == '-')
			{
				hold = getc(inputfile);
				data[j][i] = -1 * (hold - 48);
			}
			else
				data[j][i] = hold - 48;
		}
		else if(hold == '-')
		{
			hold = getc(inputfile);
			data[j][i] = -1 * (hold - 48);
		}
		else
			data[j][i] = hold - 48;
	}

	fclose(inputfile);
	return 0;
}

int writeFile(int columns, int rows, int **data, char *filename)
{
	FILE *outputfile;
	outputfile = fopen(filename, "w");
/*	if((outputfile = fopen(filename, "w")) != NULL)
	{
		perror("fileio.h write");
		fprintf(stderr,"File already exists");
		return -1;
	}
*/
	int i, j;

	for(j = 0; j < rows; j++)
	for(i = 0; i < columns; i++)
	{
		fprintf(outputfile, "%d", data[j][i]);
		if((i+1) != columns)
			fprintf(outputfile,",");
		else
			fprintf(outputfile, "\n");
	}
	fclose(outputfile);
	return 0;
}

int **createArray(int columns, int rows)
{
	int **newArray = (int**)malloc(sizeof(int *) * rows);
	int i;
	for(i = 0; i < rows; i++)
		newArray[i]  = (int *)malloc(sizeof(int) * columns);
		
	return newArray;
}

void freeArray(int columns, int rows, int **anArray)
{
	int i;
	for(i = 0; i < rows; i++)
		free(anArray[i]);
	free(anArray);
}

