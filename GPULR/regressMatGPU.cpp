#include <cuda.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include<sstream>
#include<string.h>
// #include<bits/stdc++.h> 

using namespace std;
#define index(i, j, colNum)  ((i)*(colNum)) + (j)

#include "0header.h"
#include "matrixCalculationGPU.cpp"

#define CUDA_ERROR_CHECK
#define CudaSafeCall( err ) __cudaSafeCall( err, __FILE__, __LINE__ )
#define CudaCheckError()    __cudaCheckError( __FILE__, __LINE__ )

inline void __cudaSafeCall( cudaError err, const char *file, const int line )
{
#ifdef CUDA_ERROR_CHECK
    if (cudaSuccess != err) {
        fprintf(stderr, "cudaSafeCall() failed at %s:%i : %s\n", file, line, cudaGetErrorString(err)); 
        exit(1);
    }
#endif
    return;
}

//for kernel
inline void __cudaCheckError( const char *file, const int line )
{
#ifdef CUDA_ERROR_CHECK
    cudaError err = cudaGetLastError();
    if (cudaSuccess != err) { 
        fprintf(stderr, "cudaCheckError() failed at %s:%i : %s\n", file, line, cudaGetErrorString(err));
        exit(1);
    }
    // More careful checking. However, this will affect performance. Comment away if needed.
    //err = cudaDeviceSynchronize();
    //if(cudaSuccess != err) {
    //    fprintf(stderr, "cudaCheckError() with sync failed at %s:%i : %s\n", file, line, cudaGetErrorString(err));
    //    exit(1);
    //}
#endif
    return;
}

////////////////////////// KERNELS ///////////////////////////

__global__ void simpleMultiplication(float matA[], int width, int colA, float matB[], int colB, float result[]) {
//	int tx = blockIdx.x * blockDim.x + threadIdx.x;
//	int ty = blockIdx.y * blockDim.y + threadIdx.y;
	float tempSum = 0;

	for(int k = 0; k < colA; k++) {
		float elementA = matA[ty * width + k];
		float elementB = matB[k * width + tx];
		tempSum += elementA * elementB; //matA[index(i, k, colA)]*matB[index(k, j, colB)] ;
	}
	result[ty * width + tx] = tempSum; //[index(i,j, colB)] = tempSum;

}

////////////////////////// MAIN ///////////////////////////

