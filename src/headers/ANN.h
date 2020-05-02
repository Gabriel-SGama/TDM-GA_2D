#pragma once

#include <vector>
#include <eigen3/Eigen/Dense>

#define RAND_LIMIT 0.5

//output indexs
#define INDEX_POSI_X 0
#define INDEX_POSI_Y 1
#define INDEX_SHOT 2

using namespace Eigen;

const std::vector<int> layers({80});
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
    ANN();
    ~ANN();

    void setANNParameters(int inputSize, int outputSize);
    void multiply();

    inline VectorXf *getOutputPtr() { return &output; }
    inline VectorXf *getInputPtr() { return &input; }
    inline MatrixXf *getMatrixPtr() { return matrixArray; }
    MatrixXf *setMatrix(MatrixXf *matrixArray);

    void copyWheights(MatrixXf *matrixArray);

    //evolution:
    void simpleBreeding(MatrixXf *matrixArray);
};
