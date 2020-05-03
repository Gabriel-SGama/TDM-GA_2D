#pragma once

#include <cmath>

#define NUMBER_OF_LIGHT_ASSAULTS 4
#define LIGHT_ASSAULT_HEALTH 50
#define LIGHT_ASSAULT_DAMAGE 25
#define LIGHT_ASSAULT_NUMBER_OF_RAYS 15
#define LIGHT_ASSAULT_SPEED_LIMIT 3
#define LIGHT_ASSAULT_VISION_DIST 80
#define LIGHT_ASSAULT_SHOT_INTERVAL 3

const float LIGHT_ASSAULT_ANGULAR_SPEED_LIMIT = M_PI / 30;
const float LIGHT_ASSAULT_VISION_ANGLE = M_PI / 1.1;

class LightAssault;

#include "Moderator.h"

class LightAssault : public Player
{
private:
    /* data */
public:
    LightAssault();
    ~LightAssault();
};
