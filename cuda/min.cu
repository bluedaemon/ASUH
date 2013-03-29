#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void cudamin(int *arrayD,int *numberD) {
extern __shared__ int sdata[];
unsigned int tid = threadIdx.x;
unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
sdata[tid] = arrayD[i];
__syncthreads();

for(unsigned int s=1; s < blockDim.x; s *= 2) {
if (tid % (2*s) == 0) {
sdata[tid]= (sdata[tid]<sdata[tid+s]) ? sdata[tid]: sdata[tid+s];
}
__syncthreads();
}

if (tid == 0) numberD[blockIdx.x] = sdata[0];

}

void CUDAmin(int* argv, int* numberH){
	const int sz=100*sizeof(int);
	int *numberD;
	int *arrayD;
	int i=0;
	
	/*char line[80];
	FILE *a;
	a=fopen(argv,"r");
	while((fgets(line, 80, a) != NULL) && i<100){
		sscanf (line, "%d", &numberH[i]);
		array[i]=numberH[i];
		i++;
  	}*/

	cudaMalloc((void **)&arrayD,sz);
	cudaMalloc((void **)&numberD,sz);
	cudaMemcpy(arrayD,argv,sz,cudaMemcpyHostToDevice);
	dim3 gridDim(5);
	dim3 blockDim(20);	
	cudamin<<<gridDim,blockDim>>>(arrayD,numberD);

	cudaDeviceSynchronize();
	cudaMemcpy(numberH,numberD,sz,cudaMemcpyDeviceToHost);
	cudaFree(arrayD);
	cudaFree(numberD);
}