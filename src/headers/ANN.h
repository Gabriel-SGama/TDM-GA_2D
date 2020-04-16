#pragma once

#include <vector>
#include <eigen3/Eigen/Dense>

#define RAND_LIMIT 20

using namespace Eigen;

//const std::vector<int> layers({70, 30, 9});
const std::vector<int> layers({5, 3});

class ANN
{
private:
    std::vector<int> aux;

    VectorXd input;
    VectorXd output;

    std::vector<VectorXf> intermediunOut;
    std::vector<MatrixXf> matrixArray;

public:
    ANN(/* args */);
    ~ANN();

    void setANNParameters(int inputSize, int outputSize);
};
