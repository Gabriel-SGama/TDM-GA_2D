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
    initialPos = cv::Point(LENGTH-300,HEIGHT-250);

    direction = (rand() % (int)(M_PI * 200)) / 100.0;
    visionAngle = LIGHT_ASSAULT_VISION_ANGLE;

    numberOfRays = LIGHT_ASSAULT_NUMBER_OF_RAYS;

    separationAngle = visionAngle / numberOfRays;
    angleCorrection = visionAngle / numberOfRays - separationAngle;

    separationAngle += angleCorrection;

    raysID = new int[numberOfRays];
    raysDist = new int[numberOfRays];

    //vision + position + life + direction
    ANNInputSize = numberOfRays * 2 + 2 + 1 + 1; //+ 2*(NUMBER_OF_LIGHT_ASSAULTS - 1);
    //angle + front speed + side speed + Shot rays
    ANNOutputSize = 1 + 1 + 1 + numberOfRays;

    outputTest.setZero(ANNOutputSize);

}

LightAssault::~LightAssault() {
}