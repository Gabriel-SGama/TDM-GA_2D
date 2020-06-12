#pragma once
#include <eigen3/Eigen/Dense>

#define DURATION 400

//rewards
#define LIGHT_ASSAULT_SHOT_REWARD 1
#define SNIPER_SHOT_REWARD 1
#define ASSAULT_SHOT_REWARD 1

class Moderator;

#include "Screen.h"
#include "Player.h"
#include "Light_Assault.h"
#include "Sniper.h"
#include "Assault.h"
#include "Evolution.h"

// const int NUMBER_OF_TOTAL_PLAYERS = NUMBER_OF_LIGHT_ASSAULTS + NUMBER_OF_SNIPERS + NUMBER_OF_ASSAULTS;
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

//repeats the same function call for all players alive
class Moderator {
   private:
    //players:
    LightAssault *lightAssaults;
    Sniper *snipers;
    Assault *assaults;

    cv::Point **LACenter;
    cv::Point **SCenter;
    cv::Point **ACenter;

    int turn;

   public:
    dataOfBestPlayers_t *bestLightAssault;
    dataOfBestPlayers_t *bestSniper;
    dataOfBestPlayers_t *bestAssault;

    float lightAssaultScore;
    float sniperScore;
    float assaultScore;

    cv::Point **playersCenter;

    Moderator();
    ~Moderator();

    Screen *screen;  //commun screen obj

    //initial values:
    void setPlayerCenterPtr(Player *players, int offset);
    void setPlayerCenterPtr(Player *players, cv::Point** centerPtr);
    inline cv::Point **getPlayersCenterPtr() {
        return playersCenter;
    }


    void setInicialPosAll(cv::Point *inicialPos, int start);

    void setInicialPos(Player *players, cv::Point initialPos);

    void setScreen(Screen *screen);
    void setAllPlayersValues();
    void setPlayersValues(int& playerNumber ,Player *players, cv::Point** centerPtr);

    //draw functions
    void drawAllPlayers();
    void drawPlayers(Player *players);

    //vision:
    void updateAllPlayersVision();
    void updatePlayersVision(Player *players);

    //Conflicts:
    void conflictsAllPlayers();
    void conflictsPlayers(Player *players);

    void shotPlayer(Player *shooter, enemyInfo_t enemyInfo);
    int findPlayer(Player *shooter, Player *players, cv::Point enemyPoint);

    void checkAllPlayersLife();
    void checkPlayersLife(Player *players);

    //mode:
    void moveAllPlayers();
    void movePlayers(Player *players);

    //ANN:
    void defineAllPlayersInput();
    void definePlayersInput(Player *players);

    void multiplyAllPlayers();
    void multiplyPlayers(Player *players);

    void calculateScore();

    //reset:
    void resetAllPlayers(bool resetScore);
    void resetPlayers(Player *players, int life, bool resetScore);

    //get best players
    inline LightAssault *getLightAssaults() {
        return lightAssaults;
    }
    inline Sniper *getSnipers() {
        return snipers;
    }
    inline Assault *getAssaults() {
        return assaults;
    }

    //weights
    void setAllWeights(LightAssault *lightAssaults, Sniper *snipers, Assault *assaults);
    void setWeights(Player *bestPlayer, Player *players);

    void copyAllWeights(LightAssault *lightAssaults, Sniper *snipers, Assault *assaults);
    void copyWeights(Player *bestPlayer, Player *players);

    void setAllWeightsOneMatrix(MatrixXf *inocentMatrix, MatrixXf *sniperMatrix, MatrixXf *detectiveMatrix);

    void game();
    void gameOfBest();
};
