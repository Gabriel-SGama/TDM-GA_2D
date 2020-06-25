#pragma once
#include <eigen3/Eigen/Dense>

#include "Moderator.h"

using namespace Eigen;

const cv::Point initialPos[] = {cv::Point(LENGTH - 300, HEIGHT - 250), cv::Point(LENGTH - 400, 150), cv::Point(0, 150)};

//----------------EVOLUTION VALUES----------------
#define POP_SIZE 15
const int TOURNAMENT_K = 3;
typedef struct scoreData_t {
//----------------BEST SCORES----------------
    float BLAS;
    float BSS;
    float BAS;
//----------------MEDIUN SOCRES----------------
    float MLAS;
    float MSS;
    float MAS;
} scoreData_t;

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
    // void eletism(Player *players, MatrixF *matrixOfBest);
    void tournamentAll();
    void tournament(Player **players, ANN *childs);

    // void mutation(MatrixXf *matrixArray);
    void mutation(MatrixF *matrixArray);

    void genocideAll();
    void genocide(Player **players);

    scoreData_t setBestIndvs();

    void saveANNAll(const char* fileName);
    void saveANN(ANN* bestMatrix, std::ofstream* fileObj);

    void readANNAll(const char* fileName);
    void readANN(ANN* bestMatrix, std::ifstream* fileObj);


    void reset();
};
