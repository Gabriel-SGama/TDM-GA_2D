#pragma once

#define NUMBER_OF_TRAITORS 20
#define TRAITOR_HEALTH 75
#define TRAITOR_DAMAGE 35

class Traitor;

#include "Moderator.h"

class Traitor : public Player
{
private:
    /* data */
public:
    Traitor();
    ~Traitor();
};
