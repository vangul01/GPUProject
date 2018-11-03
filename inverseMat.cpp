#include <stdlib.h>
#include <stdio.h>
#include <time.h> 

#include <iostream>
#include<bits/stdc++.h> 
using namespace std; 

#define index(i, j, colNum)  ((i)*(colNum)) + (j)

int N;
///////////////////
bool inverse(int A[], float inverse[]);
int determinant(int A[], int n);
void getCofactor(int A[], int temp[], int p, int q, int n);
void adjoint(int A[],int adj[]);

template<class T> 
void display(T A[]) { 
	for (int i=0; i<N; i++) { 
		for (int j=0; j<N; j++) 
			cout << A[index(i, j, N)] << " "; 
		cout << endl; 
	} 
}

int main() { 
	N = 4;
	int A[N*N] = {5,-2,2,7, 1,0,0,3, -3,1,5,0, 3,-1,-9,4 };
	// int A[N*N] = {5,-2,2, 1,5,7, -3,1,5, 3,4,-9};

	int adj[N*N]; // To store adjoint of A[][] 
	float inv[N*N]; // To store inverse of A[][] 

	cout << "Input matrix is :\n"; 
	display(A); 

	// cout << "\nThe Adjoint is :\n"; 
	// // adjoint(A, adj); 
	// // display(adj); 

	// cout << "\nThe Inverse is :\n"; 
	if (inverse(A, inv)) 
		display(inv); 

	return 0; 
} 
////////////////////////////////////////////
bool inverse(int A[], float inverse[]) { 
	int det = determinant(A, N); 
	if (det == 0) { 
		cout << "Singular matrix, can't find its inverse"; 
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