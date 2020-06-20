#pragma once
#include <eigen3/Eigen/Dense>

#include "Moderator.h"

using namespace Eigen;

//----------------EVOLUTION VALUES----------------
#define POP_SIZE 15
const int TOURNAMENT_K = 3;
typedef struct topScore_t {
    float BLAS;
    float BSS;
    float BAS;

} topSocre_t;
class Evolution {
   private:
    int TOTAL_NUMBER_OF_PLAYERS;

    //----------------TRAINING MODERATORS----------------
    Moderator *lightAssaultTraining;
    Moderator *snipersTraining;
    Moderator *assaultsTraining;

    //----------------TRAINING PLAYERS----------------
    Player **allLightAssaults;
    Player **allSnipers;
    Player **allAssaults;

    //----------------TRAINING CHILDS----------------
    ANN *lightAssaultChilds;
    ANN *snipersChilds;
    ANN *assaultsChilds;

   public:
    //----------------TRAINING TEAM SCORES----------------
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
    void createANN(ANN *childs, int inputSize, int outputSize);
    void setPlayersPtr();

    void game();

    void eletismAll();
    void eletism(Player *players, MatrixXf *matrixOfBest);
    void tournamentAll();
    void tournament(Player **players, ANN *childs);

    void mutation(MatrixXf *matrixArray);

    void genocideAll();
    void genocide(Player **players);

    topScore_t setBestIndvs();

    void reset();
};
