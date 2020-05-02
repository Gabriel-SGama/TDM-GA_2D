#pragma once

#define NUMBER_OF_SNIPERS 6
#define SNIPER_HEALTH 75
#define SNIPER_DAMAGE 35

class Sniper;

#include "Moderator.h"

class Sniper : public Player
{
private:
    /* data */
public:
    Sniper();
    ~Sniper();
};
