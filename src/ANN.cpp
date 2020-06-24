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

    // intermediunOut = new VectorXf[layerSize + 2];
    intermediunOutTest = new vectorF[layerSize + 2];

    // matrixArray = new MatrixXf[layerSize + 1];
    matrixArrayTest = new MatrixF[layerSize + 1];

    for (i = 0; i < layerSize + 1; i++) {
        // matrixArray[i] = MatrixXf::Random(aux[i + 1], aux[i]);
        // matrixArray[i] = (matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], 1.)) * RAND_LIMIT;
        // matrixArray[i] = matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], -RAND_LIMIT);
        matrixArrayTest[i].createMatrix(aux[i + 1], aux[i], RAND_LIMIT);
    }

    for (i = 0; i < layerSize + 2; i++) {
        // intermediunOut[i] = VectorXf(aux[i]);
        intermediunOutTest[i].createVector(aux[i]);
    }
}

void ANN::reset() {
    for (int i = 0; i < layerSize + 1; i++) {
        // matrixArray[i] = MatrixXf::Random(aux[i + 1], aux[i]);
        // matrixArray[i] = (matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], 1.)) * RAND_LIMIT;
        // matrixArray[i] = matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], -RAND_LIMIT);
        matrixArrayTest[i].createMatrix(aux[i+1], aux[i], RAND_LIMIT);
    }
}

void ANN::multiply() {
    int i;
    int j;


    for (i = 1; i < layerSize + 2; i++) {
        // intermediunOut[i] = matrixArray[i - 1] * intermediunOut[i - 1];
        intermediunOutTest[i] = matrixArrayTest[i-1] * intermediunOutTest[i-1];

        // for (j = 0; j < intermediunOut[i].size(); j++) {
        //     intermediunOut[i][j] = tanh(intermediunOut[i][j]);
        // }
        
        for (j = 0; j < intermediunOutTest[i].size; j++) {
            intermediunOutTest[i].vector[j] = tanh(intermediunOutTest[i][j]);
        }
    }
}

// MatrixXf *ANN::setMatrix(MatrixXf *matrixArray) {
//     MatrixXf *temp;
//     temp = this->matrixArray;
//     this->matrixArray = matrixArray;

//     return temp;
// }

// void ANN::copyWheights(MatrixXf *matrixArray) {
//     for (int i = 0; i < layerSize + 1; i++)
//         this->matrixArray[i] = matrixArray[i];
// }


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