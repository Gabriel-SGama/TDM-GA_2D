#include "headers/Detective.h"

Detective::Detective() : Player()
{
    playerType = DETECTIVE;
    playerColor = DETECTIVE_COLOR;
    killPotential = DETECTIVE_DAMAGE;
    life = DETECTIVE_HEALTH;

    //vision + position + life
    ANNInputSize = numberOfRays * 2 + 2 + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + 1;

    ann = new ANN(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();
}

Detective::~Detective()
{
    std::cout << "delete" << std::endl;
}