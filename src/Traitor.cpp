#include "headers/Traitor.h"

Traitor::Traitor() : Player()
{
    playerType = TRAITOR;
    playerColor = TRAITOR_COLOR;
    killPotential = TRAITOR_DAMAGE;
    life = TRAITOR_HEALTH;

    //vision + position + life
    ANNInputSize = numberOfRays * 2 + 2 + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + 1;

    ann = new ANN(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();
}

Traitor::~Traitor()
{
}