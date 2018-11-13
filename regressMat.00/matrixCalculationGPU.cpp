/*
bool inverse(int A[], float inverse[]);
int determinant(int A[], int n);
void getCofactor(int A[], int temp[], int p, int q, int n);
void adjoint(int A[],int adj[]);
*/
////////////////////////KEEP AS CPU////////////////////////////

template<class T> 
void display(T A[], int rowN, int colN) { 
	for (int i=0; i< rowN; i++) { 
		for (int j=0; j< colN; j++) 
			cout << A[index(i, j, colN)] << " "; 
		cout << endl; 
	} 
}
template<class T> 
void matrixPrint(T mat[], int rowN, int colN) {
	for(int i = 0; i < rowN; i++) {
		for(int j = 0; j < colN; j++) {
			printf("%d ", (int)mat[index(i, j, colN)] );   //cast float as int
			if(j == colN -1) printf("\n");
		}
	}
}
/////////////////////////////////////////////////////////////////

template<class T> 
void getCofactor(T A[], float temp[], int p, int q, int n) { 
	int i = 0, j = 0; 

	for (int row = 0; row < n; row++) { 
		for (int col = 0; col < n; col++) { 

			if (row != p && col != q) { 
				
				temp[index(i,j,N)] = (float)A[index(row,col,N)]; 
				j++;

				if (j == n - 1) { 
					j = 0; 
					i++; 
				} 
			} 
		} 
	} 
}

//////recursive////////
template<class T> 
float determinant(T A[], int n) {
	int D = 0; // Initialize result 

	// Base case : if matrix contains single element 
	if (n == 1) 
		return A[index(0,0,N)]; 

	// int temp[N][N]; // To store cofactors 
	float temp[N*N]; // To store cofactors 

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

template<class T> 
void adjoint(T A[],float adj[]) { 
	int sign = 1; 
	float temp[N*N]; 

	for (int i=0; i<N; i++) { 
		for (int j=0; j<N; j++) { 
			// Get cofactor of A[i][j] 
			getCofactor(A, temp, i, j, N); 

			sign = ((i+j)%2==0)? 1: -1; 

			adj[index(j,i,N)] = (sign)*(determinant(temp, N-1)); 
		} 
	} 
} 

//////recursive////////
template<class T> 
bool inverse(T A[], float inverse[]) { 
	float det = determinant(A, N); 
	if (det == 0) { 
		cout << "Singular matrix, can't find its inverse\n"; 
		return false; 
	} 

	float adj[N*N]; 

	adjoint(A, adj); 

	for (int i=0; i<N; i++) 
		for (int j=0; j<N; j++) 
			inverse[index(i,j,N)] = adj[index(i,j,N)]/det; 

	return true; 
}  


