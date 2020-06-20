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

    visionAngle = SNIPER_VISION_ANGLE;
    
    direction = (rand() % (int)(M_PI * 200)) / 100.0;
    initialPos = cv::Point(LENGTH - 400,150);
    numberOfRays = SNIPER_NUMBER_OF_RAYS;
}

Sniper::~Sniper() {
}