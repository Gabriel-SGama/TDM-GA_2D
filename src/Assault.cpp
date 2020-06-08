#include "headers/Assault.h"

Assault::Assault()
    : Player() {
    playerType = ASSAULT;
    playerColor = ASSAULT_COLOR;
    playerRay = ASSAULT_RAY;
    damage = ASSAULT_DAMAGE;
    life = ASSAULT_HEALTH;
    visionDist = ASSAULT_VISION_DIST;

    shotInterval = ASSAULT_SHOT_INTERVAL;
    speedLimit = ASSAULT_SPEED_LIMIT;
    angularSpeedLimit = ASSAULT_ANGULAR_SPEED_LIMIT;

    direction = (rand() % (int)(M_PI * 200)) / 100.0;
    //std::cout << direction << std::endl;
    visionAngle = ASSAULT_VISION_ANGLE;
    direction = (rand() % (int)(M_PI * 200)) / 100.0;

    numberOfRays = ASSAULT_NUMBER_OF_RAYS;

    separationAngle = visionAngle / numberOfRays;
    angleCorrection = visionAngle / numberOfRays - separationAngle;

    separationAngle += angleCorrection;

    raysID = new int[numberOfRays];
    raysDist = new int[numberOfRays];

    //vision + position + life  + direction + memory
    ANNInputSize = numberOfRays * 2 + 2 + 1 + 1 + 1 + MEMORY_SIZE;

    //angle + front speed + side speed + Shot rays + memory
    // ANNOutputSize = 1 + 1 + 1 + numberOfRays + MEMORY_SIZE;
    // ANNOutputSize = 1 + 1 + numberOfRays + MEMORY_SIZE;
    ANNOutputSize = 1 + 1 + 1 + MEMORY_SIZE;

    ann = new ANN;
    ann->setANNParameters(ANNInputSize, ANNOutputSize);

    input = ann->getInputPtr();
    output = ann->getOutputPtr();
}

Assault::~Assault() {
}