/* This is the sequential max function for ASUH */

#include "fileio.h"
#include <pthread.h>

//global variables
int min;
int **externaldata;
pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER; //semaphore
int event=0;
typedef struct{
	int threadid;
	int r;
	int c;
	int** data;
} param;

void * worker_min(void*);
int cpumin(int,int,char*);

int cpumin(int rows, int columns, char* filename)
{
    pthread_t threads[rows];
    int rc;    
    long t=0;

    //Read array
    externaldata=createArray(columns,rows);
    readFile(columns,rows,externaldata,"ex");
    min=externaldata[0][0];
	param parameters[rows];


    //creates threads and executes summation function
    for(t=0;t<rows;t++){
		parameters[t].threadid=t;
		parameters[t].r=rows;
		parameters[t].c=columns;
		parameters[t].data=externaldata;
        //printf("Main-creating thread %li to sum array.\n", parameters.threadid);
        rc = pthread_create(&threads[t], NULL, worker_min, (void*)&parameters[t]);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    //Event to print out the final sum only after all threads have finished
    while(1){
        if(event==rows){           
			return min;
        }
    }
}

//Min function
void *worker_min(void * parameters)
{
    
    int i;
    param *tempparam;
    tempparam=( param*) parameters;
    
    //printf("thread #%li starting\n", tempparam->threadid);
	int pmin=tempparam->data[tempparam->threadid][0];
    for(i=0;i<tempparam->c;i++){
	if(tempparam->data[tempparam->threadid][i]<pmin)
        	pmin=tempparam->data[tempparam->threadid][i];
    }

    //semaphore while accessing sum variable
    pthread_mutex_lock(&mymutex);
    min=min>pmin ? pmin : min;
    event++;
    pthread_mutex_unlock(&mymutex);

    //printf("Sum thread #%li exiting\n", tid);

    pthread_exit(0);
}

int main(){
	int a=cpumin(4,7,"ex");
	printf("min=%d\n",a);
	int **ans=createArray(1,1);
	ans[0][0]=a;
	writeFile(1,1,ans,"min_ans");
	pthread_exit(NULL); 
	freeArray(1,1,ans);
	return 0;
}
