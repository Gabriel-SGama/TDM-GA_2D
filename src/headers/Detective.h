#ifndef DETECTIVE_H
#define DETECTIVE_H

#define NUMBER_OF_DETECTIVES 3
#define DETECTIVE_HEALTH 100
#define DETECTIVE_DAMAGE 50

#include "Player.h"
#include "Screen.h"

class Detective : public Player
{
private:
    /* data */
public:
    Detective();
    ~Detective();
};

#endif