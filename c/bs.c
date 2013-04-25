/*
	This is the mandelbrot function
	It takes the number of "vectors" and number of "places" and one file name as arguments
	The resulting output file will contain a mandelbrot fractal
	example: ./mandelbrot 100 100 out.csv
*/
#include "fileio.h"
#include <pthread.h>

typedef struct comp
{
	float real, imag;
}comp;
typedef struct threadinfo
{
	int id, vector, place, **grid;
}threadinfo;

void *threadwork(void*);

int main(int argc, char **argv)
{
	int vector = atoi(argv[1]);
	int place = atoi(argv[2]);
	int i,j,check;
	threadinfo info[vector];
	int **grid = createArray(vector,place);
	pthread_t ids[vector];
	for(j=0;j<vector;j++)
	{
		info[j].id = j;
		info[j].vector = vector;
		info[j].place = place;
		info[j].grid = grid;
		check = pthread_create(&ids[j], NULL, threadwork, (void*)&info[j]);
		if(check)
			printf("\npthread problems %i\n", j);
	}
	for(j=0;j<vector;j++)
		pthread_join(ids[j],NULL);
	writeFile(vector, place, grid, argv[3]);
	freeArray(vector, place, grid);
	return 0;
}

void *threadwork(void *params)
{
	int i, j, k;
	float length, temp;
	comp z, c;
	threadinfo *info;
	info = (threadinfo*)params;
	j = info->id;
	int **tgrid = info->grid;
	for(i=0;i<info->place;i++)
	{
		z.real = z.imag = 0;
		c.real = ((float)j - (info->vector/2))/(info->vector/4);
		c.imag = ((float)i - (info->place/2))/(info->place/4);
		k = 0;
		do {
			temp = z.real * z.real - z.imag * z.imag + c.real;
			z.imag = 2.0 * z.real * z.real + c.imag;
			z.real = temp;
			length = z.real * z.real + z.imag * z.imag;
			k++;
		} while(length < 4.0 && k < 100);
		tgrid[j][i] = (k == 100);
//		pthread_exit(0);
	}
//	printf("\nid:%i, vec:%i pl:%i", j,info->vector,info->place);
}
