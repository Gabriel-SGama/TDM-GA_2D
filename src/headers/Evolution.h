#pragma once
#include "Moderator.h"

using namespace Eigen;

#define POP_SIZE 60
#define INICIAL_SCORE -1000000

//number of players for tournament
const int TOURNAMENT_K = 4;

class Evolution {
   private:
    int TOTAL_NUMBER_PLAYERS;

    Moderator *training;

    ANN *trainingChilds;

   public:
    float bestTeamScore;

    Moderator *bestTeams;

    Moderator *bestTrainingTeam;

    ANN *bestTrainingANN;

    Evolution();
    ~Evolution();

    void setParam();
    void createANN(int inputSize, int outputSize);
    void setPlayersPtr();

    void game();

    void eletism();
    void tournament();

    void tournamentMod();
    void mutation(MatrixXf *matrixArray);

    void genocide();

    void reset();
};
