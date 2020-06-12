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

    intermediunOut = new VectorXf[layerSize + 2];
    matrixArray = new MatrixXf[layerSize + 1];

    for (i = 0; i < layerSize + 1; i++) {
        matrixArray[i] = MatrixXf::Random(aux[i + 1], aux[i]);
        matrixArray[i] = (matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], 1.)) * RAND_LIMIT;
        matrixArray[i] = matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], -RAND_LIMIT);
    }

    for (i = 0; i < layerSize + 2; i++) {
        intermediunOut[i] = VectorXf(aux[i]);
    }
}

void ANN::reset() {
    for (int i = 0; i < layerSize + 1; i++) {
        matrixArray[i] = MatrixXf::Random(aux[i + 1], aux[i]);
        matrixArray[i] = (matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], 1.)) * RAND_LIMIT;
        matrixArray[i] = matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], -RAND_LIMIT);
    }
}

void ANN::multiply() {
    int i;
    int j;


    for (i = 1; i < layerSize + 2; i++) {
        intermediunOut[i] = matrixArray[i - 1] * intermediunOut[i - 1];

        for (j = 0; j < intermediunOut[i].size(); j++) {
            intermediunOut[i][j] = tanh(intermediunOut[i][j]);
        }
    }

}

MatrixXf *ANN::setMatrix(MatrixXf *matrixArray) {
    MatrixXf *temp;
    temp = this->matrixArray;
    this->matrixArray = matrixArray;

    return temp;
}

void ANN::copyWheights(MatrixXf *matrixArray) {
    for (int i = 0; i < layerSize + 1; i++)
        this->matrixArray[i] = matrixArray[i];
}

void ANN::simpleBreeding(MatrixXf *matrixArray) {
    for (int i = 0; i < layerSize + 1; i++)
        this->matrixArray[i] = (matrixArray[i] + this->matrixArray[i]) / 2;
}