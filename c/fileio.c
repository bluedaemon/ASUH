/* This file contains the functions for writing and reading files */
#include "fileio.h"

int readfile(int columns, int rows, void **data, char *filename)
{
	FILE *inputfile;
	if((inputfile = fopen(filename, "r")) == NULL)
	{
		perror("fileio.h read");
		fprintf(stderr, "File does not exist");
		exit(1);
	}

	int i, j;

	for(j = 0; j < rows; j++)
	for(i = 0; i < columns; i++)
	{
		fscanf(inputfile, "%d,", data[i][j]);
	}

	fclose(inputfile);
}

int writefile(int columns, int rows, void **data, char *filename)
{
	FILE *outputfile;
	if((outputfile = fopen(filename, "w")) != NULL)
	{
		perror("fileio.h write");
		fprintf(stderr,"File already exists");
		exit(1);
	}

	int i, j;

	for(j = 0; j < rows; j++)
	for(i = 0; i < columns; i++)
	{
		fprintf(outputfile, "%d", <long>data[i][j]);
		if((i+1) != columns)
			fprintf(outputfile,",");
		else
			fprintf(outputfile, "\n");
	}
	fclose(outputfile);
}
