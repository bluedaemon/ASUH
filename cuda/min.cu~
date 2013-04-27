#include <stdio.h>
#include <stdlib.h>
#include "cuda.h"
#include "wrap.h"

__global__ void cmax(int *arrayD,int* answerD) {
__shared__ int sdata[20];
unsigned int tid = threadIdx.x;
unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
sdata[tid] = arrayD[i];
__syncthreads();

for (unsigned int s=(blockDim.x/2)+1; s>0; s>>=1) {
if(tid < s) {
sdata[tid]= sdata[tid]<sdata[tid+s] ? sdata[tid]: sdata[tid+s];
}
__syncthreads();
}

if (tid == 0) *answerD = sdata[0];
}
extern "C"
void CUDAmax(int* inputarray, int *answer,int szofinputarray){
	const int sz=szofinputarray*sizeof(int);
	int *answerD,*arrayD;
	int blockdim=20,griddim=5;

	cudaMalloc((void **)&arrayD,sz);
	cudaMalloc(&answerD,sizeof(int));
	cudaMemcpy(arrayD,inputarray,sz,cudaMemcpyHostToDevice);
	
	dim3 gridDim(griddim);
	dim3 blockDim(blockdim);	

	cmax<<<gridDim,blockDim>>>(arrayD,answerD);
	cudaDeviceSynchronize();
	cudaMemcpy(answer,answerD,sizeof(int),cudaMemcpyDeviceToHost);
	
	cudaFree(arrayD);
}