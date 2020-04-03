#ifndef DETECTIVE_H
#define DETECTIVE_H

#define NUMBER_OF_DETECTIVES 2
#define DETECTIVE_KILL_POTENTIAL 15

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