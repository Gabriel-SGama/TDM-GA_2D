#include "headers/Inocent.h"

Inocent::Inocent() : Player()
{

    playerType = LIGHT_ASSAULT;
    playerColor = INOCENT_COLOR;
    damage = INOCENT_DAMAGE;
    life = INOCENT_HEALTH;

    //vision + position + life
    ANNInputSize = numberOfRays * 2 + 2*(NUMBER_OF_INOCENTS + NUMBER_OF_SNIPERS + NUMBER_OF_DETECTIVES - 1) + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + 1;

    //ann = new ANN(ANNInputSize, ANNOutputSize);
    ann = new ANN;
    ann->setANNParameters(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();
}

Inocent::~Inocent()
{
}