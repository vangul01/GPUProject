#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <iostream>
#include <string.h>

///////////////////
#include <cstdlib>
#include <math.h>
#include <fstream>
#include<sstream>
////////////////////
using namespace std;
#define index(i, j, colNum)  ((i)*(colNum)) + (j)

////////////////////
int main(int argc, char *argv[]) {
	int rowNum = atoi(argv[1]);
	int xNum = atoi(argv[2]);
	int colNum = xNum;

	FILE * fp;
	char fileName[100];
	sprintf(fileName, "%dby%dMat.txt", rowNum, colNum);
	remove(fileName);
	fp = fopen(fileName, "a");

	float ranFloat;
	int ranInt;
	for(int i = 0; i < rowNum; i++) {
		if(i != 0)
			fprintf(fp, "%s\n", " ");
		
		for(int j = 0; j < colNum; j++) {
			ranFloat = rand();
			ranInt = (int) rand();
			
			//first value of each row will become Y value
			if(j == 0) {
				ranFloat = ranFloat/(float)(RAND_MAX);
				//Calculate logit p(x) = ln[p(x) / 1-p(x)]
				ranFloat = (float) log(ranFloat/(1 - ranFloat));
			} else {
				ranFloat = ranFloat/(float)(RAND_MAX) + 1;	
				if(ranInt % 2 == 1) ranFloat = -1.0*ranFloat;
			}		
			fprintf(fp, "%f ", ranFloat );
		}
	}
	fclose(fp);

	cout<<"# of rows->"<<rowNum<<": # of Xs->"<<xNum<<": # of Ys->1"<<endl;
	// display(ranMatrix, rowNum, colNum);
	return 0;     
}
	

