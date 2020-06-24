#pragma once
#include <vector>
#include <eigen3/Eigen/Dense>
#include "MatrixF.h"

#define RAND_LIMIT 3

//----------------OUTPUT INDEX----------------
#define INDEX_DIRECTION 0
#define INDEX_FRONT_SPEED 1
// #define INDEX_SIDE_SPEED 2
#define INDEX_SHOT 2

#define MEMORY_SIZE 0

using namespace Eigen;

//----------------ANN LAYERS----------------
const std::vector<int> layers({25});
const int layerSize = 1;

class ANN {
   private:
    std::vector<int> aux;  //size of each layer

    // VectorXf *intermediunOut;
    vectorF *intermediunOutTest;
    // MatrixXf *matrixArray;
    MatrixF *matrixArrayTest;

   public:
    ANN();
    ~ANN();

    void setANNParameters(int inputSize, int outputSize);
    void reset();
    void multiply();

    // inline VectorXf *getOutputPtr() { return &intermediunOut[layerSize + 1]; }
    // inline VectorXf *getInputPtr() { return &intermediunOut[0]; }
    // inline MatrixXf *getMatrixPtr() { return matrixArray; }

    // MatrixXf *setMatrix(MatrixXf *matrixArray);

    // void copyWheights(MatrixXf *matrixArray);

    inline vectorF *getOutputPtr() { return &intermediunOutTest[layerSize + 1]; }
    inline vectorF *getInputPtr() { return &intermediunOutTest[0]; }
    inline MatrixF *getMatrixPtr() { return matrixArrayTest; }

    MatrixF *setMatrix(MatrixF *matrixArray);

    void copyWheights(MatrixF *matrixArray);

};
