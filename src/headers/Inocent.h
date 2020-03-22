#ifndef INOCENT_H
#define INOCENT_H

#define NUMBER_OF_INOCENTS 9
#define INOCENT_KILL_POTENTIAL 1

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