int main(int argc, char *argv[]) {
	int rowNum = atoi(argv[1]);
	int colNum = atoi(argv[2]);

	float* X = (float*)malloc(sizeof(float)*rowNum*colNum); //	float X[rowNum*colNum];
	float* transposeX = (float*)malloc(sizeof(float)*rowNum*colNum); //	float transposeX[colNum*rowNum];
	float* Y = (float*)malloc(sizeof(float)*rowNum); //	float Y[rowNum];

	readFile(X, Y, rowNum, colNum);

	for(int i = 0; i < rowNum; i++) {
		for(int j = 0; j < colNum; j++) {
			transposeX[index(j,i, rowNum)] = X[index(i,j, colNum)];	
		}
	}

	N = colNum;
	float* tranXmulxMat = (float*)malloc(sizeof(float)*colNum*colNum); //float tranXmulxMat[colNum*colNum];
	float* inv = (float*)malloc(sizeof(float)*colNum*colNum); //float inv[N*N]; // To store inverse of A[][] 
	float* resultM = (float*)malloc(sizeof(float)*rowNum*colNum); //float resultM[colNum*rowNum];
	float* finalResult = (float*)malloc(sizeof(float)*colNum); //float finalResult[colNum];

/////////////////////////////////////////DEVICE CODE////////////////////////////////////////

	//declare and allocate mem for device vars
	float* d_X; float* d_transposeX; float* d_Y; 
	float* d_tranXmulxMat; float* d_resultM; float* d_finalResult; float* d_inv; 

	//init d_arrays
	CudaSafeCall(cudaMalloc((void**)&d_X, sizeof(float)*rowNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_Y, sizeof(float)*rowNum));	
	CudaSafeCall(cudaMalloc((void**)&d_transposeX, sizeof(float)*rowNum*colNum));	
	
	//transfer data to device
	cudaMemcpy(d_X, X, sizeof(int)*rowNum*colNum, cudaMemcpyHostToDevice); 
	cudaMemcpy(d_transposeX, transposeX, sizeof(int)*rowNum*colNum, cudaMemcpyHostToDevice); 
	cudaMemcpy(d_Y, Y, sizeof(int)*rowNum, cudaMemcpyHostToDevice); 

	//allocate for results
	CudaSafeCall(cudaMalloc((void**)&d_tranXmulxMat, sizeof(float)*colNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_inv, sizeof(float)*colNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_resultM, sizeof(float)*colNum*rowNum));	
	CudaSafeCall(cudaMalloc((void**)&d_finalResult, sizeof(float)*colNum));		

	//setup kernel config
	int threadCount = 32;
	int blocks_per_grid = ((rowNum*colNum)/threadCount + 1);
	dim3 dimGrid(blocks_per_grid, 1, 1); 
	dim3 dimBlock(threadCount, 1, 1);

	cout<<"X\'-------->"<<endl;
	//matrixPrint(transposeX, colNum, rowNum);
	cout<<"X ----------->"<<endl;
	//matrixPrint(X, rowNum, colNum);

	//Call kernel
	cout<<"X\'X----------->"<<endl;
//	matMultiplication(transposeX, colNum, rowNum, X, rowNum, colNum, tranXmulxMat);
	simpleMultiplication<<<dimGrid, dimBlock>>>(d_transposeX, colNum, rowNum, d_X, colNum, d_tranXmulxMat); 
	CudaCheckError();
	cudaMemcpy(tranXmulxMat, d_tranXmulxMat, size, cudaMemcpyDeviceToHost);
	display(tranXmulxMat, colNum, colNum);

	//No kernel
	cout<<"The inverse is --------->"<<endl;
	if (inverse(tranXmulxMat, inv)) {
		display(inv, colNum, colNum); 
		cudaMemcpy(d_inv, inv, size, cudaMemcpyHostToDevice);
	}

	//Call kernel...fails
	cout<<"(X'X)^-1*X' ->"<<endl;
	matMultiplication(inv, colNum, colNum, transposeX, colNum, rowNum, resultM);
//	matMultiplication<<<dimGrid, dimBlock>>>(d_inv, colNum, colNum, d_transposeX, colNum, rowNum, d_resultM); 
//	CudaCheckError();
//	cudaMemcpy(resultM, d_resultM, size, cudaMemcpyDeviceToHost);
	display(resultM, colNum, rowNum);

	//Call kernel...fails
	cout<<"final (X'X)^-1*X'Y ->"<<endl;
	matMultiplication(resultM, colNum, rowNum, Y, rowNum, 1, finalResult);
//	matMultiplication<<<dimGrid, dimBlock>>>(d_resultM, colNum, rowNum, d_Y, rowNum, 1, d_finalResult); 
//	CudaCheckError();
//	cudaMemcpy(finalResult, d_finalResult, size, cudaMemcpyDeviceToHost);
	display(finalResult, colNum, 1);

//	cudaMemcpy(inv, d_inv, size, cudaMemcpyDeviceToHost);

////////////////////////////////////////////////////////////////////////////////////////

	free(X); free(transposeX); free(Y);
	free(tranXmulxMat); free(inv); free(resultM); free(finalResult);
	cudafree(d_X); cudafree(d_transposeX); cudafree(d_Y);
	cudafree(d_tranXmulxMat); cudafree(d_inv); cudafree(d_resultM); cudafree(d_finalResult);
	
	return 0;
}



