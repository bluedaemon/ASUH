/*
	This is the mandelbrot function
	It takes the number of "vectors" and number of "places" and one file name as arguments
	The resulting output file will contain a mandelbrot fractal
	example: ./mandelbrot 100 100 out.csv
*/
#include "fileio.h"
#include <pthread.h>
//pthread_mutex_t themutex;
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
	threadinfo *info = (threadinfo*)malloc(vector * sizeof(struct threadinfo));
	int **grid = createArray(vector,place);
	pthread_t *ids = (pthread_t*)malloc(vector * sizeof(pthread_t));
	writeFile(vector,place,grid,"ex");
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
	free(info);
	free(ids);
	writeFile(vector, place, grid, argv[3]);
	freeArray(vector, place, grid);
	return 0;
}

void *threadwork(void *params)
{
	threadinfo *info = (threadinfo*)params;
	int i, j, k;
	float length, temp;
	comp z, c;
	int ydim = info->vector;
	int xdim = info->place;
	j = info->id;
	int **grid = info->grid;
	for(i=0;i<info->place;i++)
	{
		z.real = z.imag = 0;
		c.real = ((float)j - (ydim/2))/(ydim/4);
		c.imag = ((float)i - (xdim/2))/(xdim/4);
		k = 0;
		do {
			temp = z.real*z.real - z.imag*z.imag + c.real;
			z.imag = 2.0 * z.real * z.imag + c.imag;
			z.real = temp;
			length = z.real * z.real + z.imag * z.imag;
			k++;
		} while(length < 4.0 && k < 100);
		grid[j][i] = (k == 100);
	}
}
