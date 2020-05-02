#pragma once

#define NUMBER_OF_LIGHT_ASSAULTS 8
#define LIGHT_ASSAULT_HEALTH 50

#define LIGHT_ASSAULT_DAMAGE 25

class LightAssault;

#include "Moderator.h"

class LightAssault : public Player
{
private:
    /* data */
public:
    LightAssault();
    ~LightAssault();
};
