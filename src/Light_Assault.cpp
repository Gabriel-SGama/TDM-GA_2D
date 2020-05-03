#include "headers/Light_Assault.h"

LightAssault::LightAssault() : Player()
{
    playerType = LIGHT_ASSAULT;
    playerColor = LIGHT_ASSAULT_COLOR;
    playerRay = LIGHT_ASSAULT_RAY;
    damage = LIGHT_ASSAULT_DAMAGE;
    life = LIGHT_ASSAULT_HEALTH;
    visionDist = LIGHT_ASSAULT_VISION_DIST;

    speedLimit = LIGHT_ASSAULT_SPEED_LIMIT;
    angularSpeedLimit = LIGHT_ASSAULT_ANGULAR_SPEED_LIMIT;

    direction = (rand() %(int) (M_PI * 200)) / 100.0;
    //std::cout << direction << std::endl;
    visionAngle = LIGHT_ASSAULT_VISION_ANGLE;

    numberOfRays = LIGHT_ASSAULT_NUMBER_OF_RAYS;

    separationAngle = visionAngle / numberOfRays;
    angleCorrection = visionAngle / numberOfRays - separationAngle;

    separationAngle += angleCorrection;

    raysID = new int[numberOfRays];
    raysDist = new int[numberOfRays];


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

LightAssault::~LightAssault()
{
}