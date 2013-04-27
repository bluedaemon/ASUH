/*
This function takes the number of "vectors" and number of "places" and a two file names as arguments
example : ./max 10 10 input.csv output.cs
*/

#include <cuda.h>
#include "fileio.h"

__global__ void cmax(int *arrayD,int* answerD,int size) {
unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
if(i<size){

unsigned int tid = threadIdx.x;
extern __shared__ int sdata[];

sdata[tid] = arrayD[i];
__syncthreads();

for (unsigned int s=(size%2==0 ? size>>1 : size>>1+1); s>0; s=(s%2==0 ? s>>1 : (s>>1)+1)) {
	if(tid < s){printf("%d>%d\n",sdata[tid],sdata[tid+s]);
		sdata[tid]= sdata[tid]>sdata[tid+s] ? sdata[tid]: sdata[tid+s];
	}
__syncthreads();
if(s==1)break;
}

if (tid == 0) answerD[blockIdx.x] = sdata[0];
}
}

int main(int argc, char** argv){
	const int vector=atoi(argv[1]);
	const int place=atoi(argv[2]);
	const int sz=vector*place*sizeof(int);
	int *answerD,*answerH,*arrayD,*inputarray,i;
	int **answer=createArray(1,1);
	const int blocka=place*vector%256==0 ? place*vector/256 : (place*vector/256)+1;

	inputarray=(int*)malloc(sz);
	answerD=(int*)malloc(sizeof(int)*blocka);
	answerH=(int*)malloc(sizeof(int)*blocka);
	readFileCUDA(vector,place,inputarray,argv[3]);

	cudaMalloc((void **)&arrayD,sz);
	cudaMalloc(&answerD,sizeof(int)*blocka);
	cudaMemcpy(arrayD,inputarray,sz,cudaMemcpyHostToDevice);

	dim3 blockDim(256);
	dim3 gridDim(blocka);

	cmax<<<gridDim,blockDim,sz>>>(arrayD,answerD,vector*place);
	
	cudaDeviceSynchronize();
	cudaMemcpy(answerH,answerD,sizeof(int)*blocka,cudaMemcpyDeviceToHost);

	if(blocka==1){
		answer[0][0]=answerH[0];
	}

	else{
	for(i=0;i<blocka-1;i++){
		answer[0][0]=(answerH[i]>answerH[i+1] ? answerH[i] : answerH[i+1]);
	}
	}
	writeFile(1,1,answer,argv[4]);
	free(*answer);
	free(answer);
	free(inputarray);
	cudaFree(arrayD);
	return 0;
}
