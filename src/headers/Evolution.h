#pragma once
#include <eigen3/Eigen/Dense>

#include "Moderator.h"

using namespace Eigen;

#define TOURNAMENT_K 4

class Evolution
{
private:
    Moderator *moderator;

    ANN *inocentsChilds;
    ANN *traitorsChilds;
    ANN *detectivesChilds;

public:
    Evolution(/* args */);
    ~Evolution();

    void setParam(Moderator *moderator);
    void createANN(ANN *childs, int NUMBER_OF_PLAYERS, int inputSize, int outputSize);

    void eletismAll();
    void eletism(Player *players, int NUMBER_OF_PLAYERS, MatrixXf *matrixOfBest);
    void tournamentAll();
    void tournament(Player *players, int NUMBER_OF_PLAYERS, ANN *childs, int indexOfBest);

    void mutation(MatrixXf *matrixArray);
    void crossover();
};
