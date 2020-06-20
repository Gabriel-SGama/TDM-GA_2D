#pragma once
#include <cmath>

//----------------LIGHT ASSAULT VALUES----------------
#define LIGHT_ASSAULT_HEALTH 50
#define LIGHT_ASSAULT_DAMAGE 25
#define LIGHT_ASSAULT_NUMBER_OF_RAYS 10
#define LIGHT_ASSAULT_SPEED_LIMIT 10.0
#define LIGHT_ASSAULT_VISION_DIST 120
#define LIGHT_ASSAULT_SHOT_INTERVAL 3

const float LIGHT_ASSAULT_ANGULAR_SPEED_LIMIT = M_PI / 15.0;
const float LIGHT_ASSAULT_VISION_ANGLE = M_PI / 1.1;

class LightAssault;

#include "Moderator.h"

class LightAssault : public Player {
   private:
    /* data */
   public:
    LightAssault();
    ~LightAssault();
};
