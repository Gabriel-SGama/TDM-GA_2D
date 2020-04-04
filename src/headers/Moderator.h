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

class Moderator
{
private:
    Screen *screen;

    Inocent *inocents;
    Detective *detectives;
    Traitor *traitors;

public:
    Moderator(Screen *screen);
    ~Moderator();

    void shotPlayer(Player *shooter, int damage, cv::Point enemyPoint);
    void setPlayersValues();
    void drawPlayers();
    void updatePlayersVision();
    void conflicts();
    void checkLife();
};

#endif