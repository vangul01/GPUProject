#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <iostream>
// #include<bits/stdc++.h> 

using namespace std;
#define index(i, j, colNum)  ((i)*(colNum)) + (j)

#include "0header.h"
#include "0matrixCalculation.cpp"

int main(int argc, char *argv[]) {
	int colNum = 3;
	int rowNum = 4;
	// int X[rowNum*colNum] = {2,3,-2, 7,2,5, 1,7,9, 7,8,9};
	// int X[rowNum*colNum] = {1,3,-2, 1,2,5, 1,7,9, 1,8,9};
	int X[1000000] = {1,3,-2, 1,2,5, 1,7,9, 1,8,9};
	int transposeX[colNum*rowNum];

	float Y[1000] = {3.1, 2.2, -7.1, 5.1};
	// float Y[rowNum] = {3.1, 2.2, -7.1, 5.1};

	for(int i = 0; i < rowNum; i++) {
		for(int j = 0; j < colNum; j++) {
			transposeX[index(j,i, rowNum)] = X[index(i,j, colNum)];	
		}
	}

	N = colNum;
	int tranXmulxMat[colNum*colNum];
	matMultiplication(transposeX, colNum, rowNum, X, rowNum, colNum, tranXmulxMat);
	cout<<"X\'-------->"<<endl;
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

	return 0;
}
