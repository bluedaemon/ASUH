/*
	This is the tile function
	It takes the number of "vectors", "places", tile height, tile width, input file, and output file as arguments
	example: ./tile 1000 1000 10 10 in.csv out.csv
*/

#include "fileio.h"
#include <pthread.h>
typedef struct threadinfo
{
	int id, vector, place, tileheight, tilewidth, vtiles, ptiles, **grid, count;
} threadinfo;

void *threadwork(void*);

int main(int argc, char **argv)
{
	int vector = atoi(argv[1]);
	int place = atoi(argv[2]);
	int tileheight = atoi(argv[3]);
	int tilewidth = atoi(argv[4]);
	int vtiles = vector/tileheight;
	int ptiles = place/tilewidth;
	if(vector%tileheight > 0)
		vtiles++;
	if(place%tilewidth > 0)
		ptiles++;
	int numtiles = vtiles * ptiles;
	int **grid = createArray(vector, place);
	readFile(vector, place, grid, argv[5]);
	int j,check;
	pthread_t *ids = (pthread_t*)malloc(numtiles * sizeof(pthread_t));
	threadinfo *info = (threadinfo*)malloc(numtiles * sizeof(threadinfo));
/*
	for(j=0;j<vector;j++)
	{
	for(check=0;check<place;check++)
	{
		printf("%i ", grid[j][check]);
		if(check%tilewidth == tilewidth-1)
			printf(" ");
	}
	if(j%tileheight == tileheight-1)
		printf("\n");
	printf("\n");
	}
*/
	for(j=0;j<numtiles;j++)
	{
		info[j].id = j;
		info[j].vector = vector;
		info[j].place = place;
		info[j].tileheight = tileheight;
		info[j].tilewidth = tilewidth;
		info[j].vtiles = vtiles;
		info[j].ptiles = ptiles;
		info[j].grid = grid;
		check = pthread_create(&ids[j], NULL, threadwork, (void*)&info[j]);
		if(check)
			printf("\npthread problems %i\n", j);
	}
	for(j=0;j<numtiles;j++)
		pthread_join(ids[j], NULL);

	int **percents = createArray(numtiles, 3);
	for(j=0;j<numtiles;j++)
	{
		percents[j][0] = j;
		percents[j][1] = (tileheight * tilewidth) - info[j].count;
		percents[j][2] = info[j].count;
	}
	writeFile(numtiles, 3, percents, argv[6]);

	freeArray(numtiles, 3, percents);
	free(info);
	free(ids);
	freeArray(vector, place, grid);
	return 0;
}

void *threadwork(void *params)
{
	threadinfo *info = (threadinfo*)params;
	int id, vector, place, tileheight, tilewidth, vtiles, ptiles, **grid;
	id = info->id;
	vector = info->vector;
	place = info->place;
	tileheight = info->tileheight;
	tilewidth = info->tilewidth;
	vtiles = info->vtiles;
	ptiles = info->ptiles;
	grid = info->grid;

	int i, j, count = 0;
	for(j = (id/ptiles) * tileheight; (j < (((id + ptiles)/ptiles) * tileheight)) && (j < vector); j++)
	for(i = (id%ptiles) * tilewidth; (i < (((id + 1)%ptiles) * tilewidth)) && (i < place); i++)
	{
		if(grid[j][i] == 1)
			count++;
	}
	info->count = count;
}
