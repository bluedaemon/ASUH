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
	unsigned int s;
	for (s=(blockDim.x%2==0 ? blockDim.x>>1 : (blockDim.x>>1)+1); s>0; s=(s%2==0 ? s>>1 : (s>>1)+1)) {
		if(tid < s){
			sdata[tid]= sdata[tid]>sdata[tid+s] ? sdata[tid]: sdata[tid+s];
		}
	__syncthreads();
	if(s==1)break;
	}
	if (tid == 0) answerD[blockIdx.x] = sdata[0];
}
}

int main(int argc, char** argv){
	cudaError_t cudaStatus;
	const unsigned int vector=atoi(argv[1]);
	const unsigned int place=atoi(argv[2]);
	const unsigned int sz=vector*place*sizeof(int);
	int *answerD,*answerH,*arrayD,*inputarray,i;
	int **answer=createArray(1,1);
	const int blocka=place*vector%512==0 ? place*vector/512 : (place*vector/512)+1;

	inputarray=(int*)malloc(sz);
	if(inputarray==NULL){
		printf("failed malloc\n");
		exit(-1);
	}

	answerH=(int*)malloc(sizeof(int)*blocka);
	if(answerH==NULL){
		printf("failed malloc\n");
		exit(-1);
	}

	readFileCUDA(vector,place,inputarray,argv[3]);

	cudaStatus=cudaDeviceSetLimit(cudaLimitMallocHeapSize,1073741824);
	if (cudaStatus != cudaSuccess){
		fprintf(stderr, "cudaDeviceSetLimit failed!,%s\n",cudaGetErrorString(cudaStatus));
		exit(-1);
	}

	cudaStatus=cudaMalloc((void **)&arrayD,sz);
	if (cudaStatus != cudaSuccess){
		fprintf(stderr, "cudaMalloc failed!,%s\n",cudaGetErrorString(cudaStatus));
		exit(-1);
 	}

	cudaStatus=cudaMalloc(&answerD,sizeof(int)*blocka);
	if (cudaStatus != cudaSuccess){
		fprintf(stderr, "cudaMalloc failed!,%s\n",cudaGetErrorString(cudaStatus));
		exit(-1);
	}

	cudaStatus=cudaMemcpy(arrayD,inputarray,sz,cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess){
		fprintf(stderr, "cudaMemcpy failed!,%s\n",cudaGetErrorString(cudaStatus));
		exit(-1);
	}

	dim3 blockDim(512);
	dim3 gridDim(blocka);

	cmax<<<gridDim,blockDim,512*sizeof(int)>>>(arrayD,answerD,vector*place);

	if(cudaSuccess != (cudaStatus=cudaGetLastError())){
		printf("Kernel error! %s\n" ,cudaGetErrorString(cudaStatus));
		exit(-1);
	}


  	cudaStatus=cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "devicesync failed!,%s\n",cudaGetErrorString(cudaStatus));
		exit(-1);
	}
	
	cudaStatus=cudaMemcpy(answerH,answerD,sizeof(int)*blocka,cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "memcpy failed!,%s\n",cudaGetErrorString(cudaStatus));
		exit(-1);
	}

	answer[0][0]=answerH[0];
	if(blocka!=1){
		for(i=1;i<(blocka);i++){
			answer[0][0]=(answerH[i]>answer[0][0] ? answerH[i] : answer[0][0]);
		}
	}
	writeFile(1,1,answer,argv[4]);
	freeArray(1,1,answer);
	free(answerH);
	free(inputarray);
	cudaFree(arrayD);
	return 0;
}
