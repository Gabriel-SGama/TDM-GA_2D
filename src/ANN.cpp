#include "headers/ANN.h"

#include <iostream>

ANN::ANN() {
}

ANN::~ANN() {
}

void ANN::setANNParameters(int inputSize, int outputSize) {
    int i;

    input.resize(inputSize);
    output.resize(outputSize);

    //output[INDEX_SHOT] = -1;

    aux.push_back(inputSize);

    for (i = 0; i < layerSize; i++) {
        aux.push_back(layers[i]);
    }

    aux.push_back(outputSize);

    intermediunOut = new VectorXf[layerSize + 1];  //if layer size == 0, no error
    matrixArray = new MatrixXf[layerSize + 1];

    for (i = 0; i < layerSize + 1; i++) {
        matrixArray[i] = MatrixXf::Random(aux[i + 1], aux[i]);
        matrixArray[i] = (matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], 1.)) * RAND_LIMIT;
        matrixArray[i] = matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], -RAND_LIMIT);
    }

    for (i = 0; i < layerSize; i++) {
        intermediunOut[i] = VectorXf(layers[i]);
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

    //std::cout << "input: " << std::endl;
    //std::cout << input << std::endl;

    if (layerSize > 0) {
        intermediunOut[0] = matrixArray[0] * input;

        //std::cout <<"inter: " << std::endl;
        //std::cout << intermediunOut[0] << std::endl;

        for (j = 0; j < intermediunOut[0].size(); j++) {
            intermediunOut[0][j] = tanh(intermediunOut[0][j]);
        }

        for (i = 1; i < layerSize; i++) {
            intermediunOut[i] = matrixArray[i] * intermediunOut[i - 1];

            for (j = 0; j < intermediunOut[i].size(); j++) {
                intermediunOut[i][j] = tanh(intermediunOut[i][j]);
            }
        }

        output = matrixArray[layerSize] * intermediunOut[layerSize - 1];
    } else {
        output = matrixArray[0] * input;
    }

    //std::cout << "matrix: " << matrixArray[0] << std::endl;

    for (j = 0; j < output.size(); j++) {
        output[j] = tanh(output[j]);
    }

    //std::cout <<"output: " << std::endl;
    //std::cout << output << std::endl;
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