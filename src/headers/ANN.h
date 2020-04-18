#pragma once

#include <vector>
#include <eigen3/Eigen/Dense>

#define RAND_LIMIT 1

//output indexs
#define INDEX_POSI_X 0
#define INDEX_POSI_Y 1
#define INDEX_SHOT 2

//populatio param
#define POP_SIZE 5

using namespace Eigen;

const std::vector<int> layers({70, 30, 9});
//const std::vector<int> layers({3, 2});

class ANN
{
private:
    std::vector<int> aux; //size of each layer

    VectorXf input;
    VectorXf output;

    VectorXf *intermediunOut;
    MatrixXf *matrixArray;

public:
    ANN(int inputSize, int outputSize);
    ~ANN();

    void setANNParameters();
    void multiply();

    inline VectorXf *getOutputPtr() { return &output; }
    inline VectorXf *getInputPtr() { return &input; }
    inline MatrixXf *getMatrixPtr() { return matrixArray; }
    inline void setMatrix(MatrixXf *matrixArray) { this->matrixArray = matrixArray; }
};
