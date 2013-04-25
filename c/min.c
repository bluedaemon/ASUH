/*
	This is the CPU min function for ASUH
	This function takes the number of "vectors" and number of "places" and two file names as arguments
	example: ./max 10 10 input.csv output.csv
*/
#include "fileio.h"
#include <pthread.h>

typedef struct threadinfo
{
	int id, vector, place, **grid;
} threadinfo;

void *threadwork(void *);

int answer;
pthread_mutex_t themutex;

int main(int argc, char **argv)
{
	int vector = atoi(argv[1]);
	int place = atoi(argv[2]);
	int **grid = createArray(vector, place);
	readFile(vector, place, grid, argv[3]);
	answer = grid[0][0];
	int j;
	pthread_t *threads = (pthread_t*)malloc(vector * sizeof(pthread_t));
	threadinfo *info = (threadinfo*)malloc(vector * sizeof(struct threadinfo));
	for(j=0;j<vector;j++)
	{
		info[j].id = j;
		info[j].vector = vector;
		info[j].place = place;
		info[j].grid = grid;
		pthread_create(&threads[j], NULL, threadwork, (void*)&info[j]);
	}

	for(j=0;j<vector;j++)
		pthread_join(threads[j], 0);
	free(info);
	free(threads);
	grid[0][0] = answer;
	writeFile(1, 1, grid, argv[4]);
	freeArray(vector, place, grid);
	return 0;
}

void *threadwork(void *param)
{
	threadinfo *info = (threadinfo*)param;
	int j = info->id;
	int **grid = info->grid;
	int i, min = grid[j][0];
	for(i=0;i<info->place;i++)
	{
		if(min > grid[j][i])
			min = grid[j][i];
	}
	pthread_mutex_lock(&themutex);
	if(answer > min)
		answer = min;
	pthread_mutex_unlock(&themutex);

}
