#include "stdio.h"

int main(){
int *a,x[100];
int i=0;
for(i=0;i<100;i++){
	x[i]=i*2;
}
CUDAmax(x,a,100);
printf("ans:%d\n",*a);
return 0;
}