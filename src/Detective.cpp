#include "headers/Detective.h"

Detective::Detective() : Player()
{
    playerType = DETECTIVE;
    playerColor = DETECTIVE_COLOR;
    damage = DETECTIVE_DAMAGE;
    life = DETECTIVE_HEALTH;

    //vision + positions + life
    ANNInputSize = numberOfRays * 2 + 2 * (NUMBER_OF_TOTAL_PLAYERS - 1) + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + 1;

    //ann = new ANN(ANNInputSize, ANNOutputSize);
    ann = new ANN;
    ann->setANNParameters(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();
}

Detective::~Detective()
{
    std::cout << "delete" << std::endl;
}