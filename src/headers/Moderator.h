#ifndef MODERATOR_H
#define MODERATOR_H

#include "Screen.h"
#include "Inocent.h"
#include "Traitor.h"
#include "Detective.h"

#define DURATION 750

typedef struct playerList_t
{
    Player *player;
} playerList_t;

//repeats the same function call for all players alive
class Moderator
{
private:
    float inocentsScore;
    float traitorScore;

    Screen *screen; //commun screen obj

    //players:
    Inocent *inocents;
    Detective *detectives;
    Traitor *traitors;

public:
    Moderator(Screen *screen);
    ~Moderator();
    //initial values:
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

    //ANN:
    void defineAllPlayersInput();
    void definePlayersInput(Player *players, int NUMBER_OF_PLAYERS);

    void multiplyAllPlayers();
    void multiplyPlayers(Player *players, int NUMBER_OF_PLAYERS);

    void calculateScore();
};

#endif