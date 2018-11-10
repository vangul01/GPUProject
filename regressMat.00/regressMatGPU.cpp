//#include <cuda.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <iostream>
#include <cstdlib>
#include <ctime> 
// #include<bits/stdc++.h> 

using namespace std;
#define index(i, j, colNum)  ((i)*(colNum)) + (j)
#include "0header.h"	

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
//////////////////////////////// TURN ALL OF THESE INTO KERNELS//////////////////////////////////////////// 

bool inverse(int A[], float inverse[]);
int determinant(int A[], int n);
void getCofactor(int A[], int temp[], int p, int q, int n);
void adjoint(int A[],int adj[]);


template<class T> 
void display(T A[], int rowN, int colN) { 
	for (int i=0; i< rowN; i++) { 
		for (int j=0; j< colN; j++) 
			cout << A[index(i, j, colN)] << " "; 
		cout << endl; 
	} 
}

void matrixPrint(int mat[], int rowN, int colN) {
	for(int i = 0; i < rowN; i++) {
		for(int j = 0; j < colN; j++) {
			printf("%d ", mat[index(i, j, colN)] );
			if(j == colN -1) printf("\n");
		}
	}
}
////////////////////////////////////////////
bool inverse(int A[], float inverse[]) { 
	int det = determinant(A, N); 
	if (det == 0) { 
		cout << "Singular matrix, can't find its inverse\n"; 
		return false; 
	} 

	int adj[N*N]; 

	adjoint(A, adj); 

	for (int i=0; i<N; i++) 
		for (int j=0; j<N; j++) 
			inverse[index(i,j,N)] = adj[index(i,j,N)]/float(det); 

	return true; 
}  
///
void adjoint(int A[],int adj[]) { 
	int sign = 1; 
	int temp[N*N]; 

	for (int i=0; i<N; i++) { 
		for (int j=0; j<N; j++) { 
			// Get cofactor of A[i][j] 
			getCofactor(A, temp, i, j, N); 

			sign = ((i+j)%2==0)? 1: -1; 

			adj[index(j,i,N)] = (sign)*(determinant(temp, N-1)); 
		} 
	} 
} 
////
int determinant(int A[], int n) {
	int D = 0; // Initialize result 

	// Base case : if matrix contains single element 
	if (n == 1) 
		return A[index(0,0,N)]; 

	// int temp[N][N]; // To store cofactors 
	int temp[N*N]; // To store cofactors 

	int sign = 1; // To store sign multiplier 

	// Iterate for each element of first row 
	for (int f = 0; f < n; f++) { 
		getCofactor(A, temp, 0, f, n); 
		D += sign * A[index(0,f,N)] * determinant(temp, n - 1); 

		// terms are to be added with alternate sign 
		sign = -sign; 
	} 

	return D; 
} 
//////
void getCofactor(int A[], int temp[], int p, int q, int n) { 
	int i = 0, j = 0; 

	for (int row = 0; row < n; row++) { 
		for (int col = 0; col < n; col++) { 

			if (row != p && col != q) { 
				
				temp[index(i,j,N)] = A[index(row,col,N)]; 
				j++;

				if (j == n - 1) { 
					j = 0; 
					i++; 
				} 
			} 
		} 
	} 
}
////////////////////////// TURN ALL OF THESE INTO KERNELS ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/*__global__*/ void matMultiplication(int matA[], int rowA, int colA, int matB[], int rowB, int colB, int resultMat[]) {
	int tempSum;

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
///////////////////////////////////////////////
/*__global__*/ void matMultiplFloat(float matA[], int rowA, int colA, float matB[], int rowB, int colB, float resultMat[]) {
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
///////////////////////////////////////////////
/*__global__*/ void matIntToFloat(int matA[], int rowA, int colA, float resultMat[], int rowB, int colB) {
	for(int i = 0; i < rowA; i++) {
		for(int j = 0; j < colB; j++) {
			for(int k = 0; k < colA; k ++ ) {
				resultMat[index(i,j, colB)] = (float) matA[index(i,j, colA)];
			}
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
	
	//populate X with random values between 0-9 
	int* X = (int*)malloc(sizeof(int)*rowNum*colNum); 
	for(int i = 0; i < rowNum; i++) {
		for(int j = 0; j < colNum; j++) {
			X[index(i, j, colNum)] = rand()%10;	
		}
	}

	//transpose X on X' and create new matrix
	int* transposeX = (int*)malloc(sizeof(int)*rowNum*colNum); 
	for(int i = 0; i < rowNum; i++) {
		for(int j = 0; j < colNum; j++) {
			transposeX[index(j,i, rowNum)] = X[index(i,j, colNum)];	
		}
	}

	//populate Y with random 0 or 1 value
	float* Y = (float*)malloc(sizeof(float)*rowNum); 
	for(int i = 0; i < rowNum; i++) {
		Y[i] = rand()%2;
	}


/////////////////////////////////////////DEVICE CODE///////////////////////////

	//declare and allocate mem for device vars
	int* d_X; int* d_transposeX; float* d_Y;
	CudaSafeCall(cudaMalloc((void**)&d_X, sizeof(int)*rowNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_transposeX, sizeof(int)*rowNum*colNum));	
	CudaSafeCall(cudaMalloc((void**)&d_Y, sizeof(float)*rowNum));	

	//transfer data to device
	cudaMemcpy(d_X, X, sizeof(int)*rowNum*colNum, cudaMemcpyHostToDevice); 
	cudaMemcpy(d_transposeX, transposeX, sizeof(int)*rowNum*colNum, cudaMemcpyHostToDevice); 
	cudaMemcpy(d_Y, Y, sizeof(int)*rowNum, cudaMemcpyHostToDevice); 

	//setup kernel config
	int blocks_per_grid = ((N-2)/1024) + 1;
	dim3 dimGrid(blocks_per_grid, 1, 1); 
	dim3 dimBlock(1024, 1, 1);

	//call cuda kernel
	//FindPrimes<<<dimGrid, dimBlock>>>(d_numbers, N);  //Call kernel for all the functions you wish....
	CudaCheckError();

	//transfer results 
	cudaMemcpy(number_arr, d_numbers, size, cudaMemcpyDeviceToHost);





////////////////////////////////////////

	N = colNum;
	int tranXmulxMat[colNum*colNum];

	///MAKE THIS KERNEL FUNCTION
	matMultiplication(transposeX, colNum, rowNum, X, rowNum, colNum, tranXmulxMat);
	cout<<"X\'-------->"<<endl;

	///LEAVE THIS CPU
	matrixPrint(transposeX, colNum, rowNum);
	cout<<"X ----------->"<<endl;
	matrixPrint(X, rowNum, colNum);
	cout<<"X\'X----------->"<<endl;
	display(tranXmulxMat, colNum, colNum);

	/////////////////////////////////////////////////////////////////////
	float inv[N*N]; // To store inverse of A[][] 

	cout<<"The inverse is --------->"<<endl;
	if (inverse(tranXmulxMat, inv)) 
		display(inv, colNum, colNum); 

	//cal (X'X)^-1*X'
	//------------
	float XtFloat[colNum*rowNum];
	matIntToFloat(transposeX, colNum, rowNum, XtFloat, colNum, rowNum);

	float resultM[colNum*rowNum];
	matMultiplFloat(inv, colNum, colNum, XtFloat, colNum, rowNum, resultM);

	cout<<"(X'X)^-1*X' ->"<<endl;
	display(resultM, colNum, rowNum);
	
	//cal (X'X)^-1*X'Y
	float finalResult[colNum];
	matMultiplFloat(resultM, colNum, rowNum, Y, rowNum, 1, finalResult);

	cout<<"final (X'X)^-1*X'Y ->"<<endl;
	display(finalResult, colNum, 1);





	// FIGURING OUT PREDICTION Y ////////////////////////////////////////////
	cout <<"These are my B values: "<<endl; 
	//here make Y function...
	float* B = (float*)malloc(sizeof(float)*colNum);
	for(int i = 0; i < colNum; i++) {
		B[i] = finalResult[i];
		cout << B[i] << " ";
	}
	cout << endl;

	//create random X values to multiply with the B values to get Y values
	//Then do the weird formula on Y values
	cout <<"These are my random X values: "<<endl; 
	float* newX = (float*)malloc(sizeof(float)*colNum-1);
	for(int i = 1; i < colNum; i++) {
		newX[i] = rand()%10;
		cout << newX[i] << " ";
	}
	cout << endl;

	//now figure out Y by multiplying the Bs with the new random Xs
	cout <<"This is my predicted Y!: "<<endl; 
	//float* predictY = (float*)malloc(sizeof(float));
	float predictY[1] = {0};
	float temp = 0;
	for (int i=1; i<colNum; i++) {
		temp += B[i] * newX[i];
		cout << "My B[" << i << "]: " << B[i] << endl;
		cout << "My newX[" << i << "]: " << newX[i] << endl;
		cout << "my temp: " << temp <<endl;
	}
	//Y=b0+b1x1+b2x2
	predictY[0] = B[0] + temp; 
	cout << "My prediction! " << predictY[0] << endl;
	//Now turn this Y into ln[Y/(1−Y)]=a+bX... ?


	return 0;
}
