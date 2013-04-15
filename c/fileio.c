/* This file contains the functions for writing and reading files */
#include "fileio.h"

int readFile(int vector, int place, int **data, char *filename)
{
	FILE *inputfile;
	if((inputfile = fopen(filename, "r")) == NULL)
	{
		perror("fileio.h read");
		fprintf(stderr, "File does not exist");
		exit(1);
	}

	int i, j, hold;

	for(j = 0; j < vector; j++)
	for(i = 0; i < place; i++)
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

int writeFile(int vector, int place, int **data, char *filename)
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

	for(j = 0; j < vector; j++)
	for(i = 0; i < place; i++)
	{
		fprintf(outputfile, "%d", data[j][i]);
		if((i+1) != place)
			fprintf(outputfile,",");
		else
			fprintf(outputfile, "\n");
	}
	fclose(outputfile);
	return 0;
}

int **createArray(int vector, int place)
{
	int **newArray = (int**)malloc(sizeof(int *) * vector);
	int i;
	for(i = 0; i < vector; i++)
		newArray[i]  = (int *)malloc(sizeof(int) * place);
		
	return newArray;
}

void freeArray(int vector, int place, int **anArray)
{
	int i;
	for(i = 0; i < vector; i++)
		free(anArray[i]);
	free(anArray);
}

