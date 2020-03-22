#include "headers/Traitor.h"

Traitor::Traitor() : Player()
{
    playerType = TRAITOR;
    playerColor = TRAITOR_COLOR;
    killPotential = TRAITOR_KILL_POTENTIAL;
}

Traitor::~Traitor()
{
    std::cout << "delete" << std::endl;
}