#pragma once
#include <eigen3/Eigen/Dense>

#include "Moderator.h"

using namespace Eigen;

#define POP_SIZE 4
#define TOURNAMENT_K 4

const int TOTAL_INOCENTS = NUMBER_OF_INOCENTS * POP_SIZE;
const int TOTAL_TRAITORS = NUMBER_OF_TRAITORS * POP_SIZE;
const int TOTAL_DETECTIVES = NUMBER_OF_DETECTIVES * POP_SIZE;

class Evolution
{
private:
    
    Moderator *inocentsTraining;
    Moderator *traitorsTraining;

    float bestInocentTeamScore;
    float bestTraitorTeamScore;

    Player **allInocents;
    Player **allTraitors;
    Player **allDetectives;

    ANN *inocentsChilds;
    ANN *traitorsChilds;
    ANN *detectivesChilds;

public:
    Moderator *bestPlayers;

    Moderator *bestInocents;
    Moderator *bestTraitors;

    Evolution(/* args */);
    ~Evolution();

    void setParam(Moderator *moderators);
    void createANN(ANN *childs, int NUMBER_OF_PLAYERS, int inputSize, int outputSize);
    void setPlayersPtr();

    void game();

    void eletismAll();
    void eletism(Player *players, int NUMBER_OF_PLAYERS, MatrixXf *matrixOfBest);
    void tournamentAll();
    void tournament(Player **players, int NUMBER_OF_PLAYERS, ANN *childs, int indexOfBest);

    void mutation(MatrixXf *matrixArray);
    void crossover();

    void reset();
};
