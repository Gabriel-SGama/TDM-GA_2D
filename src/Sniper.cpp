#include "headers/Sniper.h"

Sniper::Sniper()
    : Player() {
    playerType = SNIPER;
    playerColor = SNIPER_COLOR;
    playerRay = SNIPER_RAY;
    damage = SNIPER_DAMAGE;
    life = SNIPER_HEALTH;
    visionDist = SNIPER_VISION_DIST;

    shotInterval = SNIPER_SHOT_INTERVAL;

    speedLimit = SNIPER_SPEED_LIMIT;
    angularSpeedLimit = SNIPER_ANGULAR_SPEED_LIMIT;
    initialPos = cv::Point(LENGTH - 400,150);

    direction = (rand() % (int)(M_PI * 200)) / 100.0;
    //std::cout << direction << std::endl;
    visionAngle = SNIPER_VISION_ANGLE;

    numberOfRays = SNIPER_NUMBER_OF_RAYS;

    separationAngle = visionAngle / numberOfRays;
    angleCorrection = visionAngle / numberOfRays - separationAngle;

    separationAngle += angleCorrection;

    raysID = new int[numberOfRays];
    raysDist = new int[numberOfRays];

    ANNInputSize = numberOfRays * 2 + 2 + 1 + 1; //+ 2*(NUMBER_OF_SNIPERS - 1);

    ANNOutputSize = 1 + 1 + 1 + numberOfRays;

    outputTest.setZero(ANNOutputSize);

}

Sniper::~Sniper() {
}