#include "headers/Assault.h"

Assault::Assault()
    : Player() {
    playerType = ASSAULT;
    playerColor = ASSAULT_COLOR;
    playerRay = ASSAULT_RAY;
    damage = ASSAULT_DAMAGE;
    life = ASSAULT_HEALTH;
    visionDist = ASSAULT_VISION_DIST;
    initialPos = cv::Point(0,150);
    
    shotInterval = ASSAULT_SHOT_INTERVAL;
    speedLimit = ASSAULT_SPEED_LIMIT;
    angularSpeedLimit = ASSAULT_ANGULAR_SPEED_LIMIT;

    direction = (rand() % (int)(M_PI * 200)) / 100.0;
    //std::cout << direction << std::endl;
    visionAngle = ASSAULT_VISION_ANGLE;

    numberOfRays = ASSAULT_NUMBER_OF_RAYS;

    separationAngle = visionAngle / numberOfRays;
    angleCorrection = visionAngle / numberOfRays - separationAngle;

    separationAngle += angleCorrection;

    raysID = new int[numberOfRays];
    raysDist = new int[numberOfRays];

    //vision + position + life  + direction + memory
    ANNInputSize = numberOfRays * 2 + 2 + 1 + 1; //+ 2*(NUMBER_OF_ASSAULTS - 1);

    //angle + front speed + side speed + Shot rays + memory
    ANNOutputSize = 1 + 1 + 1 + numberOfRays;

    outputTest.setZero(ANNOutputSize);
}

Assault::~Assault() {
}
