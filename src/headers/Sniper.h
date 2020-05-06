#pragma once

#include <cmath>

#define NUMBER_OF_SNIPERS 5
#define SNIPER_HEALTH 50
#define SNIPER_DAMAGE 25
#define SNIPER_NUMBER_OF_RAYS 10
#define SNIPER_SPEED_LIMIT 2.5
#define SNIPER_VISION_DIST 250
#define SNIPER_SHOT_INTERVAL 10


const float SNIPER_ANGULAR_SPEED_LIMIT = M_PI / 80;
const float SNIPER_VISION_ANGLE = M_PI / 6;

class Sniper;

#include "Moderator.h"

class Sniper : public Player
{
private:
    /* data */
public:
    Sniper();
    ~Sniper();
};
