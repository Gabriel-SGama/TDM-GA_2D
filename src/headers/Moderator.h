#ifndef MODERATOR_H
#define MODERATOR_H

#include "Screen.h"
#include "Inocent.h"
#include "Traitor.h"
#include "Detective.h"

#define DURATION 150

typedef struct dataOfBestPlayers_t
{

    float score;
    Player *player;

    dataOfBestPlayers_t() : score(-100){};

} dataOfBestPlayers_t;

typedef struct playerList_t
{
    Player *player;
} playerList_t;

//repeats the same function call for all players alive
class Moderator
{
private:
    //players:
    Inocent *inocents;
    Traitor *traitors;
    Detective *detectives;

public:
    dataOfBestPlayers_t *bestInocent;
    dataOfBestPlayers_t *bestDetective;
    dataOfBestPlayers_t *bestTraitor;

    float inocentsScore;
    float traitorScore;

    Moderator();
    ~Moderator();

    Screen *screen; //commun screen obj

    //initial values:
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
    void resetAllPlayers();
    void resetPlayers(Player *players, int NUMBER_OF_PLAYERS, int life);

    //get best players
    inline Inocent *getInocents() { return inocents; }
    inline Traitor *getTraitors() { return traitors; }
    inline Detective *getDetectives() { return detectives; }

    //weights
    void setAllWeights(Inocent *inocents, Traitor *traitors, Detective *detectives);
    void setWeights(Player *bestPlayer, Player *players, int NUMBER_OF_PLAYERS);

    void copyAllWeights(Inocent *inocents, Traitor *traitors, Detective *detectives);
    void copyWeights(Player *bestPlayer, Player *players, int NUMBER_OF_PLAYERS);

    void game();
    void gameOfBest();
};

#endif