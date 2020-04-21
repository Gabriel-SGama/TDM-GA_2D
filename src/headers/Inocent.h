#pragma once

#define NUMBER_OF_INOCENTS 25
#define INOCENT_HEALTH 50

#define INOCENT_DAMAGE 25

class Inocent;

#include "Moderator.h"

class Inocent : public Player
{
private:
    /* data */
public:
    Inocent();
    ~Inocent();
};
