#pragma once
#include <vector>
#include <eigen3/Eigen/Dense>

#define RAND_LIMIT 1

//output indexs
#define INDEX_DIRECTION 0
#define INDEX_FRONT_SPEED 1
#define INDEX_SIDE_SPEED 2
#define INDEX_SHOT 3

using namespace Eigen;

const std::vector<int> layers({55});
const int layerSize = 1;

class ANN {
   private:
    std::vector<int> aux;  //size of each layer

    VectorXf input;   //ann input
    VectorXf output;  //ann output

    VectorXf *intermediunOut;
    MatrixXf *matrixArray;

   public:
    ANN();
    ~ANN();

    void setANNParameters(int inputSize, int outputSize);
    void reset();
    VectorXf* multiply();

    inline VectorXf *getOutputPtr() { return &output; }
    inline VectorXf *getInputPtr() { return &input; }
    inline MatrixXf *getMatrixPtr() { return matrixArray; }

    MatrixXf *setMatrix(MatrixXf *matrixArray);

    void copyWheights(MatrixXf *matrixArray);
};
