/* This is the sequential max function for ASUH */

/*
	This function is takes the number of "vectors" and number of "places" and two file names as arguments
	example: ./max 10 10 input.csv output.csv
*/

#include "fileio.h"
int main(int argc, char **argv)
{
	int i, j;
	int vector, place;
	vector = atoi(argv[1]);
	place = atoi(argv[2]);
	int **maxArray = createArray(vector, place);
	readFile(vector, place, maxArray, argv[3]);
	int max = maxArray[0][0];
	for(j = 0; j < vector; j++)
	for(i = 0; i < place; i++)
		if(maxArray[j][i] > max)
			max = maxArray[j][i];
	int **answer;
	answer = (int**)malloc(sizeof(int*));
	*answer =(int*)malloc(sizeof(int));
	**answer = max;
	writeFile(1, 1, answer, argv[4]);
	free(*answer);
	free(answer);
	freeArray(vector, place, maxArray);
	return 0;
}
