#pragma once

#include <vector>
#include <eigen3/Eigen/Dense>

#define RAND_LIMIT 1

using namespace Eigen;

const std::vector<int> layers({70, 30, 9});
//const std::vector<int> layers({3, 2});

class ANN
{
private:
    std::vector<int> aux; //size of each layer

    VectorXf input;
    VectorXf output;

    std::vector<VectorXf> intermediunOut;
    std::vector<MatrixXf> matrixArray;

public:
    ANN(/* args */);
    ~ANN();

    void setANNParameters(int inputSize, int outputSize);
    void multiply();

    inline VectorXf *getOutputPtr() { return &output; };
};
