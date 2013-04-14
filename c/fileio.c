/* This file contains the functions for writing and reading files */
#include "fileio.h"

int readfile(int columns, int rows, double **data, char *filename)
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
			if(hold == '-');
			{
				hold = getc(inputfile);
				data[i][j] = -1 * (hold - 48);
			}
		}
		else if(hold == '-')
		{
			hold = getc(inputfile);
			data[i][j] = -1 * (hold - 48);
		}
		else
			data[i][j] = hold - 48;
	}

	fclose(inputfile);
	return 0;
}

int writefile(int columns, int rows, double **data, char *filename)
{
	FILE *outputfile;
	if((outputfile = fopen(filename, "w")) != NULL)
	{
		perror("fileio.h write");
		fprintf(stderr,"File already exists");
		return -1;
	}

	int i, j;

	for(j = 0; j < rows; j++)
	for(i = 0; i < columns; i++)
	{
		fprintf(outputfile, "%f", data[i][j]);
		if((i+1) != columns)
			fprintf(outputfile,",");
		else
			fprintf(outputfile, "\n");
	}
	fclose(outputfile);
	return 0;
}
