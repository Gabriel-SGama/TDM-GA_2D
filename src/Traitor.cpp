#include "headers/Traitor.h"

Traitor::Traitor() : Player()
{
    playerType = TRAITOR;
    playerColor = TRAITOR_COLOR;
    damage = TRAITOR_DAMAGE;
    life = TRAITOR_HEALTH;

    //vision + position + life
    ANNInputSize = numberOfRays * 2 + 2 * (NUMBER_OF_TOTAL_PLAYERS - 1) + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + 1;

    //ann = new ANN(ANNInputSize, ANNOutputSize);
    ann = new ANN;
    ann->setANNParameters(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();
}

Traitor::~Traitor()
{
}