#ifndef MODERATOR_H
#define MODERATOR_H

#include "Screen.h"
#include "Inocent.h"
#include "Traitor.h"
#include "Detective.h"

typedef struct playerList_t
{
    Player *player;
} playerList_t;

//repeats the same function call for all players alive
class Moderator
{
private:
    Screen *screen; //commun screen obj

    //players:
    Inocent *inocents;
    Detective *detectives;
    Traitor *traitors;

public:
    Moderator(Screen *screen);
    ~Moderator();

    void shotPlayer(Player *shooter, int damage, enemyInfo_t enemyInfo);
    void findPlayer(Player *shooter, Player *players, int NUMBER_OF_PLAYERS, int damage, cv::Point enemyPoint);
    void setAllPlayersValues();
    void setPlayersValues(int &playerNumber, Player *players, int NUMBER_OF_PLAYERS);
    void drawAllPlayers();
    void drawPlayers(Player *players, int NUMBER_OF_PLAYERS);
    void updateAllPlayersVision();
    void updatePlayersVision(Player *players, int NUMBER_OF_PLAYERS);
    void conflictsAllPlayers();
    void conflictsPlayers(Player *players, int NUMBER_OF_PLAYERS);
    void checkAllPlayersLife();
    void checkPlayersLife(Player *players, int NUMBER_OF_PLAYERS);

    void multiplyAllPlayers();
    void multiplyPlayers(Player *players, int NUMBER_OF_PLAYERS);
    void defineAllPlayersInput();
    void definePlayersInput(Player *players, int NUMBER_OF_PLAYERS);
};

#endif