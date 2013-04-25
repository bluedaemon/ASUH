/*
	This function takes the number of "vectors" and number of "places" and one file name as arguments
	It produces a mandelbrot fractal in the resulting CSV file
	example execution: ./mand 100 100 output.csv
*/
#include "fileio.h"
#include <pthread.h>
void *worker(void*);

typedef struct comp
{
	float real, imag;
} comp;

int main(int argc, char **argv)
{
	int xdim, ydim;
	ydim = atoi(argv[1]);
	xdim = atoi(argv[2]);
	int i, j, k;
	float lengths, temp;
	comp z, c;
	int **grid = createArray(ydim, xdim);
	for(j=0;j<ydim;j++)
	{
		for(i=0;i<xdim;i++)
		{
			z.real = z.imag = 0;
			c.real = ((float)j - (ydim/2))/(ydim/4);
			c.imag = ((float)i - (xdim/2))/(xdim/4);
			k = 0;
			do {
				temp = z.real*z.real - z.imag * z.imag + c.real;
				z.imag = 2.0 * z.real * z.imag + c.imag;
				z.real = temp;
				lengths = z.real * z.real + z.imag * z.imag;
				k++;
			} while(lengths < 4.0 && k < 100);
			grid[j][i] = (k == 100);
		}
	}
	writeFile(ydim, xdim, grid, argv[3]);
	freeArray(ydim, xdim, grid);
	return 0;
}
