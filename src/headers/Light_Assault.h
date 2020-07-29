#pragma once
#include <cmath>

//----------------LIGHT ASSAULT TEST VALUES----------------
#define LIGHT_ASSAULT_HEALTH 9
#define LIGHT_ASSAULT_DAMAGE 6
#define LIGHT_ASSAULT_NUMBER_OF_RAYS 12
#define LIGHT_ASSAULT_SPEED_LIMIT 10.0
#define LIGHT_ASSAULT_VISION_DIST 130
#define LIGHT_ASSAULT_SHOT_INTERVAL 3

const float LIGHT_ASSAULT_ANGULAR_SPEED_LIMIT = M_PI / 15.0;
const float LIGHT_ASSAULT_VISION_ANGLE = M_PI / 1.1;

class LightAssault;

#include "Player.h"

class LightAssault : public Player {
   private:
   public:
    LightAssault();
    ~LightAssault();
};
