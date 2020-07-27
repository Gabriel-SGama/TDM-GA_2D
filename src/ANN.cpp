#include <iostream>

#include "headers/ANN.h"

ANN::ANN() {
}

ANN::~ANN() {
}

void ANN::setANNParameters(int inputSize, int outputSize) {
    int i;

    aux.push_back(inputSize);

    for (i = 0; i < layerSize; i++) {
        aux.push_back(layers[i]);
    }

    aux.push_back(outputSize);

    bias = new vectorF[layerSize + 1];
    intermediunOut = new vectorF[layerSize + 2];
    matrixArray = new MatrixF[layerSize + 1];

    for (i = 0; i < layerSize + 1; i++){
        bias[i].createVector(aux[i+1], RAND_LIMIT);
        matrixArray[i].createMatrix(aux[i + 1], aux[i], RAND_LIMIT);
    }

    for (i = 0; i < layerSize + 2; i++)
        intermediunOut[i].createVector(aux[i]);
}

void ANN::reset() {
    for (int i = 0; i < layerSize + 1; i++) {
        bias[i].createVector(aux[i+1], RAND_LIMIT);
        matrixArray[i].createMatrix(aux[i+1], aux[i], RAND_LIMIT);
    }
}

float sigmoid(float z){
    return 1/(1 + exp(-z));
}

void ANN::multiply() {
    int i;
    int j;

    for (i = 1; i < layerSize + 2; i++) {
        intermediunOut[i] = matrixArray[i-1] * intermediunOut[i-1];

        for (j = 0; j < intermediunOut[i].size; j++) {
            intermediunOut[i].vector[j] = tanh(intermediunOut[i][j] + bias[i-1][j]);
            // intermediunOut[i].vector[j] = tanh(intermediunOut[i][j]);
        }
    }
}

MatrixF *ANN::setMatrix(MatrixF *matrixArray) {
    MatrixF *temp;
    temp = this->matrixArray;
    this->matrixArray = matrixArray;

    return temp;
}

vectorF* ANN::setBias(vectorF *bias){
    vectorF *temp;
    temp = this->bias;
    this->bias = bias;

    return temp;
}

void ANN::copyWheights(MatrixF *matrixArray, vectorF* bias) {
    for (int i = 0; i < layerSize + 1; i++){
        this->bias[i] = bias[i];
        this->matrixArray[i] = matrixArray[i];
    }
}

// void ANN::copyWheights(MatrixF *matrixArray) {
//     for (int i = 0; i < layerSize + 1; i++){
//         this->matrixArray[i] = matrixArray[i];
//     }
// }