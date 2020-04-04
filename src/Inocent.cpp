#include "headers/Inocent.h"

Inocent::Inocent() : Player()
{
    playerType = INOCENT;
    playerColor = INOCENT_COLOR;
    killPotential = INOCENT_DAMAGE;
    life = INOCENT_HEALTH;
}

Inocent::~Inocent()
{
    std::cout << "delete" << std::endl;
}