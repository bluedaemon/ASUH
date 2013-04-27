/* This file contains the functions for writing and reading files */
#include "fileio.h"

int readFile(int vector, int place, int **data, char *filename)
{
	FILE *inputfile;
	char *temp=(char*)malloc(sizeof(char)*4294967295);
	if((inputfile = fopen(filename, "r")) == NULL)
	{
		perror("fileio.h read");
		fprintf(stderr, "File does not exist");
		exit(1);
	}

	int i, j, hold,index=0;

	for(j = 0; j < vector; j++)
	for(i = 0; i < place; i++)
	{
		hold = getc(inputfile);
		if(hold == '\n' || hold == ',')
		{
			hold = getc(inputfile);
			if(hold == '-')
			{
				index=0;
				temp[index] = getc(inputfile);
				while(temp[index] != '\n' && temp[index] != ','){
					index++;
					temp[index] = getc(inputfile);
				}
				temp[index]='\0';
				data[j][i] = atoi(temp);
			}
			else{
				index=0;
				temp[index]=hold;
				while(temp[index] != '\n' && temp[index] != ','){
					index++;
					temp[index] = getc(inputfile);					
				}
				temp[index]='\0';
				data[j][i] = atoi(temp);
			}
		}
		else if(hold == '-')
		{
			index=0;
			temp[index] = hold;
			while(temp[index] != '\n' && temp[index] != ','){
				index++;
				temp[index] = getc(inputfile);
			}
			temp[index]='\0';
			data[j][i] = atoi(temp);
			}
		else{
			index=0;
			temp[index]=hold;
			while(temp[index] != '\n' && temp[index] != ','){
				index++;
				temp[index] = getc(inputfile);
				
			}
			temp[index]='\0';
			data[j][i] = atoi(temp);
		}
	}

	fclose(inputfile);
	return 0;
}

int readFileCUDA(int vector, int place, int *data, char *filename)
{
	FILE *inputfile;
	char *temp=(char*)malloc(sizeof(char)*4294967295);
	if((inputfile = fopen(filename, "r")) == NULL)
	{
		perror("fileio.h read");
		fprintf(stderr, "File does not exist");
		exit(1);
	}

	int i, j, hold,index=0;

	for(j = 0; j < vector; j++)
	for(i = 0; i < place; i++)
	{
		hold = getc(inputfile);
		if(hold == '\n' || hold == ',')
		{
			hold = getc(inputfile);
			if(hold == '-')
			{
				index=0;
				temp[index] = getc(inputfile);
				while(temp[index] != '\n' && temp[index] != ','){
					index++;
					temp[index] = getc(inputfile);
				}
				temp[index]='\0';
				data[j*place+i] = atoi(temp);
			}
			else{
				index=0;
				temp[index]=hold;
				while(temp[index] != '\n' && temp[index] != ','){
					index++;
					temp[index] = getc(inputfile);					
				}
				temp[index]='\0';
				data[j*place+i] = atoi(temp);
			}
		}
		else if(hold == '-')
		{
			index=0;
			temp[index] = hold;
			while(temp[index] != '\n' && temp[index] != ','){
				index++;
				temp[index] = getc(inputfile);
			}
			temp[index]='\0';
			data[j*place+i] = atoi(temp);
			}
		else{
			index=0;
			temp[index]=hold;
			while(temp[index] != '\n' && temp[index] != ','){
				index++;
				temp[index] = getc(inputfile);
				
			}
			temp[index]='\0';
			data[j*place+i] = atoi(temp);
		}
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

