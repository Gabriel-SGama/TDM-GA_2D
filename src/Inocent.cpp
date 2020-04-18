#include "headers/Inocent.h"

Inocent::Inocent() : Player()
{

    playerType = INOCENT;
    playerColor = INOCENT_COLOR;
    damage = INOCENT_DAMAGE;
    life = INOCENT_HEALTH;

    //vision + position + life
    ANNInputSize = numberOfRays * 2 + 2 + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + 1;

    ann = new ANN(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();
}

Inocent::~Inocent()
{
}