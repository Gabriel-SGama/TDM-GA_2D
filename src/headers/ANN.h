#pragma once
#include <vector>
#include <eigen3/Eigen/Dense>
#include "MatrixF.h"

#define RAND_LIMIT 3

//----------------OUTPUT INDEX----------------
#define INDEX_DIRECTION 0
#define INDEX_FRONT_SPEED 1
#define INDEX_SHOT 2
#define INDEX_PLAYER_TYPE_CHANGE 3

#define MEMORY_SIZE 0

using namespace Eigen;

//----------------ANN LAYERS----------------
const std::vector<int> layers({35});
const int layerSize = 1;

class ANN {
   private:
    std::vector<int> aux;  //size of each layer

    //----------------ANN----------------
    vectorF *intermediunOut;
    vectorF *bias;
    MatrixF *matrixArray;

   public:
    ANN();
    ~ANN();

    void setANNParameters(int inputSize, int outputSize);
    void reset();
    void multiply();

    //----------------GET & SET----------------
    inline vectorF *getOutputPtr() { return &intermediunOut[layerSize + 1]; }
    inline vectorF *getInputPtr() { return &intermediunOut[0]; }
    inline vectorF *getBiasPtr() { return bias; }
    inline MatrixF *getMatrixPtr() { return matrixArray; }

    MatrixF *setMatrix(MatrixF *matrixArray);
    vectorF *setBias(vectorF *bias);

    //----------------COPY----------------
    void copyWheights(MatrixF *matrixArray, vectorF* bias);
    // void copyWheights(MatrixF *matrixArray);
};
