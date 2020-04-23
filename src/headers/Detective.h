#pragma once

#define NUMBER_OF_DETECTIVES 2
#define DETECTIVE_HEALTH 100
#define DETECTIVE_DAMAGE 50

class Detective;

#include "Moderator.h"

class Detective : public Player
{
private:
    /* data */
public:
    Detective();
    ~Detective();
};
