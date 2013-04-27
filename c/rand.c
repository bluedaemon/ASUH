#include "fileio.h"

int main(int argc, char **argv)
{
	if(argc < 4)
	{
		perror("rand.c: not enough arguments");
		exit(1);
	}
	int vector = atoi(argv[1]);
	int place = atoi(argv[2]);
	int **randArray = createArray(vector, place);

	int i,j;
	for(j=0;j < vector; j++)
	for(i=0;i < place; i++)
		randArray[j][i] = random() / (RAND_MAX / 1000);
	writeFile(vector, place, randArray, argv[3]);
	freeArray(vector, place, randArray);
	return 0;
}
