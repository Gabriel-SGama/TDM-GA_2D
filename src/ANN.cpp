#include <iostream>

#include "headers/ANN.h"

ANN::ANN(/* args */)
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

    input.Random(inputSize);
    output.Random(outputSize);
    aux.push_back(inputSize);

    for (i = 0; i < layers.size(); i++)
    {
        aux.push_back(layers[i]);
    }

    aux.push_back(outputSize);

    intermediunOut.resize(layers.size());
    matrixArray.resize(aux.size() - 1);

    for (i = 0; i < aux.size() - 1; i++)
    {
        matrixArray[i] = MatrixXf::Random(aux[i + 1], aux[i]);
        matrixArray[i] = (matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], 1.)) * RAND_LIMIT;
        matrixArray[i] = matrixArray[i] + MatrixXf::Constant(aux[i + 1], aux[i], -RAND_LIMIT);
        std::cout << "matrix" << i << std::endl;
        std::cout << matrixArray[i] << std::endl;
        /*
        if (i == 0)
        {
            std::cout << "input:" << std::endl;
            std::cout << input << std::endl;
        }
        */
        /*
        if (i < layers.size())
        {
            std::cout << "vector" << i << std::endl;
            intermediunOut[i] = VectorXf(layers[i]);
            std::cout << intermediunOut[i] << std::endl;
        }*/
    }

    //std::cout << "output" << std::endl;
    //std::cout << output << std::endl;

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

    std::cout << "output" << std::endl;
    std::cout << output << std::endl;
}