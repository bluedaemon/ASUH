/* This is the sequential min function for ASUH */
/*
	This function takes the number of "vectors" and number of "places" and a filename
	example : ./min 10 10 input.csv output.csv
*/
#include "fileio.h"
int main(int argc, char **argv)
{
	int i, j;
	int vector, place;
	vector = atoi(argv[1]);
	place = atoi(argv[2]);
	int **minArray = createArray(vector, place);
	readFile(vector, place, minArray, argv[3]);
	int min = minArray[0][0];
	for(j=0;j < vector; j++)
	for(i=0; i < place; i++)
	{
		printf("%i ", minArray[j][i]);
		if(minArray[j][i] < min)
			min = minArray[j][i];
	}
	int **answer;
	answer = (int **)malloc(sizeof(int *));
	*answer = (int*)malloc(sizeof(int));
	**answer = min;
	writeFile(1, 1, answer, argv[4]);
	free(*answer);
	free(answer);
	freeArray(vector, place, minArray);
	return 0;
}
