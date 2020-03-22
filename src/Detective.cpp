#include "headers/Detective.h"

Detective::Detective() : Player()
{
    playerType = DETECTIVE;
    playerColor = DETECTIVE_COLOR;
    killPotential = DETECTIVE_KILL_POTENTIAL;
}

Detective::~Detective()
{
    std::cout << "delete" << std::endl;
}