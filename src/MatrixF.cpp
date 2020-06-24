#include "headers/MatrixF.h"

#include <cstdint>
#include <cstring>
#include <iostream>

MatrixF::MatrixF() {
    lines = 0;
    coluns = 0;
    matrix = nullptr;
}

MatrixF::MatrixF(int lines, int coluns, float limits) {
    this->lines = 0;
    this->coluns = 0;
    createMatrix(lines, coluns, limits);
}

void MatrixF::createMatrix(int lines, int coluns, float limits) {
    if (this->lines != 0)
        delete[] this->matrix;

    this->lines = lines;
    this->coluns = coluns;

    this->matrix = new float[lines * coluns];

    if (limits > 0) {
        int actualLimit = limits * 1000;

        for (int i = 0; i < lines * coluns; i++) {
            this->matrix[i] = ((rand() % (int)(2 * actualLimit)) - actualLimit) / 1000.0;
        }

    } else {
        for (int i = 0; i < lines * coluns; i++) {
            this->matrix[i] = 1;
        }
    }
}

MatrixF::~MatrixF() {
    if (this->matrix != nullptr)
        delete[] this->matrix;
}

vectorF MatrixF::operator*(const vectorF &vec) {
    vectorF newVector;
    newVector.createVector(this->lines);

    float *vector = newVector.vector;
    float sum;

    int line;
    int colun;

    int lineOffset = 0;

    float *currentVector = vec.vector;

    for (line = 0; line < this->lines; line++) {
        sum = 0;

        for (colun = 0; colun < this->coluns; colun++) {
            sum += this->matrix[lineOffset + colun] * currentVector[colun];
        }

        vector[line] = sum;
        lineOffset += this->coluns;
    }

    return newVector;
}

void MatrixF::operator=(const MatrixF &matrix) {
    if (this->lines == 0) {
        this->createMatrix(matrix.lines, matrix.coluns);
    }

    int totalSize = this->lines * this->coluns;
    int memorySize = totalSize * sizeof(float);

    std::memcpy(this->matrix, matrix.matrix, memorySize);
}

MatrixF MatrixF::operator+(const MatrixF &matrix) {
    int line;
    int colun;

    int lineOffset = 0;

    MatrixF resultMatrixObj;
    resultMatrixObj.createMatrix(this->lines, this->coluns);
    
    float *resultMatrix = resultMatrixObj.matrix;

    for (line = 0; line < this->lines; line++) {
        for (colun = 0; colun < this->coluns; colun++) {
            resultMatrix[lineOffset + colun] = this->matrix[lineOffset + colun] + matrix.matrix[lineOffset + colun];
        }

        lineOffset += this->coluns;
    }

    return resultMatrixObj;
}

MatrixF MatrixF::operator/(const float &val) {
    int line;
    int colun;

    int lineOffset = 0;

    MatrixF resultDivMatrixObj;
    resultDivMatrixObj.createMatrix(this->lines, this->coluns);

    float *resultMatrix = resultDivMatrixObj.matrix;

    for (line = 0; line < this->lines; line++) {
        for (colun = 0; colun < this->coluns; colun++) {
            resultMatrix[lineOffset + colun] = this->matrix[lineOffset + colun] / val;
        }
        lineOffset += this->coluns;
    }

    return resultDivMatrixObj;
}

void MatrixF::print(){
    int line;
    int colun;

    int lineOffset = 0;

    for (line = 0; line < this->lines; line++) {
        for (colun = 0; colun < this->coluns; colun++) {
            std::cout << this->matrix[lineOffset + colun];
            std::cout << " ";
        }
    
        lineOffset += this->coluns;
        std::cout << "\n";
    }

}

vectorF::vectorF() {
    size = 0;
    vector = nullptr;
}

vectorF::vectorF(int size) {
    this->size = 0;
    createVector(size);
}

vectorF::~vectorF() {
    if (this->vector != nullptr)
        delete[] this->vector;
}

void vectorF::operator=(const vectorF &vec) {
    if (this->size == 0)
        this->createVector(vec.size);

    std::memcpy(this->vector, vec.vector, vec.size * sizeof(float));
}

void vectorF::createVector(int size) {
    if (this->size != 0)
        delete[] this->vector;

    this->size = size;

    vector = new float[size];
    for (int i = 0; i < size; i++) {
        vector[i] = 1;
    }
}

float vectorF::operator[](const int &index){
    return this->vector[index];
}
