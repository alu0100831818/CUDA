
/**
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 */
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cuda.h>

#define N 50
#define M 5


__global__ void incremento(int* ptr,int* h, int tam, int c){
	 int i = threadIdx.x;
	 int j = blockIdx.x ;
	 int pos= j*c + i;
	 if((i==0 )&&(j==0)){
		 for(int y=0;i<M;y++){
			 atomicAdd(&h[y],0);
		 }
	 }
	 int dato;
		 if(pos<tam){
			dato= ptr[pos]%M;
			h[dato]++;
		 }
}

int main(int argc, char **argv)
{
	//reservo memoria para host
	int* ptr = (int*)malloc(N*sizeof(int));
	int* H_ = (int*)malloc(M*sizeof(int));

	//reservo memoria para device
	int* device_array;
	int* h_;
	cudaMalloc((void**)&device_array, N*sizeof(int));
	cudaMalloc((void**)&h_, M*sizeof(int));

	 for(int i=0; i < N; ++i)
	  {
		ptr[i]=1+i;
	    printf("%d ",ptr[i]);
	  }



	int threadsPerBlock=(N + M -1)/ M ;
	int numBlocks =  M;

	printf("\n %d %d \n ", numBlocks,threadsPerBlock);
	cudaMemcpy(device_array,ptr, N*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(h_,H_, M*sizeof(int), cudaMemcpyHostToDevice);
	incremento<<<numBlocks,threadsPerBlock>>> (device_array,h_,N, threadsPerBlock);
	cudaMemcpy(ptr, device_array, N*sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(H_,h_, M*sizeof(int), cudaMemcpyHostToDevice);





	 for(int i=0; i < M; ++i)
	 	  {
	 	    printf("%d ",H_[i]);
	 	  }


	return 0;
}
