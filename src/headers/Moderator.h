#pragma once
#include <eigen3/Eigen/Dense>

#define DURATION 400

//----------------REWARD VALUES----------------
#define LIGHT_ASSAULT_SHOT_REWARD 1
#define SNIPER_SHOT_REWARD 1
#define ASSAULT_SHOT_REWARD 1

#define INICIAL_SCORE -1000000

class Moderator;

#include "Screen.h"
#include "Light_Assault.h"
#include "Sniper.h"
#include "Assault.h"


const int NUMBER_OF_TOTAL_PLAYERS = 3 * NUMBER_OF_PLAYERS;

using namespace Eigen;
typedef struct dataOfBestPlayers_t {
    int index;
    float score;
    Player *player;

    dataOfBestPlayers_t()
        : index(0),
          score(-100){};

} dataOfBestPlayers_t;

class Moderator {
   private:
    //----------------PLAYERS----------------
    LightAssault *lightAssaults;
    Sniper *snipers;
    Assault *assaults;

    //----------------PLAYERS CENTER----------------
    cv::Point **LACenter;
    cv::Point **SCenter;
    cv::Point **ACenter;

    int turn;

   public:
    //----------------BEST PLAYERS----------------
    dataOfBestPlayers_t *bestLightAssault;
    dataOfBestPlayers_t *bestSniper;
    dataOfBestPlayers_t *bestAssault;

    //----------------BEST PLAYERS SCORE----------------
    float lightAssaultScore;
    float sniperScore;
    float assaultScore;

    cv::Point **playersCenter;

    Moderator();
    ~Moderator();

    Screen *screen;  //commun screen obj

    //----------------INICIAL VALUES----------------
    void setPlayerCenterPtr(Player *players, int offset);
    void setPlayerCenterPtr(Player *players, cv::Point** centerPtr);
    inline cv::Point **getPlayersCenterPtr() { return playersCenter; }


    void setInicialPosAll(cv::Point *inicialPos, int start);

    void setInicialPos(Player *players, cv::Point initialPos);

    void setScreen(Screen *screen);
    void setAllPlayersValues();
    void setPlayersValues(int& playerNumber ,Player *players, cv::Point** centerPtr);

    //----------------DRAW FUNCTIONS----------------
    void drawAllPlayers();
    void drawPlayers(Player *players);

    //----------------VISION----------------
    void updateAllPlayersVision();
    void updatePlayersVision(Player *players);

    //----------------CONFLICTS----------------
    void conflictsAllPlayers();
    void conflictsPlayers(Player *players);

    void shotPlayer(Player *shooter, enemyInfo_t enemyInfo);
    int findPlayer(Player *shooter, Player *players, cv::Point enemyPoint);

    void checkAllPlayersLife();
    void checkPlayersLife(Player *players);

    //----------------MOVE----------------
    void moveAllPlayers();
    void movePlayers(Player *players);

    //----------------ANN----------------
    void defineAllPlayersInput();
    void definePlayersInput(Player *players);

    void multiplyAllPlayers();
    void multiplyPlayers(Player *players);

    void calculateScore();

    //----------------RESET----------------
    void resetAllPlayers(bool resetScore);
    void resetPlayers(Player *players, int life, bool resetScore);

    //----------------GET BEST PLAYERS----------------
    inline LightAssault *getLightAssaults() {
        return lightAssaults;
    }
    inline Sniper *getSnipers() {
        return snipers;
    }
    inline Assault *getAssaults() {
        return assaults;
    }

    //----------------WEIGHTS----------------
    void setAllWeights(LightAssault *lightAssaults, Sniper *snipers, Assault *assaults);
    void setWeights(Player *bestPlayer, Player *players);

    void copyAllWeights(LightAssault *lightAssaults, Sniper *snipers, Assault *assaults);
    void copyWeights(Player *bestPlayer, Player *players);

    void setAllWeightsOneMatrix(MatrixXf *inocentMatrix, MatrixXf *sniperMatrix, MatrixXf *detectiveMatrix);

    void game();
    void gameOfBest();
};
