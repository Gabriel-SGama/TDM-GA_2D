#include "headers/Assault.h"

Assault::Assault() : Player()
{
    playerType = ASSAULT;
    playerColor = ASSAULT_COLOR;
    damage = ASSAULT_DAMAGE;
    life = ASSAULT_HEALTH;

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

Assault::~Assault()
{
    std::cout << "delete" << std::endl;
}