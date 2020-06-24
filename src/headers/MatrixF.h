#pragma once
#include <iostream>

class vectorF {
   public:
    float *vector;

    int size;

    vectorF();
    vectorF(int size);
    ~vectorF();

    void createVector(int size);
    void operator=(const vectorF &vec);
    
    float operator[](const int &index);
};

class MatrixF {
   public:
    float *matrix;

    int lines;
    int coluns;

    MatrixF();
    MatrixF(int lines, int coluns, float limits = 0);
    ~MatrixF();

    void createMatrix(int lines, int coluns, float limits = 0);
    void print();
    

    //overloads:
    vectorF operator*(const vectorF &vec);
    MatrixF operator+(const MatrixF &vec);
    MatrixF operator/(const float &val);
    
    void operator=(const MatrixF &matrix);
};

// std::ostream & operator << (std::ostream &out, const Matrix& matrix) {
//     int line;
//     int colun;

//     int lineOffset = 0;

//     for (line = 0; line < matrix.lines; line++) {
//         for (colun = 0; colun < matrix.coluns; colun++) {
//             out << matrix.matrix[lineOffset + colun];
//             out << " ";
//         }
    
//         lineOffset += matrix.coluns;
//         out << "\n";
//     }

//     return out;
// }

// std::istream & operator >> (std::istream &in, const Matrix& matrix)
// {
//     return in;
// }