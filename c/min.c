/* This is the sequential min function for ASUH */

#include "fileio.h"
int main(int argc, char **argv)
{
	int i,j;
	int r,c;
	int **minArray = createArray(r, c);


	freeArray(r, c, minArray);
	return 0;
}
