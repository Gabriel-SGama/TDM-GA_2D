#pragma once
#include <cmath>

#define NUMBER_OF_SNIPERS 4
#define SNIPER_HEALTH 75
#define SNIPER_DAMAGE 50
#define SNIPER_NUMBER_OF_RAYS 5
#define SNIPER_SPEED_LIMIT 5.0
#define SNIPER_VISION_DIST 250
#define SNIPER_SHOT_INTERVAL 10

const float SNIPER_ANGULAR_SPEED_LIMIT = M_PI / 40;
const float SNIPER_VISION_ANGLE = M_PI / 4.5;

class Sniper;

#include "Moderator.h"

class Sniper : public Player {
   private:
    /* data */
   public:
    Sniper();
    ~Sniper();
};