/*
	//matMultiplication(transposeX, colNum, rowNum, X, colNum, tranXmulxMat);
	matMultiplication(transposeX, colNum, rowNum, X, rowNum, colNum, tranXmulxMat);
	cout<<"X\'-------->"<<endl;
	display(transposeX, colNum, rowNum);

	cout<<"X ----------->"<<endl;
	display(X, rowNum, colNum);
	
	cout<<"X\'X----------->"<<endl;
	display(tranXmulxMat, colNum, colNum);

	cout<<"The inverse is --------->"<<endl;
	if (inverse(tranXmulxMat, inv)) 
		display(inv, colNum, colNum); 

	//cal (X'X)^-1*X'
	matMultiplication(inv, colNum, colNum, transposeX, colNum, rowNum, resultM);

	cout<<"(X'X)^-1*X' ->"<<endl;
	display(resultM, colNum, rowNum);
	
	//cal (X'X)^-1*X'Y
	matMultiplication(resultM, colNum, rowNum, Y, rowNum, 1, finalResult);
	cout<<"final (X'X)^-1*X'Y ->"<<endl;
	display(finalResult, colNum, 1);
*///////////////////////////////////////////////////////////
//for cudamalloc
/*
inline void __cudaSafeCall( cudaError err, const char *file, const int line )
{
#ifdef CUDA_ERROR_CHECK
    if (cudaSuccess != err) {
        fprintf(stderr, "cudaSafeCall() failed at %s:%i : %s\n", file, line, cudaGetErrorString(err)); 
        exit(1);
    }
#endif
    return;
}

//for kernel
inline void __cudaCheckError( const char *file, const int line )
{
#ifdef CUDA_ERROR_CHECK
    cudaError err = cudaGetLastError();
    if (cudaSuccess != err) { 
        fprintf(stderr, "cudaCheckError() failed at %s:%i : %s\n", file, line, cudaGetErrorString(err));
        exit(1);
    }
    // More careful checking. However, this will affect performance. Comment away if needed.
    //err = cudaDeviceSynchronize();
    //if(cudaSuccess != err) {
    //    fprintf(stderr, "cudaCheckError() with sync failed at %s:%i : %s\n", file, line, cudaGetErrorString(err));
    //    exit(1);
    //}
#endif
    return;
}


__global__ void FindPrimes (int* d_numbers, int N) {
	int tx = blockIdx.x * blockDim.x + threadIdx.x;	

	tx = min(tx, ((N+3)/2)); //instead of if (tx <= ((N+1)/2)), thread is less than or equal to floor, doesnt include N+3/2
	for (int i = tx+1; i < N-1; i++) { //check threads in positions tx+1->N-2
		if (d_numbers[tx] != 1) {
			if (d_numbers[i] % d_numbers[tx] == 0) {
				d_numbers[i] = 1;
			}
		}
	}
}

R=2 C=3
X'-------->
0 3 
1 4 
2 5 
X ----------->
0 1 2 
3 4 5 
X'X----------->
9 12 15 
12 17 22 
15 22 29 


R=2 C=3
X'-------->
0 3 	1 4 	2 5 
X ----------->
0 1 2	 3 4 5 
X'X----------->
9 12 15 	12 17 22	 15 22 29 


// X'(0 3) * X(0 3)

for each row of X'
	for each column of X
		(i = 0; i<X'row length)
			add mult between elements

*/

////////////////////////////////////// KERNELS //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

//matMultiplication(d_transposeX, colNum, rowNum, d_X, rowNum, colNum, d_tranXmulxMat);
	//matMultiplication(inv, colNum, colNum, transposeX, colNum, rowNum, resultM);
	//matMultiplication(resultM, colNum, rowNum, Y, rowNum, 1, finalResult);
