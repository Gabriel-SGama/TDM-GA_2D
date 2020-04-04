#include "headers/Traitor.h"

Traitor::Traitor() : Player()
{
    playerType = TRAITOR;
    playerColor = TRAITOR_COLOR;
    killPotential = TRAITOR_DAMAGE;
    life = TRAITOR_HEALTH;
}

Traitor::~Traitor()
{
    std::cout << "delete" << std::endl;
}