/*
	This is the mandelbrot function
	It takes the number of "vectors" and number of "places" and one file name as arguments
	The resulting output file will contain a mandelbrot fractal
	example: ./mandelbrot 100 100 out.csv
*/
#include "fileio.h"
#include <cuda.h>
#include <math.h>

typedef struct complextype{
        float real, imag;
} Compl;

__global__ void calc(int*,int,int);

int main (int argc,char ** argv)
{
	
        int i, j;
	const int xdim=atoi(argv[1]);
	const int ydim=atoi(argv[2]);
	
	dim3 blockDim(16,16);
	const int gridx= xdim%blockDim.x==0 ? xdim/blockDim.x : xdim/blockDim.x+1; 
	const int gridy= ydim%blockDim.y==0 ? ydim/blockDim.y : ydim/blockDim.y+1; 
	dim3 gridDim(gridx,gridy);
	int* pixels;
	int* pixels2=(int*) malloc(sizeof(int)*xdim*ydim);
	int **grid = createArray(xdim,ydim);

	cudaMalloc((void**)&pixels,xdim*ydim*sizeof(int));

	calc<<<gridDim, blockDim>>>(pixels,xdim,ydim);

	cudaDeviceSynchronize();

	cudaMemcpy(pixels2,pixels,xdim*ydim*sizeof(int),cudaMemcpyDeviceToHost);

	for(i=0;i<xdim;i++){
	for(j=0;j<ydim;j++){
		grid[i][j]=pixels2[i*ydim+j];
	}
	}
	writeFile(xdim, ydim, grid, argv[3]);		

	cudaFree(pixels);

}

__global__ void calc(int* pixels, int xdim, int ydim){
	int i=blockIdx.x*blockDim.x+threadIdx.x;
	int j=blockIdx.y*blockDim.y+threadIdx.y;
	if( i<xdim*ydim && j<xdim*ydim){
		Compl	z, c;
		int k;
		float	lengthsq, temp;
	
        	z.real = z.imag = 0.0;
        	c.real = ((float) j - (ydim/2.0))/(ydim/4.0);  
		c.imag = ((float) i - (xdim/2.0))/(xdim/4.0);
        	k = 0;
	
		do{                                           
        	 	temp = z.real*z.real - z.imag*z.imag + c.real;
        	 	z.imag = 2.0*z.real*z.imag + c.imag;
        	 	z.real = temp;
        	 	lengthsq = z.real*z.real+z.imag*z.imag;
        	 	k++;
        	} while (lengthsq < 4.0 && k < 100);
	
		pixels[i*ydim+j]=(k==100);
}
}	

