#include "headers/cudaStuff.cuh"


__global__ void multi(float *matrix, float *vector, float *resultVector, int lines, int coluns) {

    if(threadIdx.x > lines)
        return;
    
    int index = threadIdx.x*coluns;
    float tempSum = 0;

    for(int colunIdx = 0; colunIdx < coluns; colunIdx++){
        tempSum += matrix[index + colunIdx]*vector[colunIdx];
    }

    resultVector[threadIdx.x] = tanh(tempSum);
}

void CUDA::matrixMultiplication(float *d_matrix, float *d_vector, float *d_resultVector, int lines, int coluns){
    multi <<<1, lines>>> (d_matrix, d_vector, d_resultVector, lines, coluns);
}
