#include "headers/Light_Assault.h"

LightAssault::LightAssault() : Player()
{

    playerType = LIGHT_ASSAULT;
    playerColor = LIGHT_ASSAULT_COLOR;
    damage = LIGHT_ASSAULT_DAMAGE;
    life = LIGHT_ASSAULT_HEALTH;

    //vision + position + life
    ANNInputSize = numberOfRays * 2 + 2*(NUMBER_OF_LIGHT_ASSAULTS + NUMBER_OF_SNIPERS + NUMBER_OF_ASSAULTS - 1) + 1;
    //angle + distance + shot id
    ANNOutputSize = 1 + 1 + 1;

    //ann = new ANN(ANNInputSize, ANNOutputSize);
    ann = new ANN;
    ann->setANNParameters(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();
}

LightAssault::~LightAssault()
{
}