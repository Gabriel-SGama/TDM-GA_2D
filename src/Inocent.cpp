#include "headers/Inocent.h"

Inocent::Inocent() : Player()
{

    playerType = INOCENT;
    playerColor = INOCENT_COLOR;
    killPotential = INOCENT_DAMAGE;
    life = INOCENT_HEALTH;

    //vision + position+ take damage + life
    ANNInputSize = numberOfRays * 2 + 2 + 1 + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + 1;

    ann = new ANN(ANNInputSize, ANNOutputSize);

    output = ann->getOutputPtr();
}

Inocent::~Inocent()
{
}