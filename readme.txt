# GPUProject
Parallelizing logistic regression and comparing it to sequential version

To generate random data:
g++ -o randata generateRanData.cpp
output: ./randata 100 10

To compile sequential logistic regression code:
g++ -o seqreg 02.regressMat.00.cpp 
output: ./seqreg 100 10

To compile cuda logistic regression code:
1. load cuda 9 for cuda code: module load cuda-9.1
2. compile with: nvcc -o cudareg 01.regressMatCUDA.00.cu
3. get output: ./cudareg 100 10
4. For time: time ./cudareg 100 10
   For profiling: nvprof ./cudareg 100 10


*Compile on snappy3 for sequential code*