/* This file contains the definitions of the functions for reading and writing input and output files */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int readFile(int, int, double **, char *);
int writeFile(int, int, double **, char *);
double **createArray(int, int);
void freeArray(int, int, double**);
