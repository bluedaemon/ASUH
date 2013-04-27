/* This file contains the definitions of the functions for reading and writing input and output files */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


/*
	These functions pass double pointers, we've chosen to name a contiguous block of memory a "vector"
	"vector" is analagous to a row in the array given
	"place" refers to the location of a value in a given "vector" or row
	Values in an "array" are referenced via arrayName[vector #][place #]
*/

/* Reads information from a CSV file into an "array" of size vector * place */
/* (vector, place, array, filename) */
int readFile(int, int, int **, char *);

/* Writes the information in an "array" to a CSV file */
/* (vector, place, array, filename) */
int writeFile(int, int, int **, char *);

/* Creates and returns a double pointer */
/* (vector, place) */
int **createArray(int, int);

/* Frees a double array created by createArray() */
/* (vector, place) */
void freeArray(int, int, int **);

int readFileCUDA(int vector, int place, int *data, char *filename);
