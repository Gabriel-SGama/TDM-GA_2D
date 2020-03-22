#include "headers/Inocent.h"

Inocent::Inocent() : Player()
{
    playerType = INOCENT;
    playerColor = INOCENT_COLOR;
    killPotential = INOCENT_KILL_POTENTIAL;
}

Inocent::~Inocent()
{
    std::cout << "delete" << std::endl;
}