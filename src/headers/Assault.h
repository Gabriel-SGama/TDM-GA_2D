#pragma once

#define NUMBER_OF_ASSAULTS 2
#define ASSAULT_HEALTH 100
#define ASSAULT_DAMAGE 50

class Assault;

#include "Moderator.h"

class Assault : public Player
{
private:
    /* data */
public:
    Assault();
    ~Assault();
};
