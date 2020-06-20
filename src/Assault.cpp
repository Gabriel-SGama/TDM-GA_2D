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

    visionAngle = ASSAULT_VISION_ANGLE;

    direction = (rand() % (int)(M_PI * 200)) / 100.0;
    initialPos = cv::Point(0,150);
    numberOfRays = ASSAULT_NUMBER_OF_RAYS;
}

Assault::~Assault() {
}