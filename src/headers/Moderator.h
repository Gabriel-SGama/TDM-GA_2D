#pragma once

#include <eigen3/Eigen/Dense>

#define DURATION 400

class Moderator;

#include "Screen.h"
#include "Player.h"
#include "Light_Assault.h"
#include "Sniper.h"
#include "Assault.h"

const int NUMBER_OF_TOTAL_PLAYERS = NUMBER_OF_LIGHT_ASSAULTS + NUMBER_OF_SNIPERS + NUMBER_OF_ASSAULTS;

using namespace Eigen;

typedef struct dataOfBestPlayers_t
{
    int index;
    float score;
    Player *player;

    dataOfBestPlayers_t() : index(0),
                            score(-100){};

} dataOfBestPlayers_t;

//repeats the same function call for all players alive
class Moderator
{
private:
    //players:
    LightAssault *lightAssaults;
    Sniper *snipers;
    Assault *assaults;

public:
    int NUMBER_OF_LIGHT_ASSAULT_TRAIN;
    int NUMBER_OF_SNIPER_TRAIN;
    int NUMBER_OF_ASSAULT_TRAIN;

    dataOfBestPlayers_t *bestLightAssault;
    dataOfBestPlayers_t *bestSniper;
    dataOfBestPlayers_t *bestAssault;

    float lightAssaultScore;
    float sniperScore;
    float assaultScore;

    cv::Point **playersCenter;

    Moderator();
    ~Moderator();

    Screen *screen; //commun screen obj

    //initial values:
    void setModerator(int NUMBER_LIGHT_ASSAULT_TRAIN, int NUMBER_OF_SNIPER_TRAIN, int NUMBER_OF_ASSAULT_TRAIN);
    void setPlayerCenterPtr(Player *players, int NUMBER_OF_PLAYERS, int offset);
    inline cv::Point **getPlayersCenterPtr() { return playersCenter; }

    void setScreen(Screen *screen);
    void setAllPlayersValues();
    void setPlayersValues(int &playerNumber, Player *players, int NUMBER_OF_PLAYERS);

    //draw functions
    void drawAllPlayers();
    void drawPlayers(Player *players, int NUMBER_OF_PLAYERS);

    //vision:
    void updateAllPlayersVision();
    void updatePlayersVision(Player *players, int NUMBER_OF_PLAYERS);

    //Conflicts:
    void conflictsAllPlayers();
    void conflictsPlayers(Player *players, int NUMBER_OF_PLAYERS);

    void shotPlayer(Player *shooter, enemyInfo_t enemyInfo);
    int findPlayer(Player *shooter, Player *players, int NUMBER_OF_PLAYERS, cv::Point enemyPoint);

    void checkAllPlayersLife();
    void checkPlayersLife(Player *players, int NUMBER_OF_PLAYERS);

    //mode:
    void moveAllPlayers();
    void movePlayers(Player *players, int NUMBER_OF_PLAYERS);

    //ANN:
    void defineAllPlayersInput();
    void definePlayersInput(Player *players, int NUMBER_OF_PLAYERS);

    void multiplyAllPlayers();
    void multiplyPlayers(Player *players, int NUMBER_OF_PLAYERS);

    void calculateScore();

    //reset:
    void resetAllPlayers(bool resetScore);
    void resetPlayers(Player *players, int NUMBER_OF_PLAYERS, int life, bool resetScore);

    //get best players
    inline LightAssault *getLightAssaults() { return lightAssaults; }
    inline Sniper *getSnipers() { return snipers; }
    Assault *getAssaults();

    //weights
    void setAllWeights(LightAssault *lightAssaults, Sniper *snipers, Assault *assaults);
    void setWeights(Player *bestPlayer, Player *players, int NUMBER_OF_PLAYERS);

    void copyAllWeights(LightAssault *lightAssaults, Sniper *snipers, Assault *assaults);
    void copyWeights(Player *bestPlayer, Player *players, int NUMBER_OF_PLAYERS);

    void setAllWeightsOneMatrix(MatrixXf *inocentMatrix, MatrixXf *sniperMatrix, MatrixXf *detectiveMatrix);

    void game();
    void gameOfBest();
};
