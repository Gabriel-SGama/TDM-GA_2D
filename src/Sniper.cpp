#include "headers/Sniper.h"

Sniper::Sniper() : Player()
{
    playerType = SNIPER;
    playerColor = SNIPER_COLOR;
    damage = SNIPER_DAMAGE;
    life = SNIPER_HEALTH;

    //vision + position + life
    ANNInputSize = numberOfRays * 2 + 2 * NUMBER_OF_TOTAL_PLAYERS + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + numberOfRays;

    //ann = new ANN(ANNInputSize, ANNOutputSize);
    ann = new ANN;
    ann->setANNParameters(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();
}

Sniper::~Sniper()
{
}