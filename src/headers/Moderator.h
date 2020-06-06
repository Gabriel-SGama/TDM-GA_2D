#pragma once
#include <eigen3/Eigen/Dense>

#define DURATION 400

//rewards
#define SHOT_REWARD 1

class Moderator;
#include "Player.h"

const int NUMBER_OF_TOTAL_PLAYERS = 2*NUMBER_OF_PLAYERS;

using namespace Eigen;
//repeats the same function call for all players alive
class Moderator {
   private:
    //players:
    Player *trainingTeam;
    Player *bestTeam;

   public:
    ANN* trainingANN;
    ANN* bestANN;

    float trainingScore;

    // cv::Point **playersCenter;
    cv::Point **trainingCenter;
    cv::Point **bestCenter;

    Moderator();
    ~Moderator();

    Screen *screen;  //commun screen obj

    //initial values:
    void setModerator();
    void setPlayerCenterPtr(Player *players, cv::Point** pointPtr);
    // inline cv::Point **getPlayersCenterPtr() {
    //     return playersCenter;
    // }

    void setScreen(Screen *screen);
    void setAllPlayersValues();
    void setPlayersValues(Player *players, cv::Point** centerPtr, int playerType, cv::Scalar color, cv::Scalar ray);

    //draw functions
    void drawAllPlayers();
    void drawPlayers(Player *players);

    //vision:
    void updateAllPlayersVision();
    void updatePlayersVision(Player *players);

    //Conflicts:
    void conflictsAllPlayers();
    void conflictsPlayers(Player *players, int numberOfrays);

    void shotPlayer(Player *shooter, enemyInfo_t enemyInfo);
    int findPlayer(Player *shooter, Player *players, cv::Point enemyPoint);

    bool checkAllPlayersLife();
    bool checkPlayersLife(Player *players);

    //mode:
    void moveAllPlayers();
    void movePlayers(Player *players);

    //ANN:
    float getScore();
    MatrixXf* getMatrixPtr(int id);

    void defineAllPlayersInput();
    void definePlayersInput(Player *players);

    void multiplyAllPlayers();
    void multiplyPlayers(ANN* ann,Player *players);

    //reset:
    void resetAllPlayers(bool resetScore);
    void resetPlayers(Player *players, bool resetScore);

    //get best players
    inline Player *getTrainingPlayers() {
        return trainingTeam;
    }
    inline Player *getBestTeam() {
        return bestTeam;
    }

    void setAllWeightsMod(ANN *trainingMatrix, ANN *bestMatrix);

    void setInicialPosAll(cv::Point *inicialPos, int start);
    void setInicialPos(Player* players, cv::Point initialPos);

    void setScore(float score);
    MatrixXf* setMatrix(MatrixXf* matrixs, int id);
    ANN* getANN(int id);

    void game();
    void gameOfBest();
};
