#pragma once
#include <iostream>

#include <cstdint>
#include <cstring>
#include <fstream>

class vectorF {
   public:
    float *vector;

    int size;
    int memSize;

    vectorF();
    vectorF(int size);
    ~vectorF();

    void createVector(int size);

    //----------------OVERLOADS----------------
    void operator=(const vectorF &vec);
    
    float operator[](const int &index);
};

class MatrixF {
   public:
    float *matrix;

    int lines;
    int coluns;
    int memSize;


    //----------------CREATION----------------
    MatrixF();
    MatrixF(int lines, int coluns, float limits = 0);
    ~MatrixF();
    void createMatrix(int lines, int coluns, float limits = 0);
    
    void print();
    
    //----------------SAVES MATRIX ON FILE----------------
    void writeMatrixToFile(std::ofstream* fileObj);
    void readMatrixFromFile(std::istream* fileObj);

    //----------------OVERLOADS----------------
    vectorF operator*(const vectorF &vec);
    MatrixF operator+(const MatrixF &matrix);
    MatrixF operator/(const float &val);
    
    void operator=(const MatrixF &matrix);
};