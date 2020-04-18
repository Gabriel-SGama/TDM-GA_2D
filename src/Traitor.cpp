#include "headers/Traitor.h"

Traitor::Traitor() : Player()
{
    playerType = TRAITOR;
    playerColor = TRAITOR_COLOR;
    killPotential = TRAITOR_DAMAGE;
    life = TRAITOR_HEALTH;

    //vision + position+ take damage + life
    ANNInputSize = numberOfRays * 2 + 2 + 1 + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + 1;

    ann = new ANN(ANNInputSize, ANNOutputSize);

    output = ann->getOutputPtr();
}

Traitor::~Traitor()
{
    std::cout << "delete" << std::endl;
}