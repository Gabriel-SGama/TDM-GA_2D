#include "headers/Light_Assault.h"

LightAssault::LightAssault()
    : Player() {
    //----------------PLAYER INICIAL VALUES----------------
    playerType = LIGHT_ASSAULT;
    lastPlayerType = LIGHT_ASSAULT;
    playerStatus = playerStatusLA + playerStatusInterval / 2.0;
    playerColor = LIGHT_ASSAULT_COLOR;
    playerRay = LIGHT_ASSAULT_RAY;
    damage = LIGHT_ASSAULT_DAMAGE;
    life = LIGHT_ASSAULT_HEALTH;
    visionDist = LIGHT_ASSAULT_VISION_DIST;

    shotInterval = LIGHT_ASSAULT_SHOT_INTERVAL;
    speedLimit = LIGHT_ASSAULT_SPEED_LIMIT;
    angularSpeedLimit = LIGHT_ASSAULT_ANGULAR_SPEED_LIMIT;

    visionAngle = LIGHT_ASSAULT_VISION_ANGLE;

    direction = (rand() % (int)(M_PI * 200)) / 100.0;
    initialPos = cv::Point(LENGTH - 300, HEIGHT - 150);
    numberOfRays = LIGHT_ASSAULT_NUMBER_OF_RAYS;
}

LightAssault::~LightAssault() {
}