#include <stdlib.h>
#include <stdio.h>
#include <time.h> 

/* To index element (i,j) of a 2D array stored as 1D */
#define index(i, j, colNum)  ((i)*(colNum)) + (j)

#include <iostream>
using namespace std;

void matrixPrint(int *mat, int rowN, int colN);
void matMultiplication(int *matA, int rowA, int colA, int *matB, int rowB, int colB);

int main(int argc, char *argv[]) {
		int colNum = 3;
		int rowNum = 4;
		int *xMat, *transposeX;
		xMat = (int *)calloc(colNum*rowNum, sizeof(int));
		transposeX = (int *)calloc(colNum*rowNum, sizeof(int));
		int dataMat[12] = {2,3,-2, 7,2,5, 1,7,9, 7,8,9};

		for(int i = 0; i < rowNum; i++) {
			for(int j = 0; j < colNum; j++) {
				xMat[index(i, j, colNum)] = dataMat[index(i,j, colNum)];
				transposeX[index(j,i, rowNum)] = dataMat[index(i,j, colNum)];	
			}
		}

		matMultiplication(transposeX, colNum, rowNum, xMat, rowNum, colNum);
		// cout<<transposeX[index(2, 1, rowNum)]<<endl;
		// matrixPrint(transposeX, colNum, rowNum);

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void matMultiplication(int *matA, int rowA, int colA, int *matB, int rowB, int colB) {
	int *tempMat ;
	int tempMatSize = rowA*colB;
	int tempSum;
	tempMat =  (int *)calloc(tempMatSize, sizeof(int));
	matrixPrint(matA, rowA, colA);
	matrixPrint(matB, rowB, colB);
	for(int i = 0; i < rowA; i++) {
		for(int j = 0; j < colB; j++) {
			tempSum = 0;
			for(int k = 0; k < colA; k ++ ) {
				tempSum += matA[index(i, k, colA)]*matB[index(k, j, colB)] ;
				// cout<<"tempSum ->"<<tempSum<<endl;
			}
			tempMat[index(i, j, colB)] = tempSum;
		}
	}

	cout<<"======================================"<<endl;
	matrixPrint(tempMat, rowA, colB);
}

void matrixPrint(int *mat, int rowN, int colN) {
	for(int i = 0; i < rowN; i++) {
		for(int j = 0; j < colN; j++) {
			printf("%d ", mat[index(i, j, colN)] );
			if(j == colN -1) printf("\n");
		}
	}
}
