#ifndef TRAITOR_H
#define TRAITOR_H

#define NUMBER_OF_TRAITORS 20
#define TRAITOR_HEALTH 75
#define TRAITOR_DAMAGE 35

#include "Player.h"
#include "Screen.h"

class Traitor : public Player
{
private:
    /* data */
public:
    Traitor();
    ~Traitor();
};

#endif