/*__global__ */ /*void matMultiplication(float* matA, int rowA, int colA, float* matB, int rowB, int colB, float* result) {
	int tempSum;
//	int tx = blockIdx.x * blockDim.x + threadIdx.x;
//	int ty = blockIdx.y * blockDim.y + threadIdx.y;
	int width = rowA;

	for(int i = 0; i < width; i++) {
		for(int j = 0; j < width; j++) {
			tempSum = 0;
			for(int k = 0; k < colA; k ++ ) {
				tempSum += matA[index(i, k, colA)]*matB[index(k, j, colB)] ;
			}
			result[index(i,j, colB)] = tempSum;
		}
	}
}
*/
/*
void matMultiplication(float* matA, int rowA, int colA, float* matB, int rowB, int colB, float* resultMat) {
	float tempSum;

	for(int i = 0; i < rowA; i++) {
		for(int j = 0; j < colB; j++) {
			tempSum = 0;
			for(int k = 0; k < colA; k ++ ) {
				tempSum += matA[index(i, k, colA)]*matB[index(k, j, colB)] ;
				// cout<<"tempSum ->"<<tempSum<<endl;
			}
			// tempMat[index(i, j, colB)] = tempSum;
			resultMat[index(i,j, colB)] = tempSum;
		}
	}
}
*/
/*
  //compute only non-edge cases and put into d_result 
  //thread is within bounds of playground dimensions 
  if (tx < N*N) 
  { 
    if (tx > 0 && tx < N-1) //threads that are not on the edges
    { 
        for (int i = 1; i < N-1; i++) //i's that are not on the edges
        { 
          d_result[index(tx, i, N)] = (d_temp[index(tx-1, i, N)] + d_temp[index(tx+1, i, N)] 
            + d_temp[index(tx, i-1, N)] + d_temp[index(tx, i+1, N)])/4.0;
        }
    }
  }

*/


