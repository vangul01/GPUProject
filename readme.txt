# GPUProject
Parallelizing logistic regression and comparing it to sequential and multicore versions

To generate random data:
g++ generateRanData.cpp
check output: ./a.out 100 10

To compile sequential logistic regression code:
g++ 01.regressMatCUDA.00.cu
check output: ./a.out 100 10

To compile cuda logistic regression code:
nvcc 01.regressMatCUDA.00.cu
check output: ./a.out 100 10

load cuda 9 for cuda code:
module load cuda-9.1

*Compile on cuda2 for cuda code* 
nvcc -o heatdist -arch=sm_60 heatdist.cu 
OR
*Compile on cuda5 for cuda code*
nvcc -o heatdist -arch=sm_35 heatdist.cu

*Compile on snappy3 for sequential code*