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

    intermediunOutTest = new vectorF[layerSize + 2];
    matrixArrayTest = new MatrixF[layerSize + 1];

    for (i = 0; i < layerSize + 1; i++) {
        matrixArrayTest[i].createMatrix(aux[i + 1], aux[i], RAND_LIMIT);
    }

    for (i = 0; i < layerSize + 2; i++) {
        intermediunOutTest[i].createVector(aux[i]);
    }
}

void ANN::reset() {
    for (int i = 0; i < layerSize + 1; i++) {
        matrixArrayTest[i].createMatrix(aux[i+1], aux[i], RAND_LIMIT);
    }
}

void ANN::multiply() {
    int i;
    int j;

    for (i = 1; i < layerSize + 2; i++) {
        intermediunOutTest[i] = matrixArrayTest[i-1] * intermediunOutTest[i-1];

        for (j = 0; j < intermediunOutTest[i].size; j++) {
            intermediunOutTest[i].vector[j] = tanh(intermediunOutTest[i][j]);
        }
    }
}

MatrixF *ANN::setMatrix(MatrixF *matrixArray) {
    MatrixF *temp;
    temp = this->matrixArrayTest;
    this->matrixArrayTest = matrixArray;

    return temp;
}

void ANN::copyWheights(MatrixF *matrixArray) {
    for (int i = 0; i < layerSize + 1; i++)
        this->matrixArrayTest[i] = matrixArray[i];
}