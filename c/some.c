/*this file was used to build and test the fileio functions */

#include "fileio.h"

int main()
{
	int **data;
//	int reals[2][2];
//	reals[0][0] = 1;
//	reals[0][1] = 2;
//	reals[1][0] = 3;
//	reals[1][1] = 4;
//	printf("\nFirst stuff worked\n");
//	data = reals;
	data = (int **)malloc(sizeof(int*) * 2);
	*data = (int *)malloc(sizeof(int) * 2);
	data[0][0] = 5.5;
	data[0][1] = 4.4;

//	(data + 1) = (double **)malloc(sizeof(double*));
	data[1] = (int *)malloc(sizeof(int) * 2);
	data[1][0] = 6.6;
	data[1][1] = 7.7;

	printf("\nAssigned data a number\n");
//	printf("\n%d %d %d\n", **data, *data, data);

//	printf("\n%d %d\n", data[0][0], data[1][0]);

//	printf("\n%d %d %d %d\n", data[0][0], data[0][1], data[1][0], data[1][1]);

	writeFile(2, 2, data, "ex");
	readFile(2, 2, data, "ex");

	printf("\n%d %d\n%d %d\n", data[0][0], data[0][1], data[1][0], data[1][1]);
	
//	writeFile(2, 2, data, "output");

	free(*data);
	free(*(data+1));
	free(data);

	int r = 10;
	int c = 15;

	int **thisarray = createArray(c, r);
	int i,j;
	for(j=0;j<c;j++)
	for(i=0;i<r;i++)
		thisarray[j][i] = j;

	writeFile(c,r,thisarray,"output");
	freeArray(c,r,thisarray);

	return 0;
}
