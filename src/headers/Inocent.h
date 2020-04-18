#ifndef INOCENT_H
#define INOCENT_H

#define NUMBER_OF_INOCENTS 10
#define INOCENT_HEALTH 50

#define INOCENT_DAMAGE 25

#include "Player.h"
#include "Screen.h"

class Inocent : public Player
{
private:
    /* data */
public:
    Inocent();
    ~Inocent();
};

#endif