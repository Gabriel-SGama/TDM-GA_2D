#ifndef TRAITOR_H
#define TRAITOR_H

#define NUMBER_OF_TRAITORS 3
#define TRAITOR_KILL_POTENTIAL 9

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