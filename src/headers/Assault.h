#pragma once
#include <cmath>

//----------------ASSAULT VALUES----------------
#define ASSAULT_HEALTH 100
#define ASSAULT_DAMAGE 35
#define ASSAULT_NUMBER_OF_RAYS 8
#define ASSAULT_SPEED_LIMIT 7.0
#define ASSAULT_VISION_DIST 160
#define ASSAULT_SHOT_INTERVAL 6

const float ASSAULT_ANGULAR_SPEED_LIMIT = M_PI / 45;
const float ASSAULT_VISION_ANGLE = M_PI / 2.5;

class Assault;

#include "Player.h"

class Assault : public Player {
   private:
   public:
    Assault();
    ~Assault();
};
