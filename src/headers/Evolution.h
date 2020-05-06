#pragma once
#include <eigen3/Eigen/Dense>

#include "Moderator.h"

using namespace Eigen;

#define POP_SIZE 15
#define INICIAL_SCORE -1000000

const int TOURNAMENT_K_LIGHT_ASSAULTS = 6;
const int TOURNAMENT_K_SNIPERS = 6;
const int TOURNAMENT_K_ASSAULTS = 6;

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
    //void crossover();

    void genocideAll();
    void genocide(Player **players, int NUMBER_OF_PLAYERS);

    void setBestIndvs();

    void reset();
};
