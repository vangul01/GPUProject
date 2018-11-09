/* 
To do: 
make arrays that need to be dynamic
make function that solves prediction Y with the B outputs
	 ln[Y/(1−Y)]=a+bX 
make another array for random values X
make arg for input random file?
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <iostream>
// #include<bits/stdc++.h> 

using namespace std;
#define index(i, j, colNum)  ((i)*(colNum)) + (j)

#include "0header.h"	//why do we need?
#include "0matrixCalculation.cpp"

int main(int argc, char *argv[]) {
	int N;

	if (argc != 3) {
		cout << "Improper number of arguments. \nArg 1: Number of Rows\nArg 2: Number of Columns" << endl; 
		exit(1);
	}

	//error check this
	string input1 = argv[(argc - 2)];
	string input2 = argv[(argc - 1)];
	int rowNum = stoi(input1);
	int colNum = stoi(input2);


	cout << rowNum << " " << colNum << endl;

	// int X[rowNum*colNum] = {2,3,-2, 7,2,5, 1,7,9, 7,8,9};
	// int X[rowNum*colNum] = {1,3,-2, 1,2,5, 1,7,9, 1,8,9};
/*	int* X = (int*)malloc(sizeof(int)*rowNum*colNum);  
	for(int i = 0; i < rowNum; i++) {
		for(int j = 0; j < colNum; j++) {
			X[i] = j;	
			cout << &X << " " ;
		}
	}
*/


	int X[1000000] = {1,3,-2, 1,2,5, 1,7,9, 1,8,9};
	int transposeX[colNum*rowNum];

	float Y[1000] = {3.1, 2.2, -7.1, 5.1}; //make this some random num
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
