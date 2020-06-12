#include "headers/Light_Assault.h"

LightAssault::LightAssault()
    : Player() {
    playerType = LIGHT_ASSAULT;
    playerColor = LIGHT_ASSAULT_COLOR;
    playerRay = LIGHT_ASSAULT_RAY;
    damage = LIGHT_ASSAULT_DAMAGE;
    life = LIGHT_ASSAULT_HEALTH;
    visionDist = LIGHT_ASSAULT_VISION_DIST;

    shotInterval = LIGHT_ASSAULT_SHOT_INTERVAL;

    speedLimit = LIGHT_ASSAULT_SPEED_LIMIT;
    angularSpeedLimit = LIGHT_ASSAULT_ANGULAR_SPEED_LIMIT;

    initialPos = cv::Point(LENGTH - 300, HEIGHT - 150);
    visionAngle = LIGHT_ASSAULT_VISION_ANGLE;
    direction = (rand() % (int)(M_PI * 200)) / 100.0;

    numberOfRays = LIGHT_ASSAULT_NUMBER_OF_RAYS;

    // separationAngle = visionAngle / numberOfRays;
    // angleCorrection = visionAngle / numberOfRays - separationAngle;

    // separationAngle += angleCorrection;

    // raysID = new int[numberOfRays];
    // raysDist = new int[numberOfRays];

    // //vision + position + life + direction + memory
    // ANNInputSize = numberOfRays * 2 + 2 + 1 + 1 + 1 + NUMBER_OF_PLAYERS*2 - 2 + MEMORY_SIZE;
    // //angle + front speed + side speed + Shot rays + memory
    // // ANNOutputSize = 1 + 1 + 1 + numberOfRays + MEMORY_SIZE;
    // // ANNOutputSize = 1 + 1 + numberOfRays + MEMORY_SIZE;
    // ANNOutputSize = 1 + 1 + 1 + MEMORY_SIZE;

    // ann = new ANN;
    // ann->setANNParameters(ANNInputSize, ANNOutputSize);

    // input = ann->getInputPtr();
    // output = ann->getOutputPtr();
}

LightAssault::~LightAssault() {
}