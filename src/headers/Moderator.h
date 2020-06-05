#pragma once
#include <eigen3/Eigen/Dense>

#define DURATION 400

//rewards
#define LIGHT_ASSAULT_SHOT_REWARD 1
#define SNIPER_SHOT_REWARD 1
#define ASSAULT_SHOT_REWARD 1

class Moderator;

#include "Light_Assault.h"
#include "Sniper.h"
#include "Assault.h"

const int NUMBER_OF_TOTAL_PLAYERS = NUMBER_OF_LIGHT_ASSAULTS + NUMBER_OF_SNIPERS + NUMBER_OF_ASSAULTS;

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

    int turn;

   public:
    ANN* lightAssaultANN;
    ANN* sniperANN;
    ANN* assaultANN;

    int NUMBER_OF_LIGHT_ASSAULT_TRAIN;
    int NUMBER_OF_SNIPER_TRAIN;
    int NUMBER_OF_ASSAULT_TRAIN;

    dataOfBestPlayers_t *bestLightAssault;
    dataOfBestPlayers_t *bestSniper;
    dataOfBestPlayers_t *bestAssault;

    float lightAssaultScore;
    float sniperScore;
    float assaultScore;

    // cv::Point **playersCenter;
    cv::Point **lightAssaultCenter;
    cv::Point **sniperCenter;
    cv::Point **assaultCenter;

    Moderator();
    ~Moderator();

    Screen *screen;  //commun screen obj

    //initial values:
    void setModerator(int NUMBER_LIGHT_ASSAULT_TRAIN, int NUMBER_OF_SNIPER_TRAIN, int NUMBER_OF_ASSAULT_TRAIN);
    void setPlayerCenterPtr(Player *players, int NUMBER_OF_PLAYERS, cv::Point** pointPtr);
    // inline cv::Point **getPlayersCenterPtr() {
    //     return playersCenter;
    // }

    void setScreen(Screen *screen);
    void setAllPlayersValues();
    void setPlayersValues(Player *players, int NUMBER_OF_PLAYERS, cv::Point** centerPtr);

    //draw functions
    void drawAllPlayers();
    void drawPlayers(Player *players, int NUMBER_OF_PLAYERS);

    //vision:
    void updateAllPlayersVision();
    void updatePlayersVision(Player *players, int NUMBER_OF_PLAYERS);

    //Conflicts:
    void conflictsAllPlayers();
    void conflictsPlayers(Player *players, int NUMBER_OF_PLAYERS, int numberOfrays);

    void shotPlayer(Player *shooter, enemyInfo_t enemyInfo);
    int findPlayer(Player *shooter, Player *players, int NUMBER_OF_PLAYERS, cv::Point enemyPoint);

    bool checkAllPlayersLife();
    bool checkPlayersLife(Player *players, int NUMBER_OF_PLAYERS);

    //mode:
    void moveAllPlayers();
    void movePlayers(Player *players, int NUMBER_OF_PLAYERS);

    //ANN:
    float getScore(int id);
    MatrixXf* getMatrixPtr(int id);

    void defineAllPlayersInput();
    void definePlayersInput(Player *players, int NUMBER_OF_PLAYERS);

    void multiplyAllPlayers();
    void multiplyPlayers(ANN* ann,Player *players, int NUMBER_OF_PLAYERS);

    void calculateScore();

    //reset:
    void resetAllPlayers(bool resetScore);
    void resetPlayers(Player *players, int NUMBER_OF_PLAYERS, int life, bool resetScore);

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

    void setAllWeightsMod(ANN *inocentMatrix, ANN *sniperMatrix, ANN *detectiveMatrix);

    void setInicialPosAll(cv::Point *inicialPos, int start);
    void setInicialPos(Player* players, int NUMBER_OF_PLAYERS, cv::Point initialPos);

    void setScore(float score, int id);
    MatrixXf* setMatrix(MatrixXf* matrixs, int id);
    ANN* getANN(int id);
    void game();
    void gameOfBest();
};