////////////////////////////////// MAIN ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*
int main(int argc, char *argv[]) {
	srand(time(NULL)); //use rand()%10 for random bt 0-9

	if (argc != 3) {
		cout << "Improper number of arguments. \nArg 1: Number of Rows\nArg 2: Number of Columns" << endl; 
		exit(1);
	}

	//error check this
	string rowInput = argv[(argc - 2)];
	string colInput = argv[(argc - 1)];
	int rowNum = stoi(rowInput);
	int colNum = stoi(colInput);

	cout <<"Rows: " << rowNum << " Columns: " << colNum << endl;
	
	//populate X with random values between 0-9..right now no for stability
	int count = 0;
	float* X = (float*)malloc(sizeof(float)*rowNum*colNum); 
	for(int i = 0; i < rowNum; i++) {
		for(int j = 0; j < colNum; j++) {
			X[index(i, j, colNum)] = count;//(float)(rand()%10);	
			count++;
		}
	}

	//populate Y with random 0 or 1 value
	float* Y = (float*)malloc(sizeof(float)*rowNum); 
	for(int i = 0; i < rowNum; i++) {
		Y[i] = rand()%2;
	}

	//transpose X on X' and create new matrix, could Potentially use GPU
	float* transposeX = (float*)malloc(sizeof(float)*rowNum*colNum); 
	for(int i = 0; i < rowNum; i++) {
		for(int j = 0; j < colNum; j++) {
			transposeX[index(j,i, rowNum)] = X[index(i,j, colNum)];	
		}
	}

	N = colNum;
	float* tranXmulxMat = (float*)malloc(sizeof(float)*colNum*colNum); 
	float* inv = (float*)malloc(sizeof(float)*colNum*colNum); 
	float* resultM = (float*)malloc(sizeof(float)*rowNum*colNum); 
	float* finalResult = (float*)malloc(sizeof(float)*colNum); 

*/
/////////////////////////////////////////DEVICE CODE////////////////////////////////////////
/*
	//declare and allocate mem for device vars
	float* d_X; float* d_transposeX; float* d_Y; 
	float* d_tranXmulxMat; float* d_resultM; float* d_finalResult;
//	float* d_inv; 

	//for already init CPU arrays
	CudaSafeCall(cudaMalloc((void**)&d_X, sizeof(float)*rowNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_Y, sizeof(float)*rowNum));	
	CudaSafeCall(cudaMalloc((void**)&d_transposeX, sizeof(float)*rowNum*colNum));	
	
	//for results
	CudaSafeCall(cudaMalloc((void**)&d_tranXmulxMat, sizeof(float)*colNum*colNum));	
//	CudaSafeCall(cudaMalloc((void**)&d_inv, sizeof(float)*colNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_resultM, sizeof(float)*colNum*rowNum));	
	CudaSafeCall(cudaMalloc((void**)&d_finalResult, sizeof(float)*colNum));		

	//transfer data to device
	cudaMemcpy(d_X, X, sizeof(int)*rowNum*colNum, cudaMemcpyHostToDevice); 
	cudaMemcpy(d_transposeX, transposeX, sizeof(int)*rowNum*colNum, cudaMemcpyHostToDevice); 
	cudaMemcpy(d_Y, Y, sizeof(int)*rowNum, cudaMemcpyHostToDevice); 

	//setup kernel config
	int threadCount = 32;
	int blocks_per_grid = ((rowNum*colNum)/threadCount + 1);
	dim3 dimGrid(blocks_per_grid, 1, 1); 
	dim3 dimBlock(threadCount, 1, 1);
*/
/*
	//No kernel
	cout<<"X\'-------->"<<endl;
	matrixPrint(transposeX, colNum, rowNum);
	
	//No kernel	
	cout<<"X ----------->"<<endl;
	matrixPrint(X, rowNum, colNum);

	//Call kernel
	cout<<"X\'X----------->"<<endl;
	matMultiplication(transposeX, colNum, rowNum, X, rowNum, colNum, tranXmulxMat); 
//	matMultiplication<<<dimGrid, dimBlock>>>(d_transposeX, colNum, rowNum, d_X, rowNum, colNum, d_tranXmulxMat); 
//	CudaCheckError();
//	cudaMemcpy(tranXmulxMat, d_tranXmulxMat, size, cudaMemcpyDeviceToHost);
	display(tranXmulxMat, colNum, colNum);

	//No kernel
	cout<<"The inverse is --------->"<<endl;
	if (inverse(tranXmulxMat, inv)) 
		display(inv, colNum, colNum); 

	//Call kernel...fails
	cout<<"(X'X)^-1*X' ->"<<endl;
	matMultiplication(inv, colNum, colNum, transposeX, colNum, rowNum, resultM);
//	matMultiplication<<<dimGrid, dimBlock>>>(d_inv, colNum, colNum, d_transposeX, colNum, rowNum, d_resultM); 
//	CudaCheckError();
//	cudaMemcpy(resultM, d_resultM, size, cudaMemcpyDeviceToHost);
	display(resultM, colNum, rowNum);

	//Call kernel...fails
	cout<<"final (X'X)^-1*X'Y ->"<<endl;
	matMultiplication(resultM, colNum, rowNum, Y, rowNum, 1, finalResult);
//	matMultiplication<<<dimGrid, dimBlock>>>(d_resultM, colNum, rowNum, d_Y, rowNum, 1, d_finalResult); 
//	CudaCheckError();
//	cudaMemcpy(finalResult, d_finalResult, size, cudaMemcpyDeviceToHost);
	display(finalResult, colNum, 1);

//	cudaMemcpy(inv, d_inv, size, cudaMemcpyDeviceToHost);

}
*/
////////////////////////////////////////////////////////////////////////////////////////////
/*	
	cout<<"X\'X----------->"<<endl;
	matMultiplication(transposeX, colNum, rowNum, X, rowNum, colNum, tranXmulxMat);
	display(tranXmulxMat, colNum, colNum);

	//cal inverse --------------
	cout<<"The inverse is --------->"<<endl;
	if (inverse(tranXmulxMat, inv)) 
		display(inv, colNum, colNum); 
	
	//cal (X'X)^-1*X' --------------
	cout<<"(X'X)^-1*X' ->"<<endl;
	matMultiplication(inv, colNum, colNum, transposeX, colNum, rowNum, resultM);	
	display(resultM, colNum, rowNum);
	
	//cal (X'X)^-1*X'Y --------------
	cout<<"final (X'X)^-1*X'Y ->"<<endl;
	matMultiplication(resultM, colNum, rowNum, Y, rowNum, 1, finalResult);	
	display(finalResult, colNum, 1);

}
*/