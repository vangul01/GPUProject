//#include <cuda.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <iostream>
#include <cstdlib>
#include <ctime> 
#include <math.h>
// #include<bits/stdc++.h> 

using namespace std;
#define index(i, j, colNum)  ((i)*(colNum)) + (j)

#include "0header.h"	
#include "matrixCalculationGPU.cpp"

/*
#define CUDA_ERROR_CHECK
#define CudaSafeCall( err ) __cudaSafeCall( err, __FILE__, __LINE__ )
#define CudaCheckError()    __cudaCheckError( __FILE__, __LINE__ )

//for cudamalloc
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
*/
////////////////////////////// TURN THESE INTO KERNELS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

//matMultiplication(transposeX, colNum, rowNum, X, tranXmulxMat);
/*__global__ */void matMultiplication(float matA[], int rowA, int colA, float matB[], float resultMat[]) {
	int tempSum;
	int rowB = colA;
	int colB = rowA;

	for(int i = 0; i < rowA; i++) {
		for(int j = 0; j < colB; j++) {
			tempSum = 0;
			for(int k = 0; k < colA; k ++ ) {
				tempSum += matA[index(i, k, colA)]*matB[index(k, j, colB)] ;
			}
			resultMat[index(i,j, colB)] = tempSum;
		}
	}
}

/*__global__ */void matMultiplFloat(float matA[], int rowA, int colA, float matB[], int rowB, int colB, float resultMat[]) {
	float tempSum;

	for(int i = 0; i < rowA; i++) {
		for(int j = 0; j < colB; j++) {
			tempSum = 0;
			for(int k = 0; k < colA; k ++ ) {
				tempSum += matA[index(i, k, colA)]*matB[index(k, j, colB)] ;
			}
			resultMat[index(i,j, colB)] = tempSum;
		}
	}
}

////////////////////////////////// MAIN ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

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

	//Could Potentially use GPU....
	//transpose X on X' and create new matrix
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


/////////////////////////////////////////DEVICE CODE///////////////////////////
/*
	//declare and allocate mem for device vars
	float* d_X; float* d_transposeX; float* d_Y; 
	float* d_tranXmulxMat; float* d_inv; float* d_resultM; float* d_finalResult;
	
	//for already init CPU arrays
	CudaSafeCall(cudaMalloc((void**)&d_X, sizeof(float)*rowNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_Y, sizeof(float)*rowNum));	
	CudaSafeCall(cudaMalloc((void**)&d_transposeX, sizeof(float)*rowNum*colNum));	
	
	//for results
	CudaSafeCall(cudaMalloc((void**)&d_tranXmulxMat, sizeof(float)*colNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_inv, sizeof(float)*colNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_resultM, sizeof(float)*colNum*rowNum));	
	CudaSafeCall(cudaMalloc((void**)&d_finalResult, sizeof(float)*colNum));		

	//transfer data to device
	cudaMemcpy(d_X, X, sizeof(int)*rowNum*colNum, cudaMemcpyHostToDevice); 
	cudaMemcpy(d_transposeX, transposeX, sizeof(int)*rowNum*colNum, cudaMemcpyHostToDevice); 
	cudaMemcpy(d_Y, Y, sizeof(int)*rowNum, cudaMemcpyHostToDevice); 

	//setup kernel config
	int blocks_per_grid = (N/1024 + 1;
	dim3 dimGrid(blocks_per_grid, 1, 1); 
	dim3 dimBlock(1024, 1, 1);

	//call cuda kernel
	//FindPrimes<<<dimGrid, dimBlock>>>(d_numbers, N);  //Call kernel for all the functions you wish....
	CudaCheckError();

	//transfer results 
	cudaMemcpy(tranXmulxMat, d_tranXmulxMat, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(inv, d_inv, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(resultM, d_resultM, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(finalResult, d_finalResult, size, cudaMemcpyDeviceToHost);
*/

////////////////////////////////////////////////////////////////////////////////////////////

	cout<<"X\'-------->"<<endl;
	matrixPrint(transposeX, colNum, rowNum);
	
	cout<<"X ----------->"<<endl;
	matrixPrint(X, rowNum, colNum);
	
	cout<<"X\'X----------->"<<endl;
	matMultiplication(transposeX, colNum, rowNum, X, tranXmulxMat);
	display(tranXmulxMat, colNum, colNum);

	//cal inverse --------------
	cout<<"The inverse is --------->"<<endl;
	if (inverse(tranXmulxMat, inv)) 
		display(inv, colNum, colNum); 

	//cal (X'X)^-1*X' --------------
	cout<<"(X'X)^-1*X' ->"<<endl;
	matMultiplFloat(inv, colNum, colNum, transposeX, colNum, rowNum, resultM);	
	display(resultM, colNum, rowNum);
	
	//cal (X'X)^-1*X'Y --------------
	cout<<"final (X'X)^-1*X'Y ->"<<endl;
	matMultiplFloat(resultM, colNum, rowNum, Y, rowNum, 1, finalResult);	
	display(finalResult, colNum, 1);
}


/*
/////////////////////////LEAVE THIS! FIGURING OUT PREDICTION Y /////////////////////////////
	cout <<"\nThese are my B values: "; 
	//here make Y function...
	float* B = (float*)malloc(sizeof(float)*colNum);
	for(int i = 0; i < colNum; i++) {
		B[i] = finalResult[i];
		cout << B[i] << " ";
	}
	cout << endl;

	//create random X values to multiply with the B values to get Y values
	//Then do the weird formula on Y values
	cout <<"These are my random X values: "; 
	float* newX = (float*)malloc(sizeof(float)*colNum-1);
	for(int i = 1; i < colNum; i++) {
		newX[i] = rand()%10;
		cout << newX[i] << " ";
	}
	cout << endl;

	//now figure out Y by multiplying the Bs with the new random Xs
	//float* predictY = (float*)malloc(sizeof(float));
	float predictY = 0;
	float temp = 0;
	for (int i=1; i<colNum; i++) {
		temp += B[i] * newX[i];
	}
	//Y=b0+b1X1+b2X2+....bNXN
	predictY = B[0] + temp; 
	cout << "b0 + b1X1 + b2X2 + bNXN ----> Y = " << predictY << endl;
	//Now turn this Y into ln[Y/(1−Y)]=a+bX... ?
	predictY = predictY/(1-predictY); //will give a+bX+...
	cout << "Y/(1−Y) = " << predictY << endl;
	
	if (predictY >= 0) {
		predictY = log(predictY);
		cout << "ln[Y/(1−Y)] = " << predictY << endl;
	} else {
		cout << "Error: Y/(1−Y) cannot be negative, 0 or 1.\n";
	}

	return 0;
}

*/