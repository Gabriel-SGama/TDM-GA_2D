#include <iostream>

#include "headers/ANN.h"

ANN::ANN()
{
}

ANN::~ANN()
{
}

void ANN::setANNParameters(int inputSize, int outputSize)
{
    unsigned int i;

    input.resize(inputSize);
    output.resize(outputSize);

    output[INDEX_SHOT] = -1;

    for (int i = 0; i < inputSize; i++)
    {
        input[i] = 1;
    }

    //input.Random(inputSize);

    //output.Random(outputSize);
    aux.push_back(inputSize);

    for (i = 0; i < layers.size(); i++)
    {
        aux.push_back(layers[i]);
    }

    aux.push_back(outputSize);

    intermediunOut = new VectorXf[layers.size()];
    matrixArray = new MatrixXf[aux.size() - 1];
    //intermediunOut.resize(layers.size());
    //matrixArray.resize(aux.size() - 1);

    for (i = 0; i < aux.size() - 1; i++)
    {
        matrixArray[i] = MatrixXf::Random(aux[i + 1], aux[i]);
        matrixArray[i] = (matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], 1.)) * RAND_LIMIT;
        matrixArray[i] = matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], -RAND_LIMIT);
    }

    for (i = 0; i < layers.size(); i++)
    {
        intermediunOut[i] = VectorXf(layers[i]);
    }
}

void ANN::multiply()
{
    intermediunOut[0] = matrixArray[0] * input;

    for (unsigned int i = 1; i < layers.size(); i++)
    {
        intermediunOut[i] = matrixArray[i] * intermediunOut[i - 1];
    }

    output = matrixArray[layers.size()] * intermediunOut[layers.size() - 1];

    if (output[0] > 4)
        output[0] = 4;

    else if (output[0] < 4)
        output[0] = -4;

    if (output[1] > 4)
        output[1] = 4;

    else if (output[1] < 4)
        output[1] = -4;

}

MatrixXf *ANN::setMatrix(MatrixXf *matrixArray)
{
    MatrixXf *temp;
    temp = this->matrixArray;
    this->matrixArray = matrixArray;

    return temp;
}

void ANN::copyWheights(MatrixXf *matrixArray)
{
    for (unsigned int i = 0; i < layers.size() + 1; i++)
    {
        this->matrixArray[i] = matrixArray[i];
    }
}

void ANN::simpleBreeding(MatrixXf *matrixArray)
{
    for (unsigned int i = 0; i < layers.size() + 1; i++)
    {
        this->matrixArray[i] = (matrixArray[i] + this->matrixArray[i]) / 2;
    }
}