#pragma once
#include <eigen3/Eigen/Dense>

#include "Moderator.h"

using namespace Eigen;

#define POP_SIZE 10
#define INICIAL_SCORE -1000000

//total pop needs to be equal to
//NUMBER_OF_LIGHT_ASSAULTS + NUMBER_OF_SNIPERS + NUMBER_OF_ASSAULTS
//and ASSAULT_POP_ASS_TRAIN > NUMBER_OF_ASSAULTS
const int LIGHT_ASSAULT_ASS_TRAIN = NUMBER_OF_LIGHT_ASSAULTS - 3;
const int SNIPER_POP_DET_TRAIN = NUMBER_OF_SNIPERS - 1;
const int ASSAULT_POP_ASS_TRAIN = NUMBER_OF_ASSAULTS + 4;

const int TOURNAMENT_K_LIGHT_ASSAULTS = (NUMBER_OF_LIGHT_ASSAULTS * POP_SIZE) / 10;
const int TOURNAMENT_K_SNIPERS = (NUMBER_OF_SNIPERS * POP_SIZE) / 10;
const int TOURNAMENT_K_ASSAULTS = (ASSAULT_POP_ASS_TRAIN * POP_SIZE) / 10;

//static int turn = 1;
class Evolution
{
private:
    int TOTAL_NUMBER_OF_LIGHT_ASSAULTS;
    int TOTAL_NUMBER_OF_SNIPERS;
    int TOTAL_NUMER_OF_ASSAULTS;

    Moderator *lightAssaultTraining;
    Moderator *snipersTraining;
    Moderator *assaultsTraining;

    Player **allLightAssaults;
    Player **allSnipers;
    Player **allAssaults;

    ANN *lightAssaultChilds;
    ANN *snipersChilds;
    ANN *assaultsChilds;

public:
    float bestLightAssaultTeamScore;
    float bestSniperTeamScore;
    float bestAssaultTeamScore;

    Moderator *bestIndvs;
    Moderator *bestTeams;

    Moderator *bestLightAssaults;
    Moderator *bestSnipers;
    Moderator *bestAssaults;

    ANN *bestLightAssaultANN;
    ANN *bestSniperANN;
    ANN *bestAssaultANN;

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
