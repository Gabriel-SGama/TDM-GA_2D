#pragma once

#include <ctype.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace CUDA{
    void matrixMultiplication(float *matrix, float *vector, float *resultVector, int lines, int coluns);
}