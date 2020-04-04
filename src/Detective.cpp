#include "headers/Detective.h"

Detective::Detective() : Player()
{
    playerType = DETECTIVE;
    playerColor = DETECTIVE_COLOR;
    killPotential = DETECTIVE_DAMAGE;
    life = DETECTIVE_HEALTH;
}

Detective::~Detective()
{
    std::cout << "delete" << std::endl;
}