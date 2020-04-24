#pragma once
#include <eigen3/Eigen/Dense>

#include "Moderator.h"

using namespace Eigen;

#define POP_SIZE 3
#define INICIAL_SCORE -100000
//#define TOURNAMENT_K 4

const int TOTAL_INOCENTS = NUMBER_OF_INOCENTS * POP_SIZE;
const int TOTAL_TRAITORS = NUMBER_OF_TRAITORS * POP_SIZE;
const int TOTAL_DETECTIVES = NUMBER_OF_DETECTIVES * POP_SIZE;

const int TOUTNAMENT_K_INOCENTS = (NUMBER_OF_INOCENTS * POP_SIZE) / 4;
const int TOUTNAMENT_K_TRAITORS = (NUMBER_OF_TRAITORS * POP_SIZE) / 4;
const int TOUTNAMENT_K_DETECTIVES = (NUMBER_OF_DETECTIVES * POP_SIZE) / 4;

class Evolution
{
private:
    Moderator *inocentsTraining;
    Moderator *traitorsTraining;

    Player **allInocents;
    Player **allTraitors;
    Player **allDetectives;

    ANN *inocentsChilds;
    ANN *traitorsChilds;
    ANN *detectivesChilds;

public:
    float bestInocentTeamScore;
    float bestTraitorTeamScore;
    float bestDetectiveTeamScore;

    Moderator *bestIndvs;
    Moderator *bestTeams;

    Moderator *bestInocents;
    Moderator *bestTraitors;
    Moderator *bestDetectives;

    ANN *bestInocentANN;
    ANN *bestTraitorANN;
    ANN *bestDetectiveANN;

    Evolution();
    ~Evolution();

    void setParam(Moderator *moderators);
    void createANN(ANN *childs, int NUMBER_OF_PLAYERS, int inputSize, int outputSize);
    void setPlayersPtr();

    void game();

    void eletismAll();
    void eletism(Player *players, int NUMBER_OF_PLAYERS, MatrixXf *matrixOfBest);
    void tournamentAll();
    void tournament(Player **players, int NUMBER_OF_PLAYERS, ANN *childs, int TOURNAMENT_K);

    void mutation(MatrixXf *matrixArray);
    void crossover();

    void setBestIndvs();

    void reset();
};
